// $Id$

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

static ACE_CString getHostName ()
{
  char host_name[MAXHOSTNAMELEN];
  ACE_OS::hostname (host_name, MAXHOSTNAMELEN);
  return ACE_CString (host_name);
}

ImR_Activator_i::ImR_Activator_i (void)
: registration_token_(0)
, debug_(0)
, notify_imr_ (false)
, induce_delay_ (0)
, name_ (getHostName ())
, env_buf_len_ (Activator_Options::ENVIRONMENT_BUFFER)
, max_env_vars_ (Activator_Options::ENVIRONMENT_MAX_VARS)
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
              ORBSVCS_DEBUG((LM_DEBUG, "ImR Activator: ImplRepoService ior=<%s>\n",
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

      // Activate ourself
      PortableServer::ObjectId_var id = PortableServer::string_to_ObjectId ("ImR_Activator");
      this->imr_poa_->activate_object_with_id (id.in (), this);
      obj = this->imr_poa_->id_to_reference (id.in ());
      ImplementationRepository::ActivatorExt_var activator =
        ImplementationRepository::ActivatorExt::_narrow (obj.in ());
      ACE_ASSERT(! CORBA::is_nil (activator.in ()));

      CORBA::String_var ior = this->orb_->object_to_string (activator.in ());

      if (this->debug_ > 0)
        ORBSVCS_DEBUG((LM_DEBUG, "ImR Activator: Starting %s\n", name_.c_str ()));

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
            "ImR Activator: The Activator IOR is: <%s>\n", ior.in ()));
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
  ACE_ARGV av (cmdline.c_str ());
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
        ORBSVCS_DEBUG ((LM_DEBUG, "ImR Activator: Unable to unregister from ImR.\n"));
    }
  catch (const CORBA::TRANSIENT&)
    {
      if (debug_ > 1)
        ORBSVCS_DEBUG ((LM_DEBUG, "ImR Activator: Unable to unregister from ImR.\n"));
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

void
ImR_Activator_i::shutdown (bool wait_for_completion)
{
  if (! CORBA::is_nil (this->locator_.in ()) && this->registration_token_ != 0)
    {
      this->locator_->unregister_activator (name_.c_str(),
                                            this->registration_token_);
    }
  this->locator_ = ImplementationRepository::Locator::_nil ();

  this->orb_->shutdown (wait_for_completion);
}

CORBA::Boolean
ImR_Activator_i::kill_server (const char* name, CORBA::Short signum)
{
  if (debug_ > 1)
    ORBSVCS_DEBUG((LM_DEBUG,
                   "ImR Activator: Killing server <%s>...\n",
                   name));
  for (ProcessMap::iterator iter = process_map_.begin();
       iter != process_map_.end (); iter++)
    {
      if (iter->item () == name)
        {
          pid_t pid = iter->key ();
          int result = (signum != 9) ? ACE_OS::kill (pid, signum)
            : ACE::terminate_process (pid);
          if (debug_ > 1)
            ORBSVCS_DEBUG((LM_DEBUG,
                           "ImR Activator: Killing server <%s> "
                           "signal %d to pid %d, result = %d\n",
                           name, signum, pid, result));
          return result == 0;
        }
    }
  return false;
}

void
ImR_Activator_i::start_server(const char* name,
                              const char* cmdline,
                              const char* dir,
                              const ImplementationRepository::EnvironmentList & env)
{
  if (debug_ > 1)
    ORBSVCS_DEBUG((LM_DEBUG, "ImR Activator: Starting server <%s>...\n", name));

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

  proc_opts.setenv (ACE_TEXT("TAO_USE_IMR"), "1");
  if (!CORBA::is_nil (this->locator_.in ()))
    {
      CORBA::String_var ior = orb_->object_to_string (locator_.in ());
      proc_opts.setenv (ACE_TEXT("ImplRepoServiceIOR"), ior.in());
    }

  for (CORBA::ULong i = 0; i < env.length (); ++i)
    {
      proc_opts.setenv (ACE_TEXT_CHAR_TO_TCHAR(env[i].name.in ()), env[i].value.in ());
    }

  int pid = this->process_mgr_.spawn (proc_opts);
  if (pid == ACE_INVALID_PID)
    {
      ORBSVCS_ERROR ((LM_ERROR,
        "ImR Activator: Cannot start server <%s> using <%s>\n", name, cmdline));

      throw ImplementationRepository::CannotActivate(
        CORBA::string_dup (
          "Process Creation Failed"));
      return;
    }
  else
    {
      if (debug_ > 1)
        {
          ORBSVCS_DEBUG((LM_DEBUG,
            "ImR Activator: register death handler for process %d\n", pid));
        }
      this->process_mgr_.register_handler (this, pid);
      this->process_map_.rebind (pid, name);

      if (!CORBA::is_nil (this->locator_.in ()))
        {
          if (this->notify_imr_)
            {
              if (debug_ > 1)
                {
                  ORBSVCS_DEBUG ((LM_DEBUG,
                                  ACE_TEXT ("ImR Activator: Notifying ImR that ")
                                  ACE_TEXT ("%s has started.\n"),
                                  name));
                }
              this->locator_->spawn_pid (name, pid);
            }
        }
    }

  if (debug_ > 0)
    {
      ORBSVCS_DEBUG ((LM_DEBUG, "ImR Activator: Successfully started <%s>, pid=%d\n", name, pid));
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

      if (!CORBA::is_nil (this->locator_.in ()))
        {
          if (this->notify_imr_)
            {
              if (debug_ > 1)
                {
                  ORBSVCS_DEBUG ((LM_DEBUG,
                                  ACE_TEXT ("ImR Activator: Notifying ImR that ")
                                  ACE_TEXT ("%s has exited.\n"),
                                  name.c_str()));
                }
              this->locator_->child_death_pid (name.c_str(), pid);
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
        ACE_TEXT ("Process %d exited with exit code %d\n"),
        process->getpid (), process->return_value ()));
    }

  if (this->induce_delay_ > 0)
    {
      ACE_Reactor *r = this->orb_->orb_core()->reactor();
      ACE_Time_Value dtv (0, this->induce_delay_ * 1000);
      pid_t pid = process->getpid();
#if (ACE_SIZEOF_VOID_P == 8)
      ACE_INT64 token = static_cast<ACE_INT64>(pid);
#else
      ACE_INT32 token = static_cast<ACE_INT32>(pid);
#endif

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
#if (ACE_SIZEOF_VOID_P == 8)
  ACE_INT64 token = reinterpret_cast<ACE_INT64>(tok);
#else
  ACE_INT32 token = reinterpret_cast<ACE_INT32>(tok);
#endif
  this->handle_exit_i (static_cast<pid_t>(token));
  return 0;
}
