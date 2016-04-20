#include "orbsvcs/Log_Macros.h"
#include "ImR_Locator_i.h"
#include "utils.h"
#include "Iterator.h"
#include "INS_Locator.h"
#include "UpdateableServerInfo.h"

#include "Locator_Repository.h"
#include "Config_Backing_Store.h"
#include "Shared_Backing_Store.h"
#include "XML_Backing_Store.h"

#include "orbsvcs/Time_Utilities.h"

#include "tao/Messaging/Messaging.h"
#include "tao/AnyTypeCode/Any.h"
#include "tao/ORB_Core.h"

#include "ace/ARGV.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/Vector_T.h"
#include "ace/Task.h"

class ORB_Runner : public ACE_Task_Base
{
public:
  ORB_Runner (CORBA::ORB_ptr o)
    : orb_ (CORBA::ORB::_duplicate (o))
  { }

  int svc (void)
  {
    this->orb_->run ();
    return 0;
  }
private:
  CORBA::ORB_var orb_;
};

/// We want to give shutdown a little more time to work, so that we
/// can guarantee to the tao_imr utility that it has shutdown. The tao_imr
/// utility prints a different message depending on whether shutdown succeeds
/// or times out.
static const ACE_Time_Value DEFAULT_SHUTDOWN_TIMEOUT (0, 5000 * 1000);

static PortableServer::POA_ptr
createPersistentPOA (PortableServer::POA_ptr root_poa, const char* poa_name) {

  PortableServer::LifespanPolicy_var life =
    root_poa->create_lifespan_policy (PortableServer::PERSISTENT);

  PortableServer::IdAssignmentPolicy_var assign =
    root_poa->create_id_assignment_policy (PortableServer::USER_ID);

  CORBA::PolicyList pols;
  pols.length (2);
  pols[0] = PortableServer::LifespanPolicy::_duplicate (life.in ());
  pols[1] = PortableServer::IdAssignmentPolicy::_duplicate (assign.in ());

  PortableServer::POAManager_var mgr = root_poa->the_POAManager ();
  PortableServer::POA_var poa =
    root_poa->create_POA (poa_name, mgr.in (), pols);

  life->destroy ();
  assign->destroy ();

  return poa._retn ();
}

int ImR_Locator_i::debug_ = 0;

ImR_Locator_i::ImR_Locator_i (void)
  : dsi_forwarder_ (*this)
  , ins_locator_ (0)
  , aam_active_ ()
  , aam_terminating_ ()
  , opts_ (0)
  , lock_ ()
  , shutdown_handler_ (this)
{
  // Visual C++ 6.0 is not smart enough to do a direct assignment
  // while allocating the INS_Locator.  So, we have to do it in
  // two steps.
  INS_Locator* locator;
  ACE_NEW (locator, INS_Locator (*this));
  ins_locator_ = locator;
}

ImR_Locator_i::~ImR_Locator_i (void)
{
}

int
ImR_Locator_i::init_with_orb (CORBA::ORB_ptr orb)
{
  this->orb_ = CORBA::ORB::_duplicate (orb);
  ImR_Locator_i::debug_ = this->opts_->debug ();
  CORBA::Object_var obj =
    this->orb_->resolve_initial_references ("RootPOA");
  this->root_poa_ = PortableServer::POA::_narrow (obj.in ());
  ACE_ASSERT (! CORBA::is_nil (this->root_poa_.in ()));

  this->dsi_forwarder_.init (orb);
  this->adapter_.init (& this->dsi_forwarder_);
  this->pinger_.init (orb, this->opts_->ping_interval ());

  this->opts_->pinger (&this->pinger_);

  // Register the Adapter_Activator reference to be the RootPOA's
  // Adapter Activator.
  root_poa_->the_activator (&this->adapter_);

  // Use a persistent POA so that any IOR
  this->imr_poa_ = createPersistentPOA (this->root_poa_.in (),
                                        "ImplRepo_Service");
  ACE_ASSERT (! CORBA::is_nil (this->imr_poa_.in ()));

  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId ("ImplRepo_Service");
  this->imr_poa_->activate_object_with_id (id.in (), this);

  obj = this->imr_poa_->id_to_reference (id.in ());
  ImplementationRepository::Locator_var locator =
    ImplementationRepository::Locator::_narrow (obj.in ());
  ACE_ASSERT(! CORBA::is_nil (locator.in ()));
  const CORBA::String_var ior = this->orb_->object_to_string (obj.in ());

  // create the selected Locator_Repository with backing store
  switch (this->opts_->repository_mode ())
    {
    case Options::REPO_REGISTRY:
      {
        repository_.reset(new Registry_Backing_Store(*this->opts_, orb));
        break;
      }
    case Options::REPO_HEAP_FILE:
      {
        repository_.reset(new Heap_Backing_Store(*this->opts_, orb));
        break;
      }
    case Options::REPO_XML_FILE:
      {
        repository_.reset(new XML_Backing_Store(*this->opts_, orb));
        break;
      }
    case Options::REPO_SHARED_FILES:
      {
        repository_.reset(new Shared_Backing_Store(*this->opts_, orb, this));
        break;
      }
    case Options::REPO_NONE:
      {
        repository_.reset(new No_Backing_Store(*this->opts_, orb));
        break;
      }
    default:
      {
        bool invalid_rmode_specified = false;
        ACE_ASSERT (invalid_rmode_specified);
        ACE_UNUSED_ARG (invalid_rmode_specified);
        ORBSVCS_ERROR_RETURN ((
                               LM_ERROR, ACE_TEXT ("Repository failed to initialize\n")), -1);
      }
    }

  // Register the ImR for use with INS
  obj = orb->resolve_initial_references ("AsyncIORTable");
  IORTable::Table_var ior_table = IORTable::Table::_narrow (obj.in ());
  ACE_ASSERT (! CORBA::is_nil (ior_table.in ()));
  ior_table->set_locator (this->ins_locator_.in ());

  // Initialize the repository. This will load any values that
  // may have been persisted before.
  int result = this->repository_->init(this->root_poa_.in (),
                                       this->imr_poa_.in (),
                                       ior);
  if (result != 0)
    {
      return result;
    }

  Locator_Repository::SIMap::ENTRY* entry = 0;
  Locator_Repository::SIMap::ITERATOR it (this->repository_->servers ());

  for (;it.next (entry) != 0; it.advance ())
    {
      UpdateableServerInfo info (this->repository_, entry->int_id_);
      bool const is_alive = this->server_is_alive (info);
      Server_Info *active = info.edit()->active_info ();
      if (this->debug_ > 0)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("server <%C> is_alive = %d\n"),
                          active->ping_id(), is_alive));
        }

      if (!is_alive)
        {
          // We have read an existing configuration from the repository
          // and when a server is not alive we reset it, it could have
          // been shutdown when we where offline
          this->pinger_.remove_server (active->ping_id());
          info.edit()->reset_runtime ();
          active->reset_runtime ();
          continue;
        }

      active->death_notify = false;
      if (active->pid > 0)
        {
          Activator_Info_Ptr ainfo =
            this->get_activator (active->activator);

          if (!(ainfo.null () || CORBA::is_nil (ainfo->activator.in ())))
            {
              ImplementationRepository::ActivatorExt_var actx =
                ImplementationRepository::ActivatorExt::_narrow (ainfo->activator.in ());
              try
                {
                  active->death_notify =
                    !CORBA::is_nil (actx.in ()) && actx->still_alive (active->pid);
                }
              catch (const CORBA::Exception &)
                {
                }
              if (this->debug_ > 0)
                {
                  ORBSVCS_DEBUG ((LM_DEBUG,
                                  ACE_TEXT ("activator says death_notify = %d\n"),
                                  active->death_notify));
                }
            }
        }
    }

  // Only after verifying do we report the IOR and become open for business
  return this->repository_->report_ior(this->imr_poa_.in ());
}

int
ImR_Locator_i::init (Options& opts)
{
  this->opts_ = &opts;
  ACE_CString cmdline = opts.cmdline ();
  cmdline += " -orbuseimr 0";
  ACE_ARGV av (ACE_TEXT_CHAR_TO_TCHAR (cmdline.c_str ()));
  int argc = av.argc ();
  ACE_TCHAR** argv = av.argv ();

  CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, "TAO_ImR_Locator");
  int err = this->init_with_orb (orb.in ());
  return err;
}

int
ImR_Locator_i::run (void)
{
  if (debug_ > 0)
    {
      // This debug message was split into two calls to
      // work around yet another bug in Visual Studio 2005.
      // When this was a single debug message, the value for
      // debug () came out garbled and the read-only string
      // caused an ACCESS VIOLATION -- Chad Elliott 10/4/2006
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("Implementation Repository: Running\n")
                      ACE_TEXT ("\tPing Interval : %dms\n")
                      ACE_TEXT ("\tStartup Timeout : %ds\n")
                      ACE_TEXT ("\tPersistence : %s\n")
                      ACE_TEXT ("\tMulticast : %C\n")
                      ACE_TEXT ("\tThreads : %d\n"),
                      this->opts_->ping_interval ().msec (),
                      this->opts_->startup_timeout ().sec (),
                      this->repository_->repo_mode (),
                      (this->repository_->multicast () != 0 ?
                       "Enabled" : "Disabled"),
                      this->opts_->threads () ));
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("\tDebug : %d\n")
                      ACE_TEXT ("\tReadOnly : %C\n\n"),
                      debug (),
                      (this->opts_->readonly () ? "True" : "False")));
    }
  this->auto_start_servers ();

  if (true /* this->opts_->threads () == 1 */)
    {
      this->orb_->run ();
    }
  else
    {
      ORB_Runner runner (this->orb_);
      runner.activate (THR_NEW_LWP | THR_JOINABLE, this->opts_->threads ());
      runner.wait ();
    }
  return 0;
}

void
ImR_Locator_i::shutdown
(ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
 CORBA::Boolean activators, CORBA::Boolean servers)
{
  this->pinger_.shutdown ();
  this->aam_active_.reset ();
  this->aam_terminating_.reset ();
  if (servers != 0 && this->repository_->servers ().current_size () > 0)
    {
      // Note : shutdown is oneway, so we can't throw
      ORBSVCS_ERROR ((
                      LM_ERROR,
                      ACE_TEXT ("(%P|%t) ImR: Shutdown of all servers not implemented.\n")));
    }
  if (activators != 0 && this->repository_->activators ().current_size () > 0)
    {
      ACE_Vector<ImplementationRepository::Activator_var> acts;
      Locator_Repository::AIMap::ENTRY* entry = 0;
      Locator_Repository::AIMap::ITERATOR it (this->repository_->activators ());
      for (;it.next (entry) != 0; it.advance ())
        {
          Activator_Info_Ptr info = entry->int_id_;
          ACE_ASSERT (! info.null ());
          this->connect_activator (*info);
          if (! CORBA::is_nil (info->activator.in ()))
            acts.push_back (info->activator);
        }

      int shutdown_errs = 0;

      for (size_t i = 0; i < acts.size (); ++i)
        {
          try
            {
              acts[i]->shutdown ();
              acts[i] = ImplementationRepository::Activator::_nil ();
            }
          catch (const CORBA::Exception& ex)
            {
              ++shutdown_errs;
              if (debug_ > 1)
                {
                  ex._tao_print_exception (
                                           ACE_TEXT ("(%P|%t) ImR: shutdown activator"));
                }
            }
        }
      if (debug_ > 0 && shutdown_errs > 0)
        {
          ORBSVCS_DEBUG (( LM_DEBUG,
                           ACE_TEXT ("(%P|%t) ImR: Some activators could not be shut down.\n")));
        }
    }
  // Technically, we should wait for all the activators to unregister, but
  // ,for now at least, it doesn't seem worth it.
  this->shutdown (false);

  _tao_rh->shutdown ();
}

int
ImR_Locator_i::Shutdown_Handler::handle_exception (ACE_HANDLE)
{
  this->owner_->shutdown (false);
  return 0;
}

void
ImR_Locator_i::signal_shutdown ()
{
  this->orb_->orb_core ()->reactor ()->notify (&this->shutdown_handler_);
}

void
ImR_Locator_i::shutdown (bool wait_for_completion)
{
  this->repository_->shutdown ();
  this->orb_->shutdown (wait_for_completion);
}

const Options *
ImR_Locator_i::opts (void) const
{
  return this->opts_;
}

int
ImR_Locator_i::fini (void)
{
  try
    {
      if (debug_ > 1)
        ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) ImR: Shutting down...\n")));

      this->root_poa_->destroy (1, 1);

      this->orb_->destroy ();

      if (debug_ > 0)
        ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) ImR: Shut down successfully.\n")));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("ImR_Locator_i::fini"));
      throw;
    }
  return 0;
}

void
ImR_Locator_i::register_activator
(ImplementationRepository::AMH_LocatorResponseHandler_ptr _tao_rh,
 const char* aname,
 ImplementationRepository::Activator_ptr activator)
{
  ACE_ASSERT (aname != 0);
  ACE_ASSERT (! CORBA::is_nil (activator));

  // Before we can register the activator, we need to ensure that any existing
  // registration is purged.
  this->unregister_activator_i (aname);

  CORBA::String_var ior =
    this->orb_->object_to_string (activator);

  CORBA::Long token = ACE_OS::gettimeofday ().msec ();

  int err = this->repository_->add_activator (aname, token, ior.in (), activator);
  ACE_ASSERT (err == 0);
  ACE_UNUSED_ARG (err);

  if (debug_ > 0)
    ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) ImR: Activator registered for %C.\n"),
                    aname));

  _tao_rh->register_activator (token);
}

void
ImR_Locator_i::unregister_activator
(ImplementationRepository::AMH_LocatorResponseHandler_ptr _tao_rh,
 const char* aname,
 CORBA::Long token)
{
  ACE_ASSERT (aname != 0);
  Activator_Info_Ptr info = this->get_activator (aname);

  if (! info.null ())
    {
      if (info->token != token && debug_ > 0)
        {
          ORBSVCS_DEBUG ((
                          LM_DEBUG,
                          ACE_TEXT ("(%P|%t) ImR: Ignoring unregister activator:%C. Wrong token.\n"),
                          aname));
          _tao_rh->unregister_activator ();
          return;
        }

      this->unregister_activator_i (aname);

      if (debug_ > 0)
        ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) ImR: Activator %C unregistered.\n"),
                        aname));
    }
  else
    {
      if (debug_ > 0)
        ORBSVCS_DEBUG ((
                        LM_DEBUG,
                        ACE_TEXT ("(%P|%t) ImR: Ignoring unregister activator: %C. ")
                        ACE_TEXT ("Unknown activator.\n"),
                        aname));
    }
  _tao_rh->unregister_activator ();
}

void
ImR_Locator_i::unregister_activator_i (const char* aname)
{
  int err = this->repository_->remove_activator (aname);
  ACE_UNUSED_ARG (err);
}

void
ImR_Locator_i::remote_access_update (const char *name,
                                     ImplementationRepository::AAM_Status state)
{
  /*  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_); */
  AsyncAccessManager_ptr aam (this->find_aam (name));
  if (aam.is_nil())
    {
      UpdateableServerInfo info (this->repository_, name);
      if (info.null ())
        {
          if (debug_ > 0)
            ORBSVCS_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("(%P|%t) ImR: remote_acccess")
                            ACE_TEXT (" <%C> unregistered.\n"),
                            name));
          return;
        }
      aam = this->create_aam (info);
    }
  /* mon.release (); */
  aam->remote_state (state);
}

void
ImR_Locator_i::child_death_i (const char* name, int pid)
{
  if (debug_ > 1)
    ORBSVCS_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) ImR: Server[%d] has died <%C>.\n"),
                    pid, name));

  this->pinger_.remove_server (name, pid);
  AsyncAccessManager_ptr aam (this->find_aam (name, false));
  bool terminated = !aam.is_nil () && aam->notify_child_death (pid);
  aam = this->find_aam (name, true);
  if (!terminated && !aam.is_nil ())
    {
      terminated = aam->notify_child_death (pid);
    }
  UpdateableServerInfo info(this->repository_, name, pid);
  if (! info.null ())
    {
      info.edit ()->reset_runtime ();
    }
  else
    {
      if (debug_ > 1)
        ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%P|%t) ImR: Failed to find server/pid in repository.\n")));
    }

}

void
ImR_Locator_i::notify_child_death
(ImplementationRepository::AMH_LocatorResponseHandler_ptr _tao_rh,
 const char* name)
{
  this->child_death_i (name, 0);
  _tao_rh->notify_child_death ();
}

void
ImR_Locator_i::child_death_pid
(ImplementationRepository::AMH_LocatorResponseHandler_ptr _tao_rh,
 const char* name, CORBA::Long pid)
{
  this->child_death_i (name, pid);
  _tao_rh->child_death_pid ();
}

void
ImR_Locator_i::spawn_pid
(ImplementationRepository::AMH_LocatorResponseHandler_ptr _tao_rh,
 const char* name, CORBA::Long pid)
{
  if (debug_ > 1)
    ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) ImR: Server[%d] spawned <%C>.\n"),
                    pid, name));

  UpdateableServerInfo info(this->repository_, name);
  if (! info.null ())
    {
      if (debug_ > 4)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) ImR: Spawn_pid prev pid was %d becoming %d\n"),
                          info.edit ()->active_info ()->pid, pid));
        }

      AsyncAccessManager_ptr aam (this->find_aam (name, true));
      if (aam.is_nil ())
        {
          aam = this->find_aam (name, false);
        }
      else
        {
          aam->update_prev_pid ();
        }
      info.edit ()->active_info ()->pid = pid;
      info.edit ()->active_info ()->death_notify = true;
    }
  else
    {
      if (debug_ > 1)
        ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%P|%t) ImR: Failed to find server in repository.\n")));
    }
  this->pinger_.set_pid (name, pid);

  _tao_rh->spawn_pid ();
}

char*
ImR_Locator_i::activate_server_by_name (const char* name, bool manual_start)
{
  ImR_SyncResponseHandler rh ("", this->orb_.in());
  this->activate_server_by_name (name, manual_start, &rh);
  return rh.wait_for_result ();
}

char*
ImR_Locator_i::activate_server_by_object (const char* object_name)
{
  Server_Info_Ptr si;
  ACE_CString key;
  ACE_CString full (object_name);
  if (this->split_key (full, key, si))
    {
      ImR_SyncResponseHandler rh (key.c_str(), this->orb_.in());
      this->activate_server_by_info (si, &rh);
      return rh.wait_for_result ();
    }
  throw ImplementationRepository::NotFound();
}

void
ImR_Locator_i::activate_server
(ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
 const char* server)
{
  if (debug_ > 1)
    {
      ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) ImR: Manually activating server <%C>\n"),
                      server));
    }

  ImR_ResponseHandler *rh = 0;
  ACE_NEW (rh,
           ImR_Loc_ResponseHandler (ImR_Loc_ResponseHandler::LOC_ACTIVATE_SERVER,
                                    _tao_rh));

  // This is the version called by tao_imr to activate the server, manually
  // starting it if necessary.
  activate_server_by_name (server, true, rh);
}

bool
ImR_Locator_i::get_info_for_name (const char* name, Server_Info_Ptr &si)
{
  si = this->repository_->get_active_server (name);
  return !si.null();
}

bool
ImR_Locator_i::split_key (ACE_CString &full, ACE_CString &key, Server_Info_Ptr &si)
{
  key = full;
  if (this->get_info_for_name (full.c_str(), si))
    {
      return true;
    }

  ACE_CString::size_type pos = full.rfind ('/');
  while (pos != ACE_CString::npos)
    {
      ACE_CString server = full.substring (0, pos);
      if (this->get_info_for_name (server.c_str (), si))
        {
          return true;
        }
      pos = server.rfind ('/');
    }

  return false;
}

void
ImR_Locator_i::activate_server_by_name (const char* name, bool manual_start,
                                        ImR_ResponseHandler *rh)
{
  // Activate the server, starting it if necessary. Don't start MANUAL
  // servers unless manual_start=true
  UpdateableServerInfo info (this->repository_, name);
  if (info.null ())
    {
      rh->send_exception ( new ImplementationRepository::NotFound );
    }
  else
    {
      this->activate_server_i (info, manual_start, rh);
    }
}

void
ImR_Locator_i::activate_server_by_info (const Server_Info_Ptr &si,
                                        ImR_ResponseHandler *rh)
{
  UpdateableServerInfo info (this->repository_, si, true);
  this->activate_server_i (info, false, rh);
}

void
ImR_Locator_i::activate_server_i (UpdateableServerInfo& info,
                                  bool manual_start,
                                  ImR_ResponseHandler *rh)
{
  AsyncAccessManager_ptr aam;
  if (info->is_mode(ImplementationRepository::PER_CLIENT))
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
      aam = this->create_aam (info);
    }
  else
    {
      aam = this->find_aam (info->ping_id ());
      if (aam.is_nil())
        {
          ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
          aam = this->create_aam (info);
        }
    }
  aam->add_interest (rh, manual_start);
}

CORBA::Object_ptr
ImR_Locator_i::set_timeout_policy (CORBA::Object_ptr obj, const ACE_Time_Value& to)
{
  CORBA::Object_var ret (CORBA::Object::_duplicate (obj));

  try
    {
      TimeBase::TimeT timeout;
      ORBSVCS_Time::Time_Value_to_TimeT (timeout, to);
      CORBA::Any tmp;
      tmp <<= timeout;

      CORBA::PolicyList policies (1);
      policies.length (1);
      policies[0] = orb_->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                                         tmp);

      ret = obj->_set_policy_overrides (policies, CORBA::ADD_OVERRIDE);

      policies[0]->destroy ();

      if (CORBA::is_nil (ret.in ()))
        {
          if (debug_ > 0)
            {
              ORBSVCS_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("(%P|%t) ImR: Unable to set timeout policy.\n")));
            }
          ret = CORBA::Object::_duplicate (obj);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
                               ACE_TEXT ("(%P|%t) ImR_Locator_i::set_timeout_policy ()"));
    }

  return ret._retn ();
}

void
ImR_Locator_i::add_or_update_server
(ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
 const char* server,
 const ImplementationRepository::StartupOptions &options)
{
  if (this->opts_->readonly ())
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) ImR: Cannot add/update server <%C> due to locked ")
                      ACE_TEXT ("database.\n"),
                      server));
      CORBA::Exception *ex =
        new CORBA::NO_PERMISSION (CORBA::SystemException::_tao_minor_code
                                  (TAO_IMPLREPO_MINOR_CODE,0),
                                  CORBA::COMPLETED_NO);
      ImplementationRepository::AMH_AdministrationExceptionHolder h (ex);
      _tao_rh->add_or_update_server_excep (&h);
      return;
    }

  if (debug_ > 0)
    ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) ImR: Add/Update server <%C>.\n"), server));

  UpdateableServerInfo info(this->repository_, server);
  if (info.null ())
    {
      if (debug_ > 1)
        ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) ImR: Adding server <%C>.\n"), server));

      this->repository_->add_server (server, options);
    }
  else
    {
      if (debug_ > 1)
        ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) ImR: Updating server <%C>.\n"),
                        server));

      info.edit ()->update_options (options);
      info.update_repo();
    }

  if (debug_ > 1)
    {
      // Note : The info var may be null, so we use options.
      ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) ImR: Server: %C\n")
                      ACE_TEXT ("\tActivator: %C\n")
                      ACE_TEXT ("\tCommand Line: %C\n")
                      ACE_TEXT ("\tWorking Directory: %C\n")
                      ACE_TEXT ("\tActivation: %C\n")
                      ACE_TEXT ("\tStart Limit: %d\n")
                      ACE_TEXT ("\n"),
                      server,
                      options.activator.in (),
                      options.command_line.in (),
                      options.working_directory.in (),
                      ImR_Utils::activationModeToString (options.activation),
                      options.start_limit
                      ));

      for (CORBA::ULong i = 0; i < options.environment.length (); ++i)
        ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("Environment variable %C=%C\n"),
                        options.environment[i].name.in (),
                        options.environment[i].value.in ()));
    }

  _tao_rh->add_or_update_server ();
}

void
ImR_Locator_i::link_servers
(ImplementationRepository::AMH_AdministrationExtResponseHandler_ptr _tao_rh,
 const char * name,
 const CORBA::StringSeq & peers)
{
  Server_Info_Ptr root_si = this->repository_->get_active_server (name);
  if (root_si.null())
    {
      CORBA::Exception *ex =
        new ImplementationRepository::NotFound;
      ImplementationRepository::AMH_AdministrationExtExceptionHolder h (ex);
      _tao_rh->link_servers_excep (&h);
      return;
    }
  else if (!root_si->alt_info_.null())
    {
      ACE_CString errstr = name;
      errstr += " is not a base POA";
      CORBA::Exception *ex =
        new ImplementationRepository::CannotComplete (errstr.c_str());
      ImplementationRepository::AMH_AdministrationExtExceptionHolder h (ex);
      _tao_rh->link_servers_excep (&h);
      return;
    }

  for (CORBA::ULong i = 0; i < peers.length(); i++)
    {
      ACE_CString peer(peers[i]);
      Server_Info_Ptr si = this->repository_->get_active_server (peer);
      if (!si.null ())
        {
          ACE_CString errstr(peers[i]);
          errstr += " is already registered";
          CORBA::Exception *ex =
            new ImplementationRepository::CannotComplete (errstr.c_str());
          ImplementationRepository::AMH_AdministrationExtExceptionHolder h (ex);
          _tao_rh->link_servers_excep (&h);
          return;
        }
    }

  this->repository_->link_peers (root_si, peers);

  _tao_rh->link_servers ();
  return;
}

bool
ImR_Locator_i::kill_server_i (const Server_Info_Ptr &si,
                              CORBA::Short signum,
                              CORBA::Exception *& ex)
{
  if (si->is_mode(ImplementationRepository::PER_CLIENT))
    {
      ex = new ImplementationRepository::CannotComplete ("per-client server");
      return false;
    }

  Activator_Info_Ptr ainfo = this->get_activator (si->activator);
  if (ainfo.null ())
    {
      ex = new ImplementationRepository::CannotComplete ("no activator");
      return false;
    }

  ImplementationRepository::ActivatorExt_var actext =
    ImplementationRepository::ActivatorExt::_narrow (ainfo->activator.in());

  if (CORBA::is_nil (actext.in()))
    {
      ex = new ImplementationRepository::CannotComplete ("activator incompatible");
      return false;
    }
  return actext->kill_server (si->key_name_.c_str(), si->pid, signum);
}

void
ImR_Locator_i::kill_server
(ImplementationRepository::AMH_AdministrationExtResponseHandler_ptr _tao_rh,
 const char * name, CORBA::Short signum)
{
  CORBA::Exception *ex = 0;
  Server_Info_Ptr si;
  if (!this->get_info_for_name (name, si))
    {
      ex = new ImplementationRepository::NotFound;
    }
  else
    {
      if (!si->alt_info_.null ())
        {
          si = si->alt_info_;
        }
      if (!this->kill_server_i (si, signum, ex))
        {
          if (ex == 0)
            {
              ex = new ImplementationRepository::CannotComplete ("server not running");
            }
        }
    }
  if (ex != 0)
    {
      ImplementationRepository::AMH_AdministrationExtExceptionHolder h (ex);
      _tao_rh->kill_server_excep (&h);
      return;
    }

  _tao_rh->kill_server ();
  AsyncAccessManager_ptr aam = this->find_aam (si->key_name_.c_str ());
  if (!aam.is_nil ())
    {
      aam->shutdown_initiated ();
    }
}

void
ImR_Locator_i::remove_server_i (const Server_Info_Ptr &info)
{
  if (debug_ > 1)
    ORBSVCS_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) ImR: Removing Server <%C>...\n"),
                    info->key_name_.c_str()));

  ACE_CString poa_name = info->poa_name;
  if (this->repository_->remove_server (info->key_name_, this) == 0)
    {
      this->destroy_poa (poa_name);
      if (debug_ > 0)
        ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%P|%t) ImR: Removed Server <%C>.\n"),
                        info->key_name_.c_str()));
    }
}

void
ImR_Locator_i::remove_server
(ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
 const char* id)
{
  if (this->opts_->readonly ())
    {
      ORBSVCS_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) ImR: Can't remove server <%C> due to locked database.\n"),
                      id));
      CORBA::Exception *ex =
        new CORBA::NO_PERMISSION (CORBA::SystemException::_tao_minor_code
                                  (TAO_IMPLREPO_MINOR_CODE, 0),
                                  CORBA::COMPLETED_NO);
      ImplementationRepository::AMH_AdministrationExceptionHolder h (ex);
      _tao_rh->remove_server_excep (&h);
      return;
    }

  Server_Info_Ptr info = this->repository_->get_active_server (id);
  if (! info.null ())
    {
      AsyncAccessManager_ptr aam(this->find_aam  (info->key_name_.c_str()));
      if (!aam.is_nil())
        {
          if (debug_ > 0)
            ORBSVCS_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("(%P|%t) ImR: Can't remove active server <%C>.\n"),
                            info->key_name_.c_str()));
          CORBA::Exception *ex =
            new CORBA::NO_PERMISSION (CORBA::SystemException::_tao_minor_code
                                      (TAO_IMPLREPO_MINOR_CODE, EBUSY),
                                      CORBA::COMPLETED_NO);
          ImplementationRepository::AMH_AdministrationExceptionHolder h (ex);
          _tao_rh->remove_server_excep (&h);
          return;
        }
      this->remove_server_i (info);
    }
  else
    {
      ORBSVCS_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) ImR: Can't remove unknown server <%C>.\n"), id));
      CORBA::Exception *ex = new ImplementationRepository::NotFound;
      ImplementationRepository::AMH_AdministrationExceptionHolder h (ex);
      _tao_rh->remove_server_excep (&h);
      return;
    }
  _tao_rh->remove_server ();
}

void
ImR_Locator_i::force_remove_server
(ImplementationRepository::AMH_AdministrationExtResponseHandler_ptr _tao_rh,
 const char *name, CORBA::Short signum)
{
  CORBA::Exception *ex = 0;
  Server_Info_Ptr si;
  AsyncAccessManager_ptr aam;
  if (this->opts_->readonly ())
    {
      ORBSVCS_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) ImR: Can't remove server <%C> due to locked database.\n"),
                      name));
      ex =
        new CORBA::NO_PERMISSION (CORBA::SystemException::_tao_minor_code
                                  (TAO_IMPLREPO_MINOR_CODE, 0),
                                  CORBA::COMPLETED_NO);
      ImplementationRepository::AMH_AdministrationExtExceptionHolder h (ex);
      _tao_rh->force_remove_server_excep (&h);
      return;
    }


  if (!this->get_info_for_name (name, si))
    {
      ex = new ImplementationRepository::NotFound;
      ImplementationRepository::AMH_AdministrationExtExceptionHolder h (ex);
      _tao_rh->force_remove_server_excep (&h);
      return;
    }

  if (!si->alt_info_.null ())
    {
      si = si->alt_info_;
    }
  aam = this->find_aam (si->key_name_.c_str ());
  if (aam.is_nil ())
    {
      this->remove_server_i (si);
      _tao_rh->force_remove_server ();
      return;
    }

  ImR_ResponseHandler *aam_rh =
    new ImR_Loc_ResponseHandler (ImR_Loc_ResponseHandler::LOC_FORCE_REMOVE_SERVER,
                                 _tao_rh);
  if (aam->force_remove_rh (aam_rh))
    {
      delete aam_rh;
      _tao_rh->force_remove_server ();
      return;
    }

  bool active = (signum > 0) ?
    this->kill_server_i (si, signum, ex) :
    this->shutdown_server_i (si, ex, true);

  if (ex != 0)
    {
      ImplementationRepository::AMH_AdministrationExtExceptionHolder h (ex);
      _tao_rh->force_remove_server_excep (&h);
      aam->force_remove_rh (0);
      return;
    }
  if (active)
    {
      aam->shutdown_initiated ();
    }
  else
    {
      aam->force_remove_rh (0);
      remove_server_i (si);
      _tao_rh->force_remove_server ();
    }
}

void
ImR_Locator_i::destroy_poa (const ACE_CString &poa_name)
{
  PortableServer::POA_var poa = findPOA (poa_name.c_str());
  if (! CORBA::is_nil (poa.in ()))
    {
      bool etherealize = true;
      bool wait = false;
      poa->destroy (etherealize, wait);
    }
}


PortableServer::POA_ptr
ImR_Locator_i::findPOA (const char* name)
{
  try
    {
      bool activate_it = false;
      return root_poa_->find_POA (name, activate_it);
    }
  catch (const CORBA::Exception&)
    {// Ignore
    }
  return PortableServer::POA::_nil ();
}

bool
ImR_Locator_i::shutdown_server_i (const Server_Info_Ptr &si,
                                  CORBA::Exception *&ex_ret,
                                  bool force)
{
  const CORBA::ULong TAO_MINOR_MASK = 0x00000f80;
  const char *id = si->key_name_.c_str();
  if (debug_ > 0)
    ORBSVCS_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) ImR: Shutting down server <%C>.\n"),
                    id));

  UpdateableServerInfo info (this->repository_, si);
  if (info.null ())
    {
      ORBSVCS_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) ImR: shutdown_server () ")
                      ACE_TEXT ("Cannot find info for server <%C>\n"),
                      id));
      ex_ret = new ImplementationRepository::NotFound;
      return false;
    }

  this->connect_server (info);

  if (CORBA::is_nil (info->active_info()->server.in ()))
    {
      ORBSVCS_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%P|%t) ImR: shutdown_server ()")
                      ACE_TEXT (" Cannot connect to server <%C>\n"),
                      id));
      ex_ret = new ImplementationRepository::NotFound;
      return false;
    }

  try
    {
      CORBA::Object_var obj = this->set_timeout_policy (info->active_info()->server.in (),
                                                        DEFAULT_SHUTDOWN_TIMEOUT);
      ImplementationRepository::ServerObject_var server =
        ImplementationRepository::ServerObject::_unchecked_narrow (obj.in ());
      server->shutdown ();
      return true;
    }
  catch (const CORBA::TIMEOUT &ex)
    {
      info.edit ()->reset_runtime ();
      // Note : This is a good thing. It means we didn't waste our time waiting for
      // the server to finish shutting down.
      if (debug_ > 1)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) ImR: Timeout while waiting for <%C> shutdown.\n"),
                          id));
        }
      if (!force && this->opts_->throw_shutdown_exceptions ())
        {
          ex_ret = ex._tao_duplicate();
        }
    }
  catch (const CORBA::COMM_FAILURE& ex)
    {
      info.edit ()->reset_runtime ();
      if (debug_ > 1)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("ImR: COMM_FAILURE while waiting for <%C> shutdown.\n"),
                          id));
        }
      if (!force && this->opts_->throw_shutdown_exceptions ())
        {
          ex_ret = ex._tao_duplicate();
        }
    }
  catch (const CORBA::TRANSIENT& ex)
    {
      CORBA::ULong minor = ex.minor () & TAO_MINOR_MASK;
      if (minor != TAO_POA_DISCARDING && minor != TAO_POA_HOLDING)
        {
          info.edit ()->reset_runtime ();
        }
      if (debug_ > 1)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("ImR: TRANSIENT while waiting for <%C> shutdown.\n"),
                          id));
        }
      if (!force && this->opts_->throw_shutdown_exceptions ())
        {
          ex_ret = ex._tao_duplicate();
        }
    }
  catch (const CORBA::Exception &ex)
    {
      if (debug_ > 1)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) ImR: Exception while shutting down <%C>\n"),
                          id));
        }
      if (!force && this->opts_->throw_shutdown_exceptions ())
        {
          ex_ret = ex._tao_duplicate();
        }
    }
  return false;
}

void
ImR_Locator_i::shutdown_server
(ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
 const char* name)
{
  CORBA::Exception *ex = 0;
  Server_Info_Ptr si;

  if (!this->get_info_for_name (name, si))
    {
      ex = new ImplementationRepository::NotFound;
      ImplementationRepository::AMH_AdministrationExceptionHolder h (ex);
      _tao_rh->shutdown_server_excep (&h);
      return;
    }

  if (this->shutdown_server_i (si, ex, false))
    {
      AsyncAccessManager_ptr aam = this->find_aam (si->ping_id ());
      if (!aam.is_nil())
        {
          aam->shutdown_initiated ();
        }
    }
  if (ex != 0)
    {
      ImplementationRepository::AMH_AdministrationExceptionHolder h (ex);
      _tao_rh->shutdown_server_excep (&h);
    }
  else
    {
      _tao_rh->shutdown_server ();
    }
}

void
ImR_Locator_i::server_is_running
(ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
 const char* id,
 const char* partial_ior,
 ImplementationRepository::ServerObject_ptr server_object)
{
  if (debug_ > 0)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) ImR: Server %C is running at %C.\n"),
                      id, partial_ior));
    }
  CORBA::String_var sior = orb_->object_to_string (server_object);

  if (debug_ > 1)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) ImR: Server %C callback at %C.\n"),
                      id, sior.in ()));
    }

  if (this->opts_->unregister_if_address_reused ())
    this->repository_->unregister_if_address_reused (id, partial_ior, this);

  CORBA::Object_var obj = this->set_timeout_policy (server_object, this->opts_->ping_timeout ());
  ImplementationRepository::ServerObject_var srvobj =
    ImplementationRepository::ServerObject::_narrow (obj.in());

  UpdateableServerInfo info (this->repository_, id);
  if (info.null ())
    {
      if (debug_ > 0)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) ImR: Auto adding NORMAL server <%C>.\n"),
                          id));
        }

      Server_Info_Ptr si;
      if (this->repository_->add_server (id, partial_ior, sior.in (), srvobj.in ()) == 0)
        {
          si = this->repository_->get_active_server (id);
        }

      if (si.null ())
        {
          if (debug_ > 0)
            {
              ORBSVCS_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("(%P|%t) ImR: Auto adding failed, giving up <%C>\n"),
                              id));
            }
          CORBA::NO_MEMORY ex;
          ImplementationRepository::AMH_AdministrationExceptionHolder h (ex._tao_duplicate());
          _tao_rh->server_is_running_excep (&h);
          return;
        }
      info.server_info (si);
      this->pinger_.add_server (si->ping_id (), this->opts_->ping_external (), srvobj.in());

      ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
      AsyncAccessManager_ptr aam (this->create_aam (info, true));
    }
  else
    {
      if (!info->is_mode(ImplementationRepository::PER_CLIENT))
        {
          info.edit ()->set_contact (partial_ior, sior.in(), srvobj.in());

          info.update_repo();
          this->pinger_.add_server (info->ping_id(), true, srvobj.in());
        }

      AsyncAccessManager_ptr aam(this->find_aam (info->ping_id ()));
      if (!aam.is_nil())
        {
          if (ImR_Locator_i::debug () > 4)
            {
              ORBSVCS_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("(%P|%t) ImR_Locator_i::server_is_running aam is not nil\n")));
            }
          aam->server_is_running (partial_ior, srvobj.in());
        }
      else
        {
          if (ImR_Locator_i::debug () > 4)
            {
              ORBSVCS_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("(%P|%t) ImR_Locator_i::server_is_running aam is nil\n")));
            }
          if (!info->is_mode(ImplementationRepository::PER_CLIENT))
            {
              ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
              aam = this->create_aam (info, true);
            }
        }
    }
  _tao_rh->server_is_running ();
}

void
ImR_Locator_i::server_is_shutting_down
(ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
 const char* fqname)
{
  UpdateableServerInfo info (this->repository_, fqname);
  if (info.null ())
    {
      if (debug_ > 1)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) ImR_Locator_i::server_is_shutting_down: ")
                          ACE_TEXT ("Unknown server: %C\n"),
                          fqname));
        }
      _tao_rh->server_is_shutting_down ();
      return;
    }

  if (debug_ > 0)
    ORBSVCS_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("(%P|%t) ImR: Server <%C> is shutting down.\n"),
                    fqname));

  if (!info->is_mode(ImplementationRepository::PER_CLIENT))
    {
      this->pinger_.remove_server (info->ping_id());
      {
        AsyncAccessManager_ptr aam = this->find_aam (info->ping_id (), false);
        if (aam.is_nil())
          {
            aam = this->find_aam(info->ping_id(), true);
          }
        if (!aam.is_nil())
          {
            aam->server_is_shutting_down ();
          }
      }
    }
  info.edit ()->reset_runtime ();
  _tao_rh->server_is_shutting_down ();
}

void
ImR_Locator_i::find
(ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
 const char* id)
{
  Server_Info_Ptr si = this->repository_->get_active_server (id);
  ImplementationRepository::ServerInformation_var imr_info;
  try
    {
      if (! si.null ())
        {
          imr_info = si->createImRServerInfo ();

          if (debug_ > 1)
            ORBSVCS_DEBUG ((LM_DEBUG, ACE_TEXT ("(%P|%t) ImR: Found server %C.\n"), id));
        }
      else
        {
          ACE_NEW_THROW_EX (imr_info,
                            ImplementationRepository::ServerInformation,
                            CORBA::NO_MEMORY ());
          imr_info->startup.activation= ImplementationRepository::NORMAL;
          if (debug_ > 1)
            ORBSVCS_DEBUG ((LM_DEBUG,
                            ACE_TEXT ("(%P|%t) ImR: Cannot find server <%C>\n"),
                            id));
        }
    }
  catch (const CORBA::Exception &ex)
    {
      ImplementationRepository::AMH_AdministrationExceptionHolder h (ex._tao_duplicate());
      _tao_rh->find_excep (&h);
      return;
    }
  _tao_rh->find (imr_info.in());
}

void
ImR_Locator_i::list
(ImplementationRepository::AMH_AdministrationResponseHandler_ptr _tao_rh,
 CORBA::ULong how_many,
 CORBA::Boolean active)
{

  AsyncListManager *l = 0;
  try
    {
      ACE_NEW_THROW_EX (l,
                        AsyncListManager (this->repository_.get(),
                                          this->root_poa_.in(),
                                          active ? &this->pinger_ : 0),
                        CORBA::NO_MEMORY ());
      AsyncListManager_ptr lister (l);
      l->list (_tao_rh, how_many);
    }
  catch (const CORBA::Exception &ex)
    {
      ImplementationRepository::AMH_AdministrationExceptionHolder h (ex._tao_duplicate());
      _tao_rh->find_excep (&h);
      return;
    }
}

Activator_Info_Ptr
ImR_Locator_i::get_activator (const ACE_CString& aname)
{
  Activator_Info_Ptr info = this->repository_->get_activator (aname);
  if (! info.null ())
    {
      this->connect_activator (*info);
    }
  return info;
}

void
ImR_Locator_i::connect_activator (Activator_Info& info)
{
  if (! CORBA::is_nil (info.activator.in ()) || info.ior.length () == 0)
    return;

  try
    {
      CORBA::Object_var obj =
        this->orb_->string_to_object (info.ior.c_str ());

      if (CORBA::is_nil (obj.in ()))
        {
          info.reset_runtime ();
          return;
        }

      if (this->opts_->startup_timeout () > ACE_Time_Value::zero)
        {
          obj = this->set_timeout_policy (obj.in (), this->opts_->startup_timeout ());
        }

      info.activator =
        ImplementationRepository::Activator::_unchecked_narrow (obj.in ());

      if (CORBA::is_nil (info.activator.in ()))
        {
          info.reset_runtime ();
          return;
        }

      if (debug_ > 1)
        ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%P|%t) ImR: Connected to activator <%C>\n"),
                        info.name.c_str ()));
    }
  catch (const CORBA::Exception& )
    {
      info.reset_runtime ();
    }
}

void
ImR_Locator_i::auto_start_servers (void)
{
  if (this->repository_->servers ().current_size () == 0)
    return;

  Locator_Repository::SIMap::ENTRY* server_entry;
  Locator_Repository::SIMap::ITERATOR server_iter (this->repository_->servers ());

  // For each of the entries in the Locator_Repository, get the startup
  // information and activate the servers, if they are not already
  // running.
  for (;server_iter.next (server_entry) != 0; server_iter.advance ())
    {
      UpdateableServerInfo info(this->repository_, server_entry->int_id_);
      ACE_ASSERT (! info.null ());

      try
        {
          if (info->is_mode (ImplementationRepository::AUTO_START)
              && info->active_info()->cmdline.length () > 0)
            {
              ImR_ResponseHandler rh;
              this->activate_server_i (info, true, &rh);
            }
        }
      catch (const CORBA::Exception& ex)
        {
          if (debug_ > 1)
            {
              ORBSVCS_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("(%P|%t) ImR: AUTO_START Could not activate <%C>\n"),
                              server_entry->ext_id_.c_str ()));
              ex._tao_print_exception ("AUTO_START");
            }
          // Ignore exceptions
        }
    }
}

void
ImR_Locator_i::connect_server (UpdateableServerInfo& info)
{
  Server_Info *sip = info.edit ()->active_info ();
  if (! CORBA::is_nil (sip->server.in ()))
    {
      if (!this->pinger_.has_server (sip->key_name_.c_str()))
        {
          this->pinger_.add_server (sip->key_name_.c_str(),
                                    this->opts_->ping_external (),
                                    sip->server.in());
        }
      return; // already connected
    }

  if (sip->ior.length () == 0)
    {
      sip->reset_runtime ();
      return; // can't connect
    }

  try
    {
      CORBA::Object_var obj = orb_->string_to_object (sip->ior.c_str ());

      if (CORBA::is_nil (obj.in ()))
        {
          sip->reset_runtime ();
          return;
        }

      obj = this->set_timeout_policy (obj.in (), this->opts_->ping_timeout ());

      sip->server =
        ImplementationRepository::ServerObject::_unchecked_narrow (obj.in ());

      if (CORBA::is_nil (sip->server.in ()))
        {
          sip->reset_runtime ();
          return;
        }

      if (debug_ > 1)
        ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%P|%t) ImR: Connected to server <%C>\n"),
                        sip->key_name_.c_str ()));
      this->pinger_.add_server (sip->key_name_.c_str(),
                                this->opts_->ping_external (),
                                sip->server.in());


    }
  catch (const CORBA::Exception& )
    {
      sip->reset_runtime ();
    }
}

bool
ImR_Locator_i::server_is_alive (UpdateableServerInfo& info)
{
  this->connect_server (info);
  SyncListener *listener = 0;
  ACE_NEW_RETURN (listener,
                  SyncListener (info->ping_id(),
                                this->orb_.in(),
                                this->pinger_),
                  false);
  LiveListener_ptr llp(listener);
  return listener->is_alive();
}

int
ImR_Locator_i::debug ()
{
  return debug_;
}

LiveCheck&
ImR_Locator_i::pinger (void)
{
  return this->pinger_;
}

PortableServer::POA_ptr
ImR_Locator_i::root_poa (void)
{
  return PortableServer::POA::_duplicate (this->root_poa_.in());
}

void
ImR_Locator_i::remove_aam (AsyncAccessManager_ptr &aam)
{
  if (this->aam_terminating_.remove (aam) == -1)
    {
      this->aam_active_.remove (aam);
    }
}

void
ImR_Locator_i::remove_aam_i (const char *name, bool active)
{
  AAM_Set &set = active ? this->aam_active_ : this->aam_terminating_;

  for (AAM_Set::ITERATOR i = set.begin(); i != set.end(); ++i)
    {
      if ((*i)->has_server (name))
        {
          ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
          set.remove (*i);
          return;
        }
    }
}

void
ImR_Locator_i::remove_aam (const char *name)
{
  this->remove_aam_i (name,true);
  this->remove_aam_i (name,false);
}

void
ImR_Locator_i::make_terminating (AsyncAccessManager_ptr &aam,
                                 const char *name, int pid)
{
  this->aam_active_.remove (aam);
  this->aam_terminating_.insert_tail (aam);
  this->pinger_.remove_server (name, pid);
}

AsyncAccessManager *
ImR_Locator_i::find_aam (const char *name, bool active)
{
  AAM_Set &set = active ? this->aam_active_ : this->aam_terminating_;
  for (AAM_Set::ITERATOR i = set.begin(); i != set.end(); ++i)
    {
      if ((*i)->has_server (name))
        {
          return (*i)->_add_ref();
        }
    }
  return 0;
}

AsyncAccessManager *
ImR_Locator_i::create_aam (UpdateableServerInfo &info, bool running)
{
  AsyncAccessManager_ptr aam;
  ACE_NEW_RETURN (aam, AsyncAccessManager (info, *this), 0);
  if (running)
    {
      aam->started_running ();
    }
  {
    this->aam_active_.insert_tail (aam);
  }
  return aam._retn ();
}

//-------------------------------------------------------------------------

SyncListener::SyncListener (const char *server,
                            CORBA::ORB_ptr orb,
                            LiveCheck &pinger)
  :LiveListener (server),
   orb_ (CORBA::ORB::_duplicate (orb)),
   pinger_ (pinger),
   status_ (LS_UNKNOWN),
   got_it_ (false),
   callback_ (false)
{
}

SyncListener::~SyncListener ()
{
}

bool
SyncListener::is_alive (void)
{
  this->callback_ = true;
  while (!this->got_it_)
    {
      if (this->callback_)
        {
          if (!this->pinger_.add_poll_listener (this))
            {
              return false;
            }
        }
      this->callback_ = false;
      ACE_Time_Value delay (10,0);
      this->orb_->perform_work (delay);
    }
  this->got_it_ = false;
  return this->status_ != LS_DEAD;
}

bool
SyncListener::status_changed (LiveStatus status)
{
  this->callback_ = true;
  this->status_ = status;
  this->got_it_ = (status != LS_TRANSIENT);
  return true;
}

//---------------------------------------------------------------------------

ImR_SyncResponseHandler::ImR_SyncResponseHandler (const char *objkey, CORBA::ORB_ptr orb)
  :excep_ (0),
   key_ (objkey),
   orb_ (CORBA::ORB::_duplicate (orb))
{
}

ImR_SyncResponseHandler::~ImR_SyncResponseHandler (void)
{
}

void
ImR_SyncResponseHandler::send_ior (const char *pior)
{
  ACE_CString full (pior);
  full += this->key_;
  this->result_ = full.c_str();
}

void
ImR_SyncResponseHandler::send_exception (CORBA::Exception *ex)
{
  this->excep_ = ex->_tao_duplicate();
}

char *
ImR_SyncResponseHandler::wait_for_result (void)
{
  while (this->result_.in() == 0 && this->excep_ == 0)
    {
      this->orb_->perform_work ();
    }
  if (this->excep_ != 0)
    {
      TAO_AMH_DSI_Exception_Holder h(this->excep_);
      h.raise_invoke ();
    }
  return this->result_._retn();
}

//---------------------------------------------------------------------------

ImR_Loc_ResponseHandler::ImR_Loc_ResponseHandler
(Loc_Operation_Id opid,
 ImplementationRepository::AMH_AdministrationResponseHandler_ptr rh)
  :op_id_ (opid),
   resp_ (ImplementationRepository::AMH_AdministrationResponseHandler::_duplicate (rh))
{
}

ImR_Loc_ResponseHandler::ImR_Loc_ResponseHandler
(Loc_Operation_Id opid,
 ImplementationRepository::AMH_AdministrationExtResponseHandler_ptr rh)
  :op_id_ (opid),
   ext_ (ImplementationRepository::AMH_AdministrationExtResponseHandler::_duplicate (rh))
{
}

ImR_Loc_ResponseHandler::~ImR_Loc_ResponseHandler (void)
{
}

void
ImR_Loc_ResponseHandler::send_ior (const char *)
{
  if (CORBA::is_nil (this->resp_))
    {
      this->send_ior_ext ("");
      return;
    }
  switch (this->op_id_)
    {
    case LOC_ACTIVATE_SERVER:
      resp_->activate_server ();
      break;
    case LOC_ADD_OR_UPDATE_SERVER:
      resp_->add_or_update_server ();
      break;
    case LOC_REMOVE_SERVER:
      resp_->remove_server ();
      break;
    case LOC_SHUTDOWN_SERVER:
      resp_->shutdown_server ();
      break;
    case LOC_SERVER_IS_RUNNING:
      resp_->server_is_running ();
      break;
    case LOC_SERVER_IS_SHUTTING_DOWN:
      resp_->server_is_shutting_down ();
      break;
    default:
      break;
    };

  delete this;
}

void
ImR_Loc_ResponseHandler::send_ior_ext (const char *)
{
  switch (this->op_id_)
    {
    case LOC_FORCE_REMOVE_SERVER:
      ext_->force_remove_server ();
      break;
    default:
      break;
    };

  delete this;
}

void
ImR_Loc_ResponseHandler::send_exception (CORBA::Exception *ex)
{
  if (CORBA::is_nil (this->resp_))
    {
      this->send_exception_ext (ex);
      return;
    }
  ImplementationRepository::AMH_AdministrationExceptionHolder h (ex);
  switch (this->op_id_)
    {
    case LOC_ACTIVATE_SERVER:
      resp_->activate_server_excep (&h);
      break;
    case LOC_ADD_OR_UPDATE_SERVER:
      resp_->add_or_update_server_excep (&h);
      break;
    case LOC_REMOVE_SERVER:
      resp_->remove_server_excep (&h);
      break;
    case LOC_SHUTDOWN_SERVER:
      resp_->shutdown_server_excep (&h);
      break;
    case LOC_SERVER_IS_RUNNING:
      resp_->server_is_running_excep (&h);
      break;
    case LOC_SERVER_IS_SHUTTING_DOWN:
      resp_->server_is_shutting_down_excep (&h);
      break;
    default:
      break;
    };
  delete this;
}
void
ImR_Loc_ResponseHandler::send_exception_ext (CORBA::Exception *ex)
{
  ImplementationRepository::AMH_AdministrationExtExceptionHolder h (ex);
  switch (this->op_id_)
    {
    case LOC_FORCE_REMOVE_SERVER:
      ext_->force_remove_server_excep (&h);
      break;
    default:
      break;
    };
  delete this;
}
