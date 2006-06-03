// $Id$

#include "tao/TAO_Internal.h"
#include "tao/TAO_Singleton.h"
#include "tao/default_server.h"
#include "tao/default_client.h"
#include "tao/default_resource.h"
#include "tao/IIOP_Factory.h"
#include "tao/MCAST_Parser.h"
#include "tao/CORBANAME_Parser.h"
#include "tao/CORBALOC_Parser.h"
#include "tao/FILE_Parser.h"
#include "tao/DLL_Parser.h"
#include "tao/ORB_Core.h"
#include "tao/Adapter_Factory.h"
#include "tao/Default_Stub_Factory.h"
#include "tao/Default_Endpoint_Selector_Factory.h"
#include "tao/Default_Protocols_Hooks.h"
#include "tao/Default_Thread_Lane_Resources_Manager.h"
#include "tao/Default_Collocation_Resolver.h"
#include "tao/Codeset_Manager_Factory_Base.h"
#include "tao/Codeset_Manager.h"
#include "tao/debug.h"
#include "tao/StringSeqC.h"

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
   * Parses the supplied command-line arguments to extract any that
   * apply to the process (globally)
   *
   * @brief Modifies the argc to reflect any arguments it has
   * "consumed"
   *
   * When multiple ORBs are being configured, the global options are
   * only processed for the first ORB loaded. However subsequent ORBs
   * may be supplied with some of these options, so they need to be
   * eliminated as though they were processed. For this reason,
   * this function is called for every ORB, but only the first call
   * sets apply_values to true
   *
   */
  int
  parse_global_args_i (int &argc,
                       char **argv,
                       CORBA::StringSeq &svc_config_argv,
                       bool apply_values);

  /**
   * Parses the supplied command-line arguments to extract any that
   * specify a Service Configurator file (-ORBSvcConf). This is done
   * separately, because depending on the context, the configuration
   * file may apply to the process-wide service repository, or to the
   * orb-specific (private) one.
   *
   * @brief Modifies the argc to reflect any arguments it has
   * "consumed"
   */
  int
  parse_svcconf_args_i (int &argc,
                       char **argv,
                       CORBA::StringSeq &svc_config_argv);

  /**
   * Initialize the ACE Service Configurator with the process-global
   * services (available to any ORB).
   *
   * @return @c 0 if successful, @c -1 with @c errno set if failure.
   *
   * @note You can provide your program a set of default `svc.conf'
   *       entries by setting @a ignore_default_svc_conf_file to
   *       non-zero and use @c default_svc_conf_entries() before
   *       calling @c open_global_services().  In addition, you can @a
   *       skip_service_config_open altogether, which used to be
   *       important when the ORB is linked in via the
   *       ACE_Service_Config, since the ACE_Service_Config was
   *       non-reentrant.  However, the ACE_Service_Config is now
   *       reentrant meaning that it is really no longer necessary to
   *       do so.
   */
  void register_global_services_i (ACE_Service_Gestalt * pcfg);
  void register_additional_services_i (ACE_Service_Gestalt * pcfg);

  /**
   * Parses the supplied command-line arguments to extract any
   * instance-specific ones.
   *
   * @brief Modifies the argc to reflect any arguments it has
   * "consumed"
   */
  int
  parse_private_args_i (int &argc,
                        char **argv,
                        CORBA::StringSeq & svc_config_argv,
                        bool & skip_service_config_open);

  /**
   * Initialize ORB-local (private) ACE Service Configurator
   * repository.
   *
   * @return @c 0 if successful, @c -1 with @c errno set if failure.
   *
   */
  int open_private_services_i (ACE_Service_Gestalt* pcfg,
                               int & argc,
                               char ** argv,
                               bool skip_service_config_open = false,
                               bool ignore_default_svc_conf_file = false);

  /**
   * Number of times open_services() has been called.  Incremented by
   * open_global_services_i(), and decremented by close_services().
   *
   * @note In/decrement operations are atomic.
   */
  long service_open_count = 0;

  /**
   * Part of a condition variable, which helps to ensure non-default
   * ORBs can not proceed with their initialization, until the globaly
   * required services have been instantiated by the default
   * ORB. Usually, the first ORB to be created is designated the
   * default ORB (reference the CORBA spec)
   */
  bool is_ubergestalt_ready = false;

  char const * resource_factory_args =
    TAO_DEFAULT_RESOURCE_FACTORY_ARGS;
  char const * server_strategy_factory_args =
    TAO_DEFAULT_SERVER_STRATEGY_FACTORY_ARGS;
  char const * client_strategy_factory_args =
    TAO_DEFAULT_CLIENT_STRATEGY_FACTORY_ARGS;

#if (TAO_NEGOTIATE_CODESETS == 1)
  bool negotiate_codesets = true;
#else
  bool negotiate_codesets = false;
#endif /* TAO_NEGOTIATE_CODESETS */
} // anonymous namespace

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// A little helper class to get around the TAO_Singleton::instance ()
/// inability to pass default initialization arguments to the
/// singleton ctor.

#if defined (ACE_HAS_THREADS)
class TAO_Ubergestalt_Ready_Condition
  : public ACE_SYNCH_RECURSIVE_CONDITION
{
public:
  TAO_Ubergestalt_Ready_Condition (void)
    : ACE_SYNCH_RECURSIVE_CONDITION
  (*ACE_Static_Object_Lock::instance())
  {
  };

  static TAO_Ubergestalt_Ready_Condition* instance (void)
  {
    // The first thread to get here will initialize the static
    // local. Any subsequent threads synchronizaton will be handled by
    // TAO_Singleton
    static TAO_Ubergestalt_Ready_Condition *i_ =
      TAO_Singleton <TAO_Ubergestalt_Ready_Condition,
                     TAO_SYNCH_RECURSIVE_MUTEX>::instance ();

    return i_;
  };
};
#endif // ACE_HAS_THREADS

int
TAO::ORB::open_services (ACE_Service_Gestalt* pcfg,
                         int &argc,
                         ACE_TCHAR **argv)
{
  {
    ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                              guard,
                              TAO_Ubergestalt_Ready_Condition::instance ()->mutex (),
                              -1));

    // Wait in line, while the default ORB (which isn't us) completes
    // initialization of the globaly reuired service objects
    if (service_open_count == 1)
      {
        if (TAO_debug_level > 4)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) Waiting for the default ")
                      ACE_TEXT ("ORB to complete the global ")
                      ACE_TEXT ("initialization\n")));

        ACE_MT (while (!is_ubergestalt_ready)
                TAO_Ubergestalt_Ready_Condition::instance ()->wait ());

        if (TAO_debug_level > 4)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) The default ")
                      ACE_TEXT ("ORB must have completed the global ")
                      ACE_TEXT ("initialization...\n")));

      }
    else
      {
        if (TAO_debug_level > 4)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) We are %sthe default ")
                      ACE_TEXT ("ORB ...\n"),
                      (service_open_count == 0) ? "" : "not "));
      }

    service_open_count++;
  }

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

  svc_config_argv.length (1);
  svc_config_argv[0] = argv0.c_str ();

  // Should we skip the ACE_Service_Config::open() method, e.g., if we
  // already being configured by the ACE Service Configurator.
  //
  // @@ This is no longer needed since the Service Configurator is now
  // reentrant.(-Ossama)
  // @@ Leaving it in, since the -ORBSkipServiceConfigOpen is still
  // available. (-Iliyan)
  bool skip_service_config_open = false;

  // Extract any ORB options from the argument vector.
  if (parse_private_args_i (argc,
                            argv,
                            svc_config_argv,
                            skip_service_config_open) == -1)
    {
      return -1;
    }

  // Construct an argument vector specific to the process-wide
  // (global) Service Configurator instance.
  CORBA::StringSeq global_svc_config_argv;

  ACE_Service_Gestalt * theone = ACE_Service_Config::global ();

  if (service_open_count == 1)
    {
      ACE_Service_Config_Guard config_guard (theone);

      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) Initializing the ")
                      ACE_TEXT ("process-wide services\n")));
        }

      register_global_services_i (theone);

      // Be certain to copy the program name so that service configurator
      // has something to skip!
      ACE_CString argv0 ("");

      if (argc > 0 && argv != 0)
        {
          argv0 = ACE_TEXT_ALWAYS_CHAR (argv[0]);
        }

      global_svc_config_argv.length (1);
      global_svc_config_argv[0] = argv0.c_str ();

      if (parse_global_args_i (argc, argv, global_svc_config_argv, true) == -1)
        {
          return -1;
        }

      if (parse_svcconf_args_i (argc, argv, global_svc_config_argv) == -1)
        {
          return -1;
        }

      int global_svc_config_argc = global_svc_config_argv.length ();
      int status =
        open_private_services_i (theone,
                                 global_svc_config_argc,
                                 global_svc_config_argv.get_buffer (),
                                 skip_service_config_open);

      if (status == -1 && TAO_debug_level > 0)
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR_RETURN ((LM_DEBUG,
                                 ACE_TEXT ("TAO (%P|%t) Failed to ")
                                 ACE_TEXT ("open process-wide service ")
                                 ACE_TEXT ("configuration\n")),
                                -1);
            }

          return -1;
        }

      register_additional_services_i (theone);


      if (TAO_debug_level > 4)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) Default ORB - global ")
                    ACE_TEXT ("initialization completed.\n")));

      // Notify all other threads that may be waiting, that the global
      // gestalt has been initialized.
      {
        ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                                  guard,
                                  TAO_Ubergestalt_Ready_Condition::instance ()->mutex (),
                                  -1));

        is_ubergestalt_ready = true;
        ACE_MT (if (TAO_Ubergestalt_Ready_Condition::instance ()->
                    broadcast () == -1)
                return -1);
      }

    }
  else
    {
      int status =
        parse_global_args_i(argc, argv,global_svc_config_argv, false);
      if (status == -1 && TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) Skipping the process-wide ")
                    ACE_TEXT ("service configuration, service_open_count ")
                    ACE_TEXT ("= %d, status = %d\n"),
                    service_open_count,
                    status));
    }

  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) Initializing the ")
                  ACE_TEXT ("orb-specific services\n")));
    }

  if (parse_svcconf_args_i (argc, argv, svc_config_argv) == -1)
    {
      return -1;
    }

  int svc_config_argc = svc_config_argv.length ();
  int status =
    open_private_services_i (pcfg,
                             svc_config_argc,
                             svc_config_argv.get_buffer (),
                             skip_service_config_open);

  if (status >= 0)
    {
      return 0;
    }

  if (TAO_debug_level > 0)
    {
      ACE_ERROR_RETURN ((LM_DEBUG,
                         ACE_TEXT ("TAO (%P|%t) Failed to ")
                         ACE_TEXT ("open orb service configuration\n")),
                        -1);
    }

  return -1;
}

int
TAO::ORB::close_services (ACE_Service_Gestalt* pcfg)
{
  ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                            guard,
                            *ACE_Static_Object_Lock::instance (),
                            -1));
  service_open_count--;

  return pcfg->close ();
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

TAO_END_VERSIONED_NAMESPACE_DECL

// -----------------------------------------------------
namespace
{
  /// Open services, belonging to the gestalt instance.

  int
  open_private_services_i (ACE_Service_Gestalt * pcfg,
                           int & argc,
                           ACE_TCHAR ** argv,
                           bool skip_service_config_open,
                           bool ignore_default_svc_conf_file)
  {

    if (skip_service_config_open)
      {
        return 0;
      }

#if defined (TAO_PLATFORM_SVC_CONF_FILE_NOTSUP)
    ignore_default_svc_conf_file = true;
#endif /* TAO_PLATFORM_SVC_CONF_FILE_NOTSUP */

    return pcfg->open (argc,
                       argv,
                       ACE_DEFAULT_LOGGER_KEY,
                       0, // Don't ignore static services.
                       ignore_default_svc_conf_file);
  }

  /// @brief registers all process-wide (global) services, available
  /// to all ORBs
  void
  register_global_services_i (ACE_Service_Gestalt * pcfg)
  {
    // This has to be done before intializing the resource
    // factory. Codesets is a special library since its configuration
    // is optional and it may be linked statically.
    if (negotiate_codesets)
      {
        TAO_Codeset_Manager_Factory_Base *factory =
          ACE_Dynamic_Service<TAO_Codeset_Manager_Factory_Base>::instance (
            "TAO_Codeset");

        if (factory == 0 || factory->is_default ())
          {
#if !defined (TAO_AS_STATIC_LIBS)
            // only for dynamic libs, check to see if default factory
            // and if so, remove it
            ACE_Service_Config::process_directive (
              ACE_REMOVE_SERVICE_DIRECTIVE ("TAO_Codeset"));

            ACE_Service_Config::process_directive (
              ACE_DYNAMIC_SERVICE_DIRECTIVE (
                "TAO_Codeset",
                "TAO_Codeset",
                "_make_TAO_Codeset_Manager_Factory",
                ""));

            factory =
              ACE_Dynamic_Service<
                  TAO_Codeset_Manager_Factory_Base
                >::instance ("TAO_Codeset");
#endif
          }

        if (factory == 0)
          {
            if (TAO_debug_level > 0)
              {
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("(%P|%t) ORB_Core: ")
                            ACE_TEXT ("Unable to initialize ")
                            ACE_TEXT ("Codeset Manager\n")));
              }
          }
      }

    pcfg->process_directive (
      ace_svc_desc_TAO_Default_Resource_Factory);
    pcfg->process_directive (
      ace_svc_desc_TAO_Default_Client_Strategy_Factory);
    pcfg->process_directive (
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

#if defined (TAO_HAS_IIOP) && (TAO_HAS_IIOP != 0)
    pcfg->process_directive (ace_svc_desc_TAO_IIOP_Protocol_Factory);
#endif /* TAO_HAS_IIOP && TAO_HAS_IIOP != 0 */

    // add descriptor to list of static objects.
    pcfg->process_directive (ace_svc_desc_TAO_MCAST_Parser);
    pcfg->process_directive (ace_svc_desc_TAO_CORBANAME_Parser);
    pcfg->process_directive (ace_svc_desc_TAO_CORBALOC_Parser);
    pcfg->process_directive (ace_svc_desc_TAO_FILE_Parser);
    pcfg->process_directive (ace_svc_desc_TAO_DLL_Parser);
    pcfg->process_directive (ace_svc_desc_TAO_Default_Stub_Factory);
    pcfg->process_directive (
      ace_svc_desc_TAO_Default_Endpoint_Selector_Factory);
    pcfg->process_directive (ace_svc_desc_TAO_Default_Protocols_Hooks);
    pcfg->process_directive (
      ace_svc_desc_TAO_Default_Thread_Lane_Resources_Manager_Factory);
    pcfg->process_directive (ace_svc_desc_TAO_Default_Collocation_Resolver);

  } /* register_global_services_i */

  void
  register_additional_services_i (ACE_Service_Gestalt * pcfg)
  {
    // @@ What the heck do these things do and do we need to avoid
    //    calling them if we're not invoking the svc.conf file?
    // @@ They are needed for platforms that have no file system,
    //    like VxWorks.

    if (resource_factory_args != 0)
      {
        pcfg->process_directive(
          ACE_TEXT_CHAR_TO_TCHAR (resource_factory_args));
      }

    if (client_strategy_factory_args != 0)
      {
        pcfg->process_directive
          (ACE_TEXT_CHAR_TO_TCHAR (client_strategy_factory_args));
      }

    if (server_strategy_factory_args != 0)
      {
        pcfg->process_directive
          (ACE_TEXT_CHAR_TO_TCHAR (server_strategy_factory_args));
      }

    // If available, allow the Adapter Factory to setup.
    ACE_Service_Object *adapter_factory =
      ACE_Dynamic_Service<ACE_Service_Object>::instance (
        pcfg,
        TAO_ORB_Core::poa_factory_name ().c_str ());

    if (adapter_factory != 0)
      {
        adapter_factory->init (0, 0);
      }

    ACE_Service_Object * const pi_server_loader =
      ACE_Dynamic_Service<ACE_Service_Object>::instance (
        pcfg,
        "PI_Server_Loader");

    if (pi_server_loader != 0)
      {
        pi_server_loader->init (0, 0);
      }

    ACE_Service_Object * const bidir_loader =
      ACE_Dynamic_Service<ACE_Service_Object>::instance (
        pcfg,
        "BiDirGIOP_Loader");

    if (bidir_loader != 0)
      {
        bidir_loader->init (0, 0);
      }

    ACE_Service_Object * const messaging_loader =
      ACE_Dynamic_Service<ACE_Service_Object>::instance (
        pcfg,
        "Messaging_Loader");

    if (messaging_loader != 0)
      {
        messaging_loader->init (0, 0);
      }

    // Handle RTCORBA library special case.  Since RTCORBA needs
    // its init method call to register several hooks, call it
    // here if it hasn't already been called.
    ACE_Service_Object * const rt_loader =
      ACE_Dynamic_Service<ACE_Service_Object>::instance (
        pcfg,
        "RT_ORB_Loader");

    if (rt_loader != 0)
      {
        rt_loader->init (0, 0);
      }

    ACE_Service_Object * const rtscheduler_loader =
      ACE_Dynamic_Service<ACE_Service_Object>::instance (
        pcfg,
        "RTScheduler_Loader");

    if (rtscheduler_loader != 0)
      {
        rtscheduler_loader->init (0, 0);
      }

  } /* register_additional_services_i */

  int
  parse_svcconf_args_i (int &argc,
                        char **argv,
                        CORBA::StringSeq &svc_config_argv)
  {
    // Extract the Service Configurator ORB options from the argument
    // vector.
    ACE_Arg_Shifter arg_shifter (argc, argv);
    CORBA::ULong len = 0;

    while (arg_shifter.is_anything_left ())
      {
        const ACE_TCHAR *current_arg =
          arg_shifter.get_the_parameter (ACE_TEXT ("-ORBSvcConf"));

        if (0 != current_arg)
          {
            // Specify the name of the svc.conf file to be used.

            // Proceeds only if the configuration file exists.
            FILE * const conf_file =
              ACE_OS::fopen (current_arg, ACE_TEXT ("r"));

            if (0 == conf_file)
              {
                // Assigning EINVAL to errno to make an exception
                // thrown.  calling code does not throw an exception if
                // the errno is set to ENOENT for some reason.
                errno = EINVAL;

                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("TAO (%P|%t) Service ")
                                   ACE_TEXT ("Configurator ")
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
            svc_config_argv[len + 1] =
              CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (current_arg));

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

    return 0;
  } /* parse_svcconf_args_i */

  int
  parse_private_args_i (int &argc,
                        char **argv,
                        CORBA::StringSeq &svc_config_argv,
                        bool & skip_service_config_open)
  {
    // Extract the Service Configurator ORB options from the argument
    // vector.
    ACE_Arg_Shifter arg_shifter (argc, argv);
    CORBA::ULong len = 0;

    while (arg_shifter.is_anything_left ())
      {
        const ACE_TCHAR *current_arg = 0;
        int result =
          arg_shifter.cur_arg_strncasecmp (
            ACE_TEXT ("-ORBSkipServiceConfigOpen"));

        if (0 == result)  // Start with the parameterless flags.
          {
            skip_service_config_open = true;

            arg_shifter.consume_arg ();
          }
        else  // Continue with flags that accept parameters.
          {
            current_arg =
              arg_shifter.get_the_parameter (
                ACE_TEXT ("-ORBSvcConfDirective"));

            if (0 != current_arg)
              {
                len = svc_config_argv.length ();
                svc_config_argv.length (len + 2);  // 2 arguments to add

                // This is used to pass arguments to the Service
                // Configurator using the "command line" to provide
                // configuration information rather than using a svc.conf
                // file.  Pass the "-S" to the service configurator.
                svc_config_argv[len] = CORBA::string_dup ("-S");
                svc_config_argv[len + 1] =
                  CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (current_arg));

                arg_shifter.consume_arg ();
              }
            else
              {
                current_arg =
                  arg_shifter.get_the_parameter (
                    ACE_TEXT ("-ORBServiceConfigLoggerKey"));

                if (0 != current_arg)
                  {
                    len = svc_config_argv.length ();
                    svc_config_argv.length (len + 2);  // 2 arguments to add

                    svc_config_argv[len] = CORBA::string_dup ("-k");
                    svc_config_argv[len + 1] =
                      CORBA::string_dup (ACE_TEXT_ALWAYS_CHAR (current_arg));

                    arg_shifter.consume_arg ();
                  }
                // Can't interpret this argument.
                // Move on to the next argument.
                else
                  {
                    // Any arguments that don't match are ignored so that
                    // the caller can still use them.
                    arg_shifter.ignore_arg ();
                  }
              }
          }
      }

    return 0;
  } /* parse_private_args_i */

  int
  parse_global_args_i (int &argc,
                       char **argv,
                       CORBA::StringSeq &svc_config_argv,
                       bool apply_values)
  {
    // NOTE: When adding new global arguments, ensure they are only
    // applied when apply_values is true, but that they are always
    // consumed, if they need to be consumed.
#if defined (TAO_DEBUG) && !defined (ACE_HAS_WINCE)
    // Make it a little easier to debug programs using this code.
    if (apply_values)
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
    CORBA::ULong len = 0;

    while (arg_shifter.is_anything_left ())
      {
        const ACE_TCHAR *current_arg = 0;
        int strcmp_result =
          arg_shifter.cur_arg_strncasecmp (ACE_TEXT ("-ORBDebug"));

        if (0 == strcmp_result)
          {
            if (apply_values)
              {
                // Later, replace all of these
                // warning this turns on a daemon.
                ACE::debug (1);
              }

            arg_shifter.consume_arg ();
          }
        else
          {
            current_arg =
              arg_shifter.get_the_parameter (
                ACE_TEXT ("-ORBNegotiateCodesets"));

            if (0 != current_arg)
              {
                if (apply_values)
                  {
                    // Don't consume, the ORB_Core::init will use it again.
                    negotiate_codesets = (ACE_OS::atoi (current_arg));
                  }
              }
            else
              {
                current_arg =
                  arg_shifter.get_the_parameter (
                    ACE_TEXT ("-ORBDebugLevel"));

                if (0 != current_arg)
                  {
                    if (apply_values)
                      {
                        TAO_debug_level =
                          ACE_OS::atoi (current_arg);
                      }

                    arg_shifter.consume_arg ();
                  }
                else
                  {
                    strcmp_result =
                      arg_shifter.cur_arg_strncasecmp (
                        ACE_TEXT ("-ORBDaemon"));

                    if (0 == strcmp_result)
                      {
                        // Be a daemon.
                        if (apply_values)
                          {
                            len = svc_config_argv.length ();
                            svc_config_argv.length (len + 1);

                            svc_config_argv[len] =
                              CORBA::string_dup ("-b");
                          }

                        arg_shifter.consume_arg ();
                      }
                    // Can't interpret this argument.
                    // Move on to the next argument.
                    else
                      {
                        // Any arguments that don't match are ignored so
                        // that the caller can still use them.
                        arg_shifter.ignore_arg ();
                      }
                  }
              }
          }
      }

    return 0;
  } /* parse_global_args_i */
} // anonymous namespace.
