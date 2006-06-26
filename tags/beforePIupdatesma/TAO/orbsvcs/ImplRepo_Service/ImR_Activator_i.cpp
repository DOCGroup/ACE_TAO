// $Id$

#include "ImR_Activator_i.h"

#include "Activator_Options.h"

#include "tao/ORB_Core.h"

#include "ace/Reactor.h"
#include "ace/ARGV.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_stdio.h"
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
, name_ (getHostName ())
{
}

static PortableServer::POA_ptr
createPersistentPOA (PortableServer::POA_ptr root_poa, const char* poa_name ACE_ENV_ARG_DECL)
{
  PortableServer::LifespanPolicy_var life =
    root_poa->create_lifespan_policy (PortableServer::PERSISTENT ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  PortableServer::IdAssignmentPolicy_var assign =
    root_poa->create_id_assignment_policy (PortableServer::USER_ID ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  CORBA::PolicyList pols;
  pols.length (2);
  pols[0] = PortableServer::LifespanPolicy::_duplicate (life.in ());
  pols[1] = PortableServer::IdAssignmentPolicy::_duplicate (assign.in ());

  PortableServer::POAManager_var mgr = root_poa->the_POAManager ();
  PortableServer::POA_var poa =
    root_poa->create_POA(poa_name, mgr.in (), pols ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(PortableServer::POA::_nil ());

  life->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());
  assign->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (PortableServer::POA::_nil ());

  return poa._retn ();
}

// It's ok if we can't register with the ImR. It just
// means we won't be able to notify it of any events
// (Currently, just that we're shutting down.)
void
ImR_Activator_i::register_with_imr (ImplementationRepository::Activator_ptr activator)
{
  ACE_TRY_NEW_ENV
    {
      if (this->debug_ > 1)
        ACE_DEBUG( (LM_DEBUG, "ImR Activator: Contacting ImplRepoService...\n"));

      // First, resolve the ImR, without this we can go no further
      CORBA::Object_var obj =
        orb_->resolve_initial_references ("ImplRepoService" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      this->process_mgr_.open (ACE_Process_Manager::DEFAULT_SIZE,
        this->orb_->orb_core ()->reactor ());

      locator_ = ImplementationRepository::Locator::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (!CORBA::is_nil (locator_.in ()))
        {
          this->registration_token_ =
            locator_->register_activator (name_.c_str (), activator ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          if (debug_ > 0)
            ACE_DEBUG((LM_DEBUG, "ImR Activator: Registered with ImR.\n"));

          return;
        }
    }
  ACE_CATCHANY
    {
      if (debug_ > 1)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ImR Activator: Can't register with ImR.");
    }
  ACE_ENDTRY;

  if (debug_ > 0)
    ACE_DEBUG ((LM_DEBUG, "ImR Activator: Not registered with ImR.\n"));
}

int
ImR_Activator_i::init_with_orb (CORBA::ORB_ptr orb, const Activator_Options& opts ACE_ENV_ARG_DECL)
{
  ACE_ASSERT(! CORBA::is_nil (orb));
  orb_ = CORBA::ORB::_duplicate (orb);
  debug_ = opts.debug ();
  notify_imr_ = opts.notify_imr ();
  if (opts.name ().length () > 0)
    {
      name_ = opts.name();
    }

  ACE_TRY
    {
      CORBA::Object_var obj = orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_ASSERT (! CORBA::is_nil (obj.in ()));
      this->root_poa_ = PortableServer::POA::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_ASSERT (! CORBA::is_nil(this->root_poa_.in ()));

      // The activator must use a persistent POA so that it can be started before the
      // locator in some scenarios, such as when the locator persists its database, and
      // wants to reconnect to running activators to auto_start some servers.
      this->imr_poa_ = createPersistentPOA (this->root_poa_.in (),
        "ImR_Activator" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_ASSERT (! CORBA::is_nil(this->imr_poa_.in ()));

      // Activate ourself
      PortableServer::ObjectId_var id = PortableServer::string_to_ObjectId ("ImR_Activator");
      this->imr_poa_->activate_object_with_id (id.in (), this ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      obj = this->imr_poa_->id_to_reference (id.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ImplementationRepository::Activator_var activator =
        ImplementationRepository::Activator::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_ASSERT(! CORBA::is_nil (activator.in ()));

      CORBA::String_var ior = this->orb_->object_to_string (activator.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (this->debug_ > 0)
        ACE_DEBUG((LM_DEBUG, "ImR Activator: Starting %s\n", name_.c_str ()));

      // initialize our process manager.
      // This requires a reactor that has signal handling.
      ACE_Reactor *reactor = ACE_Reactor::instance ();
      if (reactor != 0)
        {
          if (this->process_mgr_.open (ACE_Process_Manager::DEFAULT_SIZE, reactor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                "The ACE_Process_Manager didnt get initialized\n"), -1);
            }
        }

      this->register_with_imr (activator.in ()); // no throw

      PortableServer::POAManager_var poaman =
        this->root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
      poaman->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (this->debug_ > 1)
        {
          ACE_DEBUG ((LM_DEBUG,
            "ImR Activator: The Activator IOR is: <%s>\n", ior.in ()));
        }

      // The last thing we do is write out the ior so that a test program can assume
      // that the activator is ready to go as soon as the ior is written.
      if (opts.ior_filename ().length () > 0)
        {
          FILE* fp = ACE_OS::fopen (opts.ior_filename ().c_str (), "w");
          if (fp == 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                "ImR Activator: Could not open file: %s\n", opts.ior_filename ().c_str ()), -1);
            }
          ACE_OS::fprintf (fp, "%s", ior.in ());
          ACE_OS::fclose (fp);
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ImR_Activator_i::init_with_orb");
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
ImR_Activator_i::init (Activator_Options& opts ACE_ENV_ARG_DECL)
{
  ACE_CString cmdline = opts.cmdline();
  // Must use IOR style objrefs, because URLs sometimes get mangled when passed
  // to ACE_Process::spawn().
  cmdline += "-ORBUseImR 0 -ORBObjRefStyle IOR ";
  ACE_ARGV av (cmdline.c_str ());
  int argc = av.argc ();

  CORBA::ORB_var orb =
    CORBA::ORB_init (argc, av.argv (), "TAO_ImR_Activator" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);

  int ret = this->init_with_orb(orb.in (), opts ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);

  return ret;
}

int
ImR_Activator_i::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY_EX (try_block_1)
    {
      if (debug_ > 1)
        ACE_DEBUG ((LM_DEBUG, "ImR Activator: Shutting down...\n"));

      this->process_mgr_.close ();

      this->root_poa_->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (try_block_1);

      if (! CORBA::is_nil (this->locator_.in ()) && this->registration_token_ != 0)
        {
          this->locator_->unregister_activator (name_.c_str(),
            this->registration_token_ ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK_EX (try_block_1);
        }
    }
  ACE_CATCH(CORBA::COMM_FAILURE, ex)
    {
      if (debug_ > 1)
        ACE_DEBUG ((LM_DEBUG, "ImR Activator: Unable to unregister from ImR.\n"));
    }
  ACE_CATCH(CORBA::TRANSIENT, ex)
    {
      if (debug_ > 1)
        ACE_DEBUG ((LM_DEBUG, "ImR Activator: Unable to unregister from ImR.\n"));
    }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ImR Activator: fini");
    ACE_RE_THROW_EX (try_block_1);
  }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);

  ACE_TRY_EX (try_block_2)
    {
      this->orb_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK_EX (try_block_2);

      if (debug_ > 0)
        ACE_DEBUG ((LM_DEBUG, "ImR Activator: Shut down successfully.\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ImR Activator: fini 2");
      ACE_RE_THROW_EX (try_block_2);
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
ImR_Activator_i::run (ACE_ENV_SINGLE_ARG_DECL)
{
  this->orb_->run (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  return 0;
}

void
ImR_Activator_i::shutdown (ACE_ENV_SINGLE_ARG_DECL)
ACE_THROW_SPEC ((CORBA::SystemException))
{
  shutdown (false ACE_ENV_ARG_PARAMETER);
}

void
ImR_Activator_i::shutdown (bool wait_for_completion ACE_ENV_ARG_DECL)
{
  this->orb_->shutdown (wait_for_completion ACE_ENV_ARG_PARAMETER);
}

void
ImR_Activator_i::start_server(const char* name,
                              const char* cmdline,
                              const char* dir,
                              const ImplementationRepository::EnvironmentList & env ACE_ENV_ARG_DECL)
                              ACE_THROW_SPEC ((CORBA::SystemException, ImplementationRepository::CannotActivate))
{
  if (debug_ > 1)
    ACE_DEBUG((LM_DEBUG, "ImR Activator: Starting server <%s>...\n", name));
  if (debug_ > 1)
    ACE_DEBUG((LM_DEBUG, "\tcommand line : <%s>\n\tdirectory : <%s>\n", cmdline, dir));

  ACE_Process_Options proc_opts;
  proc_opts.command_line (cmdline);
  proc_opts.working_directory (dir);
  // Win32 does not support the CLOSE_ON_EXEC semantics for sockets
  // the way unix does, so in order to avoid having the child process
  // hold the listen socket open, we force the child to inherit no
  // handles. This includes stdin, stdout, logs, etc.
  proc_opts.handle_inheritence (0);

  proc_opts.setenv ("TAO_USE_IMR", "1");
  if (!CORBA::is_nil (this->locator_.in ()))
    {
      CORBA::String_var ior = orb_->object_to_string (locator_.in ());
      proc_opts.setenv ("ImplRepoServiceIOR", ior.in());
    }

  for (CORBA::ULong i = 0; i < env.length (); ++i)
    {
      proc_opts.setenv (env[i].name.in (), env[i].value.in ());
    }

  int pid = this->process_mgr_.spawn (proc_opts);
  if (pid == ACE_INVALID_PID)
    {
      ACE_ERROR ((LM_ERROR,
        "ImR Activator: Cannot start server <%s> using <%s>\n", name, cmdline));

      ACE_THROW(ImplementationRepository::CannotActivate(CORBA::string_dup ("Process Creation Failed")));
      return;
    }
  else
    {
      if (debug_ > 1)
        {
          ACE_DEBUG((LM_DEBUG,
            "ImR Activator: register death handler for process %d\n", pid));
        }
      this->process_mgr_.register_handler (this, pid);

      // We only bind to the process_map_ if we want to notify
      // the locator of a process' death.
      if (notify_imr_)
        {
          this->process_map_.rebind (pid, name);
        }
    }

  if (debug_ > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "ImR Activator: Successfully started <%s>. \n", name));
    }
}

int
ImR_Activator_i::handle_exit (ACE_Process * process)
{
  // We use the process_manager so that we're notified when
  // any of our launched processes die. We notify the locator
  // when this happens.

  if (debug_ > 0)
    {
      ACE_DEBUG
        ((LM_DEBUG,
        ACE_TEXT ("Process %d exited with exit code %d\n"),
        process->getpid (), process->return_value ()));
    }

  ACE_CString name;
  if (this->process_map_.find (process->getpid (), name) == 0)
    {
      this->process_map_.unbind (process->getpid ());

      if (!CORBA::is_nil (this->locator_.in ()))
        {
          if (debug_ > 1)
            {
              ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("ImR Activator: Notifying ImR that %s has exited.\n"),
                name.c_str()));
            }
          this->locator_->notify_child_death (name.c_str());
        }
    }

  return 0;
}
