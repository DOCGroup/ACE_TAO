// $Id$


#include "tao/ORB_Core.h"

#include "ace/Env_Value_T.h"
#include "ace/Arg_Shifter.h"
#include "ace/Auto_Ptr.h"

#include "tao/TAO_Internal.h"
#include "tao/default_client.h"
#include "tao/default_server.h"
#include "tao/default_resource.h"
#include "tao/debug.h"
#include "tao/MProfile.h"
#include "tao/Stub.h"
#include "tao/Reactor_Registry.h"
#include "tao/Leader_Follower.h"

#include "tao/Connector_Registry.h"
#include "tao/Acceptor_Registry.h"

#include "tao/POA.h"

#include "tao/Priority_Mapping.h"

#include "ace/Object_Manager.h"
#include "ace/Env_Value_T.h"
#include "ace/Dynamic_Service.h"
#include "ace/Arg_Shifter.h"
#include "ace/INET_Addr.h"

#include "tao/Sync_Strategies.h"

#if defined(ACE_MVS)
#include "ace/Codeset_IBM1047.h"
#endif /* ACE_MVS */

#if !defined (__ACE_INLINE__)
# include "tao/ORB_Core.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, ORB_Core, "$Id$")

// ****************************************************************

CORBA::Environment &
TAO_default_environment ()
{
  return *TAO_TSS_RESOURCES::instance ()->default_environment_;
}

// ****************************************************************

TAO_ORB_Core::TAO_ORB_Core (const char *orbid)
  : lock_ (),
    connector_registry_ (0),
    acceptor_registry_ (0),
    protocol_factories_ (0),
    implrepo_service_ (CORBA::Object::_nil ()),
    use_implrepo_ (0),
    orb_ (),
    root_poa_ (0),
    root_poa_reference_ (),
    orb_params_ (),
    orbid_ (ACE_OS::strdup (orbid ? orbid : "")),
    resource_factory_ (0),
    resource_factory_from_service_config_ (0),
    // @@ This is not needed since the default resource factory, fredk
    //    is statically added to the service configurator.
    client_factory_ (0),
    client_factory_from_service_config_ (0),
    // @@ This is not needed since the default client factory, fredk
    //    is statically added to the service configurator.
    server_factory_ (0),
    server_factory_from_service_config_ (0),
    // @@ This is not needed since the default server factory, fredk
    //    is statically added to the service configurator.
    opt_for_collocation_ (1),
    use_global_collocation_ (1),
    collocation_strategy_ (THRU_POA),

#if (TAO_HAS_CORBA_MESSAGING == 1)

    policy_manager_ (0),
    default_policies_ (0),
    policy_current_ (0),

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

    poa_current_ (0),
    object_adapter_ (0),
    tm_ (),
    from_iso8859_ (0),
    to_iso8859_ (0),
    from_unicode_ (0),
    to_unicode_ (0),
    use_tss_resources_ (0),
    tss_resources_ (),
    orb_resources_ (),
    reactor_registry_ (0),
    reactor_ (0),
    has_shutdown_ (1),
    // Start the ORB in a  "shutdown" state.  Only after
    // CORBA::ORB_init() is called will the ORB no longer be shutdown.
    // This does not mean that the ORB can be reinitialized.  It can
    // only be initialized once.
    thread_per_connection_use_timeout_ (1),
    open_lock_ (),
    open_called_ (0),
    priority_mapping_ (0),
#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)
    eager_buffering_sync_strategy_ (0),
    delayed_buffering_sync_strategy_ (0),
#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */
    transport_sync_strategy_ (0),
    svc_config_argc_ (0),
    svc_config_argv_ (0)
{
  ACE_NEW (this->poa_current_,
           TAO_POA_Current);

#if defined(ACE_MVS)
  ACE_NEW (this->from_iso8859_, ACE_IBM1047_ISO8859);
  ACE_NEW (this->to_iso8859_,   ACE_IBM1047_ISO8859);
#endif /* ACE_MVS */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  ACE_NEW (this->eager_buffering_sync_strategy_,
           TAO_Eager_Buffering_Sync_Strategy);

  ACE_NEW (this->delayed_buffering_sync_strategy_,
           TAO_Delayed_Buffering_Sync_Strategy);

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_CORBA_MESSAGING == 1)

  ACE_NEW (this->policy_manager_,
           TAO_Policy_Manager);

  ACE_NEW (this->default_policies_,
           TAO_Policy_Manager_Impl);

  ACE_NEW (this->policy_current_,
           TAO_Policy_Current);

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  ACE_NEW (this->transport_sync_strategy_,
           TAO_Transport_Sync_Strategy);

}

TAO_ORB_Core::~TAO_ORB_Core (void)
{
  ACE_OS::free (this->orbid_);

  delete this->from_iso8859_;
  delete this->to_iso8859_;

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  delete this->eager_buffering_sync_strategy_;
  delete this->delayed_buffering_sync_strategy_;

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_CORBA_MESSAGING == 1)

  delete this->policy_manager_;
  delete this->default_policies_;
  delete this->policy_current_;

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  delete this->transport_sync_strategy_;

  // This is deleted in init() so we should only get here if the
  // ORB_Core is destroyed prematurely.
  if (this->svc_config_argv_ != 0)
    {
      for (int i = 0; i < this->svc_config_argc_; i++)
        CORBA::string_free (this->svc_config_argv_[i]);

      this->svc_config_argc_ = 0;
      delete [] this->svc_config_argv_;
    }

  // Make sure these two objects are deleted last (other objects may
  // depend on this).
#if !defined (__Lynx__)  ||  !defined (__powerpc__)
  // This statement causes a seg fault on ORB shutdown, on LynxOS
  // 3.0.0/ppc only.
  delete this->poa_current_;
#endif /* ! __Lynx__  ||  ! __powerpc__ */

  delete this->object_adapter_;
}

int
TAO_ORB_Core::init (int &argc, char *argv[], CORBA::Environment &ACE_TRY_ENV)
{
  // Right now, this code expects to begin parsing in argv[1] rather
  // than argv[0].  I don't think that's wise.  I think we need to
  // change that convention to argv[0] and let the initializing code
  // make any necessary shifts.
  //
  // Parse arguments to the ORB.  Typically the ORB is passed
  // arguments straight from the command line, so we will simply pass
  // through them and respond to the ones we understand and ignore
  // those we don't.
  //
  // In some instances, we may actually build another vector of
  // arguments and stash it for use initializing other components such
  // as the ACE_Service_Config or the RootPOA.
  //
  // Prepare a copy of the argument vector for the service configurator.

  // @@ deprecated
  int old_style_endpoint = 0;

  ACE_NEW_THROW_EX (this->svc_config_argv_,
                    char *[argc + 1],
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (-1);

  // Be certain to copy the program name so that service configurator
  // has something to skip!
  ACE_Arg_Shifter arg_shifter (argc, argv);
  const char *argv0 = "";
  if (argc > 0 && argv != 0)
    argv0 = argv[0];
  this->svc_config_argv_[this->svc_config_argc_++] = CORBA::string_dup (argv0);

  ACE_CString host;
  CORBA::UShort port = 0;

  // @@ GIOPLite should be an alternative ORB Messaging protocols, fredk
  // int giop_lite = 0;

  CORBA::Boolean use_ior = 1;
  int cdr_tradeoff = ACE_DEFAULT_CDR_MEMCPY_TRADEOFF;

  // The following things should be changed to use the ACE_Env_Value<>
  // template sometime.

  // List of comma separated prefixes from ORBDefaultInitRef.
  ACE_CString default_init_ref;

  // Name Service port use for Multicast
  u_short ns_port = 0;

  // Trading Service port used for Multicast
  u_short ts_port = 0;

  // Implementation Repository Service port #.
  u_short ir_port = 0;

  // Mcast endpoint for the Naming Service discovery.
  ACE_CString mde;

  // Buffer sizes for kernel socket buffers
  // @@ should be a default defined for each protocol implementation?
  //    since we may have protocols loaded which use shared memory of
  //    some form, fredk
  size_t rcv_sock_size = 0;
  size_t snd_sock_size = 0;

  // Use TCP_NODELAY.
  size_t nodelay = 1;

  // Should we skip the <ACE_Service_Config::open> method, e.g., if we
  // already being configured by the ACE Service Configurator.
  int skip_service_config_open = 0;

  // Use dotted decimal addresses
  // @@ This option will be treated as a suggestion to each loaded
  //    protocol to use a character representation for the numeric
  //    address, otherwise use a logical name. fredk
#if (TAO_USE_DOTTED_DECIMAL_ADDRESSES == 1)
  int dotted_decimal_addresses = 1;
#else
  int dotted_decimal_addresses = 0;
#endif /* TAO_USE_DOTTED_DECIMAL_ADDRESSES */

#if defined (TAO_STD_PROFILE_COMPONENTS)
  int std_profile_components = 1;
#else
  int std_profile_components = 0;
#endif /* TAO_STD_PROFILE_COMPONENTS */

  int use_tss_resources = -1;
  // -1 is unknown, default to what the resource factory sets.
  // @@ This is just for backwards compatibility.

#if defined (TAO_DEBUG)
  // Make it a little easier to debug programs using this code.
  {
    TAO_debug_level = ACE_Env_Value<u_int> ("TAO_ORB_DEBUG", 0);

    char *value = ACE_OS::getenv ("TAO_ORB_DEBUG");

    if (value != 0)
      {
        TAO_debug_level = ACE_OS::atoi (value);
        if (TAO_debug_level <= 0)
          TAO_debug_level = 1;
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO_debug_level == %d"), TAO_debug_level));
      }
  }
#endif  /* TAO_DEBUG */

  while (arg_shifter.is_anything_left ())
    {
      char *current_arg = 0;

      ////////////////////////////////////////////////////////////////
      // begin with the 'parameterless' flags                       //
      ////////////////////////////////////////////////////////////////
      if (arg_shifter.cur_arg_strncasecmp ("-ORBDaemon") == 0)
        {
          // Be a daemon
          this->svc_config_argv_[this->svc_config_argc_++] =
            CORBA::string_dup ("-b");

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp
               ("-ORBSkipServiceConfigOpen") == 0)
        {
          skip_service_config_open = 1;

          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-ORBGIOPlite") == 0)
        {
          // @@ This will have to change since gioplite
          // will be considered as an alternate ORB
          // messaging protocols.
          // giop_lite = 1;
          ACE_DEBUG ((LM_WARNING,
                      ASYS_TEXT ("(%P|%t) This option has been deprecated \n")
                      ASYS_TEXT ("Please use svc.conf file to load the protcol \n")));
          
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-ORBDebug") == 0)
        {
          // later, replace all of these
          // warning this turns on a daemon
          ACE::debug (1);
          TAO_orbdebug = 1;
          arg_shifter.consume_arg ();
        }

      ////////////////////////////////////////////////////////////////
      // continue with the 'parameter' flags                        //
      ////////////////////////////////////////////////////////////////
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBSvcConfDirective")))
        {
          // This is used to pass arguments to the Service
          // Configurator using the "command line" to provide
          // configuration information rather than using a svc.conf
          // file.  Pass the "-S" to the service configurator.
          this->svc_config_argv_[this->svc_config_argc_++] =
            CORBA::string_dup ("-S");

          // Pass the next argument.
          this->svc_config_argv_[this->svc_config_argc_++] =
            CORBA::string_dup (current_arg);

          arg_shifter.consume_arg ();
        }

      else if ((current_arg =
                arg_shifter.get_the_parameter ("-ORBSvcConf")))
        {
          // Specify the name of the svc.conf file to be used.
          this->svc_config_argv_[this->svc_config_argc_++] =
            CORBA::string_dup ("-f");

          this->svc_config_argv_[this->svc_config_argc_++] =
            CORBA::string_dup (current_arg);

          arg_shifter.consume_arg();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBDottedDecimalAddresses")))
        {
          // Use dotted decimal addresses
          // @@ this should be renamed.  See above comment. fredk
          dotted_decimal_addresses =
            ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBDebugLevel")))
        {
          TAO_debug_level =
            ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBEndpoint")))
        {
          // Each "endpoint" is of the form:
          //
          //   protocol://V.v@addr1,addr2,...,addrN
          //
          // or:
          //
          //   protocol://addr1,addr2,...,addrN
          //
          // where "V.v" is an optional protocol version for each
          // addr.  All preconnect or endpoint strings should be of
          // the above form(s).
          //
          // Multiple sets of endpoint may be seperated by a semi-colon `;'.
          // For example:
          //
          //   iioploc://space:2001,1.2@odyssey:2010;uiop://foo,bar
          //
          // All preconnect or endpoint strings should be of the above form(s).

          ACE_CString endpts (current_arg);

          if (this->orb_params ()->endpoints (endpts) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ASYS_TEXT ("(%P|%t)\n")
                          ASYS_TEXT ("Invalid endpoint(s) specified:\n%s\n"),
                          endpts.c_str ()));
              ACE_THROW_RETURN (CORBA::BAD_PARAM (), -1);
            }

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBHost")))
        {
          // @@ This option now has the same effect as specifying
          //    an extra -ORBendpoint.  Ideally, this option
          //    should be removed so that all INET specific
          //    stuff can be removed from the ORB core but I
          //    guess we need to leave it here for backward
          //    compatibility.  C'est la vie.

          old_style_endpoint = 1;
          // Specify the name of the host (i.e., interface) on which
          // the server should listen.

          // Issue a warning since this backward compatibilty support
          // may be dropped in future releases.
          ACE_DEBUG ((LM_WARNING,
                      ASYS_TEXT ("(%P|%t) \nWARNING: The `-ORBHost' option is obsolete.\n")
                      ASYS_TEXT ("In the future, use the `-ORBEndpoint' option.\n")));

          host = current_arg;

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBNameServiceIOR")))
        {
          // Specify the IOR of the NameService.
          // Issue a warning since this backward compatibilty support
          // may be dropped in future releases.
          ACE_DEBUG ((LM_WARNING,
                      ASYS_TEXT ("(%P|%t) \nWARNING: The `-ORBNameServiceIOR' option ")
                      ASYS_TEXT ("is obsolete.\n")
                      ASYS_TEXT ("Please use the `-ORBInitRef ' option instead.\n")));

          // Construct an argument that would be equivalent to
          // "-ORBInitRef NameService=....."
          ACE_CString init_ref =
            ACE_CString (TAO_OBJID_NAMESERVICE) +
            ACE_CString ('=') +
            ACE_CString (current_arg);
          if (this->orb_params ()->add_to_ior_table (init_ref) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ASYS_TEXT ("TAO (%P|%t) Unable to add the Name ")
                          ASYS_TEXT ("Service IOR <%s> to the lookup table.\n"),
                          current_arg));
              ACE_THROW_RETURN (CORBA::INTERNAL (), -1);
            }

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBNameServicePort")))
        {
          // Specify the port number for the NameService.
          // Unrelated to ORB Protocols, this is used for multicast.

          ns_port = (CORBA::UShort) ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBMulticastDiscoveryEndpoint")))
        {
          // Specify mcast address:port for the Naming Service Multicast
          // Discovery Protocol.

          mde = current_arg;
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBNodelay")))
        {
          // Use TCP_NODELAY or not.
          nodelay =
            ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBTradingServiceIOR")))
        {
          // Specify the IOR of the Trading Service.

          // Issue a warning since this backward compatibilty support
          // may be dropped in future releases.
          ACE_DEBUG ((LM_WARNING,
                      ASYS_TEXT ("(%P|%t) \nWARNING: The `-ORBTradingServiceIOR' ")
                      ASYS_TEXT ("option is obsolete.\n")
                      ASYS_TEXT ("Please use the `-ORBInitRef' option instead.\n")));

          // Construct an argument that would be equivalent to
          // "-ORBInitRef TradingService=....."

          ACE_CString init_ref =
            ACE_CString (TAO_OBJID_TRADINGSERVICE) +
            ACE_CString ('=') +
            ACE_CString (current_arg);

          if (this->orb_params ()->add_to_ior_table (init_ref) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ASYS_TEXT ("TAO (%P|%t) Unable to add the Trading ")
                          ASYS_TEXT ("Service IOR <%s> to the lookup table.\n"),
                          current_arg));
              ACE_THROW_RETURN (CORBA::INTERNAL (), -1);
            }

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBTradingServicePort")))
        {
          // Specify the port number for the NameService.

          ts_port = (CORBA::UShort) ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBImplRepoServiceIOR")))
        {
          // Specify the IOR of the Implementation Repository

          // Issue a warning since this backward compatibilty support
          // may be dropped in future releases.
          ACE_DEBUG ((LM_WARNING,
                      ASYS_TEXT ("(%P|%t) \nWARNING: The `-ORBImplRepoServiceIOR' ")
                      ASYS_TEXT ("option is obsolete.\n")
                      ASYS_TEXT ("Please use the `-ORBInitRef' option instead.\n")));

          // Construct an argument that would be equivalent to
          // "-ORBInitRef ImplRepoService=....."

          ACE_CString init_ref =
            ACE_CString (TAO_OBJID_IMPLREPOSERVICE) +
            ACE_CString ('=') +
            ACE_CString (current_arg);

          if (this->orb_params ()->add_to_ior_table (init_ref) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ASYS_TEXT ("TAO (%P|%t) Unable to add the ")
                          ASYS_TEXT ("Implementation Repository IOR <%s> to ")
                          ASYS_TEXT ("the lookup table.\n"),
                          current_arg));
              ACE_THROW_RETURN (CORBA::INTERNAL (), -1);
            }

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBImplRepoServicePort")))
        {
          // Specify the multicast port number for the Implementation
          // Repository.
          ir_port = (CORBA::UShort) ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBPort")))
        {
          // Issue a warning since this backward compatibilty support
          // may be dropped in future releases.

          old_style_endpoint = 1;
          ACE_DEBUG ((LM_WARNING,
                      ASYS_TEXT ("(%P|%t) \nWARNING: The `-ORBPort' option is obsolete.\n")
                      ASYS_TEXT ("In the future, use the `-ORBEndpoint' option.\n")));

          // Specify the port number/name on which we should listen
          // We really shouldn't limit this to being specified as
          // an int, but oh well for now.
          port = (CORBA::UShort) ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBRcvSock")))
        {
          // @@ All protocol implementation may not use sockets, so
          //    this can either be a generic I/O Buffer size or
          //    Buffer info can be a per protocol specification, fredk

          // Specify the size of the socket's receive buffer

          rcv_sock_size = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBSndSock")))
        {
          // @@ All protocol implementation may not use sockets, so
          //    this can either be a generic I/O Buffer size or
          //    Buffer info can be a per protocol specification, fredk

          // Specify the size of the socket's send buffer
          snd_sock_size = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBObjRefStyle")))
        {
          // Specifies the style of printed objrefs: URL or IOR
          //
          // BEGIN COMMENTS FROM IIOP-1.4 On Win32, we should be
          // collecting information from the Registry such as what
          // ORBs are configured, specific configuration details like
          // whether they generate IOR or URL style stringified
          // objrefs and which addresses they listen to (e.g. allowing
          // multihomed hosts to implement firewalls), user-meaningful
          // orb names (they will normally indicate domains), and
          // more.
          //
          // On UNIX, we should collect that from some private config
          // file.
          //
          // Instead, this just treats the "internet" ORB name
          // specially and makes it always use URL-style stringified
          // objrefs, where the hostname and TCP port number are
          // explicit (and the whole objref is readable by mortals).
          // BEGIN COMMENTS FROM IIOP-1.4
          char *opt = current_arg;
          if (ACE_OS::strcasecmp (opt,
                                  "URL") == 0)
            use_ior = 0;

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBCollocationStrategy")))
        {
          // Specify which collocation policy we want to use.
          char *opt = current_arg;
          if (ACE_OS::strcasecmp (opt, "thru_poa") == 0)
            this->collocation_strategy_ = THRU_POA;
          else if (ACE_OS::strcasecmp (opt, "direct") == 0)
            this->collocation_strategy_ = DIRECT;

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBCollocation")))
        {
          // Specify whether we want to optimize against collocation
          // objects.  Valid arguments are: "yes" and "no".
          // Default is yes.

          int yes_implies_global = 0;

          char *opt = current_arg;
          if (ACE_OS::strcasecmp (opt, "YES") == 0)
            {
              yes_implies_global = 1;
              ACE_DEBUG ((LM_WARNING,
                          ASYS_TEXT ("WARNING: using '-ORBCollocation YES' is obsolete ")
                          ASYS_TEXT ("and implies '-ORBCollocation global'")
                          ASYS_TEXT ("  Please use '-ORBCollocation global' instead.\n")));
            }

          if (yes_implies_global ||
              ACE_OS::strcasecmp (opt, "global") == 0)
            {
              this->opt_for_collocation_ = 1;
              this->use_global_collocation_ = 1;
            }
          else if (ACE_OS::strcasecmp (opt, "NO") == 0)
            this->opt_for_collocation_ = 0;
          else if (ACE_OS::strcasecmp (opt, "per-orb") == 0)
            {
              this->opt_for_collocation_ = 1;
              this->use_global_collocation_ = 0;
            }

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBGlobalCollocation")))
        // Specify whether we want to use collocation across ORBs;
        // i.e. all the ORBs in the same address space use collocated
        // calls.
        {
          ACE_DEBUG ((LM_WARNING,
                      ASYS_TEXT ("WARNING: -ORBGlobalCollocation option is obsolete.")
                      ASYS_TEXT ("  Please use '-ORBCollocation global/per-orb/no'")
                      ASYS_TEXT ("instead.\n")));

          char *opt = current_arg;
          if (ACE_OS::strcasecmp (opt, "YES") == 0)
            {
              this->use_global_collocation_ = 1;
            }
          else if (ACE_OS::strcasecmp (opt, "NO") == 0)
            {
              this->use_global_collocation_ = 0;
            }

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBPreconnect")))
        {
          // Get a string which describes the connections we want to
          // cache up-front, thus reducing the latency of the first call.
          //
          // For example,  specify -ORBpreconnect once for each
          // protocol:
          //
          //   -ORBpreconnect iiop://tango:10015,watusi:10016
          //   -ORBpreconnect busX_iop://board1:0x07450000,board2,0x08450000
          //
          // Or chain all possible endpoint designations together:
          //
          //   -ORBpreconnect iiop://tango:10015,watusi:10016/;
          //              busX_iop://board1:0x07450000,board2,0x08450000/
          //
          // The old style command line only works for IIOP:
          //    -ORBpreconnect tango:10015,tango:10015,watusi:10016

          ACE_CString preconnections (current_arg);

          if (this->orb_params ()->preconnects (preconnections) != 0)
            {
              // Handle old style preconnects for backward compatibility.
              // The old style preconnects only work for IIOP!

              // Issue a warning since this backward compatibilty support
              // may be dropped in future releases.

              ACE_DEBUG ((LM_WARNING,
                          ASYS_TEXT ("(%P|%t) \nWARNING: The `host:port' pair style ")
                          ASYS_TEXT ("for `-ORBPreconnect' is obsolete.\n")
                          ASYS_TEXT ("In the future, use the URL style.\n")));

              preconnections =
                ACE_CString ("iiop://") +
                ACE_CString (preconnections);

              ACE_DEBUG ((LM_WARNING,
                          ASYS_TEXT ("(%P|%t) \nWARNING: The following preconnection ")
                          ASYS_TEXT ("will be used:\n%s\n"),
                          preconnections.c_str()));

              this->orb_params ()->preconnects (preconnections);
            }

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBCDRTradeoff")))
        {
          cdr_tradeoff = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }

      // A new <ObjectID>:<IOR> mapping has been specified. This will be
      // used by the resolve_initial_references ().

      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBInitRef")))
        {
          ACE_CString init_ref (current_arg);
          if (this->orb_params ()->add_to_ior_table (init_ref) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ASYS_TEXT ("Unable to add initial reference:\n")
                          ASYS_TEXT ("%s\n")
                          ASYS_TEXT ("to the initial reference lookup table.\n"),
                          current_arg));
              ACE_THROW_RETURN (CORBA::INTERNAL (), -1);
            }

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBDefaultInitRef")))
        {
          default_init_ref = current_arg;
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBStdProfileComponents")))
        {
          std_profile_components =
            ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBResources")))
        {
          char *opt = current_arg;
          if (ACE_OS::strcasecmp (opt, "global") == 0)
            {
              use_tss_resources = 0;
            }
          else if (ACE_OS::strcasecmp (opt, "tss") == 0)
            {
              use_tss_resources = 1;
            }
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBLogFile")))
        {
          //
          // redirect all ACE_DEBUG and ACE_ERROR output to a file
          // USAGE: -ORBLogFile <file>
          // default: if <file> is present     = append
          //          if <file> is not present = create
          //

          ASYS_TCHAR* file_name = current_arg;
          arg_shifter.consume_arg ();

          //
          // would rather use ACE_OSTREAM_TYPE out here..
          // but need ACE_FSTREAM_TYPE to call ->open(...)
          // and haven't found such a macro to rep FILE* and/or fstream*
          //

#if defined (ACE_LACKS_IOSTREAM_TOTALLY)

          FILE* output_stream = ACE_OS::fopen (file_name, "a");

          ACE_LOG_MSG->msg_ostream (output_stream, 1);

#else /* ! ACE_LACKS_IOSTREAM_TOTALLY */

          ofstream* output_stream = 0;

          //
          // note: we are allocating dynamic memory here....but
          // I assume it will persist for the life of the program
          //

          ACE_NEW_THROW_EX (output_stream,
                            ofstream (),
                            CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                              CORBA::COMPLETED_NO));
          ACE_CHECK_RETURN (1);

          output_stream->open (file_name, ios::out | ios::app);

          if (!output_stream->bad ())
            {
              ACE_LOG_MSG->msg_ostream (output_stream, 1);
            }

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

          ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER);
          ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);

        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBUseIMR")))
        {
          // Use IR or not.
          this->use_implrepo_ = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBSetUID")))
        {
          // Set the effective user ID of the current ORB process.
          uid_t orb_uid =
            ACE_static_cast (uid_t, ACE_OS::atoi (current_arg));

          arg_shifter.consume_arg ();

          if (ACE_OS::setuid (orb_uid) != 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ASYS_TEXT ("Error setting effective user ")
                                 ASYS_TEXT ("ID for ORB <%s>%p\n"),
                                 this->orbid_,
                                 ASYS_TEXT("")),
                                -1);
            }
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBSetGID")))
        {
          // Set the effective group ID of the current ORB process.

          uid_t orb_gid =
            ACE_static_cast (gid_t, ACE_OS::atoi (current_arg));

          arg_shifter.consume_arg ();

          if (ACE_OS::setgid (orb_gid) != 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ASYS_TEXT ("Error setting effective group ")
                                 ASYS_TEXT ("ID for ORB <%s>%p\n"),
                                 this->orbid_,
                                 ASYS_TEXT("")),
                                -1);
            }
        }

      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBid")))
        {
          // The ORBid is actually set in ORB_init(), and then passed
          // to the TAO_ORB_Core() constructor.  However, in the case
          // where the ORBid third argument to ORB_init() is not zero,
          // any "-ORBid" arguments in the argv argument list are
          // supposed to be ignored, according to the CORBA spec.  As
          // such, "-ORBid" must be removed from the argument list
          // since ORB_init() must remove all "-ORB" option
          // arguments.

          // We obtain a lock on the ORB table when setting the
          // ORBid.  For this reason we should *not* set the ORBid
          // here.  CORBA::ORB_init() does all of the proper locking
          // and setting.

          arg_shifter.consume_arg ();
        }

      ////////////////////////////////////////////////////////////////
      // catch any unknown -ORB args                                //
      ////////////////////////////////////////////////////////////////
      else if (arg_shifter.cur_arg_strncasecmp ("-ORB") != -1)
        {
          if (TAO_debug_level > 0)
            {
              current_arg = arg_shifter.get_current ();
              ACE_DEBUG ((LM_ERROR,
                          ASYS_TEXT ("ERROR: Unknown \"-ORB\" option ")
                          ASYS_TEXT ("<%s>.\n"),
                          ((current_arg == 0) ? "<NULL>" : current_arg)));
            }

          ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                            -1);
        }

      ////////////////////////////////////////////////////////////////
      // ok, we can't interpret this argument, move to next argument//
      ////////////////////////////////////////////////////////////////
      else
        // Any arguments that don't match are ignored so that the
        // caller can still use them.
        arg_shifter.ignore_arg ();
    }

#if defined (SIGPIPE) && !defined (ACE_LACKS_UNIX_SIGNALS)
  // There's really no way to deal with this in a portable manner, so
  // we just have to suck it up and get preprocessor conditional and
  // ugly.
  //
  // Impractical to have each call to the ORB protect against the
  // implementation artifact of potential writes to dead connections,
  // as it'd be way expensive.  Do it here; who cares about SIGPIPE in
  // these kinds of applications, anyway?
  (void) ACE_OS::signal (SIGPIPE, SIG_IGN);
#endif /* SIGPIPE */

  // Initialize the Service Configurator -check for return values.
  // Load the resource factory, connector registry, acceptor registry
  // and protocols.  Will need to call the open () method on
  // the registries!
  int result = TAO_Internal::open_services (this->svc_config_argc_,
                                            this->svc_config_argv_,
                                            0,
                                            skip_service_config_open);

  // Make sure to free up all the dynamically allocated memory.  If we
  // decide we don't need to allocate this stuff dynamically then we
  // can remove this.
  for (int i = 0; i < this->svc_config_argc_; i++)
    CORBA::string_free (this->svc_config_argv_[i]);

  delete [] this->svc_config_argv_;
  this->svc_config_argc_ = 0;
  this->svc_config_argv_ = 0;

  // Check for errors returned from <TAO_Internal::open_services>.
  if (result != 0 && errno != ENOENT)
    {
      ACE_ERROR ((LM_ERROR,
                  ASYS_TEXT ("(%P|%t) %p\n"),
                  ASYS_TEXT ("ORB Core unable to initialize the ")
                  ASYS_TEXT ("Service Configurator")));
      ACE_THROW_RETURN (CORBA::INITIALIZE (), -1);
    }

  // Initialize the pointers to resources in the ORB Core instance,
  // e.g., reactor, connector, etc.  Must do this after we open
  // services because we'll load the factory from there.
  TAO_Resource_Factory *trf = this->resource_factory ();

  if (trf == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ASYS_TEXT ("(%P|%t) %p\n"),
                  ASYS_TEXT ("ORB Core unable to find a Resource Factory instance")));
      ACE_THROW_RETURN (CORBA::INTERNAL (), -1);
    }

  if (use_tss_resources == -1)
    this->use_tss_resources_ = trf->use_tss_resources ();
  else
    this->use_tss_resources_ = use_tss_resources;

  this->reactor_registry_ =
    trf->get_reactor_registry ();
  this->reactor_registry_->open (this);

  this->priority_mapping_ =
    trf->get_priority_mapping ();

  // @@ ????
  // Make sure the reactor is initialized...
  ACE_Reactor *reactor = this->reactor ();
  if (reactor == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ASYS_TEXT ("(%P|%t) %p\n"),
                  ASYS_TEXT ("ORB Core unable to initialize reactor")));
      ACE_THROW_RETURN (CORBA::INITIALIZE (), -1);
    }

  TAO_Server_Strategy_Factory *ssf = this->server_factory ();

  if (ssf == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ASYS_TEXT ("(%P|%t) %p\n"),
                  ASYS_TEXT ("ORB Core unable to find a Server Strategy Factory ")
                  ASYS_TEXT ("instance")));
      ACE_THROW_RETURN (CORBA::INTERNAL (), -1);
    }

  ssf->open (this);

  // Obtain the timeout value for the thread-per-connection model
  this->thread_per_connection_use_timeout_ =
    ssf->thread_per_connection_timeout (this->thread_per_connection_timeout_);

  if (thread_per_connection_use_timeout_ == -1)
    {
      if (ACE_OS::strcasecmp (TAO_DEFAULT_THREAD_PER_CONNECTION_TIMEOUT,
                              "INFINITE") == 0)
        {
          this->thread_per_connection_use_timeout_ = 0;
        }
      else
        {
          this->thread_per_connection_use_timeout_ = 1;
          int milliseconds =
            ACE_OS::atoi (TAO_DEFAULT_THREAD_PER_CONNECTION_TIMEOUT);
          // Use a temporary to obtain automatic normalization.
          this->thread_per_connection_timeout_ =
            ACE_Time_Value (0, 1000 * milliseconds);
        }
    }
  if (this->thread_per_connection_use_timeout_ == 0)
    {
      // Do not wait for the server threads because they may block
      // forever.
      this->tm_.wait_on_exit (0);
    }

  // Initialize the "ORB" pseudo-object now.
  CORBA::ORB_ptr temp_orb = CORBA::ORB::_nil ();

  ACE_NEW_THROW_EX (temp_orb,
                    CORBA_ORB (this),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (-1);

  this->orb_ = temp_orb;

  // This should probably move into the ORB Core someday rather then
  // being done at this level.
  this->orb_->_use_omg_ior_format (use_ior);

  // @@ Michael: I don't know if this is the best spot,
  //    we might have to discuss that.
  //this->leader_follower_lock_ptr_ =  this->client_factory ()
  //                                       ->create_leader_follower_lock ();

  // Set all kinds of orb parameters whose setting needed to be
  // deferred until after the service config entries had been
  // determined.

  // @@ Set the endpoint string to iiop://host:port
  //    Add a string to hold the endpoint designation for this ORB
  //    for now it will be iiop://host:port
  //              -fredk
  if (old_style_endpoint)
    {
      ACE_CString iiop_endpoint;
      if (this->set_iiop_endpoint (dotted_decimal_addresses,
                                   port,
                                   host,
                                   iiop_endpoint) == -1)
        ACE_THROW_RETURN (CORBA::BAD_PARAM (), -1);
      // Add the endpoint
      this->orb_params ()->endpoints (iiop_endpoint);
    }

  // Set the list of prefixes from -ORBDefaultInitRef.
  this->orb_params ()->default_init_ref (default_init_ref);

  this->orb_params ()->service_port (NAMESERVICE, ns_port);
  this->orb_params ()->service_port (TRADINGSERVICE, ts_port);
  this->orb_params ()->service_port (IMPLREPOSERVICE, ir_port);

  this->orb_params ()->mcast_discovery_endpoint (mde);
  this->orb_params ()->use_dotted_decimal_addresses (dotted_decimal_addresses);
  this->orb_params ()->nodelay (nodelay);
  if (rcv_sock_size != 0)
    this->orb_params ()->sock_rcvbuf_size (rcv_sock_size);
  if (snd_sock_size != 0)
    this->orb_params ()->sock_sndbuf_size (snd_sock_size);
  if (cdr_tradeoff >= 0)
    this->orb_params ()->cdr_memcpy_tradeoff (cdr_tradeoff);

  this->orb_params ()->std_profile_components (std_profile_components);

  // Set up the pluggable protocol infrastructure.  First get a
  // pointer to the protocol factories set, then obtain pointers to
  // all factories loaded by the service configurator.
  // Load all protocol factories!
  if (trf->init_protocol_factories () == -1)
    ACE_THROW_RETURN (CORBA::INITIALIZE (), -1);

  // init the ORB core's pointer
  this->protocol_factories_ = trf->get_protocol_factories ();

  // Now that we have a complete list of available protocols and their
  // related factory objects, initialize the registries!

  // Initialize the connector registry and create a connector for each
  // configured protocol.
  if (this->connector_registry ()->open (this) != 0)
    ACE_THROW_RETURN (CORBA::INITIALIZE (), -1);

  // Have the connector registry parse the preconnects.
  if (this->orb_params ()->preconnects ().is_empty () == 0)
    this->connector_registry ()->preconnect (
                      this,
                      this->orb_params ()->preconnects ());

  // The ORB has been initialized, meaning that the ORB is no longer
  // in the shutdown state.
  this->has_shutdown_ = 0;

  return 0;
}

int
TAO_ORB_Core::set_iiop_endpoint (int dotted_decimal_addresses,
                                 CORBA::UShort port,
                                 ACE_CString &host,
                                 ACE_CString &endpoint)
{
  // @@ It would be nice to get rid of this environment variable at
  //    some point in the near future.
  //       -Ossama

  // Check if the a default port has been specified by the user in an
  // environment variable.
  ACE_Env_Value<int> defport ("TAO_DEFAULT_SERVER_PORT",
                              TAO_DEFAULT_SERVER_PORT);

  if (port == 0)
    port = ACE_static_cast (CORBA::UShort, defport);

  // No host specified; find it
  if (host.length () == 0)
    {
      ASYS_TCHAR name[MAXHOSTNAMELEN + 1];
      if (ACE_OS::hostname (name, MAXHOSTNAMELEN + 1) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ASYS_TEXT ("(%P|%t) %p\n"),
                             ASYS_TEXT ("failed to look up local host name")),
                            -1);
        }
      host.set (name, 1);
    }

  // @@ For compatibility (ug) with how things were done before,
  //    get the local host name in the correct format.  This will be
  //    stored away in the ORB!  fredk
  ACE_INET_Addr rendezvous;
  rendezvous.set (port, host.c_str ());

  char buffer[MAXHOSTNAMELEN + 1];

  if (rendezvous.addr_to_string (buffer,
                                 MAXHOSTNAMELEN,
                                 dotted_decimal_addresses) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("(%P|%t) %p\n"),
                         ASYS_TEXT ("failed in addr_to_string ()")),
                        -1);
    }

  // endpoint == iiop://host:port
  endpoint.set ("iiop://", 1);
  endpoint += buffer;

  return 0;
}

int
TAO_ORB_Core::fini (void)
{
  // Wait for any server threads, ignoring any failures.
  (void) this->thr_mgr ()->wait ();

  CORBA::release (this->implrepo_service_);

  if (TAO_debug_level >= 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("Destroying ORB <%s>\n"),
                  this->orbid_));
    }

  // Close connectors before acceptors!
  // Ask the registry to close all registered connectors.
  if (this->connector_registry_ != 0)
    {
      this->connector_registry_->close_all ();
      delete this->connector_registry_;
    }

  // Ask the registry to close all registered acceptors.
  if (this->acceptor_registry_ != 0)
    {
      this->acceptor_registry_->close_all ();
      delete this->acceptor_registry_;
    }

  TAO_Internal::close_services ();

  // @@ This is not needed since the default resource factory
  //    is statically added to the service configurator, fredk
  if (!this->resource_factory_from_service_config_)
    delete resource_factory_;
  

  // @@ This is not needed since the default client factory
  //    is statically added to the service configurator, fredk
  if (!this->client_factory_from_service_config_)
    delete client_factory_;


  // @@ This is not needed since the default server factory
  //    is statically added to the service configurator, fredk
  if (!this->server_factory_from_service_config_)
    delete server_factory_;

  {
    ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard,
                              *ACE_Static_Object_Lock::instance (), 0));
    TAO_ORB_Table::instance ()->unbind (this->orbid_);
  }

  delete this->reactor_registry_;

#if (TAO_HAS_RT_CORBA == 1)
  delete this->priority_mapping_;
#endif /* TAO_HAS_RT_CORBA == 1 */

  delete this;

  return 0;
}

TAO_Resource_Factory *
TAO_ORB_Core::resource_factory (void)
{
  if (this->resource_factory_ == 0)
    {
      // Look in the service repository for an instance.
      this->resource_factory_ =
        ACE_Dynamic_Service<TAO_Resource_Factory>::instance (
                                                       "Resource_Factory");
      // @@ Not needed!
      this->resource_factory_from_service_config_ = 1;
    }

  //@@ None of this stuff is needed since the default resource factory
  //   is statically adde to the Service Configurator!
  if (this->resource_factory_ == 0)
    {
      // Still don't have one, so let's allocate the default.  This
      // will throw an exception if it fails on exception-throwing
      // platforms.
      if (TAO_orbdebug)
        ACE_ERROR ((LM_WARNING,
                    ASYS_TEXT ("(%P|%t) WARNING - No Resource Factory found ")
                    ASYS_TEXT ("in Service Repository.\n")
                    ASYS_TEXT ("  Using default instance with GLOBAL resource ")
                    ASYS_TEXT ("source specifier.\n")));

      TAO_Default_Resource_Factory *default_factory;
      ACE_NEW_RETURN (default_factory,
                      TAO_Default_Resource_Factory,
                      0);

      // @@ Not needed.
      this->resource_factory_from_service_config_ = 0;
      this->resource_factory_ = default_factory;

      // @@ At this point we need to register this with the
      // Service_Repository in order to get it cleaned up properly.
      // But, for now we let it leak.
    }
  return this->resource_factory_;
}

TAO_Client_Strategy_Factory *
TAO_ORB_Core::client_factory (void)
{
  if (this->client_factory_ == 0)
    {
      // Look in the service repository for an instance.
      this->client_factory_ =
        ACE_Dynamic_Service<TAO_Client_Strategy_Factory>::instance (
                                                  "Client_Strategy_Factory");
      // @@ Not needed!
      this->client_factory_from_service_config_ = 1;
    }

  //@@ None of this stuff is needed since the default client factory
  //   is statically added to the Service Configurator, fredk
  if (this->client_factory_ == 0)
    {
      // Still don't have one, so let's allocate the default.  This
      // will throw an exception if it fails on exception-throwing
      // platforms.
      if (TAO_orbdebug)
        ACE_ERROR ((LM_WARNING,
                    ASYS_TEXT ("(%P|%t) WARNING - No Client Strategy Factory found ")
                    ASYS_TEXT ("in Service Repository.\n")
                    ASYS_TEXT ("  Using default instance.\n")));

      ACE_NEW_RETURN (this->client_factory_,
                      TAO_Default_Client_Strategy_Factory,
                      0);

      this->client_factory_from_service_config_ = 0;
      // At this point we need to register this with the
      // Service_Repository in order to get it cleaned up properly.
      // But, for now we let it leak.
    }
  return this->client_factory_;
}

TAO_Server_Strategy_Factory *
TAO_ORB_Core::server_factory (void)
{
  if (this->server_factory_ == 0)
    {
      // Look in the service repository for an instance.
      this->server_factory_ =
        ACE_Dynamic_Service<TAO_Server_Strategy_Factory>::instance (
                                                "Server_Strategy_Factory");
      // @@ Not needed!
      this->server_factory_from_service_config_ = 1;
    }

  //@@ None of this stuff is needed since the default server factory
  //   is statically added to the Service Configurator, fredk

  // If the <server_factory_> isn't found it's usually because the ORB
  // hasn't been intialized correctly...
  if (this->server_factory_ == 0)
    {
      // Still don't have one, so let's allocate the default.
      if (TAO_orbdebug)
        ACE_ERROR ((LM_WARNING,
                    ASYS_TEXT ("(%P|%t) WARNING - No %s found in Service Repository.")
                    ASYS_TEXT ("  Using default instance.\n"),
                    ASYS_TEXT ("Server Strategy Factory")));

      ACE_NEW_RETURN (this->server_factory_,
                      TAO_Default_Server_Strategy_Factory,
                      0);

      // @@ Not needed!
      this->server_factory_from_service_config_ = 0;
      // At this point we need to register this with the
      // <Service_Repository> to get it cleaned up properly.  But, for
      // now we let it leak.
    }

  return this->server_factory_;
}

int
TAO_ORB_Core::inherit_from_parent_thread (
    TAO_ORB_Core_TSS_Resources *tss_resources)
{
  // Inherit properties/objects used in ORB_Core from the
  // parent thread.  Stuff inherited here must already exist
  // in the "parent" orbcore.
  // This is used in the thread-per-connection concurrency model where
  // each ORB spawned thread must use the resources of the spawning
  // thread...

  if (tss_resources == 0)
    return -1;
#if 0
  if (tss_resources->reactor_ != 0)
    {
      // We'll use the spawning thread's reactor.
      TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
      if (tss->reactor_ != 0 && TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) non nil reactor on thread startup!\n"));

          if (tss == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%P|%t) %p\n",
                               "TAO_ORB_Core::inherit_from_parent_thread"
                               " (); no more TSS keys"),
                              -1);

          if (/* tss->owns_resources_ != 0 && */ !tss->inherited_reactor_)
            delete tss->reactor_;
        }
      tss->reactor_ = tss_resources->reactor_;
      tss->inherited_reactor_ = 1;
    }
#endif /* 0 */
  // this->connection_cache (tss_resources->connection_cache_);
  // Inherit connection cache?
  return 0;
}

PortableServer::POA_ptr
TAO_ORB_Core::root_poa_reference (CORBA::Environment &ACE_TRY_ENV,
                                  const char *adapter_name,
                                  TAO_POA_Manager *poa_manager,
                                  const TAO_POA_Policies *policies)
{
  if (CORBA::is_nil (this->root_poa_reference_.in ()))
    {
      this->create_and_set_root_poa (adapter_name,
                                     poa_manager,
                                     policies,
                                     ACE_TRY_ENV);
      ACE_CHECK_RETURN (PortableServer::POA::_nil ());
    }

  return PortableServer::POA::_duplicate (this->root_poa_reference_.in ());
}

void
TAO_ORB_Core::create_and_set_root_poa (const char *adapter_name,
                                       TAO_POA_Manager *poa_manager,
                                       const TAO_POA_Policies *policies,
                                       CORBA::Environment &ACE_TRY_ENV)
{
  if (this->root_poa_ == 0)
    {
      // Double checked locking
      ACE_GUARD (ACE_SYNCH_MUTEX, ace_mon, this->lock_);
      if (this->root_poa_ == 0)
        {
          // Only set the auto_ptr if poa_manager is allocated here.
          auto_ptr<TAO_POA_Manager> safe_poa_manager;
          if (poa_manager == 0)
            {
              ACE_NEW_THROW_EX (poa_manager,
                                TAO_POA_Manager (*this->object_adapter_i ()),
                                CORBA::NO_MEMORY ());
              ACE_CHECK;

              ACE_AUTO_PTR_RESET (safe_poa_manager,
                                  poa_manager,
                                  TAO_POA_Manager);
            }

          TAO_POA_Policies root_poa_policies;
          if (policies == 0)
            {
              // RootPOA policies defined in spec
              root_poa_policies.implicit_activation (
                  PortableServer::IMPLICIT_ACTIVATION);

              policies = &root_poa_policies;
            }

          // Construct a new POA
          ACE_NEW_THROW_EX (this->root_poa_,
                            TAO_POA (adapter_name,
                                     *poa_manager,
                                     *policies,
                                     0,
                                     this->object_adapter_i ()->lock (),
                                     this->object_adapter_i ()->thread_lock (),
                                     *this,
                                     ACE_TRY_ENV),
                            CORBA::NO_MEMORY ());
          ACE_CHECK;

          this->root_poa_reference_ =
            this->root_poa_->_this (ACE_TRY_ENV);
          ACE_CHECK;

          // Release the auto_ptr since we got here without error.
          poa_manager = safe_poa_manager.release ();
        }
    }
}

TAO_Object_Adapter *
TAO_ORB_Core::object_adapter (void)
{
  if (this->object_adapter_ == 0)
    {
      // Double checked locking
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, 0);
      (void) this->object_adapter_i ();
    }

  return this->object_adapter_;
}

TAO_Object_Adapter *
TAO_ORB_Core::object_adapter_i (void)
{
  if (this->object_adapter_ == 0)
    {
      ACE_NEW_RETURN (this->object_adapter_,
                      TAO_Object_Adapter (this->server_factory ()->
                                    active_object_map_creation_parameters (),
                                          *this),
                      0);
    }
  return this->object_adapter_;
}

ACE_SYNCH_CONDITION *
TAO_ORB_Core::leader_follower_condition_variable (void)
{
  // Always using TSS.

  // Get tss key.
  TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();

  if (tss->leader_follower_condition_variable_ == 0)
    {
      // Create a new one and return.
      ACE_NEW_RETURN (tss->leader_follower_condition_variable_,
                      ACE_SYNCH_CONDITION (this->leader_follower ().lock ()),
                      0);
    }

  return tss->leader_follower_condition_variable_;
}

TAO_Stub *
TAO_ORB_Core::create_stub_object (const TAO_ObjectKey &key,
                                  const char *type_id,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  (void) this->open (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  CORBA::String_var id;

  if (type_id)
    id = CORBA::string_dup (type_id);

  TAO_Stub *stub = 0;

  size_t pfile_count =
    this->acceptor_registry ()->endpoint_count ();

  // First we create a profile list, well actually the empty container
  TAO_MProfile mp (pfile_count);

  if (this->acceptor_registry ()->make_mprofile (key, mp) == -1)
    ACE_THROW_RETURN (CORBA::INTERNAL (
                        CORBA::SystemException::_tao_minor_code (
                           TAO_MPROFILE_CREATION_ERROR,
                           0),
                        CORBA::COMPLETED_NO),
                      0);

  ACE_NEW_THROW_EX (stub,
                    TAO_Stub (id._retn (), mp, this),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_MAYBE));
  ACE_CHECK_RETURN (stub);

  return stub;
}

int
TAO_ORB_Core::is_collocated (const TAO_MProfile& mprofile)
{
  if (this->acceptor_registry_ == 0)
    return 0;

  // @@ Lots of issues arise when dealing with collocation.  What about
  //    forwarding or what if this is a multi-profile IOR where the order is
  //    significant and only one of the profiles is collocated.  For example
  //    when using a multiple servers for fault tolerance.  For now, we just
  //    look through all profiles and if any are colocated then we assume
  //    the object is collocated.
  // @@ Note, if collocated we can not be forwarded!
  //    Also, acceptor_registry_->is_collocated (...) will check the
  //    address (ORB Host) but not the object_key.  This should be checked
  //    also.

  return this->acceptor_registry_->is_collocated (mprofile);
}

// ****************************************************************

TAO_Leader_Follower &
TAO_ORB_Core::leader_follower (void)
{
  return this->reactor_registry_->leader_follower ();
}

int
TAO_ORB_Core::run (ACE_Time_Value *tv,
                   int perform_work,
                   CORBA::Environment &ACE_TRY_ENV)
{
  if (TAO_debug_level >= 3)
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("TAO (%P|%t) - start of run/perform_work\n")));

  TAO_Leader_Follower &leader_follower = this->leader_follower ();
  TAO_LF_Event_Loop_Thread_Helper event_loop_thread_helper (leader_follower);

  int result = event_loop_thread_helper.set_event_loop_thread (tv);
  if (result != 0)
    {
      if (errno == ETIME)
        return 0;
      else
        return result;
    }

  ACE_Reactor *r = this->reactor ();

  // @@ Do we really need to do this?
  // Set the owning thread of the Reactor to the one which we're
  // currently in.  This is necessary b/c it's possible that the
  // application is calling us from a thread other than that in which
  // the Reactor's CTOR (which sets the owner) was called.
  r->owner (ACE_Thread::self ());

  // This method should only be called by servers, so now we set up
  // for listening!

  int ret = this->open (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (ret == -1)
    return -1;

  result = 1;
  // 1 to detect that nothing went wrong

  // Loop handling client requests until the ORB is shutdown.

  // @@ We could use the leader-follower lock to check for the state
  //    of this variable or use the lock <create_event_loop_lock> in
  //    the server strategy factory.
  //    We don't need to do this because we use the Reactor
  //    mechanisms to shutdown in a thread-safe way.
  while (this->has_shutdown () == 0)
    {
      if (TAO_debug_level >= 3)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("TAO (%P|%t) - blocking on handle events\n")));
      switch (r->handle_events (tv))
        {
        case 0:
          // Make sure that a timed out occured.  If so, we return to
          // caller.
          if (tv != 0 && *tv == ACE_Time_Value::zero)
            result = 0;
          break;
          /* NOTREACHED */
        case -1: // Something else has gone wrong, so return to caller.
          result = -1;
          break;
          /* NOTREACHED */
        default:
          // Some handlers were dispatched, so keep on processing
          // requests until we're told to shutdown .
          break;
          /* NOTREACHED */
        }
      if (result == 0 || result == -1)
        break;

      // In perform_work, we only run the loop once.
      if (perform_work)
        break;
    }

  if (TAO_debug_level >= 3)
    ACE_DEBUG ((LM_DEBUG, ASYS_TEXT ("TAO (%P|%t) - end of run/perform_work %d\n"), result));

  return result;
}

void
TAO_ORB_Core::shutdown (CORBA::Boolean wait_for_completion,
                        CORBA::Environment &ACE_TRY_ENV)
{
  // Is the <wait_for_completion> semantics for this thread correct?
  TAO_POA::check_for_valid_wait_for_completions (wait_for_completion,
                                                 ACE_TRY_ENV);
  ACE_CHECK;

  // If the ORB::shutdown operation is called, it makes a call on
  // deactivate with a TRUE etherealize_objects parameter for each POA
  // manager known in the process; the wait_for_completion parameter
  // to deactivate will be the same as the similarly named parameter
  // of ORB::shutdown.
  this->object_adapter ()->deactivate (wait_for_completion,
                                       ACE_TRY_ENV);
  ACE_CHECK;

  // Set the shutdown flag
  this->has_shutdown_ = 1;

  // Shutdown all the reactors....
  this->reactor_registry_->shutdown_all ();

  // Grab the thread manager
  ACE_Thread_Manager *tm = this->thr_mgr ();

  // Try to cancel all the threads in the ORB.
  tm->cancel_all ();

  // If <wait_for_completion> is set, wait for all threads to exit.
  if (wait_for_completion != 0)
    tm->wait ();
}

void
TAO_ORB_Core::destroy (CORBA_Environment &ACE_TRY_ENV)
{
  if (this->has_shutdown () == 0)
    {
      // Shutdown the ORB and block until the shutdown is complete.
      this->shutdown (1, ACE_TRY_ENV);
      ACE_CHECK;
    }

  // Now remove it from the ORB table so that it's ORBid may be
  // reused.
  {
    ACE_MT (ACE_GUARD (ACE_SYNCH_RECURSIVE_MUTEX, guard,
                       *ACE_Static_Object_Lock::instance ()));
    TAO_ORB_Table::instance ()->unbind (this->orbid_);
  }

  // Destroy the ORB_Core.
  if (this->fini () != 0)
    {
      ACE_THROW (CORBA::INTERNAL (TAO_DEFAULT_MINOR_CODE,
                                  CORBA::COMPLETED_MAYBE));
    }
}


// Set up listening endpoints.

int
TAO_ORB_Core::open (CORBA::Environment &ACE_TRY_ENV)
{
  // Double check pattern
  if (this->open_called_ == 1)
    return 1;

  ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, tao_mon, this->open_lock_, -1);

  if (this->open_called_ == 1)
    return 1;

  TAO_Acceptor_Registry *ar = this->acceptor_registry ();
  // get a reference to the acceptor_registry!

  int ret = ar->open (this, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (ret == -1)
    return -1;

  this->open_called_ = 1;

  return 0;
}

// ****************************************************************

ACE_Allocator*
TAO_ORB_Core::input_cdr_dblock_allocator_i (TAO_ORB_Core_TSS_Resources *tss)
{
  if (tss->input_cdr_dblock_allocator_ == 0)
    tss->input_cdr_dblock_allocator_ =
      this->resource_factory ()->input_cdr_dblock_allocator ();

  return tss->input_cdr_dblock_allocator_;
}

ACE_Allocator*
TAO_ORB_Core::input_cdr_dblock_allocator (void)
{
  if (this->use_tss_resources_)
    {
      TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
      if (tss == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("(%P|%t) %p\n"),
                           ASYS_TEXT ("TAO_ORB_Core::input_cdr_dblock_allocator (); ")
                           ASYS_TEXT ("no more TSS keys")),
                          0);
      return this->input_cdr_dblock_allocator_i (tss);
    }

  if (this->orb_resources_.input_cdr_dblock_allocator_ == 0)
    {
      // Double checked locking
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, 0);
      if (this->orb_resources_.input_cdr_dblock_allocator_ == 0)
        this->orb_resources_.input_cdr_dblock_allocator_ =
          this->resource_factory ()->input_cdr_dblock_allocator ();
    }
  return this->orb_resources_.input_cdr_dblock_allocator_;
}

ACE_Allocator*
TAO_ORB_Core::input_cdr_buffer_allocator_i (TAO_ORB_Core_TSS_Resources *tss)
{
  if (tss->input_cdr_buffer_allocator_ == 0)
    tss->input_cdr_buffer_allocator_ =
      this->resource_factory ()->input_cdr_buffer_allocator ();

  return tss->input_cdr_buffer_allocator_;
}

ACE_Allocator*
TAO_ORB_Core::input_cdr_buffer_allocator (void)
{
  if (this->use_tss_resources_)
    {
      TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
      if (tss == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("(%P|%t) %p\n"),
                           ASYS_TEXT ("TAO_ORB_Core::input_cdr_buffer_allocator (); ")
                           ASYS_TEXT ("no more TSS keys")),
                          0);

      return this->input_cdr_buffer_allocator_i (tss);
    }

  if (this->orb_resources_.input_cdr_buffer_allocator_ == 0)
    {
      // Double checked locking
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, 0);
      if (this->orb_resources_.input_cdr_buffer_allocator_ == 0)
        this->orb_resources_.input_cdr_buffer_allocator_ =
          this->resource_factory ()->input_cdr_buffer_allocator ();
    }
  return this->orb_resources_.input_cdr_buffer_allocator_;
}

ACE_Allocator*
TAO_ORB_Core::output_cdr_dblock_allocator (void)
{
#if 0
  if (this->use_tss_resources_)
#endif /* 0 */
    {
      TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
      if (tss == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("(%P|%t) %p\n"),
                           ASYS_TEXT ("TAO_ORB_Core::output_cdr_dblock_allocator (); ")
                           ASYS_TEXT ("no more TSS keys")),
                          0);

      if (tss->output_cdr_dblock_allocator_ == 0)
        tss->output_cdr_dblock_allocator_ =
          this->resource_factory ()->output_cdr_dblock_allocator ();

      return tss->output_cdr_dblock_allocator_;
    }

#if 0
  if (this->orb_resources_.output_cdr_dblock_allocator_ == 0)
    {
      // Double checked locking
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, 0);
      if (this->orb_resources_.output_cdr_dblock_allocator_ == 0)
        this->orb_resources_.output_cdr_dblock_allocator_ =
          this->resource_factory ()->output_cdr_dblock_allocator ();
    }
  return this->orb_resources_.output_cdr_dblock_allocator_;
#endif /* 0 */
}

ACE_Allocator*
TAO_ORB_Core::output_cdr_buffer_allocator (void)
{
#if 0
  if (this->use_tss_resources_)
#endif /* 0 */
    {
      TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
      if (tss == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("(%P|%t) %p\n"),
                           ASYS_TEXT ("TAO_ORB_Core::input_cdr_buffer_allocator (); ")
                           ASYS_TEXT ("no more TSS keys")),
                          0);

      if (tss->output_cdr_buffer_allocator_ == 0)
        tss->output_cdr_buffer_allocator_ =
          this->resource_factory ()->output_cdr_buffer_allocator ();

      return tss->output_cdr_buffer_allocator_;
    }

#if 0
  if (this->orb_resources_.output_cdr_buffer_allocator_ == 0)
    {
      // Double checked locking
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, 0);
      if (this->orb_resources_.output_cdr_buffer_allocator_ == 0)
        this->orb_resources_.output_cdr_buffer_allocator_ =
          this->resource_factory ()->output_cdr_buffer_allocator ();
    }
  return this->orb_resources_.output_cdr_buffer_allocator_;
#endif /* 0 */
}

ACE_Data_Block*
TAO_ORB_Core::create_input_cdr_data_block (size_t size)
{
  ACE_Data_Block *nb = 0;

  ACE_Allocator *dblock_allocator;
  ACE_Allocator *buffer_allocator;

  if (this->use_tss_resources_)
    {
      TAO_ORB_Core_TSS_Resources *tss = this->get_tss_resources ();
      if (tss == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("(%P|%t) %p\n"),
                           ASYS_TEXT ("TAO_ORB_Core::create_input_cdr_data_block (); ")
                           ASYS_TEXT ("no more TSS keys")),
                          0);

      dblock_allocator =
        this->input_cdr_dblock_allocator_i (tss);
      buffer_allocator =
        this->input_cdr_buffer_allocator_i (tss);
    }
  else
    {
      dblock_allocator =
        this->input_cdr_dblock_allocator ();
      buffer_allocator =
        this->input_cdr_buffer_allocator ();
    }

  ACE_Lock* lock_strategy = 0;
  if (this->resource_factory ()->use_locked_data_blocks ())
    {
      lock_strategy = &this->data_block_lock_;
    }

  ACE_NEW_MALLOC_RETURN (
                         nb,
                         ACE_static_cast(ACE_Data_Block*,
                                         dblock_allocator->malloc (sizeof (ACE_Data_Block))),
                         ACE_Data_Block (size,
                                         ACE_Message_Block::MB_DATA,
                                         0,
                                         buffer_allocator,
                                         lock_strategy,
                                         0,
                                         dblock_allocator),
                         0);

  return nb;
}

ACE_Reactor *
TAO_ORB_Core::reactor (void)
{
  if (this->reactor_registry_ != 0)
    return this->reactor_registry_->reactor ();

  if (this->reactor_ == 0)
    {
      // Double checked locking
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, 0);
      if (this->reactor_ == 0)
        {
          this->reactor_ =
            this->resource_factory ()->get_reactor ();
        }
    }
  return this->reactor_;
}

ACE_Reactor *
TAO_ORB_Core::reactor (TAO_Acceptor *acceptor)
{
  if (this->reactor_registry_ != 0)
    return this->reactor_registry_->reactor (acceptor);

  // @@ ????
  if (this->reactor_ == 0)
    {
      // Double checked locking
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, 0);
      if (this->reactor_ == 0)
        {
          this->reactor_ =
            this->resource_factory ()->get_reactor ();
        }
    }
  return this->reactor_;
}

int
TAO_ORB_Core::get_thread_priority (CORBA::Short &priority)
{
#if (TAO_HAS_RT_CORBA == 0)
  priority = 0;
  return 0;
#else
  ACE_hthread_t current;
  ACE_Thread::self (current);

  int native_priority;
  if (ACE_Thread::getprio (current, native_priority) == -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("TAO (%P|%t) - ORB_Core::get_thread_priority: ")
                  ASYS_TEXT (" ACE_Thread::get_prio\n")));
      return -1;
    }

  TAO_Priority_Mapping *priority_mapping =
    this->priority_mapping ();

  if (priority_mapping->to_CORBA (native_priority, priority) == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("TAO (%P|%t) - ORB_Core::get_thread_priority: ")
                  ASYS_TEXT (" Priority_Mapping::to_CORBA\n")));
      return -1;
    }

  return 0;
#endif /* TAO_HAS_RT_CORBA == 0 */
}

int
TAO_ORB_Core::set_thread_priority (CORBA::Short priority)
{
#if (TAO_HAS_RT_CORBA == 0)
  ACE_UNUSED_ARG (priority);
  return 0;
#else
  TAO_Priority_Mapping *priority_mapping =
    this->priority_mapping ();

  CORBA::Short native_priority;
  if (priority_mapping->to_native (priority, native_priority) == 0)
    return -1;
  ACE_hthread_t current;
  ACE_Thread::self (current);

  if (ACE_Thread::setprio (current, native_priority) == -1)
    return -1;

  return 0;
#endif /* TAO_HAS_RT_CORBA == 0 */
}

CORBA::Object_ptr
TAO_ORB_Core::implrepo_service (void)
{
  if (!this->use_implrepo_)
    return CORBA::Object::_nil ();

  if (CORBA::is_nil (this->implrepo_service_))
    {

      ACE_TRY_NEW_ENV
        {
          CORBA::Object_var temp = this->orb_->resolve_initial_references ("ImplRepoService", ACE_TRY_ENV);
          ACE_TRY_CHECK;

          ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, CORBA::Object::_nil ());

          // @@ Worry about assigning a different IOR? (brunsch)
          this->implrepo_service_ = temp._retn ();
        }
      ACE_CATCHANY
        {
          // Just make sure that we have a null pointer.  Ignore the exception anyway.
          this->implrepo_service_ = CORBA::Object::_nil ();
        }
      ACE_ENDTRY;
    }

  return CORBA::Object::_duplicate (this->implrepo_service_);
}

#if (TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1)

TAO_RelativeRoundtripTimeoutPolicy *
TAO_ORB_Core::stubless_relative_roundtrip_timeout (void)
{
  TAO_RelativeRoundtripTimeoutPolicy *result = 0;

  // No need to lock, the object is in TSS storage....
  TAO_Policy_Current &policy_current =
    this->policy_current ();
  result = policy_current.relative_roundtrip_timeout ();

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->relative_roundtrip_timeout ();
    }

  if (result == 0)
    result = this->default_relative_roundtrip_timeout ();

  return result;
}

#endif /* TAO_HAS_RELATIVE_ROUNDTRIP_TIMEOUT_POLICY == 1 */

// ****************************************************************

TAO_ORB_Core_TSS_Resources::TAO_ORB_Core_TSS_Resources (void)
  :  output_cdr_dblock_allocator_ (0),
     output_cdr_buffer_allocator_ (0),
     output_cdr_msgblock_allocator_ (0),
     input_cdr_dblock_allocator_ (0),
     input_cdr_buffer_allocator_ (0),
     connection_cache_ (0),
     event_loop_thread_ (0),
     client_leader_thread_ (0),
     leader_follower_condition_variable_ (0),
     reactor_registry_ (0),
     reactor_registry_cookie_ (0)
{
}

TAO_ORB_Core_TSS_Resources::~TAO_ORB_Core_TSS_Resources (void)
{
  if (this->output_cdr_dblock_allocator_ != 0)
    this->output_cdr_dblock_allocator_->remove ();
  delete this->output_cdr_dblock_allocator_;

  if (this->output_cdr_buffer_allocator_ != 0)
    this->output_cdr_buffer_allocator_->remove ();
  delete this->output_cdr_buffer_allocator_;

  if (this->output_cdr_msgblock_allocator_ != 0)
    this->output_cdr_msgblock_allocator_->remove ();
  delete this->output_cdr_msgblock_allocator_;

  if (this->input_cdr_dblock_allocator_ != 0)
    this->input_cdr_dblock_allocator_->remove ();
  delete this->input_cdr_dblock_allocator_;

  if (this->input_cdr_buffer_allocator_ != 0)
    this->input_cdr_buffer_allocator_->remove ();
  delete this->input_cdr_buffer_allocator_;

  // UNIMPLEMENTED delete this->connection_cache_;
  this->connection_cache_ = 0;

  delete this->leader_follower_condition_variable_;
  this->leader_follower_condition_variable_ = 0;

  if (this->reactor_registry_ != 0)
    this->reactor_registry_->destroy_tss_cookie (this->reactor_registry_cookie_);
}

// ****************************************************************

TAO_TSS_Resources::TAO_TSS_Resources (void)
  :  poa_current_impl_ (0),
     default_environment_ (&this->tss_environment_)

#if (TAO_HAS_CORBA_MESSAGING == 1)

  , policy_current_ (&this->initial_policy_current_)

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

{
}

TAO_TSS_Resources::~TAO_TSS_Resources (void)
{
}

// ****************************************************************

TAO_ORB_Table::TAO_ORB_Table (void)
  : first_orb_ (0)
{
}

TAO_ORB_Table::~TAO_ORB_Table (void)
{
  for (Iterator i = this->begin ();
       i != this->end ();
       i = this->begin ())
    {
      // Destroy the ORB_Core
      (*i).int_id_->fini ();
    }
  this->table_.close ();

  // free up all the ORB owned Exceptions
  TAO_Exceptions::fini ();

  // free up all the ORB owned TypeCodes
  TAO_TypeCodes::fini ();
}

TAO_ORB_Table::Iterator
TAO_ORB_Table::begin (void)
{
  return this->table_.begin ();
}

TAO_ORB_Table::Iterator
TAO_ORB_Table::end (void)
{
  return this->table_.end ();
}

int
TAO_ORB_Table::bind (const char *orb_id,
                     TAO_ORB_Core *orb_core)
{
  if (this->first_orb_ == 0)
    {
      this->first_orb_ = orb_core;
    }
  ACE_CString id (orb_id);
  return this->table_.bind (id, orb_core);
}

TAO_ORB_Core*
TAO_ORB_Table::find (const char *orb_id)
{
  TAO_ORB_Core *found = 0;
  ACE_CString id (orb_id);
  this->table_.find (id, found);
  return found;
}

int
TAO_ORB_Table::unbind (const char *orb_id)
{
  ACE_CString id (orb_id);
  TAO_ORB_Core *orb_core;
  int result = this->table_.unbind (id, orb_core);
  if (result == 0)
    {
      if (orb_core == this->first_orb_)
        {
          Iterator begin = this->begin ();
          Iterator end = this->end ();
          if (begin != end)
            this->first_orb_ = (*begin).int_id_;
          else
            this->first_orb_ = 0;
        }
    }
  return result;
}

// ****************************************************************

TAO_Export TAO_ORB_Core *
TAO_ORB_Core_instance (void)
{
  // @@ This is a slight violation of layering, we should use
  //    TAO_ORB_Core_instance(), but that breaks during startup.
  TAO_ORB_Table *orb_table = TAO_ORB_Table::instance ();
  if (orb_table->first_orb () == 0)
    {
      ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard,
                                *ACE_Static_Object_Lock::instance (), 0));

      if (orb_table->first_orb () == 0)
        {
          int argc = 0;
          ACE_DECLARE_NEW_CORBA_ENV;
          ACE_TRY
            {
              (void) CORBA::ORB_init (argc, 0, 0, ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          ACE_CATCHANY
            {
              // @@ What should we do here?
            }
          ACE_ENDTRY;
        }
    }

  return orb_table->first_orb ();
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Reverse_Lock<ACE_SYNCH_MUTEX>;
template class ACE_Guard<ACE_Reverse_Lock<ACE_SYNCH_MUTEX> >;

template class ACE_Env_Value<int>;
template class ACE_Env_Value<u_int>;

template class ACE_TSS_Singleton<TAO_TSS_Resources, ACE_SYNCH_MUTEX>;
template class ACE_TSS<TAO_TSS_Resources>;
template class ACE_TSS<TAO_ORB_Core_TSS_Resources>;

template class ACE_Singleton<TAO_ORB_Table,ACE_SYNCH_MUTEX>;
template class ACE_Map_Entry<ACE_CString,TAO_ORB_Core*>;
template class ACE_Map_Manager<ACE_CString,TAO_ORB_Core*,ACE_Null_Mutex>;
template class ACE_Map_Iterator_Base<ACE_CString,TAO_ORB_Core*,ACE_Null_Mutex>;
template class ACE_Map_Iterator<ACE_CString,TAO_ORB_Core*,ACE_Null_Mutex>;
template class ACE_Map_Reverse_Iterator<ACE_CString,TAO_ORB_Core*,ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Reverse_Lock<ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Guard<ACE_Reverse_Lock<ACE_SYNCH_MUTEX> >

#pragma instantiate ACE_Env_Value<int>
#pragma instantiate ACE_Env_Value<u_int>

#pragma instantiate ACE_TSS_Singleton<TAO_TSS_Resources, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_TSS<TAO_TSS_Resources>
#pragma instantiate ACE_TSS<TAO_ORB_Core_TSS_Resources>

#pragma instantiate ACE_Singleton<TAO_ORB_Table,ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Map_Entry<ACE_CString,TAO_ORB_Core*>
#pragma instantiate ACE_Map_Manager<ACE_CString,TAO_ORB_Core*,ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator_Base<ACE_CString,TAO_ORB_Core*,ACE_Null_Mutex>
#pragma instantiate ACE_Map_Iterator<ACE_CString,TAO_ORB_Core*,ACE_Null_Mutex>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_CString,TAO_ORB_Core*,ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
