// -*- C++ -*-
// $Id$

#include "Test_Server_Module.h"
#include "tao/TAO_Singleton_Manager.h"
#include "tao/StringSeqC.h"

#include "ace/Service_Config.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/Get_Opt.h"


const ACE_TCHAR *ior_file = ACE_TEXT("test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':

        ior_file = get_opts.opt_arg ();
        break;

      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage: -o IOR\n"),
                          -1);
      }

  return 0;
}

Test_Server_Module::~Test_Server_Module (void)
{
}

int
Test_Server_Module::init (int argc, ACE_TCHAR *argv[])
{
  // -----------------------------------------------------------------
  // Pre-ORB initialization steps necessary for proper DLL ORB
  // support.
  // -----------------------------------------------------------------
  // Make sure TAO's singleton manager is initialized, and set to not
  // register itself with the ACE_Object_Manager since it is under the
  // control of the Service Configurator.  If we register with the
  // ACE_Object_Manager, then the ACE_Object_Manager will still hold
  // (dangling) references to instances of objects created by this
  // module and destroyed by this object when it is dynamically
  // unloaded.
  int register_with_object_manager = 0;

  if (TAO_Singleton_Manager::instance ()->init (
        register_with_object_manager) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Test_Server_Module::init -- ORB pre-initialization "
                       "failed."),
                      -1);  // No CORBA exceptions available yet, so
                            // return an error status.


  // -----------------------------------------------------------------
  // Boilerplate CORBA/TAO server-side ORB initialization code.
  // -----------------------------------------------------------------
  try
    {
      // Add one to the new argc since "dummy" is being added to the
      // argv vector.
      int new_argc = argc + 1;

      CORBA::StringSeq new_argv (new_argc);
      new_argv.length (new_argc);

      // Prevent the ORB from opening the Service Configurator file
      // again since the Service Configurator file is already in the
      // process of being opened.
      new_argv[0] = CORBA::string_dup ("dummy");

      // Copy the remaining arguments into the new argument vector.
      for (int i = new_argc - argc, j = 0;
           j < argc;
           ++i, ++j)
        new_argv[i] = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(argv[j]));

      // Initialize the ORB.
      this->orb_ = CORBA::ORB_init (new_argc,
                                    new_argv.get_buffer (),
                                    "SERVER");

      if (CORBA::is_nil (this->orb_.in ()))
        return -1;

      CORBA::Object_var poa_object =
        this->orb_->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      this->poa_ =
        PortableServer::POA::_narrow (poa_object.in ());

      this->poa_manager_ = this->poa_->the_POAManager ();

      this->poa_manager_->activate ();

      ACE_Argv_Type_Converter converter (new_argc, new_argv.get_buffer ());
      if (::parse_args (new_argc, converter.get_TCHAR_argv ()) != 0)
        return -1;

      PortableServer::ObjectId_var id =
        this->poa_->activate_object (&servant_);

      CORBA::Object_var obj = this->poa_->id_to_reference (id.in ());

      CORBA::String_var ior =
        this->orb_->object_to_string (obj.in ());

      ACE_DEBUG ((LM_DEBUG,
                  "Servant:\n<%C>\n",
                  ior.in ()));

      // Write IOR to a file.
      FILE *output_file= ACE_OS::fopen (ior_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file <%s> for writing "
                           "IOR: %s",
                           ior.in ()),
                          1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      this->servant_.orb (this->orb_.in ());
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Test_Server_Module::init"));
      return -1;
    }

#if defined (ACE_HAS_THREADS)

  // Become an Active Object so that the ORB will execute in a
  // separate thread.
  return this->activate ();

#else

  return 0;

#endif /* ACE_HAS_THREADS */
}

int
Test_Server_Module::fini (void)
{
  ACE_DEBUG ((LM_INFO,
              "Server is being finalized.\n"));

  // ------------------------------------------------------------
  // Pre-Test_Server_Module termination steps.
  // ------------------------------------------------------------
  // Explicitly clean up singletons created by TAO before
  // unloading this module.
  if (TAO_Singleton_Manager::instance ()->fini () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Test_Server_Module::fini -- ORB pre-termination "
                       "failed."),
                      -1);

  return 0;
}

int
Test_Server_Module::svc (void)
{
  try
    {
      // Run the ORB event loop in its own thread.
      this->orb_->run ();

      ACE_DEBUG ((LM_INFO,
                  "Server is being destroyed.\n"));

      // Make sure the ORB is destroyed here - before the thread
      // exits, because it may be holding global resources, owned by
      // the Object Manager (thru its core, which is in turn owned by
      // the ORB table; which is owned by the Object Manager).
      // Otherwise the Object Manager will have clobbered them by the
      // time it gets to destroy the ORB Table, which calls our
      // fini(). Had we destroyed the ORB in our fini(), its core
      // fininalization would have required access to those already
      // deleted resources.
      if (!CORBA::is_nil (this->orb_.in ()))
        {
          this->orb_->destroy ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Test_Server_Module::svc"));
      return -1;
    }

  return 0;
}


ACE_STATIC_SVC_DEFINE (Test_Server_Module,
                       ACE_TEXT ("Test_Server_Module"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Test_Server_Module),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (Test_Server_Module, Test_Server_Module)
