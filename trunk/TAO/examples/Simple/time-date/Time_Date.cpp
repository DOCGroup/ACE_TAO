// $Id$

#define ACE_BUILD_SVC_DLL
#define Alt_Resource_Factory_BUILD_DLL

#include "Time_Date.h"
#include "tao/TAO_Singleton_Manager.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Dynamic_Service.h"

ACE_Reactor *
My_Resource_Factory::get_reactor (void)
{
#if defined (ACE_HAS_THREADS)
  // Use whatever the default is if we've got threads.
  return TAO_Default_Resource_Factory::get_reactor ();
#else
  // Just use the Singleton Reactor.
  return ACE_Reactor::instance ();
#endif /* ACE_HAS_THREADS */
}

ACE_FACTORY_DEFINE (Alt_Resource_Factory, My_Resource_Factory)

int
DLL_ORB::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "\n\trunning ORB event loop (%t)\n\n"));

  ACE_TRY_NEW_ENV
    {
      // Run the ORB event loop in its own thread.
      this->orb_manager_.run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "System Exception");
      return -1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, "User Exception");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

int
DLL_ORB::init (int argc, char *argv[])
{
  // Prevent TAO from registering with the ACE_Object_Manager so
  // that it can be dynamically unloaded successfully.
  int register_with_object_manager = 0;
  if (TAO_Singleton_Manager::instance ()->init (
        register_with_object_manager) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "%p\n",
                       "Unable to pre-initialize ORB"),
                      -1);

  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n\tInitialize ORB (%t)\n\n"));

      // Initialize the ORB.
      this->orb_manager_.init (argc,
                               argv
                               TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

#if defined (ACE_HAS_THREADS)
      // Become an Active Object so that the ORB
      // will execute in a separate thread.
      return this->activate ();
#else
      return 0;
#endif /* ACE_HAS_THREADS */
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "DLL_ORB::init");
    }
  ACE_ENDTRY;
  return -1;
}

int
DLL_ORB::fini (void)
{
  return TAO_Singleton_Manager::instance ()->fini ();
}

int
Time_Date_Servant::parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "dn:o:");
  int c = 0;

  this->orb_ = "ORB";

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag.
        TAO_debug_level++;
        break;
      case 'o':  // output the IOR to a file.
        this->ior_output_file_ = ACE_OS::fopen (get_opts.opt_arg (), "w");
        if (this->ior_output_file_ == 0)
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             get_opts.opt_arg ()), -1);
        break;
        // Find the ORB in the Service Repository.
      case 'n':
        this->orb_ = get_opts.opt_arg ();
        break;
      case '?':  // display help for use of the server.
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-o] <ior_output_file>"
                           "\n",
                           argv [0]),
                          -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

int
Time_Date_Servant::init (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\n\tTime_Date servant\n\n"));

      this->parse_args (argc, argv);

      DLL_ORB *orb =
        ACE_Dynamic_Service<DLL_ORB>::instance (this->orb_);

      if (orb == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "can't find %s in the Service Repository\n",
                           this->orb_),
                          -1);

      CORBA::String_var str = orb->orb_manager_.activate (&servant_
                                                          TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (this->ior_output_file_)
        {
          ACE_OS::fprintf (this->ior_output_file_,
                           "%s",
                           str.in ());
          ACE_OS::fclose (this->ior_output_file_);
        }

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "DLL_ORB::init");
      return -1;
    }
  ACE_ENDTRY;
  return 0;
}

// The following Factory is used by the <ACE_Service_Config> and
// dll_orb.conf file to dynamically initialize the state of the
// Time_Date service.
ACE_SVC_FACTORY_DEFINE (DLL_ORB)
ACE_SVC_FACTORY_DEFINE (Time_Date_Servant)

# if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<DLL_ORB>;

# elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<DLL_ORB>

# endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
