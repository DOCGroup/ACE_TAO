// "$Id$"

#include "ImR_Activator_i.h"

#include "Activator_Options.h"

#include "tao/ORB_Core.h"

#include "ace/ARGV.h"

#include "ace/OS.h"

static ACE_CString getActivatorName()
{
  char host_name[MAXHOSTNAMELEN];
  ACE_OS::hostname (host_name, MAXHOSTNAMELEN);
  return ACE_CString(host_name);
}

ImR_Activator_i::ImR_Activator_i (void)
: registration_token_(0)
, debug_(0)
, name_(getActivatorName())
{
}

int
ImR_Activator_i::init_with_orb(CORBA::ORB_ptr orb, const Options& opts ACE_ENV_ARG_DECL)
{
  ACE_ASSERT(! CORBA::is_nil(orb));
  orb_ = CORBA::ORB::_duplicate(orb);
  debug_ = opts.debug();

  ACE_TRY
  {
    if (this->debug_ > 0)
      ACE_DEBUG((LM_DEBUG, "ImR Activator: Contacting ImplRepoService...\n"));

    // First, resolve the ImR Locator, without this we can go no further
    CORBA::Object_var obj =
      orb_->resolve_initial_references ("ImplRepoService" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    if (CORBA::is_nil(obj.in ()))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
        "Unable to resolve_initial_references \"ImplRepoService\"\n"), -1);
    }
    locator_ = ImplementationRepository::Locator::_narrow (obj.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_ASSERT(! CORBA::is_nil(locator_.in()));

    // Create a new poa and poa manager
    obj = orb->resolve_initial_references ("RootPOA" ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_ASSERT(! CORBA::is_nil(obj.in()));
    this->root_poa_ = PortableServer::POA::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_ASSERT(! CORBA::is_nil(this->root_poa_.in()));

    // Note : There is no point in making the activator use a persistent POA, because
    // it will always re-register with the ImR on startup anyway.
    CORBA::PolicyList policies;
    this->imr_poa_ = this->root_poa_->create_POA("ImR_Activator",
      PortableServer::POAManager::_nil(), policies ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_ASSERT(! CORBA::is_nil(this->imr_poa_.in()));

    // Activate ourself
    PortableServer::ObjectId_var id =
      this->imr_poa_->activate_object(this ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    obj = this->imr_poa_->id_to_reference (id.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ImplementationRepository::Activator_var activator =
      ImplementationRepository::Activator::_narrow(obj.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;
    ACE_ASSERT(! CORBA::is_nil(activator.in()));

    CORBA::String_var ior = this->orb_->object_to_string(activator.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (this->debug_ > 0)
      ACE_DEBUG((LM_DEBUG, "ImR Activator: Starting %s\n", name_.c_str()));

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

    // Register with the ImR Locator
    this->registration_token_ =
      locator_->register_activator (name_.c_str(), activator.in() ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    // Activate the manager for the root poa
    PortableServer::POAManager_var poaman =
      this->root_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
    poaman->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    // Activate the manager for the activator poa
    poaman = this->imr_poa_->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;
    poaman->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (this->debug_ > 1)
    {
      ACE_DEBUG ((LM_DEBUG,
        "ImR_Activator_i::init: The Activator IOR is: <%s>\n", ior.in ()));
    }

    // The last thing we do is write out the ior so that a test program can assume
    // that the activator is ready to go as soon as the ior is written.
    if (opts.ior_filename().length() > 0)
    {
      FILE* fp = ACE_OS::fopen(opts.ior_filename().c_str(), "w");
      if (fp == 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
          "ImR Activator: Could not open file: %s\n", opts.ior_filename().c_str()), -1);
      }
      ACE_OS::fprintf(fp, "%s", ior.in());
      ACE_OS::fclose(fp);
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
ImR_Activator_i::init (Options& opts ACE_ENV_ARG_DECL)
{
  ACE_ARGV av(opts.cmdline().c_str());
  int argc = av.argc();

  CORBA::ORB_var orb =
    CORBA::ORB_init(argc, av.argv(), "TAO_ImR_Activator" ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);

  int ret = this->init_with_orb(orb.in(), opts ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(-1);

  return ret;
}

int
ImR_Activator_i::fini (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_TRY
  {
    if (debug_ > 0)
      ACE_DEBUG((LM_DEBUG, "ImR Activator: Shutting down...\n"));

    this->root_poa_->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    this->unblock_all_servers();

    this->locator_->unregister_activator (name_.c_str(),
      this->registration_token_ ACE_ENV_ARG_PARAMETER);
    ACE_TRY_CHECK;

    this->orb_->destroy(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK;

    if (debug_ > 0)
      ACE_DEBUG((LM_DEBUG, "ImR Activator: Shut down successfully.\n"));
  }
  ACE_CATCHANY
  {
    ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ImR_Activator_i::fini");
    ACE_RE_THROW;
  }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  return 0;
}

int
ImR_Activator_i::run (ACE_ENV_SINGLE_ARG_DECL)
{
  this->orb_->run(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  return 0;
}

void
ImR_Activator_i::start_server
(
 ImplementationRepository::AMH_ActivatorResponseHandler_ptr rh,
 const char* name,
 const char* cmdline,
 const char* dir,
 const ImplementationRepository::EnvironmentList & env ACE_ENV_ARG_DECL)
 ACE_THROW_SPEC ((CORBA::SystemException))
{
  RHListPtr server;
  waiting_servers_.find(name, server);
  if (server.null())
  {
    server = RHListPtr(new RHList);
    int err = waiting_servers_.bind(name, server);
    ACE_ASSERT(err == 0);
    ACE_UNUSED_ARG(err);
  }

  if (server->size() == 0)
  {
    if (debug_ > 0)
      ACE_DEBUG((LM_DEBUG, "ImR Activator: Spawn server <%s>\n", name));
    if (debug_ > 1)
      ACE_DEBUG((LM_DEBUG, "\tcommand line : <%s>\n\tdirectory : <%s>\n", cmdline, dir));

    // Launch the process using the given command line.
    ACE_Process_Options proc_opts;
    proc_opts.command_line (cmdline);
    proc_opts.working_directory (dir);
    // Win32 does not support the CLOSE_ON_EXEC semantics for sockets
    // the way unix does, so in order to avoid having the child process
    // hold the listen socket open, we force the child to inherit no
    // handles. This includes stdin, stdout, logs, etc.
    proc_opts.handle_inheritence (0);

    for (CORBA::ULong i = 0; i < env.length(); ++i)
      proc_opts.setenv (env[i].name.in(), env[i].value.in());

    int pid = this->process_mgr_.spawn (proc_opts);
    if (pid == ACE_INVALID_PID)
    {
      ACE_ERROR ((LM_ERROR,
        "ImR Activator: Cannot spawn server <%s> using <%s>\n", name, cmdline));

      ImplementationRepository::CannotActivate* ex = new
        ImplementationRepository::CannotActivate(CORBA::string_dup ("Process Creation Failed"));

      ImplementationRepository::AMH_ActivatorExceptionHolder holder(ex);

      ACE_TRY
      {
        rh->start_server_excep (&holder ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
      ACE_CATCHANY
      {
        ACE_ERROR((LM_ERROR, "Exception during asynch exception throw.\n"));
      }
      ACE_ENDTRY;
      return;
    }

    if (debug_ > 0)
    {
      ACE_DEBUG((LM_DEBUG,
        "ImR Activator: Successfully spawned. Waiting for status of <%s>...\n", name));
    }
  }
  else if (debug_ > 0)
  {
    ACE_DEBUG((LM_DEBUG, "ImR Activator: Waiting for status of <%s>...\n", name));
  }

  server->push_back(ImplementationRepository::AMH_ActivatorResponseHandler::_duplicate(rh));
}

void
ImR_Activator_i::server_status_changed
(
 ImplementationRepository::AMH_ActivatorResponseHandler_ptr rh,
 const char* name ACE_ENV_ARG_DECL)
 ACE_THROW_SPEC ((CORBA::SystemException))
{
  // First, send the reply, because the servers server_is_running shouldn't
  // have to wait for all of the waiting clients to return.
  ACE_TRY_EX(ACE_TRY_LABEL2)
  {
    rh->server_status_changed(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_TRY_CHECK_EX(ACE_TRY_LABEL2);
  }
  ACE_CATCHANY
  {
    if (debug_ > 1)
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ImR_Activator_i::server_status_changed() return");
  }
  ACE_ENDTRY;
  ACE_CHECK;

  RHListPtr server;
  this->waiting_servers_.find(name, server);
  if (! server.null() && server->size() > 0)
  {
    this->unblock_server(server);
  }
  else
  {
    if (debug_ > 0)
      ACE_DEBUG((LM_DEBUG, "ImR Activator: No waiting clients.\n"));
  }
}

void
ImR_Activator_i::unblock_all_servers()
{
  ServerMap::ENTRY* entry = 0;
  ServerMap::ITERATOR it(this->waiting_servers_);
  for (; it.next(entry); it.advance())
  {
    ACE_CString name = entry->ext_id_;
    RHListPtr server = entry->int_id_;
    this->unblock_server(server);
  }
  this->waiting_servers_.unbind_all();
}

void
ImR_Activator_i::unblock_server(RHListPtr server)
{
  if (debug_ > 0)
    ACE_DEBUG((LM_DEBUG, "ImR Activator: Unblocking %u waiting clients.\n", server->size()));

  RHList& svr = *server;

  for (size_t i = 0; i < svr.size(); ++i)
  {
    ACE_DECLARE_NEW_CORBA_ENV;
    ACE_TRY
    {
      ImplementationRepository::AMH_ActivatorResponseHandler_var ssrh = svr[i];
      ssrh->start_server(ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
    ACE_CATCHANY
    {
      if (debug_ > 1)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "ImR_Activator_i::unblock_server()");
    }
    ACE_ENDTRY;
  }
  server->clear();
}

//@@ todo : Use the process_manager so that we're notified when any of our launched
//          processes die. Add a new operation to the Locator idl, so that we can
//          notify the locator when this happens.

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Array_Base<ImplementationRepository::AMH_ActivatorResponseHandler_var>;
template class ACE_Array<ImplementationRepository::AMH_ActivatorResponseHandler_var>;
template class ACE_Vector<ImplementationRepository::AMH_ActivatorResponseHandler_var>;

template class ACE_Strong_Bound_Ptr<ACE_Vector<ImplementationRepository::AMH_ActivatorResponseHandler_var>, ACE_Null_Mutex>;

template class ACE_Hash_Map_Entry<ACE_CString, ACE_Strong_Bound_Ptr<ACE_Vector<ImplementationRepository::AMH_ActivatorResponseHandler_var>, ACE_Null_Mutex> >;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, ACE_Strong_Bound_Ptr<ACE_Vector<ImplementationRepository::AMH_ActivatorResponseHandler_var>, ACE_Null_Mutex>, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, ACE_Strong_Bound_Ptr<ACE_Vector<ImplementationRepository::AMH_ActivatorResponseHandler_var>, ACE_Null_Mutex>,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, ACE_Strong_Bound_Ptr<ACE_Vector<ImplementationRepository::AMH_ActivatorResponseHandler_var>, ACE_Null_Mutex>,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, ACE_Strong_Bound_Ptr<ACE_Vector<ImplementationRepository::AMH_ActivatorResponseHandler_var>, ACE_Null_Mutex>,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Array_Base<ImplementationRepository::AMH_ActivatorResponseHandler_var>
#pragma instantiate ACE_Array<ImplementationRepository::AMH_ActivatorResponseHandler_var>
#pragma instantiate ACE_Vector<ImplementationRepository::AMH_ActivatorResponseHandler_var>

#pragma instantiate ACE_Strong_Bound_Ptr<ACE_Vector<ImplementationRepository::AMH_ActivatorResponseHandler_var>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, ACE_Strong_Bound_Ptr<ACE_Vector<ImplementationRepository::AMH_ActivatorResponseHandler_var>, ACE_Null_Mutex> >
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, ACE_Strong_Bound_Ptr<ACE_Vector<ImplementationRepository::AMH_ActivatorResponseHandler_var>, ACE_Null_Mutex> ,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, ACE_Strong_Bound_Ptr<ACE_Vector<ImplementationRepository::AMH_ActivatorResponseHandler_var>, ACE_Null_Mutex> ,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, ACE_Strong_Bound_Ptr<ACE_Vector<ImplementationRepository::AMH_ActivatorResponseHandler_var>, ACE_Null_Mutex> ,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, ACE_Strong_Bound_Ptr<ACE_Vector<ImplementationRepository::AMH_ActivatorResponseHandler_var>, ACE_Null_Mutex> ,ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

