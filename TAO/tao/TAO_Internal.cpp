// $Id$

#include "TAO_Internal.h"
#include "default_server.h"
#include "default_client.h"
#include "default_resource.h"
#include "IIOP_Factory.h"
#include "MCAST_Parser.h"
#include "CORBANAME_Parser.h"
#include "CORBALOC_Parser.h"
#include "FILE_Parser.h"
#include "DLL_Parser.h"
#include "ORB_Core.h"
#include "Adapter_Factory.h"
#include "Default_Stub_Factory.h"
#include "Default_Endpoint_Selector_Factory.h"
#include "Default_Protocols_Hooks.h"
#include "Default_Thread_Lane_Resources_Manager.h"
#include "Default_Collocation_Resolver.h"
#include "debug.h"
#include "StringSeqC.h"

#include "ace/Dynamic_Service.h"
#include "ace/Arg_Shifter.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/Env_Value_T.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Static_Object_Lock.h"

ACE_RCSID (tao,
           TAO_Internal,
           "$Id$")


#ifndef TAO_DEFAULT_RESOURCE_FACTORY_ARGS
#  define TAO_DEFAULT_RESOURCE_FACTORY_ARGS 0
#endif  /* !TAO_DEFAULT_RESOURCE_FACTORY_ARGS */

#ifndef TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS
#  define TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS 0
#endif  /* !TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS */

#ifndef TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS
#  define TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS 0
#endif  /* !TAO_DEFAULT_RESOURCE_FACTORY_ARGS */


namespace
{
  /**
   * Initialize the ACE Service Configurator.  This is a one-shot
   * method, i.e., it can be called multiple times but it will only do
   * its work once.  It does, however, track the number of times it's
   * called (see @c open_services()).  It is fully thread-safe.
   *
   * @return @c 0 if successful, @c -1 with @c errno set if failure.
   *
   * @note You can provide your program a set of default `svc.conf'
   *       entries by setting @a ignore_default_svc_conf_file to
   *       non-zero and use @c default_svc_conf_entries() before
   *       calling @c open_services().  In addition, you can @a
   *       skip_service_config_open altogether, which used to be
   *       important when the ORB is linked in via the
   *       ACE_Service_Configurator, since the
   *       ACE_Service_Configurator was non-reentrant.  However, the
   *       ACE_Service_Configurator is now reentrant meaning that it
   *       is really no longer necessary to do so.
   */
  int open_services_i (int & argc,
                       char ** argv,
                       bool ignore_default_svc_conf_file = false,
                       bool skip_service_config_open = false);

  /// Number of times open_services() has been called.  Incremented by
  /// open_services(), and decremented by close_services().
  /**
   * @note In/decrement operations are atomic.
   */
  long service_open_count = 0;

  char const * resource_factory_args =
    TAO_DEFAULT_RESOURCE_FACTORY_ARGS;
  char const * server_strategy_factory_args =
    TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS;
  char const * client_strategy_factory_args =
    TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS;
}

int
TAO::ORB::open_services (int &argc, ACE_TCHAR **argv)
{
  // Construct an argument vector specific to the Service
  // Configurator.
  CORBA::StringSeq svc_config_argv;

  // Be certain to copy the program name so that service configurator
  // has something to skip!
  ACE_CString argv0 ("");

  if (argc > 0 && argv != 0)
    {
      argv0 = ACE_TEXT_ALWAYS_CHAR (argv[0]);
    }

  CORBA::ULong len = 0;
  svc_config_argv.length (1);
  svc_config_argv[0] = argv0.c_str ();

  // Should we skip the ACE_Service_Config::open() method, e.g., if we
  // already being configured by the ACE Service Configurator.
  //
  //   @@ This is no longer needed since the Service Configurator is
  //      now reentrant.
  //         -Ossama
  bool skip_service_config_open = false;

#if defined (TAO_DEBUG) && !defined (ACE_HAS_WINCE)
  // Make it a little easier to debug programs using this code.
  {
    TAO_debug_level = ACE_Env_Value<u_int> ("TAO_ORB_DEBUG", 0);

    char * const value = ACE_OS::getenv ("TAO_ORB_DEBUG");

    if (value != 0)
      {
        TAO_debug_level = ACE_OS::atoi (value);

        if (TAO_debug_level <= 0)
          {
            TAO_debug_level = 1;
          }

        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO_debug_level == %d\n"),
                    TAO_debug_level));
      }
  }
#endif  /* TAO_DEBUG && !ACE_HAS_WINCE */

  // Extract the Service Configurator ORB options from the argument
  // vector.
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *current_arg = 0;

      // Start with the parameterless flags.
      if (arg_shifter.cur_arg_strncasecmp
          (ACE_TEXT ("-ORBSkipServiceConfigOpen")) == 0)
        {
          skip_service_config_open = true;

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT ("-ORBDebug")) == 0)
        {
          // later, replace all of these
          // warning this turns on a daemon
          ACE::debug (1);
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT ("-ORBDebugLevel"))))
        {
          TAO_debug_level =
            ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT ("-ORBDaemon")) == 0)
        {
          // Be a daemon

          len = svc_config_argv.length ();
          svc_config_argv.length (len + 1);

          svc_config_argv[len] = CORBA::string_dup ("-b");

          arg_shifter.consume_arg ();
        }
      // Continue with flags that accept parameters.
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT ("-ORBSvcConfDirective"))))
        {
          len = svc_config_argv.length ();
          svc_config_argv.length (len + 2);  // 2 arguments to add

          // This is used to pass arguments to the Service
          // Configurator using the "command line" to provide
          // configuration information rather than using a svc.conf
          // file.  Pass the "-S" to the service configurator.
          svc_config_argv[len] = CORBA::string_dup ("-S");
          svc_config_argv[len + 1] = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(current_arg));

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT ("-ORBSvcConf"))))
        {
          // Specify the name of the svc.conf file to be used.

          // Proceeds only if the configuration file exists.
          FILE * const conf_file = ACE_OS::fopen (current_arg, ACE_TEXT ("r"));

          if (conf_file == 0)
            {
              // Assigning EINVAL to errno to make an exception
              // thrown.  calling code does not throw an exception if
              // the errno is set to ENOENT for some reason.
              errno = EINVAL;

              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("TAO (%P|%t) Service Configurator ")
                                 ACE_TEXT ("unable to open file %s\n"),
                                           current_arg),
                                -1);

            }
          else
            {
              ACE_OS::fclose (conf_file);
            }

          len = svc_config_argv.length ();
          svc_config_argv.length (len + 2);  // 2 arguments to add

          svc_config_argv[len] = CORBA::string_dup ("-f");
          svc_config_argv[len + 1] = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(current_arg));

          arg_shifter.consume_arg();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT ("-ORBServiceConfigLoggerKey"))))
        {
          len = svc_config_argv.length ();
          svc_config_argv.length (len + 2);  // 2 arguments to add

          svc_config_argv[len] = CORBA::string_dup ("-k");
          svc_config_argv[len + 1] = CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR(current_arg));

          arg_shifter.consume_arg ();
        }
      // Can't interpret this argument.  Move on to the next argument.
      else
        {
          // Any arguments that don't match are ignored so that the
          // caller can still use them.
          arg_shifter.ignore_arg ();
        }
    }

  int svc_config_argc = svc_config_argv.length ();
  return open_services_i (svc_config_argc,
                          svc_config_argv.get_buffer (),
                          0,  // @@ What about this argument?
                          skip_service_config_open);
}

int
TAO::ORB::close_services (void)
{
  ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                            guard,
                            *ACE_Static_Object_Lock::instance (),
                            -1));

  --service_open_count;

  return 0;
}

void
TAO::ORB::default_svc_conf_entries (char const * rf_args,
                                    char const * ssf_args,
                                    char const * csf_args)
{
  resource_factory_args        = rf_args;
  server_strategy_factory_args = ssf_args;
  client_strategy_factory_args = csf_args;
}

// -----------------------------------------------------
namespace
{
  int
  open_services_i (int & argc,
                   char ** argv,
                   bool ignore_default_svc_conf_file,
                   bool skip_service_config_open)
  {
#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
    ignore_default_svc_conf_file = true;
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */

    {
      ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                                guard,
                                *ACE_Static_Object_Lock::instance (),
                                -1));

      if (service_open_count++ != 0)  // Atomic increment
        return 0;
    }

    ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_Default_Resource_Factory);
    ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_Default_Client_Strategy_Factory);
    ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_Default_Server_Strategy_Factory);

    // Configure the IIOP factory. You do *NOT* need modify this
    // code to add your own protocol, instead simply add the
    // following to your svc.conf file:
    //
    // dynamic PN_Factory Service_Object * LIB:_make_PN_Protocol_Factory() ""
    // static Resource_Factory "-ORBProtocolFactory PN_Factory"
    //
    // where PN is the name of your protocol and LIB is the base
    // name of the shared library that implements the protocol.
    ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_IIOP_Protocol_Factory);

    // add descriptor to list of static objects.
    ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_MCAST_Parser);
    ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_CORBANAME_Parser);
    ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_CORBALOC_Parser);
    ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_FILE_Parser);
    ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_DLL_Parser);
    ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_Default_Stub_Factory);
    ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_Default_Endpoint_Selector_Factory);
    ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_Default_Protocols_Hooks);
    ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_Default_Thread_Lane_Resources_Manager_Factory);
    ACE_Service_Config::process_directive (
      ace_svc_desc_TAO_Default_Collocation_Resolver);

    int result = 0;

    if (!skip_service_config_open)
      {
        // Copy command line parameter not to use original.
        ACE_Argv_Type_Converter command_line (argc, argv);

        result =
          ACE_Service_Config::open (command_line.get_argc(),
                                    command_line.get_TCHAR_argv(),
                                    ACE_DEFAULT_LOGGER_KEY,
                                    0, // Don't ignore static services.
                                    ignore_default_svc_conf_file);
      }

    // If available, allow the Adapter Factory to setup.
    ACE_Service_Object *adapter_factory =
      ACE_Dynamic_Service<TAO_Adapter_Factory>::instance (
        TAO_ORB_Core::poa_factory_name ().c_str());

    if (adapter_factory != 0)
      {
        adapter_factory->init (0, 0);
      }

    ACE_Service_Object * const pi_server_loader =
    ACE_Dynamic_Service<ACE_Service_Object>::instance ("PI_Server_Loader");

    if (pi_server_loader != 0)
      {
        pi_server_loader->init (0, 0);
      }

    ACE_Service_Object * const messaging_loader =
    ACE_Dynamic_Service<ACE_Service_Object>::instance ("Messaging_Loader");

    if (messaging_loader != 0)
      {
        messaging_loader->init (0, 0);
      }

    // Handle RTCORBA library special case.  Since RTCORBA needs
    // its init method call to register several hooks, call it
    // here if it hasn't already been called.
    ACE_Service_Object * const rt_loader =
      ACE_Dynamic_Service<ACE_Service_Object>::instance ("RT_ORB_Loader");

    if (rt_loader != 0)
      {
        rt_loader->init (0, 0);
      }

    ACE_Service_Object * const rtscheduler_loader =
      ACE_Dynamic_Service<ACE_Service_Object>::instance ("RTScheduler_Loader");

    if (rtscheduler_loader != 0)
      {
        rtscheduler_loader->init (0, 0);
      }

    // @@ What the heck do these things do and do we need to avoid
    //    calling them if we're not invoking the svc.conf file?
    // @@ They are needed for platforms that have no file system,
    //    like VxWorks.
    if (resource_factory_args != 0)
      {
        ACE_Service_Config::process_directive (
          ACE_TEXT_CHAR_TO_TCHAR (resource_factory_args));
      }

    if (client_strategy_factory_args != 0)
      {
        ACE_Service_Config::process_directive (
          ACE_TEXT_CHAR_TO_TCHAR (client_strategy_factory_args));
      }

    if (server_strategy_factory_args != 0)
      {
        ACE_Service_Config::process_directive (
          ACE_TEXT_CHAR_TO_TCHAR (server_strategy_factory_args));
      }

    return result;
  }
}

