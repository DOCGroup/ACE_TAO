// $Id$

#include "ORB_Core.h"
#include "ORB_Table.h"


#include "TAO_Internal.h"
#include "default_client.h"
#include "default_server.h"
#include "default_resource.h"
#include "debug.h"
#include "MProfile.h"
#include "Stub.h"
#include "Reactor_Registry.h"
#include "Leader_Follower.h"

#include "Connector_Registry.h"
#include "Acceptor_Registry.h"

#include "Sync_Strategies.h"

#include "Object_Loader.h"

#include "ObjectIDList.h"

#include "ace/Object_Manager.h"
#include "ace/Env_Value_T.h"
#include "ace/Dynamic_Service.h"
#include "ace/Arg_Shifter.h"
#include "Services_Activate.h"
#include "Invocation.h"

#include "Invocation_Endpoint_Selectors.h"
#include "RT_Invocation_Endpoint_Selectors.h"

#include "IORInfo.h"

#if defined(ACE_MVS)
#include "ace/Codeset_IBM1047.h"
#endif /* ACE_MVS */

#if !defined (__ACE_INLINE__)
# include "ORB_Core.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, ORB_Core, "$Id$")


// ****************************************************************

CORBA::Environment &
TAO_default_environment ()
{
  return *TAO_TSS_RESOURCES::instance ()->default_environment_;
}

// ****************************************************************

TAO_ORB_Core::Timeout_Hook TAO_ORB_Core::timeout_hook_ = 0;
TAO_ORB_Core::Sync_Scope_Hook TAO_ORB_Core::sync_scope_hook_ = 0;
CORBA::Object_ptr TAO_ORB_Core::priority_mapping_manager_ = 0;
const char * TAO_ORB_Core::resource_factory_name_ = "Resource_Factory";
const char *TAO_ORB_Core::protocols_hooks_name_ = "Protocols_Hooks";
const char * TAO_ORB_Core::dynamic_adapter_name_ = "Dynamic_Adapter";

TAO_ORB_Core::TAO_ORB_Core (const char *orbid)
  : protocols_hooks_ (0),
    lock_ (),
    connector_registry_ (0),
    acceptor_registry_ (0),
    protocol_factories_ (0),
    implrepo_service_ (CORBA::Object::_nil ()),
    use_implrepo_ (0),
    typecode_factory_ (CORBA::Object::_nil ()),
    dynany_factory_ (CORBA::Object::_nil ()),
    ior_manip_factory_ (CORBA::Object::_nil ()),
    ior_table_ (CORBA::Object::_nil ()),
    orb_ (),
    orb_params_ (),
    init_ref_map_ (),
    object_ref_table_ (),
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
    adapter_registry_ (this),
    poa_adapter_ (0),
    tm_ (),
    from_iso8859_ (0),
    to_iso8859_ (0),
    from_unicode_ (0),
    to_unicode_ (0),
    tss_cleanup_funcs_ (),
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
    endpoint_selector_factory_ (0),
    default_endpoint_selector_ (0),
#if (TAO_HAS_RT_CORBA == 1)
    priority_endpoint_selector_ (0),
    bands_endpoint_selector_ (0),
    protocol_endpoint_selector_ (0),
    priority_protocol_selector_ (0),
    bands_protocol_selector_ (0),
    client_priority_policy_selector_ (0),
    rt_orb_ (0),
    rt_current_ (0),
    //    priority_mapping_manager_ (0),
#endif /* TAO_HAS_RT_CORBA == 1 */
#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)
    eager_buffering_sync_strategy_ (0),
    delayed_buffering_sync_strategy_ (0),
#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */
    transport_sync_strategy_ (0),
    refcount_ (1),
    policy_factory_registry_ (),
#if (TAO_HAS_INTERCEPTORS == 1)
    client_request_interceptors_ (),
    server_request_interceptors_ (),
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
    ior_interceptors_ (),
    parser_registry_ (),
    connection_cache_ (),
    bidir_giop_policy_ (0)
{
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

  ACE_NEW (this->endpoint_selector_factory_,
           TAO_Endpoint_Selector_Factory);

  ACE_NEW (this->priority_endpoint_selector_,
           TAO_Priority_Endpoint_Selector);

  ACE_NEW (this->bands_endpoint_selector_,
           TAO_Bands_Endpoint_Selector);

  ACE_NEW (this->protocol_endpoint_selector_,
           TAO_Protocol_Endpoint_Selector);

  ACE_NEW (this->priority_protocol_selector_,
           TAO_Priority_Protocol_Selector);

  ACE_NEW (this->bands_protocol_selector_,
           TAO_Bands_Protocol_Selector);

  ACE_NEW (this->client_priority_policy_selector_,
           TAO_Client_Priority_Policy_Selector);

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  ACE_NEW (this->default_endpoint_selector_,
           TAO_Default_Endpoint_Selector);

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

  delete this->endpoint_selector_factory_;
  delete this->default_endpoint_selector_;

#if (TAO_HAS_RT_CORBA == 1)

  delete this->priority_endpoint_selector_;
  delete this->bands_endpoint_selector_;
  delete this->protocol_endpoint_selector_;
  delete this->priority_protocol_selector_;
  delete this->bands_protocol_selector_;
  delete this->client_priority_policy_selector_;
  // delete this->rt_orb_;
  // delete this->rt_current_;
  //  delete this->priority_mapping_manager_;

#endif /* TAO_HAS_RT_CORBA == 1 */

  delete this->transport_sync_strategy_;
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
  // as the RootPOA.

  // @@ GIOPLite should be an alternative ORB Messaging protocols, fredk
  // int giop_lite = 0;

  CORBA::Boolean use_ior = 1;
  int cdr_tradeoff = ACE_DEFAULT_CDR_MEMCPY_TRADEOFF;

  // The following things should be changed to use the ACE_Env_Value<>
  // template sometime.

  // Name Service port use for Multicast
  u_short ns_port = 0;

  // Trading Service port used for Multicast
  u_short ts_port = 0;

  // Implementation Repository Service port #.
  u_short ir_port = 0;

  // Buffer sizes for kernel socket buffers
  // @@ should be a default defined for each protocol implementation?
  //    since we may have protocols loaded which use shared memory of
  //    some form, fredk
  size_t rcv_sock_size = 0;
  size_t snd_sock_size = 0;

  // Use TCP_NODELAY.
  size_t nodelay = 1;

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
                    ACE_TEXT ("TAO_debug_level == %d"), TAO_debug_level));
      }
  }
#endif  /* TAO_DEBUG */

  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      char *current_arg = 0;

      ////////////////////////////////////////////////////////////////
      // begin with the 'parameterless' flags                       //
      ////////////////////////////////////////////////////////////////
      if (arg_shifter.cur_arg_strncasecmp ("-ORBGIOPlite") == 0)
        {
          // @@ This will have to change since gioplite
          // will be considered as an alternate ORB
          // messaging protocols.
          // giop_lite = 1;
          ACE_DEBUG ((LM_WARNING,
                      ACE_TEXT ("(%P|%t) This option has been deprecated \n")
                      ACE_TEXT ("Please use svc.conf file to load the protcol \n")));

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
          //   corbaloc:space:2001,1.2@odyssey:2010;uiop://foo,bar
          //
          // All preconnect or endpoint strings should be of the above form(s).

          ACE_CString endpts (current_arg);

          if (this->orb_params ()->endpoints (endpts) != 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%P|%t)\n")
                          ACE_TEXT ("Invalid endpoint(s) specified:\n%s\n"),
                          endpts.c_str ()));
              ACE_THROW_RETURN (CORBA::BAD_PARAM (
                                  CORBA::SystemException::_tao_minor_code (
                                    TAO_ORB_CORE_INIT_LOCATION_CODE,
                                    EINVAL),
                                  CORBA::COMPLETED_NO),
                                -1);
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

          this->orb_params ()->mcast_discovery_endpoint (current_arg);

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
                ("-ORBTradingServicePort")))
        {
          // Specify the port number for the NameService.

          ts_port = (CORBA::UShort) ACE_OS::atoi (current_arg);

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
                          ACE_TEXT ("WARNING: using '-ORBCollocation YES' is obsolete ")
                          ACE_TEXT ("and implies '-ORBCollocation global'")
                          ACE_TEXT ("  Please use '-ORBCollocation global' instead.\n")));
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
                      ACE_TEXT ("WARNING: -ORBGlobalCollocation option is obsolete.")
                      ACE_TEXT ("  Please use '-ORBCollocation global/per-orb/no'")
                      ACE_TEXT ("instead.\n")));

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
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%P|%t)\n")
                          ACE_TEXT ("Invalid preconnect(s)")
                          ACE_TEXT ("specified:\n%s\n"),
                          preconnections.c_str ()));
              ACE_THROW_RETURN (CORBA::BAD_PARAM (
                                  CORBA::SystemException::_tao_minor_code (
                                    TAO_ORB_CORE_INIT_LOCATION_CODE,
                                    EINVAL),
                                  CORBA::COMPLETED_NO),
                                -1);
            }

          // validate_connection() supports the same functionality as
          // the -ORBPreconnect option, and more.  Multiple
          // preconnections are also provided by validate_connection()
          // via "banded connections."
          ACE_ERROR ((LM_WARNING,
                      ACE_TEXT ("(%P|%t) -ORBPreconnect is ")
                      ACE_TEXT ("deprecated.\n")
                      ACE_TEXT ("(%P|%t) Use validate_connection()")
                      ACE_TEXT ("at run-time, instead.\n")));

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
          char * pos = ACE_OS::strchr (current_arg, '=');
          if (pos == 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("Invalid ORBInitRef argument '%s'")
                          ACE_TEXT ("format is ObjectID=IOR\n"),
                          current_arg));
              ACE_THROW_RETURN (CORBA::INTERNAL (
                                  CORBA::SystemException::_tao_minor_code (
                                    TAO_ORB_CORE_INIT_LOCATION_CODE,
                                    0),
                                  CORBA::COMPLETED_NO),
                                -1);
            }
          ACE_CString object_id (current_arg,
                                 pos - current_arg);
          ACE_CString IOR (pos + 1);
          if (this->init_ref_map_.bind (object_id, IOR) != 0)
            {              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("Cannot store ORBInitRef ")
                          ACE_TEXT ("argument '%s'\n"),
                          current_arg));
              ACE_THROW_RETURN (CORBA::INTERNAL (
                                  CORBA::SystemException::_tao_minor_code (
                                    TAO_ORB_CORE_INIT_LOCATION_CODE,
                                    0),
                                  CORBA::COMPLETED_NO),
                                -1);
            }
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBDefaultInitRef")))
        {
          // Set the list of prefixes from -ORBDefaultInitRef.
          this->orb_params ()->default_init_ref (current_arg);

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

          ACE_TCHAR* file_name = current_arg;
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
                            CORBA::NO_MEMORY (
                              CORBA::SystemException::_tao_minor_code (
                                TAO_ORB_CORE_INIT_LOCATION_CODE,
                                ENOMEM),
                              CORBA::COMPLETED_NO));
          ACE_CHECK_RETURN (-1);

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
          // @@ This option introduces a security hole.  Do not
          //    reinstate it.

          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("-ORBSetUID is deprecated.\n")));

          ACE_THROW_RETURN (CORBA::BAD_PARAM (
                              CORBA_SystemException::_tao_minor_code (
                                TAO_ORB_CORE_INIT_LOCATION_CODE,
                                EINVAL),
                              CORBA::COMPLETED_NO),
                            -1);
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                ("-ORBSetGID")))
        {
          // @@ This option introduces a security hole.  Do not
          //    reinstate it.

          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("-ORBSetGID is deprecated.\n")));

          ACE_THROW_RETURN (CORBA::BAD_PARAM (
                              CORBA_SystemException::_tao_minor_code (
                                TAO_ORB_CORE_INIT_LOCATION_CODE,
                                EINVAL),
                              CORBA::COMPLETED_NO),
                            -1);
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
                          ACE_TEXT ("ERROR: Unknown \"-ORB\" option ")
                          ACE_TEXT ("<%s>.\n"),
                          ((current_arg == 0) ? "<NULL>" : current_arg)));
            }

          ACE_THROW_RETURN (CORBA::BAD_PARAM (
                              CORBA::SystemException::_tao_minor_code (
                                TAO_ORB_CORE_INIT_LOCATION_CODE,
                                EINVAL),
                              CORBA::COMPLETED_NO),
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

  // Calling the open method here so that the svc.conf file is
  // opened and TAO_default_resource_factory::init () is called by the
  // time this method is called.
  this->parser_registry_.open (this);

  // Initialize the pointers to resources in the ORB Core instance,
  // e.g., reactor, connector, etc.  Must do this after we open
  // services because we'll load the factory from there.
  TAO_Resource_Factory *trf = this->resource_factory ();

  if (trf == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("ORB Core unable to find a Resource Factory instance")));
      ACE_THROW_RETURN (CORBA::INTERNAL (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_ORB_CORE_INIT_LOCATION_CODE,
                            0),
                          CORBA::COMPLETED_NO),
                        -1);
    }

  if (use_tss_resources == -1)
    this->use_tss_resources_ = trf->use_tss_resources ();
  else
    this->use_tss_resources_ = use_tss_resources;

  this->reactor_registry_ =
    trf->get_reactor_registry ();
  this->reactor_registry_->open (this);

#if (TAO_HAS_RT_CORBA == 1)
  this->get_protocols_hooks()->set_priority_mapping (this,
                                                     trf,
                                                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

#endif /* TAO_HAS_RT_CORBA == 1 */

  // @@ ????
  // Make sure the reactor is initialized...
  ACE_Reactor *reactor = this->reactor ();
  if (reactor == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("ORB Core unable to initialize reactor")));
      ACE_THROW_RETURN (CORBA::INITIALIZE (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_ORB_CORE_INIT_LOCATION_CODE,
                            0),
                          CORBA::COMPLETED_NO),
                        -1);
    }

  TAO_Server_Strategy_Factory *ssf = this->server_factory ();

  if (ssf == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("ORB Core unable to find a ")
                  ACE_TEXT ("Server Strategy Factory instance")));
      ACE_THROW_RETURN (CORBA::INTERNAL (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_ORB_CORE_INIT_LOCATION_CODE,
                            0),
                          CORBA::COMPLETED_NO),
                        -1);
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
                    CORBA::ORB (this),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_ORB_CORE_INIT_LOCATION_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
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

  this->orb_params ()->service_port (NAMESERVICE, ns_port);
  this->orb_params ()->service_port (TRADINGSERVICE, ts_port);
  this->orb_params ()->service_port (IMPLREPOSERVICE, ir_port);

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
    ACE_THROW_RETURN (CORBA::INITIALIZE (
                        CORBA::SystemException::_tao_minor_code (
                          TAO_ORB_CORE_INIT_LOCATION_CODE,
                          0),
                        CORBA::COMPLETED_NO),
                      -1);

  // init the ORB core's pointer
  this->protocol_factories_ = trf->get_protocol_factories ();

  // Now that we have a complete list of available protocols and their
  // related factory objects, set default policies and initialize the
  // registries!

  // Set ORB-level policy defaults.
  if (this->get_protocols_hooks ()->set_default_policies (this) != 0)
    ACE_THROW_RETURN (CORBA::INITIALIZE (
                        CORBA::SystemException::_tao_minor_code (
                          TAO_ORB_CORE_INIT_LOCATION_CODE,
                          0),
                        CORBA::COMPLETED_NO),
                      -1);

  // Initialize the connector registry and create a connector for each
  // configured protocol.
  if (this->connector_registry ()->open (this) != 0)
    ACE_THROW_RETURN (CORBA::INITIALIZE (
                        CORBA::SystemException::_tao_minor_code (
                          TAO_ORB_CORE_INIT_LOCATION_CODE,
                          0),
                        CORBA::COMPLETED_NO),
                      -1);

  // Have the connector registry parse the preconnects.
  if (this->orb_params ()->preconnects ().is_empty () == 0)
    this->connector_registry ()->preconnect (
            this,
            this->orb_params ()->preconnects ());

  // Open the Connection Cache
  // @@ This seems to be a nice place to configure the connection
  // cache for the number of allowed entries
  this->connection_cache_.open (this);

  // As a last step perform initializations of the service callbacks
  this->services_callbacks_init ();

  // The ORB has been initialized, meaning that the ORB is no longer
  // in the shutdown state.
  this->has_shutdown_ = 0;

  return 0;
}

int
TAO_ORB_Core::fini (void)
{
  // Wait for any server threads, ignoring any failures.
  (void) this->thr_mgr ()->wait ();

  CORBA::release (this->implrepo_service_);

  CORBA::release (this->typecode_factory_);

  CORBA::release (this->dynany_factory_);

  CORBA::release (this->ior_manip_factory_);

  CORBA::release (this->ior_table_);

  if (TAO_debug_level >= 3)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Destroying ORB <%s>\n"),
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

  // Set of file descriptors corresponding to open connections.  This
  // handle set is used to explicitly deregister the connection event
  // handlers from the Reactor.  This is particularly important for
  // dynamically loaded ORBs where an application level reactor, such
  // as the Singleton reactor, is used instead of an ORB created one.

  ACE_Handle_Set handle_set;

  // Close the connection cache and return the handle set that needs
  // to be de-registered from the reactor.
  this->connection_cache_.close (handle_set);

  // Shutdown all open connections that are registered with the ORB
  // Core.  Note that the ACE_Event_Handler::DONT_CALL mask is NOT
  // used here since the reactor should invoke each handle's
  // corresponding ACE_Event_Handler::handle_close() method to ensure
  // that the connection is shutdown gracefully prior to destroying
  // the ORB Core.
  if (handle_set.num_set () > 0)
    (void) this->reactor ()->remove_handler (handle_set,
                                             ACE_Event_Handler::ALL_EVENTS_MASK);

  // Pass reactor back to the resource factory.
  if (this->resource_factory_ != 0)
    this->resource_factory_->reclaim_reactor (this->reactor_);

  (void) TAO_Internal::close_services ();


  delete this->reactor_registry_;


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

  delete this;

  return 0;
}

void
TAO_ORB_Core::set_resource_factory (const char *resource_factory_name)
{
  TAO_ORB_Core::resource_factory_name_ = resource_factory_name;
}

void
TAO_ORB_Core::dynamic_adapter_name (const char *name)
{
  TAO_ORB_Core::dynamic_adapter_name_ = name;
}

const char *
TAO_ORB_Core::dynamic_adapter_name (void)
{
  return TAO_ORB_Core::dynamic_adapter_name_;
}

TAO_Resource_Factory *
TAO_ORB_Core::resource_factory (void)
{
  if (this->resource_factory_ == 0)
    {
      // Look in the service repository for an instance.
      this->resource_factory_ =
        ACE_Dynamic_Service<TAO_Resource_Factory>::instance
        (TAO_ORB_Core::resource_factory_name_);
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
                    ACE_TEXT ("(%P|%t) WARNING - No Resource Factory found ")
                    ACE_TEXT ("in Service Repository.\n")
                    ACE_TEXT ("  Using default instance with GLOBAL resource ")
                    ACE_TEXT ("source specifier.\n")));

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

void
TAO_ORB_Core::set_protocols_hooks (const char *protocols_hooks)
{
  TAO_ORB_Core::protocols_hooks_name_ = protocols_hooks;
}

TAO_Protocols_Hooks *
TAO_ORB_Core::get_protocols_hooks (void)
{
  if (TAO_ORB_Core::protocols_hooks_ == 0)
    {
      // Look in the service repository for an instance.
      this->protocols_hooks_ =
        ACE_Dynamic_Service<TAO_Protocols_Hooks>::instance
        (TAO_ORB_Core::protocols_hooks_name_);
    }

  if (TAO_ORB_Core::protocols_hooks_name_ == 0)
    {
      // Still don't have one, so let's allocate the default.  This
      // will throw an exception if it fails on exception-throwing
      // platforms.
      TAO_Protocols_Hooks *protocols_hooks;
      ACE_NEW_RETURN (protocols_hooks,
                      TAO_Protocols_Hooks,
                      0);

      this->protocols_hooks_ = protocols_hooks;
    }

  return this->protocols_hooks_;
}

void
TAO_ORB_Core::services_callbacks_init (void)
{
  // We (should) know what are the services that would need
  // callbacks. So, what we do is go through the Service Configurator
  // list for looking at the services that we want to load.
  this->ft_service_.init (this);

  // @@ Other service callbacks can be added here
}

int
TAO_ORB_Core::service_raise_comm_failure (TAO_GIOP_Invocation *invoke,
                                          TAO_Profile *profile,
                                          CORBA::Environment &ACE_TRY_ENV)
{
  if (this->ft_service_.service_callback ())
    {
      return this->ft_service_.service_callback ()->
                 raise_comm_failure (invoke,
                                     profile,
                                     ACE_TRY_ENV);
    }

  invoke->close_connection ();

  ACE_THROW_RETURN (CORBA::COMM_FAILURE (
      CORBA_SystemException::_tao_minor_code (
          TAO_INVOCATION_RECV_REQUEST_MINOR_CODE,
          errno),
      CORBA::COMPLETED_MAYBE),
      TAO_INVOKE_EXCEPTION);
}

int
TAO_ORB_Core::service_raise_transient_failure (TAO_GIOP_Invocation *invoke,
                                               TAO_Profile *profile,
                                               CORBA::Environment &ACE_TRY_ENV)
{
  if (this->ft_service_.service_callback ())
    {
      return this->ft_service_.service_callback ()->
                 raise_transient_failure (invoke,
                                          profile,
                                          ACE_TRY_ENV);
    }

  ACE_THROW_RETURN (CORBA::TRANSIENT (
        CORBA_SystemException::_tao_minor_code (
          TAO_INVOCATION_SEND_REQUEST_MINOR_CODE,
          errno),
        CORBA::COMPLETED_MAYBE),
        TAO_INVOKE_EXCEPTION);
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
                    ACE_TEXT ("(%P|%t) WARNING - No Client Strategy Factory found ")
                    ACE_TEXT ("in Service Repository.\n")
                    ACE_TEXT ("  Using default instance.\n")));

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
                    ACE_TEXT ("(%P|%t) WARNING - No %s found in Service Repository.")
                    ACE_TEXT ("  Using default instance.\n"),
                    ACE_TEXT ("Server Strategy Factory")));

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

CORBA::Object_ptr
TAO_ORB_Core::root_poa (CORBA::Environment &ACE_TRY_ENV)
{
  if (!CORBA::is_nil (this->root_poa_.in ()))
    return CORBA::Object::_duplicate (this->root_poa_.in ());

  TAO_Adapter_Factory *factory =
    ACE_Dynamic_Service<TAO_Adapter_Factory>::instance ("TAO_POA");
  if (factory == 0)
    {
      // Try again, using the default directive...
      ACE_Service_Config::process_directive (
                                             "dynamic TAO_POA Service_Object * TAO_PortableServer:_make_TAO_Object_Adapter_Factory()"
                                             );
      factory =
        ACE_Dynamic_Service<TAO_Adapter_Factory>::instance ("TAO_POA");
    }

  if (factory == 0)
    {
      // It really failed, raise an exception!
      ACE_THROW_RETURN (CORBA::ORB::InvalidName (),
                        CORBA::Object::_nil ());
    }

  // @@ Not exception safe
  TAO_Adapter *poa_adapter = factory->create (this);
  poa_adapter->open (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  this->root_poa_ = poa_adapter->root ();

  this->adapter_registry_.insert (poa_adapter, ACE_TRY_ENV);

  return CORBA::Object::_duplicate (this->root_poa_.in ());
}

TAO_Adapter *
TAO_ORB_Core::poa_adapter (void)
{
  if (this->poa_adapter_ == 0)
    {
      ACE_GUARD_RETURN (ACE_SYNCH_MUTEX, ace_mon, this->lock_, 0);
      if (this->poa_adapter_ == 0)
        {
          this->poa_adapter_ =
            this->adapter_registry_.find_adapter ("RootPOA");
        }
    }
  return this->poa_adapter_;
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
                                  CORBA::PolicyList *policy_list,
                                  TAO_Acceptor_Filter *filter,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  (void) this->open (ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  TAO_Stub *stub = 0;

  // Create a profile container and have Acceptor_Registry populate it
  // with profiles as appropriate.
  TAO_MProfile mp (0);
  if (this->acceptor_registry ()->make_mprofile (key, mp, filter) == -1)
    {
      ACE_THROW_RETURN (CORBA::INTERNAL (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_MPROFILE_CREATION_ERROR,
                            0),
                          CORBA::COMPLETED_NO),
                        0);
    }

  // Make sure we have at least one profile.  <mp> may end up being
  // empty if none of the acceptor endpoints have the right priority
  // for this object, for example.
  if (mp.profile_count () == 0)
    {
      ACE_THROW_RETURN (CORBA::BAD_PARAM (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_MPROFILE_CREATION_ERROR,
                            0 ),
                          CORBA::COMPLETED_NO),
                        0);
    }

  //  Add the Polices contained in "policy_list" to each profile
  //  so that those policies will be exposed to the client in the IOR.
  //  In particular each CORBA::Policy has to be converted in to
  //  Messaging::PolicyValue, and then all the Messaging::PolicyValue
  //  should be embedded inside a Messaging::PolicyValueSeq which became
  //  in turns the "body" of the IOP::TaggedComponent. This conversion
  //  is a responsability of the CORBA::Profile class.
  //  (See orbos\98-05-05.pdf Section 5.4)

  if (policy_list->length () != 0)
    {
      TAO_Profile * profile;

      for (CORBA::ULong i = 0; i < mp.profile_count (); ++i)
        {
          // Get the ith profile
          profile = mp.get_profile (i);
          profile->the_stub (stub);
          profile->policies (policy_list);
        }
    }

  // Iterate over the registered IOR interceptors so that they may be
  // given the opportunity to add tagged components to the profiles
  // for this servant.
  this->establish_components (mp, policy_list, ACE_TRY_ENV);
  ACE_CHECK_RETURN (0);

  // Done creating profiles.  Initialize a TAO_Stub object with them.
  ACE_NEW_THROW_EX (stub,
                    TAO_Stub (type_id, mp, this),
                    CORBA::NO_MEMORY (TAO_DEFAULT_MINOR_CODE,
                                      CORBA::COMPLETED_MAYBE));
  ACE_CHECK_RETURN (stub);

  stub->base_profiles ().policy_list (policy_list);

  return stub;
}

void
TAO_ORB_Core::establish_components (TAO_MProfile &mp,
                                    CORBA::PolicyList *policy_list,
                                    CORBA::Environment &ACE_TRY_ENV)
{
  // Iterate over the registered IOR interceptors so that they may be
  // given the opportunity to add tagged components to the profiles
  // for this servant.
  TAO_IORInterceptor_List::TYPE &interceptors =
    this->ior_interceptors ();

  size_t interceptor_count = interceptors.size ();
  if (interceptor_count == 0)
    return;

  PortableInterceptor::IORInfo_ptr info_temp;
  ACE_NEW_THROW_EX (info_temp,
                    TAO_IORInfo (this, mp, policy_list),
                    CORBA::NO_MEMORY (
                      CORBA_SystemException::_tao_minor_code (
                        TAO_MPROFILE_CREATION_ERROR,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::IORInfo_var info = info_temp;

  for (size_t i = 0; i < interceptor_count; ++i)
    {
      ACE_TRY
        {
          interceptors[i]->establish_components (
            info.in ()
            TAO_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCHANY
        {
          // According to the Portable Interceptors specification,
          // IORInterceptor::establish_components() must not throw an
          // exception.  If it does, then the ORB is supposed to
          // ignore it and continue processing the remaining
          // IORInterceptors.
          if (TAO_debug_level > 1)
            {
              CORBA::String_var name = interceptors[i]->name (
                TAO_ENV_SINGLE_ARG_PARAMETER);
              // @@ What do we do if we get an exception here?

              if (name.in () != 0)
                {
                  ACE_DEBUG ((LM_WARNING,
                              "(%P|%t) Exception thrown while processing "
                              "IORInterceptor \"%s\">\n",
                              name.in ()));
                }

              ACE_PRINT_TAO_EXCEPTION (ACE_ANY_EXCEPTION,
                                       "Ignoring exception in "
                                       "IORInterceptor::establish_components");
            }
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }
}

CORBA::Object_ptr
TAO_ORB_Core::create_object (TAO_Stub *stub)
{
  // @@ What about forwarding.  With this approach we are never forwarded
  //    when we use collocation!
  const TAO_MProfile &mprofile = stub->base_profiles ();

  {
    // @@ Ossama: maybe we need another lock for the table, to
    //    reduce contention on the Static_Object_Lock below, if so
    //    then we need to use that lock in the ORB_init() function.
    ACE_MT (ACE_GUARD_RETURN (ACE_SYNCH_RECURSIVE_MUTEX, guard,
                              *ACE_Static_Object_Lock::instance (),
                              0));

    TAO_ORB_Table *table = TAO_ORB_Table::instance ();
    TAO_ORB_Table::Iterator end = table->end ();
    for (TAO_ORB_Table::Iterator i = table->begin ();
         i != end;
         ++i)
      {
        TAO_ORB_Core *other_core = (*i).int_id_;
        CORBA::Object_ptr x =
          this->create_collocated_object (stub,
                                          other_core,
                                          mprofile);
        if (x != 0)
          return x;
      }
  }

  // @@ We should thow CORBA::NO_MEMORY in platforms with exceptions,
  // but we are stuck in platforms without exceptions!
  CORBA::Object_ptr x;

  // The constructor sets the proxy broker as the
  // Remote one.
  ACE_NEW_RETURN (x,
                  CORBA_Object (stub, 0),
                  0);
  return x;
}

CORBA::Object_ptr
TAO_ORB_Core::create_collocated_object (TAO_Stub *stub,
                                        TAO_ORB_Core *orb_core,
                                        const TAO_MProfile &mprofile)
{
  if (!orb_core->optimize_collocation_objects ())
    return 0;

  if (!orb_core->use_global_collocation () && orb_core != this)
    return 0;

  if (!orb_core->is_collocated (mprofile))
    return 0;

  // OK, the target ORB and the mprofile match, use the Adapter
  // Registry of each ORB to find the right one.

  return orb_core->adapter_registry ()->create_collocated_object (stub,
                                                                  mprofile);
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
                ACE_TEXT ("TAO (%P|%t) - start of run/perform_work\n")));

  // This method should only be called by servers, so now we set up
  // for listening!

  int ret = this->open (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (ret == -1)
    return -1;

  // Fetch the Reactor
  ACE_Reactor *r = this->reactor ();

  int result = 1;
  // 1 to detect that nothing went wrong

  // Loop handling client requests until the ORB is shutdown.

  // We could use the leader-follower lock to check for the state
  // if this variable or use the lock <create_event_loop_lock> in
  // the server strategy factory.
  // We don't need to do this because we use the Reactor
  // mechanisms to shutdown in a thread-safe way.

  while (this->has_shutdown () == 0)
    {
      // Every time we perform an interation we have to become the
      // leader again, because it is possible that a client has
      // acquired the leader role...
      TAO_Leader_Follower &leader_follower =
        this->leader_follower ();
      TAO_LF_Event_Loop_Thread_Helper helper (leader_follower);

      result = helper.set_event_loop_thread (tv);

      if (result != 0)
        {
          if (errno == ETIME)
            return 0;
          else
            return result;
        }

      // Set the owning thread of the Reactor to the one which we're
      // currently in.  This is necessary b/c it's possible that the
      // application is calling us from a thread other than that in which
      // the Reactor's CTOR (which sets the owner) was called.
      //
      // We need to do this on every iteration because the reactor may be
      // acquired by one of the client threads in the LF waiting
      // strategy
      r->owner (ACE_Thread::self ());

      if (TAO_debug_level >= 3)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - blocking on handle events\n")));
      result = r->handle_events (tv);

      if (result == -1)
        {
          // An error, terminate the loop
          break;
        }
      if (result == 0
          && tv != 0
          && *tv == ACE_Time_Value::zero)
        {
          // A timeout, terminate the loop...
          break;
        }
      if (perform_work)
        {
          // This is running on behalf of a perform_work() call,
          // The loop should run only once.
          break;
        }
      // Otherwise just continue..
    }

  if (TAO_debug_level >= 3)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("TAO (%P|%t) - "
                          "end of run/perform_work %d\n"),
                result));

  return result;
}

void
TAO_ORB_Core::shutdown (CORBA::Boolean wait_for_completion,
                        CORBA::Environment &ACE_TRY_ENV)
{
  this->adapter_registry_.check_close (wait_for_completion,
                                       ACE_TRY_ENV);
  this->adapter_registry_.close (wait_for_completion,
                                 ACE_TRY_ENV);

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

#if (TAO_HAS_RT_CORBA == 1)

void
TAO_ORB_Core::resolve_rt_orb_i (CORBA::Environment &ACE_TRY_ENV)
{
  // @@
  TAO_Object_Loader *loader = 
    ACE_Dynamic_Service<TAO_Object_Loader>::instance ("RT_ORB_Loader");

  if (loader == 0)
    {
      // The Loader has not been statically configured, try to
      // dynamically load it...
      ACE_Service_Config::process_directive (
                                             "dynamic RT_ORB_Loader Service_Object *"
                                             "TAO:_make_TAO_RT_ORB_Loader()"
                                             );

      loader =
        ACE_Dynamic_Service<TAO_Object_Loader>::instance ("RT_ORB_Loader");
      if (loader == 0)
        ACE_THROW (CORBA::ORB::InvalidName ());
    }

  /// Create RT_ORB object.
  this->rt_orb_ =
    loader->create_object (this->orb_.in (), 0, 0, ACE_TRY_ENV);
  
}

#endif /* TAO_HAS_RT_CORBA == 1 */

void
TAO_ORB_Core::resolve_typecodefactory_i (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Object_Loader *loader =
    ACE_Dynamic_Service<TAO_Object_Loader>::instance ("TypeCodeFactory");
  if (loader == 0)
    {
      ACE_Service_Config::process_directive (
                                             "dynamic TypeCodeFactory Service_Object *"
                                             "TypeCodeFactory_DLL:_make_TCF_Loader()"
                                             );
      loader =
        ACE_Dynamic_Service<TAO_Object_Loader>::instance ("TypeCodeFactory");
      if (loader == 0)
        ACE_THROW (CORBA::ORB::InvalidName ());
    }
  this->typecode_factory_ =
    loader->create_object (this->orb_.in (), 0, 0, ACE_TRY_ENV);
}

void
TAO_ORB_Core::resolve_dynanyfactory_i (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Object_Loader *loader =
    ACE_Dynamic_Service<TAO_Object_Loader>::instance ("DynamicAny_Loader");
  if (loader == 0)
    {
      // The Loader has not been statically configured, try to
      // dynamically load it...
      ACE_Service_Config::process_directive (
                                             "dynamic DynamicAny_Loader Service_Object *"
                                             "TAO_DynamicAny:_make_TAO_DynamicAny_Loader()"
                                             );

      loader =
        ACE_Dynamic_Service<TAO_Object_Loader>::instance ("DynamicAny_Loader");
      if (loader == 0)
        ACE_THROW (CORBA::ORB::InvalidName ());
    }
  this->dynany_factory_ =
    loader->create_object (this->orb_.in (), 0, 0, ACE_TRY_ENV);
}

void
TAO_ORB_Core::resolve_iormanipulation_i (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Object_Loader *loader =
    ACE_Dynamic_Service<TAO_Object_Loader>::instance ("IORManip_Loader");

  if (loader == 0)
    {
      // The Loader has not been statically configured, try to
      // dynamically load it...
      ACE_Service_Config::process_directive (
                                             "dynamic IORManip_Loader Service_Object *"
                                             "TAO_IORManip:_make_TAO_IORManip_Loader()"
                                             );

      loader =
        ACE_Dynamic_Service<TAO_Object_Loader>::instance ("IORManip_Loader");
      if (loader == 0)
        ACE_THROW (CORBA::ORB::InvalidName ());
    }
  this->ior_manip_factory_ =
    loader->create_object (this->orb_.in (), 0, 0, ACE_TRY_ENV);
}

void
TAO_ORB_Core::resolve_ior_table_i (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Adapter_Factory *factory =
    ACE_Dynamic_Service<TAO_Adapter_Factory>::instance ("TAO_IORTable");
  if (factory == 0)
    {
      // The Loader has not been statically configured, try to
      // dynamically load it...
      ACE_Service_Config::process_directive (
                                             "dynamic TAO_IORTable Service_Object *"
                                             "TAO_IORTable:_make_TAO_Table_Adapter_Factory()"
                                             );

      factory =
        ACE_Dynamic_Service<TAO_Adapter_Factory>::instance ("TAO_IORTable");
    }
  if (factory == 0)
    ACE_THROW (CORBA::ORB::InvalidName ());

  // @@ Not exception safe
  TAO_Adapter *iortable_adapter = factory->create (this);
  this->adapter_registry_.insert (iortable_adapter, ACE_TRY_ENV);
  iortable_adapter->open (ACE_TRY_ENV);

  this->ior_table_ = iortable_adapter->root ();
}

CORBA::Object_ptr
TAO_ORB_Core::resolve_rir (const char *name,
                           CORBA::Environment &ACE_TRY_ENV)
{
  // Get the table of initial references specified through
  // -ORBInitRef.
  ACE_CString ior;
  ACE_CString object_id ((const char *) name);

  // Is the service name in the IOR Table.
  if (this->init_ref_map_.find (object_id, ior) == 0)
    return this->orb ()->string_to_object (ior.c_str (), ACE_TRY_ENV);

  // Get the list of initial reference prefixes specified through
  // -ORBDefaultInitRef.
  char * default_init_ref =
    this->orb_params ()->default_init_ref ();

  // Check if a DefaultInitRef was specified.
  if (ACE_OS::strlen (default_init_ref) != 0)
    {
      ACE_CString list_of_profiles (default_init_ref);

      // Clean up.
      delete [] default_init_ref;

      // Obtain the appropriate object key delimiter for the
      // specified protocol.
      const char object_key_delimiter =
        this->connector_registry ()->object_key_delimiter (
                     list_of_profiles.c_str ());

      // Make sure that the default initial reference doesn't end
      // with the object key delimiter character.
      if (list_of_profiles[list_of_profiles.length() - 1] !=
          object_key_delimiter)
        list_of_profiles += ACE_CString (object_key_delimiter);

      list_of_profiles += object_id;

      return this->orb ()->string_to_object (list_of_profiles.c_str (),
                                             ACE_TRY_ENV);
    }
  else
    {
      // Clean up.
      delete [] default_init_ref;
    }
  return CORBA::Object::_nil ();
}

CORBA_ORB_ObjectIdList_ptr
TAO_ORB_Core::list_initial_references (CORBA::Environment &ACE_TRY_ENV)
{
  // Unsupported initial services should NOT be included in the below list!
  const char *initial_services[] = { TAO_LIST_OF_INITIAL_SERVICES };
  // Make sure the "terminating" zero is the last array element so
  // that there is a stop condition when iterating through the list.

  const size_t initial_services_size =
    sizeof (initial_services) / sizeof (initial_services[0]);

  const size_t total_size =
    initial_services_size + this->init_ref_map_.current_size ();

  CORBA_ORB_ObjectIdList_ptr tmp = 0;

  ACE_NEW_THROW_EX (tmp,
                    CORBA_ORB_ObjectIdList (total_size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  CORBA_ORB_ObjectIdList_var list = tmp;
  list->length (total_size);

  CORBA::ULong index = 0;
  // Index for ObjectIdList members.

  // Iterate over TAO's "built-in" initial references.
  for (index = 0; index < initial_services_size; ++index)
    list[index] = initial_services[index];

  // Now iterate over the initial references created by the user and
  // add them to the sequence.
  InitRefMap::iterator end  = this->init_ref_map_.end ();

  for (InitRefMap::iterator i =this-> init_ref_map_.begin ();
       i != end;
       ++i, ++index)
    list[index] = (*i).int_id_.c_str ();

  return list._retn ();
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
                           ACE_TEXT ("(%P|%t) %p\n"),
                           ACE_TEXT ("TAO_ORB_Core::input_cdr_dblock_allocator (); ")
                           ACE_TEXT ("no more TSS keys")),
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
                           ACE_TEXT ("(%P|%t) %p\n"),
                           ACE_TEXT ("TAO_ORB_Core::input_cdr_buffer_allocator (); ")
                           ACE_TEXT ("no more TSS keys")),
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
                           ACE_TEXT ("(%P|%t) %p\n"),
                           ACE_TEXT ("TAO_ORB_Core::output_cdr_dblock_allocator (); ")
                           ACE_TEXT ("no more TSS keys")),
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
                           ACE_TEXT ("(%P|%t) %p\n"),
                           ACE_TEXT ("TAO_ORB_Core::input_cdr_buffer_allocator (); ")
                           ACE_TEXT ("no more TSS keys")),
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
                           ACE_TEXT ("(%P|%t) %p\n"),
                           ACE_TEXT ("TAO_ORB_Core::create_input_cdr_data_block (); ")
                           ACE_TEXT ("no more TSS keys")),
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

void
TAO_ORB_Core::call_sync_scope_hook (TAO_Stub *stub,
                                    int &has_synchronization,
                                    int &scope)
{
  if (TAO_ORB_Core::sync_scope_hook_ == 0)
    {
      has_synchronization = 0;
      return;
    }

  (*TAO_ORB_Core::sync_scope_hook_) (this, stub, has_synchronization, scope);
}

TAO_Sync_Strategy &
TAO_ORB_Core::get_sync_strategy (TAO_Stub *,
                                 int &scope)
{

  if (scope == TAO::SYNC_WITH_TRANSPORT ||
      scope == TAO::SYNC_WITH_SERVER ||
      scope == TAO::SYNC_WITH_TARGET)
    return this->transport_sync_strategy ();

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  if (scope == TAO::SYNC_NONE ||
      scope == TAO::SYNC_EAGER_BUFFERING)
    return this->eager_buffering_sync_strategy ();

  if (scope == TAO::SYNC_DELAYED_BUFFERING)
    return this->delayed_buffering_sync_strategy ();

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

  return this->transport_sync_strategy ();
}

void
TAO_ORB_Core::set_sync_scope_hook (Sync_Scope_Hook hook)
{
  TAO_ORB_Core::sync_scope_hook_ = hook;
  return;
}

#if (TAO_HAS_SYNC_SCOPE_POLICY == 1)

void
TAO_ORB_Core::stubless_sync_scope (CORBA::Policy *&result)
{
  // No need to lock, the object is in TSS storage....
  TAO_Policy_Current &policy_current =
    this->policy_current ();
  result = policy_current.sync_scope ();

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  if (result == 0)
    {
      TAO_Policy_Manager *policy_manager =
        this->policy_manager ();
      if (policy_manager != 0)
        result = policy_manager->sync_scope ();
    }

  if (result == 0)
    result = this->default_sync_scope ();

  return;
}

#endif /* TAO_HAS_SYNC_SCOPE_POLICY == 1 */

void
TAO_ORB_Core::call_timeout_hook (TAO_Stub *stub,
                                 int &has_timeout,
                                 ACE_Time_Value &time_value)
{
  if (TAO_ORB_Core::timeout_hook_ == 0)
    {
      has_timeout = 0;
      return;
    }
  (*TAO_ORB_Core::timeout_hook_) (this, stub, has_timeout, time_value);
}

void
TAO_ORB_Core::set_timeout_hook (Timeout_Hook hook)
{
  TAO_ORB_Core::timeout_hook_ = hook;
  // Saving the hook pointer so that we can use it later when needed.

  return;
}

CORBA::Policy *
TAO_ORB_Core::stubless_relative_roundtrip_timeout (void)
{
  CORBA::Policy *result = 0;

#if (TAO_HAS_CORBA_MESSAGING == 1)

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
#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  return result;

}

#if (TAO_HAS_RT_CORBA==1)

CORBA::Object_ptr
TAO_ORB_Core::priority_mapping_manager (void)
{
  if (CORBA::is_nil (TAO_ORB_Core::priority_mapping_manager_))
    return TAO_ORB_Core::priority_mapping_manager_;
  else
    return CORBA::Object::_nil ();
}

void
TAO_ORB_Core::priority_mapping_manager (CORBA::Object_ptr manager)
{
  TAO_ORB_Core::priority_mapping_manager_ = 
    CORBA::Object::_duplicate (manager);
}

CORBA::Policy *
TAO_ORB_Core::threadpool (void)
{
  CORBA::Policy *result = 0;

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  TAO_Policy_Manager *policy_manager =
    this->policy_manager ();
  if (policy_manager != 0)
    result = policy_manager->threadpool ();

  if (result == 0)
    result = this->default_threadpool ();

  return result;
}

CORBA::Policy *
TAO_ORB_Core::priority_model (void)
{
  CORBA::Policy *result = 0;

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  TAO_Policy_Manager *policy_manager =
    this->policy_manager ();
  if (policy_manager != 0)
    result = policy_manager->priority_model ();

  if (result == 0)
    result = this->default_priority_model ();

  return result;
}

CORBA::Policy *
TAO_ORB_Core::server_protocol (void)
{
  CORBA::Policy *result = 0;

  // @@ Must lock, but is is harder to implement than just modifying
  //    this call: the ORB does take a lock to modify the policy
  //    manager
  TAO_Policy_Manager *policy_manager =
    this->policy_manager ();
  if (policy_manager != 0)
    result = policy_manager->server_protocol ();

  if (result == 0)
    result = this->default_server_protocol ();

  return result;
}

#endif /* TAO_HAS_RT_CORBA == 1 */

// ****************************************************************

TAO_ORB_Core_TSS_Resources::TAO_ORB_Core_TSS_Resources (void)
  : output_cdr_dblock_allocator_ (0),
    output_cdr_buffer_allocator_ (0),
    output_cdr_msgblock_allocator_ (0),
    input_cdr_dblock_allocator_ (0),
    input_cdr_buffer_allocator_ (0),
    connection_cache_ (0),
    event_loop_thread_ (0),
    client_leader_thread_ (0),
    leader_follower_condition_variable_ (0),
    reactor_registry_ (0),
    reactor_registry_cookie_ (0),
    ts_objects_ (),
    orb_core_ (0)
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
    this->reactor_registry_->destroy_tss_cookie (
      this->reactor_registry_cookie_);

  //@@ This is broken on platforms that use TSS emulation since this
  //   destructor is invoked after the ORB.  Since we're under
  //   pressure to release a beta, we'll have to leak the TSS objects
  //   stores in the <ts_objects_> array.  However, the only service
  //   known to currently use this array is the SSLIOP pluggable
  //   protocol.  Fortunately, it registeres a null cleanup function
  //   so we're not leaking anything yet.  We *do* need to fix this
  //   before other services start to use this array.
  //      -Ossama
  // if (this->orb_core_ != 0)
  //   this->orb_core_->tss_cleanup_funcs ()->cleanup (this->ts_objects_);
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
          // Calling CORBA::ORB_init() returns a duplicated ORB
          // reference, so make sure that reference is stored in an
          // ORB_var so that no leak occurs.  The duplicate ORB
          // reference isn't needed outside the scope of this function
          // since the corresponding ORB Core instance will still
          // exist in the ORB table after the ORB reference is
          // destroyed.

          CORBA::ORB_var orb;

          int argc = 0;
          ACE_DECLARE_NEW_CORBA_ENV;
          ACE_TRY
            {
              orb = CORBA::ORB_init (argc, 0, 0, ACE_TRY_ENV);
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


TAO_ORB_Core::TAO_Collocation_Strategies
TAO_ORB_Core::collocation_strategy (CORBA::Object_ptr object)
{

  TAO_Stub *stub = object->_stubobj ();
  if (stub->servant_orb_ptr () != 0 &&
      stub->servant_orb_var ()->orb_core () != 0 &&
      object->_servant () != 0)
    {
      switch (stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ())
        {
        case THRU_POA:
          return TAO_ORB_Core::THRU_POA_STRATEGY;

        case DIRECT:
          return TAO_ORB_Core::DIRECT_STRATEGY;
        }
    }

  // In this case the Object is a client.
  return TAO_ORB_Core::REMOTE_STRATEGY;
}


// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Lock_Adapter<ACE_Null_Mutex>;
#if defined (ACE_HAS_THREADS)
template class ACE_Lock_Adapter<ACE_Recursive_Thread_Mutex>;
template class ACE_Lock_Adapter<ACE_Thread_Mutex>;
#endif /* ACE_HAS_THREADS */

template class ACE_Reverse_Lock<ACE_SYNCH_MUTEX>;
template class ACE_Guard<ACE_Reverse_Lock<ACE_SYNCH_MUTEX> >;

template class ACE_Env_Value<int>;
template class ACE_Env_Value<u_int>;

template class TAO_TSS_Singleton<TAO_TSS_Resources, ACE_SYNCH_MUTEX>;
template class ACE_TSS<TAO_TSS_Resources>;
template class ACE_TSS<TAO_ORB_Core_TSS_Resources>;

template class ACE_Hash_Map_Manager<ACE_CString, ACE_CString, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ACE_CString,ACE_CString,ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<ACE_CString, ACE_CString>;
template class ACE_Hash_Map_Reverse_Iterator<ACE_CString, ACE_CString, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

template class ACE_Array_Base<void *>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Lock_Adapter<ACE_Null_Mutex>
#if defined (ACE_HAS_THREADS)
#pragma instantiate ACE_Lock_Adapter<ACE_Recursive_Thread_Mutex>
#pragma instantiate ACE_Lock_Adapter<ACE_Thread_Mutex>
#endif /* ACE_HAS_THREADS */

#pragma instantiate ACE_Reverse_Lock<ACE_SYNCH_MUTEX>
#pragma instantiate ACE_Guard<ACE_Reverse_Lock<ACE_SYNCH_MUTEX> >

#pragma instantiate ACE_Env_Value<int>
#pragma instantiate ACE_Env_Value<u_int>

#pragma instantiate TAO_TSS_Singleton<TAO_TSS_Resources, ACE_SYNCH_MUTEX>
#pragma instantiate ACE_TSS<TAO_TSS_Resources>
#pragma instantiate ACE_TSS<TAO_ORB_Core_TSS_Resources>

#pragma instantiate ACE_Hash_Map_Manager<ACE_CString,ACE_CString,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ACE_CString,ACE_CString,ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, ACE_CString>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ACE_CString, ACE_CString, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#pragma instantiate ACE_Array_Base<void *>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
