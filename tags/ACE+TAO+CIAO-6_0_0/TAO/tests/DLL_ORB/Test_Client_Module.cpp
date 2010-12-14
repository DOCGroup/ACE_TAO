// -*- C++ -*-
// $Id$

#include "Test_Client_Module.h"
#include "tao/TAO_Singleton_Manager.h"
#include "tao/StringSeqC.h"

#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  -k <ior>\n"),
                          -1);
      }

  // Indicates successful parsing of the command line
  return 0;
}

int
Test_Client_Module::init (int argc, ACE_TCHAR *argv[])
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
                       "Test_Client_Module::init -- ORB pre-initialization "
                       "failed."),
                      -1);  // No exceptions available yet, so return
                            // an error status.

  // -----------------------------------------------------------------
  // Boilerplate CORBA/TAO client-side ORB initialization code.
  // -----------------------------------------------------------------
  try
    {
      // Prepend a "dummy" program name argument to the Service
      // Configurator argument vector.
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
                                    "CLIENT");

      if (CORBA::is_nil (this->orb_.in ()))
        return -1;

      ACE_Argv_Type_Converter converter (new_argc, new_argv.get_buffer ());

      if (::parse_args (new_argc, converter.get_TCHAR_argv ()) != 0)
        return 1;

      CORBA::Object_var obj =
        this->orb_->string_to_object (ior);

      this->test_ =
        Test::_narrow (obj.in ());

      if (CORBA::is_nil (this->test_.in ()))
        {
          ACE_ERROR_RETURN ((LM_DEBUG,
                             ACE_TEXT ("Nil Test reference <%s>\n"),
                             ior),
                            1);
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Test_Client_Module::init"));

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
Test_Client_Module::fini (void)
{
  ACE_DEBUG ((LM_INFO,
              "Client is being finalized.\n"));

  // ------------------------------------------------------------
  // Pre-Test_Client_Module termination steps.
  // ------------------------------------------------------------
  // Explicitly clean up singletons and other objects created by TAO
  // before unloading this module.
  if (TAO_Singleton_Manager::instance ()->fini () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Test_Client_Module::fini -- ORB pre-termination "
                       "failed."),
                      -1);

  return 0;
}

int
Test_Client_Module::svc (void)
{
  try
    {
      // Invoke an operation on the Test object.
      this->test_->invoke_me ();

      /// Shutdown the remote ORB.
      this->test_->shutdown ();

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

      // This is a bit of a hack.  The ORB Core's lifetime is tied to the
      // lifetime of an object reference.  We need to wipe out all object
      // references before we call fini() on the TAO_Singleton_Manager.
      //
      // Note that this is only necessary if the default resource factory
      // is used, i.e. one isn't explicitly loaded prior to initializing
      // the ORB.
      (void) this->test_.out ();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Test_Client_Module::svc"));
      return -1;
    }

  return 0;
}


ACE_STATIC_SVC_DEFINE (Test_Client_Module,
                       ACE_TEXT ("Client_Module"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (Test_Client_Module),
                       ACE_Service_Type::DELETE_THIS
                       | ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (Test_Client_Module, Test_Client_Module)
