// $Id$

#include "ace/Service_Config.h"
#include "ace/Service_Repository.h"
#include "ace/Object_Manager.h"
#include "ace/Arg_Shifter.h"

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
#include "StringSeqC.h"

#include "Object_Loader.h"
#include "ace/Dynamic_Service.h"

#include "Default_Stub_Factory.h"
#include "Default_Endpoint_Selector_Factory.h"
#include "Default_Protocols_Hooks.h"
#include "Default_Thread_Lane_Resources_Manager.h"
#include "Default_Collocation_Resolver.h"

ACE_RCSID (tao,
           TAO_Internal,
           "$Id$")

#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP) && \
    defined (TAO_DEFAULT_RESOURCE_FACTORY_ARGS)
const char *TAO_Internal::resource_factory_args_ =
            TAO_DEFAULT_RESOURCE_FACTORY_ARGS;
#else
const char *TAO_Internal::resource_factory_args_ = 0;
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP &&
          TAO_DEFAULT_RESOURCE_FACTORY_ARGS */

#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP) && \
    defined (TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS)
const char *TAO_Internal::server_strategy_factory_args_ =
            TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS;
#else
const char *TAO_Internal::server_strategy_factory_args_ = 0;
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP &&
          TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS */

#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP) && \
    defined (TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS)
const char *TAO_Internal::client_strategy_factory_args_ =
            TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS;
#else
const char *TAO_Internal::client_strategy_factory_args_ = 0;
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP &&
          TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS */

int TAO_Internal::service_open_count_ = 0;

int
TAO_Internal::open_services (int &argc, char **argv)
{
  // Construct an argument vector specific to the Service
  // Configurator.
  CORBA::StringSeq svc_config_argv;

  // Be certain to copy the program name so that service configurator
  // has something to skip!
  const char *argv0 = "";
  if (argc > 0 && argv != 0)
    argv0 = argv[0];

  CORBA::ULong len = 0;
  svc_config_argv.length (1);
  svc_config_argv[0] = argv0;

  // Should we skip the <ACE_Service_Config::open> method, e.g., if we
  // already being configured by the ACE Service Configurator.
  int skip_service_config_open = 0;

  // Extract the Service Configurator ORB options from the argument
  // vector.
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const char *current_arg = 0;

      // Start with the parameterless flags.
      if (arg_shifter.cur_arg_strncasecmp
          ("-ORBSkipServiceConfigOpen") == 0)
        {
          skip_service_config_open = 1;

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-ORBDebug") == 0)
        {
          // later, replace all of these
          // warning this turns on a daemon
          ACE::debug (1);
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBDebugLevel")))
        {
          TAO_debug_level =
            ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-ORBDaemon") == 0)
        {
          // Be a daemon

          len = svc_config_argv.length ();
          svc_config_argv.length (len + 1);

          svc_config_argv[len] = CORBA::string_dup ("-b");

          arg_shifter.consume_arg ();
        }
      // Continue with flags that accept parameters.
      else if ((current_arg = arg_shifter.get_the_parameter ("-ORBSvcConfDirective")))
        {
          len = svc_config_argv.length ();
          svc_config_argv.length (len + 2);  // 2 arguments to add

          // This is used to pass arguments to the Service
          // Configurator using the "command line" to provide
          // configuration information rather than using a svc.conf
          // file.  Pass the "-S" to the service configurator.
          svc_config_argv[len] = CORBA::string_dup ("-S");
          svc_config_argv[len + 1] = CORBA::string_dup (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-ORBSvcConf")))
        {
          // Specify the name of the svc.conf file to be used.

          // Proceeds only if the configuration file exists.
          FILE *conf_file = ACE_OS::fopen (current_arg, "r");
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
            ACE_OS::fclose (conf_file);

          len = svc_config_argv.length ();
          svc_config_argv.length (len + 2);  // 2 arguments to add

          svc_config_argv[len] = CORBA::string_dup ("-f");
          svc_config_argv[len + 1] = CORBA::string_dup (current_arg);

          arg_shifter.consume_arg();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-ORBServiceConfigLoggerKey")))
        {
          len = svc_config_argv.length ();
          svc_config_argv.length (len + 2);  // 2 arguments to add

          svc_config_argv[len] = CORBA::string_dup ("-k");
          svc_config_argv[len + 1] = CORBA::string_dup (current_arg);

          arg_shifter.consume_arg ();
        }
      // Can't interpret this argument.  Move on to the next argument.
      else
        // Any arguments that don't match are ignored so that the
        // caller can still use them.
        arg_shifter.ignore_arg ();
    }

  int svc_config_argc = svc_config_argv.length ();
  return TAO_Internal::open_services_i (svc_config_argc,
                                        svc_config_argv.get_buffer (),
                                        0,  // @@ What about this argument?
                                        skip_service_config_open);
}


void
TAO_Internal::default_svc_conf_entries (const char *resource_factory_args,
                                        const char *server_strategy_factory_args,
                                        const char *client_strategy_factory_args)
{
  TAO_Internal::resource_factory_args_ = resource_factory_args;
  TAO_Internal::server_strategy_factory_args_ = server_strategy_factory_args;
  TAO_Internal::client_strategy_factory_args_ = client_strategy_factory_args;
}

int
TAO_Internal::open_services_i (int &argc,
                               char **argv,
                               int ignore_default_svc_conf_file,
                               int skip_service_config_open)
{
  ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                            guard, *ACE_Static_Object_Lock::instance (),
                            -1));

#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
  ignore_default_svc_conf_file = 1;
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */

  if (TAO_Internal::service_open_count_++ == 0)
    {
      ACE_Service_Config::process_directive (ace_svc_desc_TAO_Default_Resource_Factory);
      ACE_Service_Config::process_directive (ace_svc_desc_TAO_Default_Client_Strategy_Factory);
      ACE_Service_Config::process_directive (ace_svc_desc_TAO_Default_Server_Strategy_Factory);

      // Configure the IIOP factory. You do *NOT*
      // need modify this code to add your own protocol, instead
      // simply add the following to your svc.conf file:
      //
      // dynamic PN_Factory Service_Object * LIB:_make_PN_Protocol_Factory() ""
      // static Resource_Factory "-ORBProtocolFactory PN_Factory"
      //
      // where PN is the name of your protocol and LIB is the base
      // name of the shared library that implements the protocol.
      ACE_Service_Config::process_directive (ace_svc_desc_TAO_IIOP_Protocol_Factory);

      // add descriptor to list of static objects.
      ACE_Service_Config::process_directive (ace_svc_desc_TAO_MCAST_Parser);
      ACE_Service_Config::process_directive (ace_svc_desc_TAO_CORBANAME_Parser);
      ACE_Service_Config::process_directive (ace_svc_desc_TAO_CORBALOC_Parser);
      ACE_Service_Config::process_directive (ace_svc_desc_TAO_FILE_Parser);
      ACE_Service_Config::process_directive (ace_svc_desc_TAO_DLL_Parser);
      ACE_Service_Config::process_directive (ace_svc_desc_TAO_Default_Stub_Factory);
      ACE_Service_Config::process_directive (ace_svc_desc_TAO_Default_Endpoint_Selector_Factory);
      ACE_Service_Config::process_directive (ace_svc_desc_TAO_Default_Protocols_Hooks);
      ACE_Service_Config::process_directive (ace_svc_desc_TAO_Default_Thread_Lane_Resources_Manager_Factory);
      ACE_Service_Config::process_directive (ace_svc_desc_TAO_Default_Collocation_Resolver);

      int result = 0;

      if (skip_service_config_open == 0)
        {
            result = ACE_Service_Config::open (argc, argv,
                                               ACE_DEFAULT_LOGGER_KEY,
                                               0, // Don't ignore static services.
                                               ignore_default_svc_conf_file);
        }

      // Handle RTCORBA library special case.  Since RTCORBA needs
      // its init method call to register several hooks, call it here
      // if it hasn't already been called.
      ACE_Service_Object *rt_loader =
        ACE_Dynamic_Service<ACE_Service_Object>::instance ("RT_ORB_Loader");
      if (rt_loader != 0)
          rt_loader->init (0, 0);

      // @@ What the heck do these things do and do we need to avoid
      // calling them if we're not invoking the svc.conf file?
      if (TAO_Internal::resource_factory_args_ != 0)
        ACE_Service_Config::process_directive (TAO_Internal::resource_factory_args_);
      if (TAO_Internal::client_strategy_factory_args_ != 0)
        ACE_Service_Config::process_directive (TAO_Internal::client_strategy_factory_args_);
      if (TAO_Internal::server_strategy_factory_args_ != 0)
        ACE_Service_Config::process_directive (TAO_Internal::server_strategy_factory_args_);
      return result;
    }
  else
    return 0;
}

TAO_Internal::TAO_Internal (void)
{
}

int
TAO_Internal::close_services (void)
{
  ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX, guard,
                            *ACE_Static_Object_Lock::instance (), -1));
  --service_open_count_;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<ACE_Service_Object>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<ACE_Service_Object>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
