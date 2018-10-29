// -*- C++ -*-

//=============================================================================
/**
 * @file   FT_Naming_Replication_Manager.cpp
 *
 * @author Kevin Stanley <stanleyk@ociweb.com>
 * @author Phil Mesnier
 */
//=============================================================================

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Replication_Manager.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Server.h"
#include "tao/ORB_Core.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_ctype.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Log_Msg.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

FT_Update_Replicant_i::FT_Update_Replicant_i (TAO_FT_Naming_Replication_Manager &owner)
  :owner_ (owner)
{
}

void
FT_Update_Replicant_i::notify_update (CORBA::ULongLong seq_num,
                                      FT_Naming::UpdateInfoSeq& info)
{
  CORBA::ULongLong expected = ++this->owner_.replica_seq_num_;
  if (expected < seq_num)
    {
      if (TAO_debug_level > 0)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) FT_Update_Replicant_i::notify_updated_entity ")
                          ACE_TEXT ("expected %Lu got %Lu\n"),
                          expected, seq_num ));
        }
      this->owner_.replica_seq_num_ = seq_num;
    }
  else if (expected > seq_num)
    {
      if (TAO_debug_level > 0)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) FT_Update_Replicant_i::notify_updated_entity ")
                          ACE_TEXT ("expected %Lu got %Lu\n"),
                          expected, seq_num ));
        }
      --this->owner_.replica_seq_num_;
    }
  this->owner_.server_.update_info (info);
}

void
FT_Update_Replicant_i::register_peer (FT_Naming::UpdatePushNotifier_ptr peer,
                                      const FT_Naming::ReplicaInfo& iors,
                                      CORBA::ULongLong seq_num)
{
  this->owner_.peer_ = FT_Naming::UpdatePushNotifier::_duplicate (peer);
  this->owner_.replica_seq_num_ = seq_num;
  this->owner_.server_.update_iors (iors);
}

//---------------------------------------------------------------------------

TAO_FT_Naming_Replication_Manager::TAO_FT_Naming_Replication_Manager (TAO_FT_Naming_Server &owner)
  : me_ (),
    peer_ (),
    seq_num_ (0),
    replica_seq_num_ (0),
    server_ (owner),
    orb_ (),
    reactor_ (0),
    lock_ (),
    notified_ (false),
    to_send_ (10),
    endpoint_ (owner.ft_endpoint ()),
    update_delay_ (owner.ft_update_delay ()),
    replica_ior_ (),
    send_combos_ (false),
    refresh_peer_ (false)
{
}

TAO_FT_Naming_Replication_Manager::~TAO_FT_Naming_Replication_Manager(void)
{
}

void
TAO_FT_Naming_Replication_Manager::init_orb (void)
{
  int argc = 8;
  ACE_TCHAR *argv[11];
  argv[0] = ACE_OS::strdup (ACE_TEXT (""));
  argv[1] = ACE_OS::strdup (ACE_TEXT ("-ORBIgnoreDefaultSvcConfFile"));
  argv[2] = ACE_OS::strdup (ACE_TEXT ("-ORBGestalt"));
  argv[3] = ACE_OS::strdup (ACE_TEXT ("Local"));
  argv[4] = ACE_OS::strdup (ACE_TEXT ("-ORBSvcConfDirective"));
  argv[5] = ACE_OS::strdup (ACE_TEXT ("static Client_Strategy_Factory \"-ORBConnectStrategy Blocked -ORBDefaultSyncScope server\""));
  argv[6] = ACE_OS::strdup (ACE_TEXT ("-ORBDottedDecimalAddresses"));
  argv[7] = ACE_OS::strdup (ACE_TEXT ("1"));
  if (endpoint_.length ())
    {
      argv[8] = ACE_OS::strdup (ACE_TEXT ("-ORBListenEndpoints"));
      argv[9] = ACE_OS::strdup (ACE_TEXT_CHAR_TO_TCHAR (endpoint_.c_str ()));
      argc += 2;
    }
  else
    {
      argv[8] = 0;
      argv[9] = 0;
    }
  argv[10] = 0;

  this->orb_ = CORBA::ORB_init (argc, argv, "replicator_orb");
  this->reactor_ = this->orb_->orb_core ()->reactor ();
  CORBA::Object_var obj = this->orb_->resolve_initial_references ("RootPOA");
  PortableServer::POA_var poa = PortableServer::POA::_narrow (obj.in ());
  PortableServer::ServantBase_var servant;
  ACE_NEW (servant, FT_Update_Replicant_i (*this));
  PortableServer::ObjectId_var oid = poa->activate_object (servant.in ());
  obj = poa->id_to_reference (oid.in ());
  this->me_ =
    FT_Naming::UpdatePushNotifier::_narrow (obj.in ());
  PortableServer::POAManager_var mgr = poa->the_POAManager ();
  mgr->activate ();
}

char *
TAO_FT_Naming_Replication_Manager::ior (void)
{
  return this->orb_->object_to_string (this->me_.in ());
}

bool
TAO_FT_Naming_Replication_Manager::peer_available (void)
{
  return !CORBA::is_nil (this->peer_.in ());
}

void
TAO_FT_Naming_Replication_Manager::stop (void)
{
  if (this->reactor_ == 0)
    {
      return;
    }
  this->orb_->shutdown (true);
  this->wait();
}

int
TAO_FT_Naming_Replication_Manager::svc (void)
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
      if (TAO_debug_level > 0)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                         ACE_TEXT ("(%P|%t) TAO_FT_Naming_Replication_Manager::svc caught %C\n"),
                          ex._name ()));
        }
    }
  return 0;
}


int
TAO_FT_Naming_Replication_Manager::handle_exception (ACE_HANDLE )
{
  ACE_OS::sleep (this->update_delay_);
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, mon, this->lock_, -1);
  if (!this->peer_available())
    {
      mon.release ();
      if (!this->peer_init_i ())
        {
          this->server_.no_replica();
        }
      mon.acquire();
      this->notified_ = false;
      return 0;
    }
  if (this->refresh_peer_)
    {
      this->refresh_peer_ = false;
      mon.release ();
      if (!this->update_peer_registration())
        {
          mon.acquire ();
          this->notified_ = false;
          return 0;
        }
      mon.acquire ();
    }

  if (this->to_send_.length () == 0)
    {
      this->notified_ = false;
      return 0;
    }
  if (CORBA::is_nil (this->peer_))
    {
      this->to_send_.length (0);
      this->notified_ = false;
      return 0;
    }
  try
    {
      CORBA::Long len = this->to_send_.length ();
      FT_Naming::UpdateInfoSeq payload (len);
      payload.length (len);
      CORBA::Long p = 0;
      for (CORBA::Long l = 0; l < len; l++)
        {
           payload[p++] = this->to_send_[l];
        }
      payload.length (p);
      this->to_send_.length (0);
      CORBA::ULongLong seq = ++this->seq_num_;
      mon.release ();

      this->peer_->notify_update (seq, payload);
    }
  catch (const CORBA::Exception &)
    {
      this->peer_ = FT_Naming::UpdatePushNotifier::_nil ();
    }
  this->notified_ = false;
  return 0;
}

void
TAO_FT_Naming_Replication_Manager::send_objgrp_update (PortableGroup::ObjectGroupId id,
                                   FT_Naming::ChangeType change)
{
  if (this->reactor_ == 0)
    {
      return;
    }
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
  CORBA::Long len = this->to_send_.length ();
  bool found = false;
  for (CORBA::Long i = len -1; !found && i >= 0; i--)
    {
      if (this->to_send_[i]._d() == FT_Naming::obj_grp &&
          this->to_send_[i].grp().id == id)
        {
          found = true;
          FT_Naming::ChangeType pending = this->to_send_[i].grp().change_type;
          if (pending == FT_Naming::DELETED)
            {
              if (change == FT_Naming::UPDATED)
                {
                // shouldn't have an update after a deleted
                  change = FT_Naming::DELETED;
                }
              else if (change == FT_Naming::NEW)
                {// put both in cueue
                  found = false;
                  break;
                }
            }
          this->to_send_[i].grp().change_type = change;
        }
    }
  if (!found)
    {
      this->to_send_.length (len+1);
      FT_Naming::ObjectGroupUpdate ogu;
      ogu.id = id;
      ogu.change_type = change;
      this->to_send_[len].grp(ogu);
    }
  if (!this->notified_)
    {
      this->notified_ = true;
      this->reactor_->notify (this);
    }
}

void
TAO_FT_Naming_Replication_Manager::send_context_update (const ACE_CString & name,
                                    FT_Naming::ChangeType change)
{
  if (this->reactor_ == 0)
    {
      return;
    }

  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
  CORBA::Long len = this->to_send_.length ();
  bool found = false;

  for (CORBA::Long i = len -1; !found && i >= 0; i--)
    {
      if (this->to_send_[i]._d() == FT_Naming::ctx_name &&
          ACE_OS::strcmp (this->to_send_[i].ctx().context_name,
                          name.c_str()) == 0)
        {
          found = true;
          FT_Naming::ChangeType pending = this->to_send_[i].ctx().change_type;
          if (pending == FT_Naming::DELETED)
            {
              if (change == FT_Naming::UPDATED)
                {
                // shouldn't have an update after a deleted
                  change = FT_Naming::DELETED;
                }
              else if (change == FT_Naming::NEW)
                {// put both in cueue
                  found = false;
                  break;
                }
            }
          this->to_send_[i].ctx().change_type = change;
        }
    }

  if (!found)
    {
      this->to_send_.length (len+1);
      FT_Naming::NamingContextUpdate ncu;
      ncu.context_name = CORBA::string_dup (name.c_str());
      ncu.change_type = change;
      this->to_send_[len].ctx(ncu);
    }
  if (!this->notified_)
    {
      this->notified_ = true;
      this->reactor_->notify (this);
    }
}

void
TAO_FT_Naming_Replication_Manager::send_registration (bool use_combo_refs)
{

  if (TAO_debug_level > 1)
    {
      ORBSVCS_DEBUG((LM_INFO,
        ACE_TEXT("(%P|%t) Registering with peer naming server \n")));
    }
  if (this->reactor_ == 0)
    {
      return;
    }

  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
  this->send_combos_ = use_combo_refs;
  if (!this->notified_)
    {
      this->refresh_peer_ = true;
      this->notified_ = true;
      this->reactor_->notify (this);
    }
}

bool
TAO_FT_Naming_Replication_Manager::peer_init_i (void)
{
  CORBA::Object_var obj = this->orb_->string_to_object (replica_ior_.c_str());
  bool non_exist = true;
  bool was_nil = CORBA::is_nil (this->peer_.in());
  if (!CORBA::is_nil (obj.in ()))
    {
      try
        {
          this->peer_ = FT_Naming::UpdatePushNotifier::_narrow (obj.in());
          non_exist = (this->peer_->_non_existent() == 1);
        }
      catch (const CORBA::Exception& ex)
        {
          if (TAO_debug_level > 0)
            {
              ORBSVCS_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("(%P|%t) FT_Repl_mgr::peer_init_i ")
                              ACE_TEXT ("caught %C\n"), ex._name()));
            }
        }
    }
  if (non_exist)
    {
      this->peer_ = FT_Naming::UpdatePushNotifier::_nil();
      return was_nil;
    }

  if (this->peer_available ())
    {
      if (!this->update_peer_registration ())
        {
          this->peer_ = FT_Naming::UpdatePushNotifier::_nil ();
          return was_nil;
        }
    }
  else
    {
      return was_nil;
    }

  if (this->to_send_.length () > 0)
    {
      this->notified_ = true;
      this->reactor_->notify (this);
    }
  return true;
}

bool
TAO_FT_Naming_Replication_Manager::update_peer_registration (void)
{
  FT_Naming::ReplicaInfo info;
  info.combined = this->send_combos_;
  if (this->send_combos_)
    {
      info.root_context = this->server_.ft_root_context ();
      info.naming_manager = this->server_.ft_naming_manager ();
      this->send_combos_ = false;
    }
  else
    {
      info.root_context = this->server_.my_root_context ();
      info.naming_manager = this->server_.my_naming_manager ();
    }
  bool success = false;
  try
    {
      this->peer_->register_peer (this->me_, info, this->seq_num_);
      success = true;
    }
  catch (const CORBA::Exception &)
    {
    }
  return success;
}


bool
TAO_FT_Naming_Replication_Manager::init_peer (void)
{
  ACE_TString replica_ior_file = this->server_.replica_ior_filename (true);

  if (TAO_debug_level > 1)
    {
      ORBSVCS_DEBUG ((LM_INFO,
                      ACE_TEXT("Resolving Naming replica from file %C\n"),
                      replica_ior_file.c_str()));
    }

  if (ACE_OS::access (replica_ior_file.c_str (), F_OK) != 0)
    {
      this->peer_ =
        FT_Naming::UpdatePushNotifier::_nil();
      return false;
    }

  this->replica_ior_ = ACE_CString ("file://") +
#if defined (ACE_USES_WCHAR)
    ACE_CString (ACE_TEXT_ALWAYS_CHAR (replica_ior_file.c_str()));
#else
    replica_ior_file;
#endif /* ACE_USES_WCHAR */
  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL
