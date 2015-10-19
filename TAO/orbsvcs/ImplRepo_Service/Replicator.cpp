#include "orbsvcs/Log_Macros.h"
#include "Replicator.h"
#include "Shared_Backing_Store.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_unistd.h"

UPN_i::UPN_i (Replicator &owner)
  :owner_ (owner)
{
}

void
UPN_i::notify_update (CORBA::ULongLong seq_num,
                      const ImplementationRepository::UpdateInfoSeq& info)
{
  bool missed = false;
  CORBA::ULongLong expected = ++this->owner_.replica_seq_num_;
  if (expected < seq_num)
    {
      missed = true;
      if (this->owner_.debug_ > 0)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) UPN_i::notify_updated_entity ")
                          ACE_TEXT ("expected %Lu got %Lu\n"),
                          expected, seq_num ));
        }
      this->owner_.replica_seq_num_ = seq_num;
    }
  else if (expected > seq_num)
    {
      if (this->owner_.debug_ > 0)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) UPN_i::notify_updated_entity ")
                          ACE_TEXT ("expected %Lu got %Lu\n"),
                          expected, seq_num ));
        }
      --this->owner_.replica_seq_num_;
    }
  this->owner_.repo_.updates_available (info, missed);
}

void
UPN_i::register_replica
  (ImplementationRepository::UpdatePushNotification_ptr replica,
   char*& ft_imr_ior,
   CORBA::ULongLong_out seq_num)
{
  this->owner_.peer_ =
    ImplementationRepository::UpdatePushNotification::_duplicate (replica);
  this->owner_.replica_seq_num_ = 0;

  this->owner_.repo_.gen_ior (ft_imr_ior);
  seq_num = this->owner_.seq_num_;
}

//---------------------------------------------------------------------------

Replicator::Replicator (Shared_Backing_Store &repo, const Options& opts)
  : me_ (),
    peer_ (),
    seq_num_ (0),
    replica_seq_num_ (0),
    repo_ (repo),
    prev_update_ (),
    orb_ (),
    reactor_ (0),
    lock_ (),
    notified_ (false),
    to_send_ (10),
    debug_ (opts.debug ()),
    endpoint_ (opts.ft_endpoint ()),
    update_delay_ (opts.ft_update_delay ())
{
}

Replicator::~Replicator()
{
}

void
Replicator::init_orb (void)
{
  int argc = 6;
  ACE_TCHAR *argv[8];
  argv[0] = ACE_OS::strdup (ACE_TEXT (""));
  argv[1] = ACE_OS::strdup (ACE_TEXT ("-ORBIgnoreDefaultSvcConfFile"));
  argv[2] = ACE_OS::strdup (ACE_TEXT ("-ORBGestalt"));
  argv[3] = ACE_OS::strdup (ACE_TEXT ("Local"));
  argv[4] = ACE_OS::strdup (ACE_TEXT ("-ORBSvcConfDirective"));
  argv[5] = ACE_OS::strdup (ACE_TEXT ("static Client_Strategy_Factory \"-ORBConnectStrategy Blocked -ORBDefaultSyncScope server\""));
  if (endpoint_.length ())
    {
      argv[6] = ACE_OS::strdup (ACE_TEXT ("-ORBListenEnpoints"));
      argv[7] = ACE_OS::strdup (ACE_TEXT_CHAR_TO_TCHAR (endpoint_.c_str ()));
    }
  else
    {
      argv[6] = 0;
      argv[7] = 0;
    }
  this->orb_ = CORBA::ORB_init (argc, argv, "replicator_orb");
  this->reactor_ = this->orb_->orb_core ()->reactor ();

  CORBA::Object_var obj = this->orb_->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (obj.in ());
  PortableServer::ServantBase_var servant;
  ACE_NEW (servant, UPN_i (*this));
  PortableServer::ObjectId_var oid = poa->activate_object (servant.in ());
  obj = poa->id_to_reference (oid.in ());
  this->me_ =
    ImplementationRepository::UpdatePushNotification::_narrow (obj.in ());
  PortableServer::POAManager_var mgr = poa->the_POAManager ();
  mgr->activate ();
}

char *
Replicator::ior (void)
{
  return this->orb_->object_to_string (this->me_.in ());
}

bool
Replicator::peer_available (void)
{
  return !CORBA::is_nil (this->peer_.in ());
}

void
Replicator::stop (void)
{
  if (this->reactor_ == 0)
    {
      return;
    }
  this->orb_->shutdown (true);
}

int
Replicator::svc (void)
{
  if (this->reactor_ == 0)
    {
      return 0;
    }
  try
    {
      this->orb_->run ();
    }
  catch (const CORBA::SystemException &ex)
    {
      if (this->debug_ > 0)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                         ACE_TEXT ("(%P|%t) Replicator::svc caught %C\n"),
                          ex._name ()));
        }
    }
  return 0;
}


int
Replicator::handle_exception (ACE_HANDLE )
{
  ACE_OS::sleep (this->update_delay_);
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_, -1);
  this->notified_ = false;
  if (this->to_send_.length () == 0)
    {
      return 0;
    }
  if (CORBA::is_nil (this->peer_))
    {
      this->to_send_.length (0);
      return 0;
    }
  try
    {
      CORBA::Long len = this->to_send_.length ();
      ImplementationRepository::UpdateInfoSeq payload (len);
      payload.length (len);
      CORBA::Long p = 0;
      for (CORBA::Long l = 0; l < len; l++)
        {
          if (this->to_send_[l].action._d () != ImplementationRepository::access ||
              this->to_send_[l].action.state () != ImplementationRepository::AAM_UPDATE_FAILED)
            {
              payload[p++] = this->to_send_[l];
              if (this->to_send_[l].action._d () != ImplementationRepository::access)
                {
                  ACE_CString sid = this->to_send_[l].name.in();
                  this->prev_update_.bind (sid, this->to_send_[l].action.state ());
                }
            }
        }
      payload.length (p);
      this->to_send_.length (0);
      CORBA::ULongLong seq = ++this->seq_num_;
      mon.release ();
      this->peer_->notify_update (seq, payload);
    }
  catch (const CORBA::Exception &)
    {
      this->peer_ =
        ImplementationRepository::UpdatePushNotification::_nil ();
    }
  return 0;
}

void
Replicator::send_access_state (const char *name, ImplementationRepository::AAM_Status state)
{
  if (this->reactor_ == 0)
    {
      return;
    }
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
  ImplementationRepository::AAM_Status p =
    ImplementationRepository::AAM_UPDATE_FAILED;
  ACE_CString sid = name;
  bool prev = this->prev_update_.find (sid, p) == 0 && p == state;

  CORBA::Long len = this->to_send_.length ();
  bool found = false;
  for (CORBA::Long i = 0; !found && i < len; i++)
    {
      if (this->to_send_[i].action._d () == ImplementationRepository::access &&
          ACE_OS::strcmp (this->to_send_[i].name, name) == 0)
        {
          found = true;
          this->to_send_[i].action.state
            (prev ? ImplementationRepository::AAM_UPDATE_FAILED : state);
        }
    }
  if (!found && !prev)
    {
      this->to_send_.length (len+1);
      this->to_send_[len].name = CORBA::string_dup (name);
      this->to_send_[len].action.state (state);
    }
  if (this->notified_)
    return;
  this->notified_ = true;
  this->reactor_->notify (this);
}

void
Replicator::send_entity (ImplementationRepository::UpdateInfo &info)
{
  if (this->reactor_ == 0)
    {
      return;
    }
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
  CORBA::Long len = this->to_send_.length ();
  bool found = false;
  for (CORBA::Long i = 0; !found && i < len; i++)
    {
      if (ACE_OS::strcmp (this->to_send_[i].name, info.name) == 0)
        {
          if (this->to_send_[i].action._d () == info.action._d ())
            {
              switch (info.action._d ())
                {
                case ImplementationRepository::repo_update:
                  {
                    found = (this->to_send_[i].action.info ().kind == info.action.info ().kind );
                    break;
                  }
                case ImplementationRepository::repo_remove:
                  {
                    found = (this->to_send_[i].action.kind () == info.action.kind ());
                    break;
                  }
                default:
                  {
                  }
                }
            }
          else
            {
              if (this->to_send_[i].action._d () == ImplementationRepository::access)
                {
                  continue;
                }
              switch (info.action._d ())
                {
                case ImplementationRepository::repo_update:
                  {
                    found = (this->to_send_[i].action.kind () == info.action.info ().kind);
                    break;
                  }
                case ImplementationRepository::repo_remove:
                  {
                    found = (this->to_send_[i].action.info ().kind == info.action.kind ());
                    break;
                  }
                default:
                  {
                  }
                }
              if (found)
                {
                  this->to_send_[i].action = info.action;
                }
            }
        }
    }
  if (!found)
    {
      this->to_send_.length (len+1);
      this->to_send_[len] = info;
    }
  if (this->notified_)
    return;
  this->notified_ = true;
  this->reactor_->notify (this);
}

int
Replicator::send_registration (char *&imr_ior)
{

  if (this->debug_ > 1)
    {
      ORBSVCS_DEBUG((LM_INFO,
        ACE_TEXT("Registering with previously running ImR replica\n")));
    }

  try
    {
      this->peer_->register_replica(this->me_,
                                    imr_ior,
                                    this->replica_seq_num_);
    }
  catch (const ImplementationRepository::InvalidPeer& ip)
    {
      ORBSVCS_ERROR_RETURN ((LM_ERROR,
        ACE_TEXT("Error: Replicator::send_registration invalid ImR replica because %s\n"),
        ip.reason.in()), -1);
    }

  if (this->debug_ > 9)
    {
      ORBSVCS_DEBUG ((LM_INFO,
        ACE_TEXT("Initializing repository with ft ior=<%C> ")
        ACE_TEXT("and replica seq number %Lu\n"),
        imr_ior, replica_seq_num_));
    }

  return 0;
}

bool
Replicator::init_peer (const ACE_CString &replica_ior_file)
{
  if (this->debug_ > 1)
    {
      ORBSVCS_DEBUG ((LM_INFO,
                      ACE_TEXT("Resolving ImR replica %s\n"),
                      replica_ior_file.c_str()));
    }

  if (ACE_OS::access (replica_ior_file.c_str (), F_OK) != 0)
    {
      this->peer_ =
        ImplementationRepository::UpdatePushNotification::_nil();
      return false;
    }

  ACE_CString replica_ior = "file://" + replica_ior_file;
  CORBA::Object_var obj =
    this->orb_->string_to_object (replica_ior.c_str());

  if (!CORBA::is_nil (obj.in ()))
    {
      bool non_exist = true;
      try
        {
          this->peer_ = ImplementationRepository::
            UpdatePushNotification::_narrow (obj.in());
          non_exist = (this->peer_->_non_existent() == 1);
        }
      catch (const CORBA::Exception& )
        {
        }

      if (non_exist)
        {
          this->peer_ =
            ImplementationRepository::UpdatePushNotification::_nil();
        }
    }
  return true;
}
