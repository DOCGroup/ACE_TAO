// -*- C++ -*-
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
#include "tao/HTTP_Parser.h"
#include "tao/DLL_Parser.h"
#include "tao/ORB_Core.h"
#include "tao/Adapter_Factory.h"
#include "tao/Default_Stub_Factory.h"
#include "tao/Default_Endpoint_Selector_Factory.h"
#include "tao/Default_Thread_Lane_Resources_Manager.h"
#include "tao/Default_Collocation_Resolver.h"
#include "tao/Codeset_Manager_Factory_Base.h"
#include "tao/Codeset_Manager.h"
#include "tao/debug.h"

#include "ace/Dynamic_Service.h"
#include "ace/Arg_Shifter.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/ARGV.h"
#include "ace/Env_Value_T.h"
#include "ace/ACE.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Static_Object_Lock.h"
#include "ace/OS_NS_sys_stat.h"

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
                       ACE_TCHAR **argv,
                       ACE_ARGV &svc_config_argv,
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
                        ACE_TCHAR **argv,
                        ACE_ARGV &svc_config_argv);

  /**
   * Checks if there is -ORBGestalt option with non-GLOBAL value.
   *
   * @brief Modifies the argc to reflect any arguments it has
   * "consumed"
   *
   * If the first ORB has some special configuration and uses non
   * GLOBAL gestalt then it's expected the this configuration will
   * not become default for other ORBs created after it. This function
   * allows to avoid the above situation.
   */
  bool
  using_global_gestalt_i (int &argc,
                          ACE_TCHAR **argv,
                          bool &skip_service_config_open);

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
  void
  register_global_services_i (ACE_Service_Gestalt * pcfg);
  void
  register_additional_services_i (ACE_Service_Gestalt * pcfg);

  /**
   * Parses the supplied command-line arguments to extract any
   * instance-specific ones.
   *
   * @brief Modifies the argc to reflect any arguments it has
   * "consumed"
   */
  int
  parse_private_args_i (int &argc,
                        ACE_TCHAR **argv,
                        ACE_ARGV & svc_config_argv,
                        bool & skip_service_config_open,
                        bool & ignore_default_svc_conf_file);

  /**
   * Initialize ORB-local (private) ACE Service Configurator
   * repository.
   *
   * @return @c 0 if successful, @c -1 with @c errno set if failure.
   *
   */
  int
  open_private_services_i (ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> pcfg,
                           int & argc,
                           ACE_TCHAR **argv,
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

#if defined (ACE_HAS_THREADS)
/// A little helper class to get around the TAO_Singleton::instance ()
/// inability to pass default initialization arguments to the
/// singleton ctor.

class TAO_Ubergestalt_Ready_Condition
  : public ACE_SYNCH_RECURSIVE_CONDITION
{
public:
  static TAO_Ubergestalt_Ready_Condition* instance (void)
  {
    return TAO_Singleton <TAO_Ubergestalt_Ready_Condition,
      TAO_SYNCH_RECURSIVE_MUTEX>::instance ();
  }

  TAO_Ubergestalt_Ready_Condition (void)
    : ACE_SYNCH_RECURSIVE_CONDITION (mutex_)
  {
  }


private:
  /// The mutex, associated with the condition. Do not use the ACE
  /// global mutex, because it causes deadlocks with other threads that
  /// may be in DLL_Manager::open()
  ACE_Recursive_Thread_Mutex mutex_;
};
#endif // ACE_HAS_THREADS




// ****************************************************************
/// Note that the argument vector will be corrupted upon return
int
TAO::ORB::open_global_services (int argc, ACE_TCHAR **argv)
{
  {
    // Count of the number of (times we did this for all) ORBs.
    static int orb_init_count = 0;

    // Using ACE_Static_Object_Lock::instance() precludes ORB_init()
    // from being called within a static object CTOR.
    ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
            guard,
            *ACE_Static_Object_Lock::instance (),
            -1));

    // Make sure TAO's singleton manager is initialized.
    // We need to initialize before TAO_default_environment() is called
    // since that call instantiates a TAO_TSS_Singleton.
    if (TAO_Singleton_Manager::instance ()->init () == -1)
      return -1;

    // Prevent multiple initializations.
    if (++orb_init_count > 1)
      return 0;
  }

  // Prevent any other thread from going through ORB initialization before the
  // uber-gestalt is initialized.
  ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
          guard,
          TAO_Ubergestalt_Ready_Condition::instance ()->mutex (),
          -1));

  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
      ACE_TEXT ("TAO (%P|%t) Initializing the ")
      ACE_TEXT ("process-wide service context\n")));
    }

  ACE_Service_Gestalt* theone = ACE_Service_Config::global ();
  ACE_Service_Config_Guard auto_config_guard (theone);

  // Construct an argument vector specific to the process-wide
  // (global) Service Configurator instance.
  // Be certain to copy the program name so that service configurator
  // has something to skip!
  ACE_ARGV global_svc_config_argv (true); // only this ctor allows
  // subsequent use of add()!
  global_svc_config_argv.add ((argc <= 0 || argv == 0) ?
            ACE_TEXT ("") : argv[0], true);

  // Will expand the environment variables, if any were used.
  // Is this a good thing? I guess it provides greater flexibility
  // for deployment,so let's leave it. Will also quote arguments.
  ACE_ARGV copyargv (argc, argv, true, true);

  // Adjust to proper type
  int tmpargc = copyargv.argc (); // use copied count, not original
  ACE_Argv_Type_Converter cvtargv (tmpargc, copyargv.argv());

  tmpargc = cvtargv.get_argc ();
  ACE_TCHAR **tmpargv = cvtargv.get_TCHAR_argv ();

  // Collect global SC parameters. True means "immediately
  // apply global setting" like debug flag, etc.
  if (parse_global_args_i (tmpargc,
         tmpargv,
         global_svc_config_argv,
         true) == -1)
    return -1;

  bool skip_service_config_open = false; // by default we shouldn't

  if (using_global_gestalt_i (tmpargc,
                              tmpargv,
                              skip_service_config_open))
    {
      if (parse_svcconf_args_i (tmpargc,
              tmpargv,
              global_svc_config_argv) == -1)
        return -1;
    }

  bool ignore_default_svc_conf_file = false;

  if (parse_private_args_i (tmpargc,
          tmpargv,
          global_svc_config_argv,
          skip_service_config_open,
          ignore_default_svc_conf_file) == -1)
    return -1;

  // register_global_services_i depends on the parsing of at least the
  // -ORBNegotiateCodesets option, and must be invoked after all the
  // parsing methods, but still must preceed the opening of other services.

  register_global_services_i (theone);

  // Perform the open magic (unless SC::open() has been called already)
  int global_svc_config_argc = global_svc_config_argv.argc ();
  int status = open_private_services_i (theone,
                                        global_svc_config_argc,
                                        global_svc_config_argv.argv (),
                                        skip_service_config_open,
                                        ignore_default_svc_conf_file);

  // okay?
  if (status == -1)
    {
      if (errno != ENOENT)
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("TAO (%P|%t) - Failed to open process-")
                          ACE_TEXT ("wide service configuration context\n")));
            }
          return -1;
        }
      else
        {
          if (TAO_debug_level > 4)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - Did not find default ")
                        ACE_TEXT ("svc.conf\n")));
          status = 0;
        }
    }

  if (status > 0)
    {
      // one or more directives failed, but we don't know which
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - process-wide service ")
                      ACE_TEXT ("configuration context had %d failed ")
                      ACE_TEXT ("directives\n"), status));
        }
    }

  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - Completed initializing the ")
                ACE_TEXT ("process-wide service context\n")));

  if (TAO_debug_level > 4)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - Default ORB services initialization begins\n")));

  // Load more ORB-related services
  register_additional_services_i (theone);

  if (TAO_debug_level > 4)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - Default ORB services initialization completed\n")));

  // Notify all other threads that may be waiting, that the global
  // gestalt has been initialized.
  is_ubergestalt_ready = true;
  ACE_MT (if (TAO_Ubergestalt_Ready_Condition::instance ()->
              broadcast () == -1)
            return -1);

  return 0;
}

int
TAO::ORB::open_services (ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> pcfg,
                         int &argc,
                         ACE_TCHAR **argv)
{
  {
    ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                              guard,
                              TAO_Ubergestalt_Ready_Condition::instance ()->mutex (),
                              -1));

    // Wait in line, while the default ORB (which isn't us) completes
    // initialization of the globaly required service objects
    if (service_open_count == 1)
      {
        if (TAO_debug_level > 4)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Waiting for the default ")
                      ACE_TEXT ("ORB to complete the global ")
                      ACE_TEXT ("initialization\n")));

      ACE_MT (while (!is_ubergestalt_ready)
      TAO_Ubergestalt_Ready_Condition::instance ()->wait ());

        if (TAO_debug_level > 4)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - The default ")
                      ACE_TEXT ("ORB must have completed the global ")
                      ACE_TEXT ("initialization...\n")));

      }
    else
      {
        if (TAO_debug_level > 4)
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - We are%Cthe default ")
                      ACE_TEXT ("ORB ...\n"),
                      (service_open_count == 0) ? " " : " not "));
      }

    ++service_open_count;
  }

  // Construct an argument vector specific to the Service Configurator.
  // Be certain to copy the program name so that service configurator
  // has something to skip!
  ACE_ARGV svc_config_argv (true);  // only this ctor allows subsequent
                                    // use of add()!
  svc_config_argv.add ((argc <= 0 || argv == 0) ? ACE_TEXT ("") : argv[0],
                       true);

  // Should we skip the ACE_Service_Config::open() method?,
  // e.g., because of -ORBSkipServiceConfigOpen
  bool skip_service_config_open = false;
  bool ignore_default_svc_conf_file = false;

  // Extract any ORB options from the argument vector.
  if (parse_private_args_i (argc,
                            argv,
                            svc_config_argv,
                            skip_service_config_open,
                            ignore_default_svc_conf_file) == -1)
    {
      return -1;
    }

  // Construct an argument vector specific to the process-wide
  // (global) Service Configurator instance.
  ACE_ARGV global_svc_config_argv;

  // Parse any globally applicable arguments, but do not make them effective.
  // We are effectively purging the command line from them without affecting
  // the global state - after all, it may be a private (local) configuration
  // context.
  int status = parse_global_args_i(argc, argv, global_svc_config_argv, false);

  if (status == -1 && TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("TAO (%P|%t) - Skipping the process-wide ")
    ACE_TEXT ("service configuration, service_open_count ")
    ACE_TEXT ("= %d, status = %d\n"),
    service_open_count,
    status));

  if (TAO_debug_level > 2)
    ACE_DEBUG ((LM_DEBUG,
    ACE_TEXT ("TAO (%P|%t) - Initializing the ")
    ACE_TEXT ("orb-specific services\n")));

  if (parse_svcconf_args_i (argc, argv, svc_config_argv) == -1)
    return -1;
  // Not a big deal to call open_private_services_i() again (if it was the global one). The SG::open() would not run if it has already been executed.
  // only open the private context if it is not also the global context
  if (pcfg != ACE_Service_Config::global())
    {
      int svc_config_argc = svc_config_argv.argc ();
      status =
        open_private_services_i (pcfg,
                                 svc_config_argc,
                                 svc_config_argv.argv (),
                                 skip_service_config_open,
                                 ignore_default_svc_conf_file);
    }

  if (status == -1)
    {
      if (errno != ENOENT)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - Failed to open ORB-specific ")
                        ACE_TEXT ("service configuration\n")));
          return -1;
        }
      else
        {
          if (TAO_debug_level > 4)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("TAO (%P|%t) - Did not find default ")
                        ACE_TEXT ("svc.conf\n")));
          status = 0;
        }
    }

  if (status > 0)
    {
      // one or more directives failed, but we don't know which
      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - ORB-specific service ")
                      ACE_TEXT ("configuration context had %d failed ")
                      ACE_TEXT ("directives\n"), status));
        }
    }


  return status;
}

int
TAO::ORB::close_services (ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> pcfg)
{
  ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX,
                            guard,
                            *ACE_Static_Object_Lock::instance (),
                            -1));
  --service_open_count;

  int result = 0;

  if (pcfg != ACE_Service_Config::global())
    {
      result = pcfg->close ();
    }

  return result;
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
  open_private_services_i (ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> pcfg,
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

    // Copy command line parameter to allow conversion
    ACE_Argv_Type_Converter command_line (argc, argv);

    return pcfg->open (command_line.get_argc (),
                       command_line.get_TCHAR_argv (),
                       0,
                       false, // Don't ignore static services.
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
#if !defined (TAO_AS_STATIC_LIBS) && !(defined (ACE_VXWORKS) && !defined (__RTP__))
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
#endif /* !TAO_AS_STATIC_LIBS && !(ACE_VXWORKS && !__RTP__) */
          }

        if (factory == 0)
          {
            if (TAO_debug_level > 0)
              {
                ACE_ERROR ((LM_ERROR,
                            ACE_TEXT ("TAO (%P|%t) ORB_Core: ")
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
#if (TAO_HAS_MCAST_PARSER == 1)
    pcfg->process_directive (ace_svc_desc_TAO_MCAST_Parser);
#endif /* TAO_HAS_MCAST_PARSER == 1 */
#if (TAO_HAS_CORBANAME_PARSER == 1)
    pcfg->process_directive (ace_svc_desc_TAO_CORBANAME_Parser);
#endif /* TAO_HAS_CORBANAME_PARSER == 1 */
#if (TAO_HAS_CORBALOC_PARSER == 1)
    pcfg->process_directive (ace_svc_desc_TAO_CORBALOC_Parser);
#endif /* TAO_HAS_CORBALOC_PARSER == 1 */
#if (TAO_HAS_FILE_PARSER == 1)
    pcfg->process_directive (ace_svc_desc_TAO_FILE_Parser);
#endif /* TAO_HAS_FILE_PARSER == 1 */
#if (TAO_HAS_DDL_PARSER == 1)
    pcfg->process_directive (ace_svc_desc_TAO_DLL_Parser);
#endif /* TAO_HAS_DDL_PARSER == 1 */
#if (TAO_HAS_HTTP_PARSER == 1)
    pcfg->process_directive (ace_svc_desc_TAO_HTTP_Parser);
#endif /* TAO_HAS_HTTP_PARSER */
    pcfg->process_directive (ace_svc_desc_TAO_Default_Stub_Factory);
    pcfg->process_directive (
      ace_svc_desc_TAO_Default_Endpoint_Selector_Factory);
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

#if defined (TAO_HAS_ZIOP) && TAO_HAS_ZIOP == 1
    ACE_Service_Object * const ziop_loader =
      ACE_Dynamic_Service<ACE_Service_Object>::instance (
        pcfg,
        "ZIOP_Loader");

    if (ziop_loader != 0)
      {
        ziop_loader->init (0, 0);
      }
#endif

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

    ACE_Service_Object * const csd_framework_loader =
      ACE_Dynamic_Service<ACE_Service_Object>::instance (
        pcfg,
        "CSD_Framework_Loader");

    if (csd_framework_loader != 0)
      {
        csd_framework_loader->init (0, 0);
      }

    ACE_Service_Object * const endpoint_policy_loader =
      ACE_Dynamic_Service<ACE_Service_Object>::instance (
        pcfg,
        "EndpointPolicy_Initializer");

    if (endpoint_policy_loader != 0)
      {
        endpoint_policy_loader->init (0, 0);
      }

    ACE_Service_Object * const diffserv_policy_loader =
      ACE_Dynamic_Service<ACE_Service_Object>::instance (
        pcfg,
        "DiffservPolicy_Initializer");

    if (diffserv_policy_loader != 0)
      {
        diffserv_policy_loader->init (0, 0);
      }
  } /* register_additional_services_i */

  int
  parse_svcconf_args_i (int &argc,
                        ACE_TCHAR **argv,
                        ACE_ARGV &svc_config_argv)
  {
    // Extract the Service Configurator ORB options from the argument
    // vector.
    ACE_Arg_Shifter arg_shifter (argc, argv);

    while (arg_shifter.is_anything_left ())
      {
        // Can't interpret this argument.  Move on to the next argument.
        if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT ("-ORBSvcConf")) == 0)
          {
            const ACE_TCHAR *current_arg =
                        arg_shifter.get_the_parameter (ACE_TEXT ("-ORBSvcConf"));
            arg_shifter.consume_arg ();

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
                                   ACE_TEXT ("TAO (%P|%t) - Error, unable to open file <%s>")
                                   ACE_TEXT (", referenced by -ORBSvcConf option\n"),
                                   current_arg),
                                  -1);
              }
            else
              {
                ACE_OS::fclose (conf_file);
              }

            svc_config_argv.add (ACE_TEXT ("-f"));
            svc_config_argv.add (current_arg, true);
          }
        else if (arg_shifter.cur_arg_strncasecmp
                 (ACE_TEXT ("-ORBSvcConfDirective")) == 0)
          {
            const ACE_TCHAR *current_arg =
              arg_shifter.get_the_parameter (ACE_TEXT ("-ORBSvcConfDirective"));

            // This is used to pass arguments to the Service
            // Configurator using the "command line" to provide
            // configuration information rather than using a svc.conf
            // file.  Pass the "-S" to the service configurator.
            svc_config_argv.add (ACE_TEXT ("-S"));
            svc_config_argv.add (current_arg, true); // quote args!

            arg_shifter.consume_arg ();
          }
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
                        ACE_TCHAR **argv,
                        ACE_ARGV &svc_config_argv,
                        bool & skip_service_config_open,
                        bool & ignore_default_svc_conf_file)
  {
    // Extract the Service Configurator ORB options from the argument
    // vector.
    ACE_Arg_Shifter arg_shifter (argc, argv);

    while (arg_shifter.is_anything_left ())
      {
        const ACE_TCHAR *current_arg = 0;
        if (0 == arg_shifter.cur_arg_strncasecmp
            (ACE_TEXT ("-ORBSkipServiceConfigOpen")))
          {
            skip_service_config_open = true;

            arg_shifter.consume_arg ();
          }
        if (0 == arg_shifter.cur_arg_strncasecmp
            (ACE_TEXT ("-ORBIgnoreDefaultSvcConfFile")))
          {
            ignore_default_svc_conf_file = true;

            arg_shifter.consume_arg ();
          }
        else if (0 != (current_arg = arg_shifter.get_the_parameter
                       (ACE_TEXT ("-ORBServiceConfigLoggerKey"))))
          {
            svc_config_argv.add (ACE_TEXT ("-k"));
            svc_config_argv.add (current_arg, true); // quote args!

            arg_shifter.consume_arg ();
          }
        else if (0 == arg_shifter.cur_arg_strncasecmp
                       (ACE_TEXT ("-ORBNegotiateCodesets")))
          {
            // Negotiate codesets must be evaluated prior to calling
            // register_global_services_i.

            // Don't consume, the ORB_Core::init will use it again.
            arg_shifter.ignore_arg();

            if (0 != (current_arg = arg_shifter.get_current()))
              negotiate_codesets = (ACE_OS::atoi (current_arg));

            arg_shifter.ignore_arg();
          }
        else if (0 != (current_arg =
                       arg_shifter.get_the_parameter
                       (ACE_TEXT ("-ORBDebugLevel"))))
          {
            // Allowing different ORBs to change the global debug
            // level may be unexpected, but since this
            TAO_debug_level = ACE_OS::atoi (current_arg);

            arg_shifter.consume_arg ();
          }
        else
          {
            // Can't interpret this argument.  Move on to the next
            // argument.  Any arguments that don't match are ignored
            // so that the caller can still use them.
            arg_shifter.ignore_arg ();
          }
      }

    return 0;
  } /* parse_private_args_i */

  int
  parse_global_args_i (int &argc,
                       ACE_TCHAR **argv,
                       ACE_ARGV &svc_config_argv,
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

        if (TAO_debug_level != 0)
          {
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
        if (0 == arg_shifter.cur_arg_strncasecmp (ACE_TEXT ("-ORBDebug")))
          {
            if (apply_values)
              {
                // Later, replace all of these
                // warning this turns on a daemon.
                ACE::debug (1);
              }

            arg_shifter.consume_arg ();
          }
        else if (0 == arg_shifter.cur_arg_strncasecmp (ACE_TEXT ("-ORBDaemon")))
          {
            // Be a daemon.
            if (apply_values)
              {
                svc_config_argv.add (ACE_TEXT("-b"));
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
    return 0;
  } /* parse_global_args_i */

  bool
  using_global_gestalt_i (int &argc,
                          ACE_TCHAR **argv,
                          bool &skip_service_config_open)
  {
    bool with_global_gestalt = true;

    ACE_Arg_Shifter arg_shifter (argc, argv);

    while (arg_shifter.is_anything_left ())
      {
        if (0 == arg_shifter.cur_arg_strncasecmp (ACE_TEXT ("-ORBGestalt")))
          {
            // Skip -ORBGestalt. This option is necessary in later stages.
            arg_shifter.ignore_arg ();

            // This should set current_arg to the value of ORBGestalt option.
            const ACE_TCHAR *current_arg = arg_shifter.get_current ();

            if (0 != current_arg &&
                ACE_OS::strcasecmp (current_arg, ACE_TEXT("GLOBAL")) != 0)
              {
                with_global_gestalt = false;

                ACE_stat exists;
                if (ACE_OS::stat (ACE_DEFAULT_SVC_CONF, &exists) == 0)
                  {
                    // In case svc.conf exists and we are asked for a local
                    // gestalt then no matter whether -ORBSvcConf or
                    // -ORBSvcConfDirective are provided or not we ignore them
                    // while setting up a global gestalt. They will be
                    // processed later in a local gestalt.
                    skip_service_config_open = true;
                  }
              }

            // Skip anything that goes after -ORBGestalt.
            arg_shifter.ignore_arg ();
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
    return with_global_gestalt;
  } /* using_global_gestalt_i */
} // anonymous namespace.
