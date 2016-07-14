#include "orbsvcs/Log_Macros.h"
#include "ImR_Activator_i.h"

#include "Activator_Options.h"

#include "tao/ORB_Core.h"

#include "ace/Reactor.h"
#include "ace/ARGV.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_signal.h"

#include "ace/os_include/os_netdb.h"

static const char *unique_prefix = "\001\002\003\004";
static size_t unique_prefix_len = 4;

static ACE_CString getHostName ()
{
  char host_name[MAXHOSTNAMELEN];
  ACE_OS::hostname (host_name, MAXHOSTNAMELEN);
  return ACE_CString (host_name);
}

Active_Pid_Setter::Active_Pid_Setter(ImR_Activator_i &owner, pid_t pid)
  :owner_(owner)
{
  owner_.active_check_pid_ = pid;
}

Active_Pid_Setter::~Active_Pid_Setter()
{
  owner_.active_check_pid_ = ACE_INVALID_PID;
}

ImR_Activator_i::ImR_Activator_i (void)
  : registration_token_(0)
  , debug_(0)
  , notify_imr_ (false)
  , induce_delay_ (0)
  , name_ (getHostName ())
  , env_buf_len_ (Activator_Options::ENVIRONMENT_BUFFER)
  , max_env_vars_ (Activator_Options::ENVIRONMENT_MAX_VARS)
  , detach_child_ (false)
  , active_check_pid_ (ACE_INVALID_PID)
{
}

static PortableServer::POA_ptr
createPersistentPOA (PortableServer::POA_ptr root_poa, const char* poa_name)
{
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
    root_poa->create_POA(poa_name, mgr.in (), pols);

  life->destroy ();
  assign->destroy ();

  return poa._retn ();
}

// It's ok if we can't register with the ImR. It just
// means we won't be able to notify it of any events
// (Currently, just that we're shutting down and to
// notify of the ImR when a child process exits.)
void
ImR_Activator_i::register_with_imr (ImplementationRepository::Activator_ptr activator)
{
  try
    {
      if (this->debug_ > 1)
        ORBSVCS_DEBUG( (LM_DEBUG, "ImR Activator: Contacting ImplRepoService...\n"));

      // First, resolve the ImR, without this we can go no further
      CORBA::Object_var obj =
        orb_->resolve_initial_references ("ImplRepoService");

      this->process_mgr_.open (ACE_Process_Manager::DEFAULT_SIZE,
                               this->orb_->orb_core ()->reactor ());

      locator_ = ImplementationRepository::Locator::_narrow (obj.in ());

      if (!CORBA::is_nil (locator_.in ()))
        {
          if (this->debug_ > 9)
            {
              CORBA::String_var ior = orb_->object_to_string (obj.in ());
              ORBSVCS_DEBUG((LM_DEBUG, "ImR Activator: ImplRepoService ior=<%C>\n",
                             ior.in()));
            }

          this->registration_token_ =
            locator_->register_activator (name_.c_str (), activator);

          if (debug_ > 0)
            ORBSVCS_DEBUG((LM_DEBUG, "ImR Activator: Registered with ImR.\n"));

          return;
        }
      else if (this->debug_ > 1)
        ORBSVCS_DEBUG((LM_DEBUG, "ImR Activator: ImplRepoService not found\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      if (debug_ > 1)
        ex._tao_print_exception (
                                 "ImR Activator: Can't register with ImR.");
    }

  if (debug_ > 0)
    ORBSVCS_DEBUG ((LM_DEBUG, "ImR Activator: Not registered with ImR.\n"));
}

int
ImR_Activator_i::init_with_orb (CORBA::ORB_ptr orb, const Activator_Options& opts)
{
  ACE_ASSERT(! CORBA::is_nil (orb));
  orb_ = CORBA::ORB::_duplicate (orb);
  debug_ = opts.debug ();
  notify_imr_ = opts.notify_imr ();
  induce_delay_ = opts.induce_delay ();
  env_buf_len_ = opts.env_buf_len ();
  max_env_vars_ = opts.max_env_vars ();
  detach_child_ = opts.detach_child ();

  if (opts.name ().length () > 0)
    {
      name_ = opts.name();
    }

  try
    {
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA");
      ACE_ASSERT (! CORBA::is_nil (obj.in ()));
      this->root_poa_ = PortableServer::POA::_narrow (obj.in ());
      ACE_ASSERT (! CORBA::is_nil(this->root_poa_.in ()));

      // The activator must use a persistent POA so that it can be started before the
      // locator in some scenarios, such as when the locator persists its database, and
      // wants to reconnect to running activators to auto_start some servers.
      this->imr_poa_ = createPersistentPOA (this->root_poa_.in (),
                                            "ImR_Activator");
      ACE_ASSERT (! CORBA::is_nil(this->imr_poa_.in ()));

      obj = orb->resolve_initial_references ("POACurrent");
      ACE_ASSERT (! CORBA::is_nil (obj.in ()));
      this->current_ = PortableServer::Current::_narrow (obj.in ());

      // Activate ourself
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("ImR_Activator");
      this->imr_poa_->activate_object_with_id (id.in (), this);
      obj = this->imr_poa_->id_to_reference (id.in ());
      ImplementationRepository::ActivatorExt_var activator =
        ImplementationRepository::ActivatorExt::_narrow (obj.in ());
      ACE_ASSERT(! CORBA::is_nil (activator.in ()));

      CORBA::String_var ior = this->orb_->object_to_string (activator.in ());

      if (this->debug_ > 0)
        ORBSVCS_DEBUG((LM_DEBUG, "ImR Activator: Starting %C\n", name_.c_str ()));

      // initialize our process manager.
      // This requires a reactor that has signal handling.
      ACE_Reactor *reactor = ACE_Reactor::instance ();
      if (reactor != 0)
        {
          if (this->process_mgr_.open (ACE_Process_Manager::DEFAULT_SIZE, reactor) == -1)
            {
              ORBSVCS_ERROR_RETURN ((LM_ERROR,
                                     "The ACE_Process_Manager didnt get initialized\n"), -1);
            }
        }

      this->register_with_imr (activator.in ()); // no throw

      PortableServer::POAManager_var poaman =
        this->root_poa_->the_POAManager ();
      poaman->activate ();

      if (this->debug_ > 1)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          "ImR Activator: The Activator IOR is: <%C>\n", ior.in ()));
        }

      // The last thing we do is write out the ior so that a test program can assume
      // that the activator is ready to go as soon as the ior is written.
      if (opts.ior_filename ().length () > 0)
        {
          FILE* fp = ACE_OS::fopen (opts.ior_filename ().c_str (), "w");
          if (fp == 0)
            {
              ORBSVCS_ERROR_RETURN ((LM_ERROR,
                                     "ImR Activator: Could not open file: %s\n", opts.ior_filename ().c_str ()), -1);
            }
          ACE_OS::fprintf (fp, "%s", ior.in ());
          ACE_OS::fclose (fp);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (
                               "ImR_Activator_i::init_with_orb");
      throw;
    }
  return 0;
}

int
ImR_Activator_i::init (Activator_Options& opts)
{
  ACE_CString cmdline = opts.cmdline();
  // Must use IOR style objrefs, because URLs sometimes get mangled when passed
  // to ACE_Process::spawn().
  cmdline += "-ORBUseImR 0 -ORBObjRefStyle IOR ";
  ACE_ARGV av (ACE_TEXT_CHAR_TO_TCHAR (cmdline.c_str ()));
  int argc = av.argc ();

  CORBA::ORB_var orb =
    CORBA::ORB_init (argc, av.argv (), "TAO_ImR_Activator");

  int ret = this->init_with_orb(orb.in (), opts);

  return ret;
}

int
ImR_Activator_i::fini (void)
{
  try
    {
      if (debug_ > 1)
        ORBSVCS_DEBUG ((LM_DEBUG, "ImR Activator: Shutting down...\n"));

      this->process_mgr_.close ();

      this->root_poa_->destroy (1, 1);

      if (! CORBA::is_nil (this->locator_.in ()) && this->registration_token_ != 0)
        {
          this->locator_->unregister_activator (name_.c_str(),
                                                this->registration_token_);
        }
    }
  catch (const CORBA::COMM_FAILURE&)
    {
      if (debug_ > 1)
        ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%P|%t) ImR Activator: Unable to unregister from ImR.\n")));
    }
  catch (const CORBA::TRANSIENT&)
    {
      if (debug_ > 1)
        ORBSVCS_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%P|%t) ImR Activator: Unable to unregister from ImR.\n")));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("ImR Activator: fini");
      throw;
    }

  try
    {
      this->orb_->destroy ();

      if (debug_ > 0)
        ORBSVCS_DEBUG ((LM_DEBUG, "ImR Activator: Shut down successfully.\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("ImR Activator: fini 2");
      throw;
    }
  return 0;
}

int
ImR_Activator_i::run (void)
{
  this->orb_->run ();
  return 0;
}

void
ImR_Activator_i::shutdown (void)
{
  this->shutdown (false);
}

bool
ImR_Activator_i::in_upcall (void)
{
  try
    {
      PortableServer::POA_var poa = current_->get_POA ();
      return !CORBA::is_nil (poa.in ());
    }
  catch (const CORBA::Exception& )
    {
      // no-op
    }
  return false;
}

void
ImR_Activator_i::shutdown (bool signaled)
{
  if (signaled && this->in_upcall ())
    {
      if (debug_ > 0)
        ORBSVCS_DEBUG ((LM_DEBUG, "ImR Activator: ignoring signal during upcall.\n"));
      return;
    }
  if (! CORBA::is_nil (this->locator_.in ()) && this->registration_token_ != 0)
    {
      try
        {
          this->locator_->unregister_activator (name_.c_str(),
                                                this->registration_token_);
        }
      catch (const CORBA::Exception& )
        {
          // the locator may have already shutdown
        }
    }
  this->locator_ = ImplementationRepository::Locator::_nil ();

  this->orb_->shutdown (false);
}

CORBA::Boolean
ImR_Activator_i::kill_server (const char* name, CORBA::Long lastpid, CORBA::Short signum)
{
  if (debug_ > 1)
    ORBSVCS_DEBUG((LM_DEBUG,
                   "ImR Activator: Killing server <%C>, lastpid = %d\n",
                   name, lastpid));
  pid_t lpid = static_cast<pid_t>(lastpid);
  pid_t pid = 0;
  bool found = false;
  int result = -1;
  for (ProcessMap::iterator iter = process_map_.begin();
       !found && iter != process_map_.end (); iter++)
    {
      if (iter->item () == name)
        {
          pid = iter->key ();
          found = pid == lpid;
        }
    }
  if (!found && pid == 0)
    {
      pid = lpid;
    }

#if defined (ACE_WIN32)
  found = false; // sigchild apparently doesn't work on windows
#endif
  if (pid != 0)
    {
      result =
#if !defined (ACE_WIN32)
        (signum != 9) ? ACE_OS::kill (pid, signum) :
#endif
        ACE::terminate_process (pid);

      if (this->running_server_list_.remove (name) == 0)
        {
          this->dying_server_list_.insert (name);
        }

      if (debug_ > 1)
        ORBSVCS_DEBUG((LM_DEBUG,
                       "ImR Activator: Killing server <%C> "
                       "signal %d to pid %d, found %d, this->notify_imr_ %d,  result = %d\n",
                       name, signum, static_cast<int> (pid), found, this->notify_imr_, result));
      if (!found && result == 0 && this->notify_imr_)
        {
          this->process_map_.bind (pid, name);
          ACE_Reactor *r = this->orb_->orb_core()->reactor();
          Act_token_type token = static_cast<Act_token_type>(pid);
          r->schedule_timer (this, reinterpret_cast<void *>(token), ACE_Time_Value ());
        }
    }
  return result == 0;
}

CORBA::Boolean
ImR_Activator_i::still_alive (CORBA::Long pid)
{
  pid_t pt = static_cast<pid_t>(pid);
  bool is_running =  this->process_map_.find (pt) == 0;
#if defined (ACE_WIN32)
  if (is_running)
    {
      pid_t waitp = this->process_mgr_.wait (pt, ACE_Time_Value::zero);
      is_running = (waitp != pt);
    }
#endif /* ACE_WIN32 */
  return is_running;
}

bool
ImR_Activator_i::still_running_i (const char *name, pid_t &pid)
{
  bool is_running =  this->running_server_list_.find (name) == 0;

  if (is_running)
    {
      pid = ACE_INVALID_PID;
      for (ProcessMap::ITERATOR iter = this->process_map_.begin ();
           iter != process_map_.end ();
           iter++)
        {
          if (ACE_OS::strcmp (name, iter->item ().c_str()) == 0)
            {
              pid = iter->key ();
              break;
            }
        }
#if defined (ACE_WIN32)
      if (pid != ACE_INVALID_PID)
        {
          Active_Pid_Setter aps(*this, pid);
          pid_t waitp = this->process_mgr_.wait (pid, ACE_Time_Value::zero);
          is_running = (waitp != pid);
        }
#endif /* ACE_WIN32 */
    }
  return is_running;
}

void
ImR_Activator_i::start_server(const char* name,
                              const char* cmdline,
                              const char* dir,
                              const ImplementationRepository::EnvironmentList & env)
{
  bool unique = false;
  if (ACE_OS::strlen (name) > unique_prefix_len &&
      ACE_OS::strncmp (name, unique_prefix, unique_prefix_len) == 0)
    {
      unique = true;
      name += unique_prefix_len;
    }

  // if (debug_ > 1)
  ORBSVCS_DEBUG((LM_DEBUG,
                 "ImR Activator: Starting %C <%C>...\n",
                 (unique ? "unique server" : "server"), name));
  pid_t pid;
  if (unique && this->still_running_i (name, pid))
    {
      // if (debug_ > 1)
      ORBSVCS_DEBUG((LM_DEBUG,
                     "ImR Activator: Unique instance already running %d\n",
                     static_cast<int> (pid)));
      char reason[32];
      ACE_OS::snprintf (reason,32,"pid:%d",static_cast<int> (pid));
      throw ImplementationRepository::CannotActivate(
                                                     CORBA::string_dup (reason));
    }

  ACE_TString cmdline_tstr(ACE_TEXT_CHAR_TO_TCHAR(cmdline));
  size_t cmdline_buf_len = cmdline_tstr.length();
  if (debug_ > 1)
    ORBSVCS_DEBUG((LM_DEBUG,
                   "\tcommand line : len=%d <%s>\n\tdirectory : <%C>\n",
                   cmdline_buf_len, cmdline_tstr.c_str(), dir)  );

  ACE_Process_Options proc_opts (
                                 1,
                                 cmdline_buf_len + 1,
                                 this->env_buf_len_, this->max_env_vars_);
  proc_opts.command_line (ACE_TEXT_CHAR_TO_TCHAR(cmdline));
  proc_opts.working_directory (dir);
  // Win32 does not support the CLOSE_ON_EXEC semantics for sockets
  // the way unix does, so in order to avoid having the child process
  // hold the listen socket open, we force the child to inherit no
  // handles. This includes stdin, stdout, logs, etc.
  proc_opts.handle_inheritance (0);

  // We always enable the unicode environmet buffer on Windows.  This works
  // around a 32kb environment buffer limitation.  This must come before any of
  // the setenv() calls, since the first of those will copy the current
  // process's environment.
  proc_opts.enable_unicode_environment ();

  // Guard against possible signal reflection which can happen on very heavily
  // loaded systems. Detaching the child process avoids that possibility but at
  // the cost of required explicit child termination prior to activator shutdown
  if (this->detach_child_)
    {
      proc_opts.setgroup (0);
    }

  proc_opts.setenv (ACE_TEXT ("TAO_USE_IMR"), ACE_TEXT ("1"));
  if (!CORBA::is_nil (this->locator_.in ()))
    {
      CORBA::String_var ior = orb_->object_to_string (locator_.in ());
      proc_opts.setenv (ACE_TEXT ("ImplRepoServiceIOR"),
                        ACE_TEXT_CHAR_TO_TCHAR (ior.in ()));
    }

  for (CORBA::ULong i = 0; i < env.length (); ++i)
    {
      proc_opts.setenv (ACE_TEXT_CHAR_TO_TCHAR (env[i].name.in ()),
                        ACE_TEXT_CHAR_TO_TCHAR (env[i].value.in ()));
    }

  pid = this->process_mgr_.spawn (proc_opts, this);
  if (pid == ACE_INVALID_PID)
    {
      ORBSVCS_ERROR ((LM_ERROR,
                      "ImR Activator: Cannot start server <%C> using <%C>\n", name, cmdline));

      throw ImplementationRepository::CannotActivate(
                                                     CORBA::string_dup (
                                                                        "Process Creation Failed"));
    }
  else
    {
      if (debug_ > 1)
        {
          ORBSVCS_DEBUG((LM_DEBUG,
                         "ImR Activator: register death handler for process %d\n",
                         static_cast<int> (pid)));
        }
      this->process_map_.rebind (pid, name);
      if (unique)
        {
          this->running_server_list_.insert (name);
        }
      if (!CORBA::is_nil (this->locator_.in ()))
        {
          if (this->notify_imr_)
            {
              if (debug_ > 1)
                {
                  ORBSVCS_DEBUG ((LM_DEBUG,
                                  ACE_TEXT ("(%P|%t) ImR Activator: Notifying ImR that ")
                                  ACE_TEXT ("<%C> has started.\n"),
                                  name));
                }
              this->locator_->spawn_pid (name, pid);
            }
        }
    }

  if (debug_ > 0)
    {
      ORBSVCS_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%P|%t) ImR Activator: Successfully started <%C>, pid=%d\n"),
                      name, static_cast<int> (pid)));
    }
}

int
ImR_Activator_i::handle_exit_i (pid_t pid)
{
  // We use the process_manager so that we're notified when
  // any of our launched processes die. We notify the locator
  // when this happens.

  ACE_CString name;
  if (this->process_map_.find (pid, name) == 0)
    {
      this->process_map_.unbind (pid);
    }
  if (this->running_server_list_.remove (name) == -1)
    {
      this->dying_server_list_.remove (name);
    }

  if (this->notify_imr_ && !CORBA::is_nil (this->locator_.in ()))
    {
      if (debug_ > 1)
        {
          ORBSVCS_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%P|%t) ImR Activator: Notifying ImR that ")
                          ACE_TEXT ("server[%d], <%C> has exited.\n"),
                          static_cast<int> (pid), name.c_str()));
        }
      try
        {
          this->locator_->child_death_pid (name.c_str(), pid);
        }
      catch (const CORBA::Exception &ex)
        {
          if (debug_ > 1)
            {
              ORBSVCS_DEBUG ((LM_DEBUG,
                              ACE_TEXT ("(%P|%t) ImR Activator: caught %s from locator::child_death_pid\n"),
                              ex._name()));
            }
        }
    }

  return 0;
}

int
ImR_Activator_i::handle_exit (ACE_Process * process)
{
  if (debug_ > 0)
    {
      ORBSVCS_DEBUG
        ((LM_DEBUG,
          ACE_TEXT ("Process %d exited with exit code %d, delay = %d\n"),
          process->getpid (), process->return_value (), this->induce_delay_));
    }

  if (this->induce_delay_ > 0 && this->active_check_pid_ == ACE_INVALID_PID)
    {
      ACE_Reactor *r = this->orb_->orb_core()->reactor();
      ACE_Time_Value dtv (0, this->induce_delay_ * 1000);
      pid_t pid = process->getpid();
      Act_token_type token = static_cast<Act_token_type>(pid);
      r->schedule_timer (this, reinterpret_cast<void *>(token), dtv );
    }
  else
    {
      this->handle_exit_i (process->getpid());
    }
  return 0;
}


int
ImR_Activator_i::handle_timeout (const ACE_Time_Value &, const void * tok)
{
  Act_token_type token = reinterpret_cast<Act_token_type>(tok);
  this->handle_exit_i (static_cast<pid_t>(token));
  return 0;
}
