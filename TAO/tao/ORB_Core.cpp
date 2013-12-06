// $Id$

#include "tao/ORB_Core.h"
#include "tao/ORB_Core_TSS_Resources.h"
#include "tao/ORB_Table.h"
#include "tao/TAO_Internal.h"
#include "tao/default_server.h"
#include "tao/Profile.h"
#include "tao/Stub.h"
#include "tao/LF_Follower.h"
#include "tao/Leader_Follower.h"
#include "tao/LF_Event_Loop_Thread_Helper.h"
#include "tao/Connector_Registry.h"
#include "tao/Transport_Queueing_Strategies.h"
#include "tao/Object_Loader.h"
#include "tao/ObjectIdListC.h"
#include "tao/BiDir_Adapter.h"
#include "tao/ZIOP_Adapter.h"
#include "tao/Collocation_Resolver.h"
#include "tao/Flushing_Strategy.h"
#include "tao/Request_Dispatcher.h"
#include "tao/Stub_Factory.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Thread_Lane_Resources_Manager.h"
#include "tao/TSS_Resources.h"
#include "tao/Protocols_Hooks.h"
#include "tao/Network_Priority_Protocols_Hooks.h"
#include "tao/IORInterceptor_Adapter.h"
#include "tao/IORInterceptor_Adapter_Factory.h"
#include "tao/debug.h"
#include "tao/TAOC.h"
#include "tao/Endpoint_Selector_Factory.h"
#include "tao/Client_Strategy_Factory.h"
#include "tao/Adapter_Factory.h"
#include "tao/Adapter.h"
#include "tao/GUIResource_Factory.h"
#include "tao/PolicyFactory_Registry_Adapter.h"
#include "tao/PolicyFactory_Registry_Factory.h"
#include "tao/ORBInitializer_Registry_Adapter.h"
#include "tao/Codeset_Manager.h"
#include "tao/GIOP_Fragmentation_Strategy.h"
#include "tao/SystemException.h"

#include "tao/Valuetype_Adapter.h"
#include "tao/Valuetype_Adapter_Factory.h"

#if (TAO_HAS_CORBA_MESSAGING == 1)
#include "tao/Policy_Manager.h"
#include "tao/Policy_Current.h"
#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

#include "ace/Reactor.h"
#include "ace/Dynamic_Service.h"
#include "ace/Arg_Shifter.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/Static_Object_Lock.h"
#include "ace/Auto_Ptr.h"
#include "ace/CORBA_macros.h"
#include "ace/Logging_Strategy.h"

#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
// Needed to set ACE_LOG_MSG::msg_ostream()
// FUZZ: disable check_for_streams_include
#  include "ace/streams.h"
#endif /* !ACE_LACKS_IOSTREAM_TOTALLY */

#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_string.h"
#include "ace/Message_Block.h"

#if TAO_HAS_INTERCEPTORS == 1
# include "tao/ClientRequestInterceptor_Adapter.h"
# include "tao/ClientRequestInterceptor_Adapter_Factory.h"
# include "tao/ServerRequestInterceptor_Adapter.h"
# include "tao/ServerRequestInterceptor_Adapter_Factory.h"
#endif  /* TAO_HAS_INTERCEPTORS == 1  */


#if !defined (__ACE_INLINE__)
# include "tao/ORB_Core.inl"
#endif /* ! __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE(TAO_ORB_Core_Static_Resources,
                      ACE_TEXT ("TAO_ORB_Core_Static_Resources"),
                      ACE_SVC_OBJ_T,
                      &ACE_SVC_NAME (TAO_ORB_Core_Static_Resources),
                      ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO, TAO_ORB_Core_Static_Resources)

// ****************************************************************

CORBA::Environment&
TAO_default_environment (void)
{
  return *TAO_TSS_Resources::instance ()->default_environment_;
}

// ****************************************************************

// Initialize instance_ to 0, since this is what we test for in the call
// to instance ().  Note that this does not require a constructor call, so
// it is always initialized by the time that instance () can be called.
//TAO_ORB_Core_Static_Resources* TAO_ORB_Core_Static_Resources::instance_ = 0;

// Force an instance to be created at module initialization time,
// since we do not want to worry about double checked locking and
// the race condition to initialize the lock.
TAO_ORB_Core_Static_Resources*
TAO_ORB_Core_Static_Resources::initialization_reference_ =
  TAO_ORB_Core_Static_Resources::instance ();

TAO_ORB_Core_Static_Resources*
TAO_ORB_Core_Static_Resources::instance (void)
{
  ACE_Service_Gestalt *current = ACE_Service_Config::current();
  TAO_ORB_Core_Static_Resources* tocsr =
    ACE_Dynamic_Service<TAO_ORB_Core_Static_Resources>::instance
    (current, "TAO_ORB_Core_Static_Resources", true);

  if (tocsr == 0)
    {
      // This new is never freed on purpose.  The data specified by
      // it needs to be around for the last shared library that references
      // this class.  This could occur in a destructor in a shared library
      // that is unloaded after this one.  One solution to avoid this
      // harmless memory leak would be to use reference counting.
      current->process_directive(ace_svc_desc_TAO_ORB_Core_Static_Resources);
      tocsr = ACE_Dynamic_Service<TAO_ORB_Core_Static_Resources>::instance
        (current, "TAO_ORB_Core_Static_Resources", true);

      ACE_Service_Gestalt *global = ACE_Service_Config::global();
      if (current != global)
        {
          TAO_ORB_Core_Static_Resources* global_tocsr =
            ACE_Dynamic_Service<TAO_ORB_Core_Static_Resources>::instance
            (global,"TAO_ORB_Core_Static_Resources");

          if (global_tocsr != 0)
            *tocsr = *global_tocsr;
        }
    }

  return tocsr;
}

TAO_ORB_Core_Static_Resources::TAO_ORB_Core_Static_Resources (void)
  : network_priority_protocols_hooks_name_ (
      "Network_Priority_Protocols_Hooks"),
    connection_timeout_hook_ (0),
    resource_factory_name_ ("Resource_Factory"),
    dynamic_adapter_name_ ("Dynamic_Adapter"),
    ifr_client_adapter_name_ ("IFR_Client_Adapter"),
    typecodefactory_adapter_name_ ("TypeCodeFactory_Adapter"),
    iorinterceptor_adapter_factory_name_ ("IORInterceptor_Adapter_Factory"),
    valuetype_adapter_factory_name_ ("Valuetype_Adapter_Factory"),
    alt_connection_timeout_hook_ (0)
{
}

TAO_ORB_Core_Static_Resources&
TAO_ORB_Core_Static_Resources::operator=(const TAO_ORB_Core_Static_Resources& other)
{
  this->network_priority_protocols_hooks_name_ =
    other.network_priority_protocols_hooks_name_;
  this->connection_timeout_hook_ = other.connection_timeout_hook_;
  this->resource_factory_name_ = other.resource_factory_name_;
  this->dynamic_adapter_name_ = other.dynamic_adapter_name_;
  this->ifr_client_adapter_name_ = other.ifr_client_adapter_name_;
  this->typecodefactory_adapter_name_ = other.typecodefactory_adapter_name_;
  this->iorinterceptor_adapter_factory_name_ =
    other.iorinterceptor_adapter_factory_name_;
  this->valuetype_adapter_factory_name_ =
    other.valuetype_adapter_factory_name_;
  this->alt_connection_timeout_hook_ = other.alt_connection_timeout_hook_;
  return *this;
}

// ****************************************************************

TAO_ORB_Core::TAO_ORB_Core (const char *orbid,
                            ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> gestalt)
  : protocols_hooks_ (0),
    network_priority_protocols_hooks_ (0),
#if TAO_USE_LOCAL_MEMORY_POOL == 1
    use_local_memory_pool_ (true),
#else
    use_local_memory_pool_ (false),
#endif
    lock_ (),
    thread_lane_resources_manager_ (0),
    collocation_resolver_ (0),
    stub_factory_ (0),
    protocol_factories_ (0),
    implrepo_service_ (CORBA::Object::_nil ()),
    use_implrepo_ (0),
    imr_endpoints_in_ior_ (1),
    typecode_factory_ (CORBA::Object::_nil ()),
    codec_factory_ (CORBA::Object::_nil ()),
    compression_manager_ (CORBA::Object::_nil ()),
    dynany_factory_ (CORBA::Object::_nil ()),
    ior_manip_factory_ (CORBA::Object::_nil ()),
    ior_table_ (CORBA::Object::_nil ()),
    async_ior_table_ (CORBA::Object::_nil ()),
    monitor_ (CORBA::Object::_nil ()),
    orb_ (CORBA::ORB::_nil ()),
    root_poa_ (),
    orb_params_ (),
    init_ref_map_ (TAO_DEFAULT_OBJECT_REF_TABLE_SIZE),
    object_ref_table_ (),
    object_key_table_ (),
    orbid_ (ACE_OS::strdup (orbid ? orbid : "")),
    resource_factory_ (0),
    client_factory_ (0),
    server_factory_ (0),
    ft_send_extended_sc_ (false),
    opt_for_collocation_ (true),
    use_global_collocation_ (true),
    collocation_strategy_ (TAO_DEFAULT_COLLOCATION_STRATEGY),

#if (TAO_HAS_CORBA_MESSAGING == 1)

    policy_manager_ (0),
    default_policies_ (0),
    policy_current_ (0),

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

    poa_current_ (),
    adapter_registry_ (this),
    poa_adapter_ (0),
    tm_ (),
    tss_cleanup_funcs_ (),
    tss_resources_ (),
    has_shutdown_ (true),  // Start the ORB in a  "shutdown" state.  Only
                           // after CORBA::ORB_init() is called will the
                           // ORB no longer be shutdown.  This does not
                           // mean that the ORB can be reinitialized.  It
                           // can only be initialized once.
    thread_per_connection_use_timeout_ (1),
    open_lock_ (),
    endpoint_selector_factory_ (0),
#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)
    eager_transport_queueing_strategy_ (0),
    delayed_transport_queueing_strategy_ (0),
    flush_transport_queueing_strategy_ (0),
#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */
    refcount_ (1),
    policy_factory_registry_ (0),
    orbinitializer_registry_ (0),
#if (TAO_HAS_INTERCEPTORS == 1)
    pi_current_ (CORBA::Object::_nil ()),
    client_request_interceptor_adapter_ (0),
    server_request_interceptor_adapter_ (0),
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
    ior_interceptor_adapter_ (0),
    valuetype_adapter_ (0),
    parser_registry_ (),
    bidir_adapter_ (0),
    bidir_giop_policy_ (0),
    ziop_adapter_ (0),
    ziop_enabled_ (false),
    flushing_strategy_ (0),
    codeset_manager_ (0),
    config_ (gestalt),
    sync_scope_hook_ (0),
    timeout_hook_ (0)
{
#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  ACE_NEW (this->flush_transport_queueing_strategy_,
           TAO::Flush_Transport_Queueing_Strategy);

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_CORBA_MESSAGING == 1)

  ACE_NEW (this->policy_manager_,
           TAO_Policy_Manager);

  ACE_NEW (this->default_policies_,
           TAO_Policy_Set (TAO_POLICY_ORB_SCOPE));

  ACE_NEW (this->policy_current_,
           TAO_Policy_Current);

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  // Initialize the default request dispatcher.
  ACE_NEW (this->request_dispatcher_,
           TAO_Request_Dispatcher);

  this->set_sync_scope_hook (TAO_ORB_Core::default_sync_scope_hook);
}

TAO_ORB_Core::~TAO_ORB_Core (void)
{
  delete this->thread_lane_resources_manager_;

  delete this->flushing_strategy_;

  ACE_OS::free (this->orbid_);

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  delete this->eager_transport_queueing_strategy_;
  delete this->delayed_transport_queueing_strategy_;
  delete this->flush_transport_queueing_strategy_;

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_CORBA_MESSAGING == 1)

  ::CORBA::release (this->policy_manager_);
  delete this->default_policies_;
  ::CORBA::release (this->policy_current_);

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  delete this->request_dispatcher_;

  delete this->policy_factory_registry_;

  // Don't delete, is a process wide singleton shared by all orbs
  orbinitializer_registry_ = 0;

  ::CORBA::release (this->orb_);

  delete this->codeset_manager_;
  this->codeset_manager_ = 0;

  // This will destroy the service repository for this core
  (void) TAO::ORB::close_services (this->config_);

}

int
TAO_ORB_Core::init (int &argc, char *argv[] )
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

  bool use_ior = true;
  int cdr_tradeoff = ACE_DEFAULT_CDR_MEMCPY_TRADEOFF;

  // The following things should be changed to use the ACE_Env_Value<>
  // template sometime.

  // Name Service port use for Multicast
  unsigned short ns_port = 0;

  // Trading Service port used for Multicast
  unsigned short ts_port = 0;

  // Implementation Repository Service port #.
  unsigned short ir_port = 0;

  // Buffer sizes for kernel socket buffers
  // @@ should be a default defined for each protocol implementation?
  //    since we may have protocols loaded which use shared memory of
  //    some form, fredk
  int rcv_sock_size = -1;
  int snd_sock_size = -1;

  // Use TCP_NODELAY.
  int nodelay = 1;

  // Use SO_KEEPALIVE (default 0).
  int so_keepalive = 0;

  // Use SO_DONTROUTE (default 0)
  int so_dontroute = 0;

  // Number of hops for a datagram sent through socket.
  int ip_hoplimit = -1;

  // Use IP_MILTICAST_LOOP (default 1).
  int ip_multicastloop = 1;

  // Use dotted decimal addresses
  // @@ This option will be treated as a suggestion to each loaded
  //    protocol to use a character representation for the numeric
  //    address, otherwise use a logical name. fredk
#if (TAO_USE_DOTTED_DECIMAL_ADDRESSES == 1)
  int dotted_decimal_addresses = 1;
#else
  int dotted_decimal_addresses = 0;
#endif /* TAO_USE_DOTTED_DECIMAL_ADDRESSES */

  // Disable looking up the host name for incoming connections.
  int no_server_side_name_lookups = 0;

#if defined (TAO_STD_PROFILE_COMPONENTS)
  bool std_profile_components = true;
#else
  bool std_profile_components = false;
#endif /* TAO_STD_PROFILE_COMPONENTS */

  int linger = TAO_SO_LINGER;
  time_t accept_error_delay = TAO_ACCEPT_ERROR_DELAY;
  bool use_parallel_connects = TAO_USE_PARALLEL_CONNECT;

  // Copy command line parameter not to use original.
  ACE_Argv_Type_Converter command_line (argc, argv);

  ACE_Arg_Shifter arg_shifter (command_line.get_argc (),
                               command_line.get_TCHAR_argv ());

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    monitor,
                    this->lock_,
                    -1);

#if (TAO_NEGOTIATE_CODESETS == 1)
  bool negotiate_codesets = true;
#else
  bool negotiate_codesets = false;
#endif /* TAO_NEGOTIATE_CODESETS */

  // Pick up the value of the use_implrepo_ flag from an environment variable
  // called "TAO_USE_IMR". Do it here so that it can be overridden by
  // the "-ORBUseIMR" command line argument.
  //
  char* const use_IMR_env_var_value = ACE_OS::getenv  ("TAO_USE_IMR") ;
  if  (use_IMR_env_var_value != 0)
    {
      this->use_implrepo_ = ACE_OS::atoi  (use_IMR_env_var_value) ;
    }

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR *current_arg = 0;

      ////////////////////////////////////////////////////////////////
      // begin with the 'parameterless' flags                       //
      ////////////////////////////////////////////////////////////////
      if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBDottedDecimalAddresses"))))
        {
          // Use dotted decimal addresses
          // @@ this should be renamed.  See above comment. fredk
          dotted_decimal_addresses =
            ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBNoServerSideNameLookups"))))
        {
          // Don't look up the host name for incoming connections
          no_server_side_name_lookups =
            ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBNameServicePort"))))
        {
          // Specify the port number for the NameService.
          // Unrelated to ORB Protocols, this is used for multicast.

          ns_port = static_cast <CORBA::UShort> (ACE_OS::atoi (current_arg));

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBMulticastDiscoveryEndpoint"))))
        {
          // Specify mcast address:port@network_interface for the
          // Naming Service Multicast Discovery Protocol.
          // If there is no colon, its only the port no.
          // If there is a '@' also, it means that the network
          // interface name is specified.
          this->orb_params ()->mcast_discovery_endpoint (
            ACE_TEXT_ALWAYS_CHAR(current_arg));

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBNodelay"))))
        {
          // Use TCP_NODELAY or not.
          nodelay =
            ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBKeepalive"))))
        {
          // Use SO_KEEPALIVE or not.
          so_keepalive =
            ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBDontRoute"))))
        {
          // Use SO_DONTROUTE or not.
          so_dontroute =
            ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBIPHopLimit"))))
        {
          // Number of IP hops.
          ip_hoplimit =
            ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBIPMulticastLoop"))))
        {
          // Use IP_MULTICAST_LOOP or not.
          ip_multicastloop =
            ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBTradingServicePort"))))
        {
          // Specify the port number for the TradingService.

          ts_port = static_cast <CORBA::UShort> (ACE_OS::atoi (current_arg));

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBImplRepoServicePort"))))
        {
          // Specify the multicast port number for the Implementation
          // Repository.
          ir_port = static_cast <CORBA::UShort> (ACE_OS::atoi (current_arg));

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBRcvSock"))))
        {
          // @@ All protocol implementation may not use sockets, so
          //    this can either be a generic I/O Buffer size or
          //    Buffer info can be a per protocol specification, fredk

          // Specify the size of the socket's receive buffer

          rcv_sock_size = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBSndSock"))))
        {
          // @@ All protocol implementation may not use sockets, so
          //    this can either be a generic I/O Buffer size or
          //    Buffer info can be a per protocol specification, fredk

          // Specify the size of the socket's send buffer
          snd_sock_size = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBObjRefStyle"))))
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
          const ACE_TCHAR *opt = current_arg;
          if (ACE_OS::strcasecmp (opt, ACE_TEXT("URL")) == 0)
            use_ior = false;
          else if (ACE_OS::strcasecmp (opt, ACE_TEXT("IOR")) == 0)
            use_ior = true;

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBCollocationStrategy"))))
        {
          // Specify which collocation policy we want to use.
          const ACE_TCHAR *opt = current_arg;
          if (ACE_OS::strcasecmp (opt, ACE_TEXT("thru_poa")) == 0)
            {
              this->collocation_strategy_ = TAO_COLLOCATION_THRU_POA;
            }
          else if (ACE_OS::strcasecmp (opt, ACE_TEXT("direct")) == 0)
            {
              this->collocation_strategy_ = TAO_COLLOCATION_DIRECT;
            }
          else if (ACE_OS::strcasecmp (opt, ACE_TEXT("best")) == 0)
            {
              this->collocation_strategy_ = TAO_COLLOCATION_BEST;
            }

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBCollocation"))))
        {
          // Specify whether we want to optimize against collocation
          // objects.  Valid arguments are: "global", "no", and "per-orb".
          // Default is global.

          const ACE_TCHAR *opt = current_arg;
          if (ACE_OS::strcasecmp (opt, ACE_TEXT("global")) == 0)
            {
              this->opt_for_collocation_ = true;
              this->use_global_collocation_ = true;
            }
          else if (ACE_OS::strcasecmp (opt, ACE_TEXT("NO")) == 0)
            {
              this->opt_for_collocation_ = false;
              this->use_global_collocation_ = false;
            }
          else if (ACE_OS::strcasecmp (opt, ACE_TEXT("per-orb")) == 0)
            {
              this->opt_for_collocation_ = true;
              this->use_global_collocation_ = false;
            }
          else
            {
              TAOLIB_DEBUG ((LM_WARNING,
                          ACE_TEXT ("WARNING: Unknown option to ")
                          ACE_TEXT ("'-ORBCollocation': %s\n"), opt));
            }

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBPreferredInterfaces"))) ||
               0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBPreferredInterface"))) )
        {
          if (this->orb_params ()->preferred_interfaces (
                ACE_TEXT_ALWAYS_CHAR (current_arg)) == false)
            throw ::CORBA::INTERNAL (
              CORBA::SystemException::_tao_minor_code (
                TAO_ORB_CORE_INIT_LOCATION_CODE,
                0),
              CORBA::COMPLETED_NO);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBEnforcePreferredInterfaces"))) ||
               0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBEnforcePreferredInterface"))) )
        {
          this->orb_params ()->enforce_pref_interfaces (
            !!ACE_OS::atoi (current_arg));
          arg_shifter.consume_arg ();
        }
#if defined (ACE_HAS_IPV6)
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBPreferIPV6Interfaces"))))
        {
          int prefer_ipv6_interfaces = ACE_OS::atoi (current_arg);
          if (prefer_ipv6_interfaces)
            this->orb_params ()->prefer_ipv6_interfaces (true);
          else
            this->orb_params ()->prefer_ipv6_interfaces (false);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBConnectIPV6Only"))))
        {
          int connect_ipv6_only = ACE_OS::atoi (current_arg);
          if (connect_ipv6_only)
            this->orb_params ()->connect_ipv6_only (true);
          else
            this->orb_params ()->connect_ipv6_only (false);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBUseIPV6LinkLocal"))))
        {
          int use_ipv6_link_local = ACE_OS::atoi (current_arg);
          if (use_ipv6_link_local)
            this->orb_params ()->use_ipv6_link_local (true);
          else
            this->orb_params ()->use_ipv6_link_local (false);

          arg_shifter.consume_arg ();
        }
#endif /* ACE_HAS_IPV6 */
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBCDRTradeoff"))))
        {
          cdr_tradeoff = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }

      // A new <ObjectID>:<IOR> mapping has been specified. This will be
      // used by the resolve_initial_references ().

      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBInitRef"))))
        {
          const ACE_TCHAR *pos = ACE_OS::strchr (current_arg, '=');
          if (pos == 0)
            {
              TAOLIB_ERROR ((LM_ERROR,
                          ACE_TEXT ("Invalid ORBInitRef argument '%s'")
                          ACE_TEXT ("format is ObjectID=IOR\n"),
                          current_arg));
              throw ::CORBA::INTERNAL (
                CORBA::SystemException::_tao_minor_code (
                  TAO_ORB_CORE_INIT_LOCATION_CODE,
                  0),
                CORBA::COMPLETED_NO);
            }
          ACE_CString object_id (ACE_TEXT_ALWAYS_CHAR(current_arg),
                                 pos - current_arg);
          ACE_CString IOR (ACE_TEXT_ALWAYS_CHAR(pos + 1));
          if (!this->init_ref_map_.insert (
                 std::make_pair (InitRefMap::key_type (object_id),
                                 InitRefMap::data_type (IOR))).second)
            {
              TAOLIB_ERROR ((LM_ERROR,
                          ACE_TEXT ("Duplicate -ORBInitRef ")
                          ACE_TEXT ("argument '%s'\n"),
                          current_arg));
              throw ::CORBA::INTERNAL (
                CORBA::SystemException::_tao_minor_code (
                  TAO_ORB_CORE_INIT_LOCATION_CODE,
                  0),
                CORBA::COMPLETED_NO);
            }
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBDefaultInitRef"))))
        {
          // Set the list of prefixes from -ORBDefaultInitRef.
          this->orb_params ()->default_init_ref
            (ACE_TEXT_ALWAYS_CHAR(current_arg));

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBStdProfileComponents"))))
        {
          std_profile_components =
            ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBAMICollocation"))))
        {
          int const ami_collocation = ACE_OS::atoi (current_arg);
          if (ami_collocation)
            this->orb_params ()->ami_collication (true);
          else
            this->orb_params ()->ami_collication (false);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBResources"))))
        {
          TAOLIB_DEBUG ((LM_WARNING,
                      ACE_TEXT ("\"-ORBResources\" has been ")
                      ACE_TEXT ("deprecated.\n")));

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBLogFile"))))
        {
          // redirect all TAOLIB_DEBUG and TAOLIB_ERROR output to a file
          // USAGE: -ORBLogFile <file>
          // default: if <file> is present     = append
          //          if <file> is not present = create

          const ACE_TCHAR *file_name = current_arg;
          arg_shifter.consume_arg ();

          // would rather use ACE_OSTREAM_TYPE out here..
          // but need ACE_FSTREAM_TYPE to call ->open(...)
          // and haven't found such a macro to rep FILE* and/or fstream*

#if defined (ACE_LACKS_IOSTREAM_TOTALLY)

          FILE* output_stream = ACE_OS::fopen (file_name, ACE_TEXT ("a"));

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

          output_stream->open (ACE_TEXT_ALWAYS_CHAR (file_name),
                               ios::out | ios::app);

          if (!output_stream->bad ())
            {
              ACE_LOG_MSG->msg_ostream (output_stream, 1);
            }

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

          ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER);
          ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBVerboseLogging"))))
        {
          unsigned long const verbose_logging = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();

          typedef void (ACE_Log_Msg::*PTMF)(u_long);
          PTMF flagop = &ACE_Log_Msg::set_flags;
          u_long value;

          switch (verbose_logging)
            {
            case 0:
              flagop = &ACE_Log_Msg::clr_flags;
              value = ACE_Log_Msg::VERBOSE | ACE_Log_Msg::VERBOSE_LITE;
              break;
            case 1:
              value = ACE_Log_Msg::VERBOSE_LITE; break;
            default:
              value = ACE_Log_Msg::VERBOSE; break;
            }

          (ACE_LOG_MSG->*flagop)(value);
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBHandleLoggingStrategyEvents"))))
        {
          ACE_Logging_Strategy *logging_strategy =
            dynamic_cast<ACE_Logging_Strategy*> (
              ACE_Dynamic_Service<ACE_Service_Object>::instance (current_arg));

          if (logging_strategy != 0)
            {
              logging_strategy->reactor (this->reactor ());
            }

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBUseIMR"))))
        {
          // Use IR or not.
          this->use_implrepo_ = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBIMREndpointsInIOR"))))
        {
          this->imr_endpoints_in_ior_ = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBid"))))
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
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBServerId"))))
        {
          // The this->server_id_ is to uniquely identify a server to
          // an IMR.
          // Fill in later.
          this->server_id_.set(ACE_TEXT_ALWAYS_CHAR(current_arg));

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
               (ACE_TEXT("-ORBLingerTimeout"))))
        {
          linger = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
               (ACE_TEXT("-ORBAcceptErrorDelay"))))
        {
          accept_error_delay = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBEndpoint"))))
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
          // addr.  All endpoint strings should be of the above
          // form(s).
          //
          // Multiple sets of endpoint may be seperated by a semi-colon `;'.
          // For example:
          //
          //   corbaloc:space:2001,1.2@odyssey:2010;uiop://foo,bar
          //
          // All endpoint strings should be of the above form(s).

          this->set_endpoint_helper (TAO_DEFAULT_LANE,
                                     ACE_TEXT_ALWAYS_CHAR (current_arg));

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBListenEndpoints"))))
        {
          // This option is similar to the -ORBEndPoint option. May be
          // ORBEndpoint option will be deprecated in future. But, for
          // now, I (Priyanka) am leaving so that both options can be
          // used.

          this->set_endpoint_helper (TAO_DEFAULT_LANE,
                                     ACE_TEXT_ALWAYS_CHAR (current_arg));

          arg_shifter.consume_arg ();
        }
      else if ((0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBLaneEndpoint")))) ||
               (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBLaneListenEndpoints")))))
        {
          // This option is similar to the -ORBEndPoint option but
          // specifies endpoints for each lane.

          if (arg_shifter.is_option_next ())
            return -1;

          ACE_CString lane (ACE_TEXT_ALWAYS_CHAR (current_arg));
          arg_shifter.consume_arg ();

          if (arg_shifter.is_option_next ())
            return -1;

          ACE_CString endpoints (ACE_TEXT_ALWAYS_CHAR
                                  (arg_shifter.get_current ()));
          arg_shifter.consume_arg ();

          this->set_endpoint_helper (lane, endpoints);
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBNoProprietaryActivation"))))
        {
          // This option can be used to set to not use any proprietary
          // activation framework. The only TAO proprietary activation
          // framework is IMR. So, by setting this option in TAO, the
          // IMR shouldnt be used .. even if the ORBUseIMR option is
          // set.
          // Fill in later
          // @@ To do later: Priyanka.

          throw ::CORBA::NO_IMPLEMENT ();
        }
       else if (0 != (current_arg = arg_shifter.get_the_parameter
                 (ACE_TEXT("-ORBUseSharedProfile"))))
         {
           this->orb_params ()->shared_profile (ACE_OS::atoi (current_arg));

           arg_shifter.consume_arg ();
         }
       else if (0 != (current_arg = arg_shifter.get_the_parameter
                 (ACE_TEXT("-ORBNegotiateCodesets"))))
         {
           negotiate_codesets = (ACE_OS::atoi (current_arg));

           arg_shifter.consume_arg ();
         }
       else if (0 != (current_arg = arg_shifter.get_the_parameter
                 (ACE_TEXT("-ORBUseParallelConnects"))))
         {
           use_parallel_connects = ACE_OS::atoi (current_arg);
           arg_shifter.consume_arg ();
         }
       else if (0 != (current_arg = arg_shifter.get_the_parameter
                 (ACE_TEXT("-ORBParallelConnectDelay"))))
         {
           this->orb_params ()->parallel_connect_delay
             (ACE_OS::atoi (current_arg));
           arg_shifter.consume_arg ();
         }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBSingleReadOptimization"))))
        {
          this->orb_params ()->single_read_optimization
            (ACE_OS::atoi (current_arg));

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBDisableRTCollocation"))))
        {
          int disable_rt_collocation = ACE_OS::atoi (current_arg);
          if (disable_rt_collocation)
            this->orb_params ()->disable_rt_collocation_resolver (true);
          else
            this->orb_params ()->disable_rt_collocation_resolver (false);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBUseLocalMemoryPool"))))
        {
          this->use_local_memory_pool_ = (0 != ACE_OS::atoi (current_arg));

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBMaxMessageSize"))))
        {
          this->orb_params_.max_message_size (ACE_OS::atoi (current_arg));

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBFTSendFullGroupTC"))))
        {
          this->ft_send_extended_sc_ = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBForwardInvocationOnObjectNotExist"))))
        {
          int forward = ACE_OS::atoi (current_arg);
          if (forward)
            this->orb_params_.forward_invocation_on_object_not_exist (true);
          else
            this->orb_params_.forward_invocation_on_object_not_exist (false);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBForwardOnTransientLimit"))))
        {
          int limit = ACE_OS::atoi (current_arg);
          this->orb_params_.forward_on_exception_limit (TAO::FOE_TRANSIENT, limit);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                     (ACE_TEXT("-ORBForwardOnCommFailureLimit"))))
        {
          int limit = ACE_OS::atoi (current_arg);
          this->orb_params_.forward_on_exception_limit (
                              TAO::FOE_COMM_FAILURE, limit);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                     (ACE_TEXT("-ORBForwardOnObjectNotExistLimit"))))
        {
          int limit = ACE_OS::atoi (current_arg);
          this->orb_params_.forward_on_exception_limit (
                              TAO::FOE_OBJECT_NOT_EXIST, limit);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBForwardOnInvObjrefLimit"))))
        {
          int limit = ACE_OS::atoi (current_arg);
          this->orb_params_.forward_on_exception_limit (
                              TAO::FOE_INV_OBJREF, limit);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                     (ACE_TEXT("-ORBForwardOnReplyClosedLimit"))))
        {
          int limit = ACE_OS::atoi (current_arg);
          this->orb_params_.invocation_retry_params ()
            .forward_on_reply_closed_limit_ = limit;
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBForwardDelay"))))
        {
          int msecs = ACE_OS::atoi (current_arg);
          ACE_Time_Value delay(0, 1000*msecs);
          this->orb_params_.forward_on_exception_delay (delay);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                     (ACE_TEXT("-ORBForwardOnceOnObjectNotExist"))))
        {
          int forward = ACE_OS::atoi (current_arg);
          if (forward)
            this->orb_params_.forward_once_exception (TAO::FOE_OBJECT_NOT_EXIST);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBForwardOnceOnCommFailure"))))
        {
          int forward = ACE_OS::atoi (current_arg);
          if (forward)
            this->orb_params_.forward_once_exception (TAO::FOE_COMM_FAILURE);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBForwardOnceOnTransient"))))
        {
          int forward = ACE_OS::atoi (current_arg);
          if (forward)
            this->orb_params_.forward_once_exception (TAO::FOE_TRANSIENT);

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBForwardOnceOnInvObjref"))))
        {
          int forward = ACE_OS::atoi (current_arg);
          if (forward)
            this->orb_params_.forward_once_exception (TAO::FOE_INV_OBJREF);

          arg_shifter.consume_arg ();
        }
     else if (0 != (current_arg = arg_shifter.get_the_parameter
                    (ACE_TEXT("-ORBAllowZIOPNoServerPolicies"))))
        {
          // This option takes a boolean 0 (off/dissallow) or 1 (on/allow)
          this->orb_params_.allow_ziop_no_server_policies (!!ACE_OS::atoi (current_arg));
          arg_shifter.consume_arg ();
        }
     else if (0 != (current_arg = arg_shifter.get_the_parameter
                    (ACE_TEXT("-ORBDynamicThreadPoolName"))))
        {
          this->orb_params_.dynamic_thread_pool_config_name (ACE_TEXT_ALWAYS_CHAR
                                                             (current_arg));
          arg_shifter.consume_arg ();
        }

      ////////////////////////////////////////////////////////////////
      // catch any unknown -ORB args                                //
      ////////////////////////////////////////////////////////////////
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-ORB")) != -1)
        {
          if (TAO_debug_level > 0)
            {
              current_arg = arg_shifter.get_current ();
              TAOLIB_ERROR ((LM_ERROR,
                          ACE_TEXT ("ERROR: Unknown \"-ORB\" option ")
                          ACE_TEXT ("<%s>.\n"),
                          ((current_arg == 0) ? ACE_TEXT("<NULL>")
                                              : current_arg)));
            }

          throw ::CORBA::BAD_PARAM (
            CORBA::SystemException::_tao_minor_code (
              TAO_ORB_CORE_INIT_LOCATION_CODE,
              EINVAL),
            CORBA::COMPLETED_NO);
        }

      ////////////////////////////////////////////////////////////////
      // ok, we can't interpret this argument, move to next argument//
      ////////////////////////////////////////////////////////////////
      else
        {
          // Any arguments that don't match are ignored so that the
          // caller can still use them.
          arg_shifter.ignore_arg ();
        }
    }

  const char *env_endpoint =
    ACE_OS::getenv ("TAO_ORBENDPOINT");

  if (env_endpoint != 0)
    {
      int result =
        this->orb_params ()->add_endpoints (TAO_DEFAULT_LANE, env_endpoint);

      if (result != 0)
        {
          if (TAO_debug_level > 0)
            {
              TAOLIB_ERROR ((LM_ERROR,
                          ACE_TEXT ("ERROR: Environment variable ")
                          ACE_TEXT ("TAO_ORBENDPOINT set to invalid value ")
                          ACE_TEXT ("<%C>.\n"),
                          env_endpoint));
            }

          throw ::CORBA::BAD_PARAM (
            CORBA::SystemException::_tao_minor_code (
              TAO_ORB_CORE_INIT_LOCATION_CODE,
              EINVAL),
            CORBA::COMPLETED_NO);
        }
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
  (void) ACE_OS::signal (SIGPIPE, (ACE_SignalHandler) SIG_IGN);
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
      TAOLIB_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - %p\n"),
                  ACE_TEXT ("ORB Core unable to find a ")
                  ACE_TEXT ("Resource Factory instance")));
      throw ::CORBA::INTERNAL (
        CORBA::SystemException::_tao_minor_code (
          TAO_ORB_CORE_INIT_LOCATION_CODE,
          0),
        CORBA::COMPLETED_NO);
    }

  // Set whether or not to use the local memory pool for the cdr allocators.

  trf->use_local_memory_pool (this->use_local_memory_pool_);

  // @@ ????
  // Make sure the reactor is initialized...
  ACE_Reactor *reactor = this->reactor ();
  if (reactor == 0)
    {
      TAOLIB_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - %p\n"),
                  ACE_TEXT ("ORB Core unable to initialize reactor")));
      throw ::CORBA::INITIALIZE (
        CORBA::SystemException::_tao_minor_code (
          TAO_ORB_CORE_INIT_LOCATION_CODE,
          0),
        CORBA::COMPLETED_NO);
    }

  TAO_Server_Strategy_Factory *ssf = this->server_factory ();

  if (ssf == 0)
    {
      TAOLIB_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - %p\n"),
                  ACE_TEXT ("ORB Core unable to find a ")
                  ACE_TEXT ("Server Strategy Factory instance")));
      throw ::CORBA::INTERNAL (
        CORBA::SystemException::_tao_minor_code (
          TAO_ORB_CORE_INIT_LOCATION_CODE,
          0),
        CORBA::COMPLETED_NO);
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
          int const milliseconds =
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
  this->orb_ = CORBA::ORB::_tao_make_ORB (this);

  // This should probably move into the ORB Core someday rather then
  // being done at this level.
  this->orb_->_use_omg_ior_format (use_ior);

  // Set all kinds of orb parameters whose setting needed to be
  // deferred until after the service config entries had been
  // determined.

  this->orb_params ()->service_port (TAO::MCAST_NAMESERVICE, ns_port);

  if (ns_port != 0)
    {
      static char const mcast_fmt[] = "mcast://:%d::";
      static size_t const PORT_BUF_SIZE = 256;

      char def_init_ref[PORT_BUF_SIZE] = { 0 };

      ACE_OS::snprintf (def_init_ref,
                        PORT_BUF_SIZE,
                        mcast_fmt,
                        ns_port);

      this->orb_params ()->default_init_ref (def_init_ref);
    }

  this->orb_params ()->service_port (TAO::MCAST_TRADINGSERVICE, ts_port);
  this->orb_params ()->service_port (TAO::MCAST_IMPLREPOSERVICE, ir_port);

  this->orb_params ()->use_dotted_decimal_addresses (dotted_decimal_addresses);
  // When caching incoming transports don't use the host name if
  // -ORBDottedDecimalAddresses or -ORBNoServerSideNameLookups is true.
  this->orb_params ()->cache_incoming_by_dotted_decimal_address
                                            (no_server_side_name_lookups
                                             || dotted_decimal_addresses);

  this->orb_params ()->use_parallel_connects
    (use_parallel_connects != 0);

  this->orb_params ()->linger (linger);
  this->orb_params ()->accept_error_delay (accept_error_delay);
  this->orb_params ()->nodelay (nodelay);
  this->orb_params ()->sock_keepalive (so_keepalive);
  this->orb_params ()->sock_dontroute (so_dontroute);
  this->orb_params ()->ip_hoplimit (ip_hoplimit);
  this->orb_params ()->ip_multicastloop (ip_multicastloop);
  if (rcv_sock_size >= 0)
    this->orb_params ()->sock_rcvbuf_size (rcv_sock_size);
  if (snd_sock_size >= 0)
    this->orb_params ()->sock_sndbuf_size (snd_sock_size);
  if (cdr_tradeoff >= 0)
    this->orb_params ()->cdr_memcpy_tradeoff (cdr_tradeoff);

  this->orb_params ()->std_profile_components (std_profile_components);

  this->orb_params ()->negotiate_codesets (negotiate_codesets);

  if (this->codeset_manager())
    this->codeset_manager_->open(*this);
  else
    if  (TAO_debug_level > 0)
        TAOLIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT("TAO (%P|%t) - ORB_Core: ")
                    ACE_TEXT("Codeset Manager not available\n")));

  // Set up the pluggable protocol infrastructure.  First get a
  // pointer to the protocol factories set, then obtain pointers to
  // all factories loaded by the service configurator.
  // Load all protocol factories!
  if (trf->init_protocol_factories () == -1)
    throw ::CORBA::INITIALIZE (
      CORBA::SystemException::_tao_minor_code (
        TAO_ORB_CORE_INIT_LOCATION_CODE,
        0),
      CORBA::COMPLETED_NO);

  // init the ORB core's pointer
  this->protocol_factories_ = trf->get_protocol_factories ();

  // Initialize the flushing strategy
  this->flushing_strategy_ = trf->create_flushing_strategy ();

  // Look in the service repository for an instance of the Protocol Hooks.
  const char *protocols_hooks_name = this->orb_params ()->protocols_hooks_name ();

  this->protocols_hooks_ =
    ACE_Dynamic_Service<TAO_Protocols_Hooks>::instance
    (this->configuration (),
     ACE_TEXT_CHAR_TO_TCHAR (protocols_hooks_name));

  if (this->protocols_hooks_ != 0)
    {
      // Initialize the protocols hooks instance.
      this->protocols_hooks_->init_hooks (this);
    }

  // If available, allow the Adapter Factory to setup.
  ACE_Service_Object *adapter_factory =
    ACE_Dynamic_Service<ACE_Service_Object>::instance (
      this->configuration (),
      this->orb_params ()->poa_factory_name ());

  if (adapter_factory != 0)
    {
      adapter_factory->init (0, 0);
    }

  // Look in the service repository for an instance of the
  // Network Priority Protocol Hooks.
  const ACE_CString &network_priority_protocols_hooks_name =
     TAO_ORB_Core_Static_Resources::instance ()->
       network_priority_protocols_hooks_name_;

  this->network_priority_protocols_hooks_ =
    ACE_Dynamic_Service<TAO_Network_Priority_Protocols_Hooks>::instance
    (this->configuration (),
     ACE_TEXT_CHAR_TO_TCHAR (network_priority_protocols_hooks_name.c_str()));

  if (this->network_priority_protocols_hooks_ != 0)
    {
      // Initialize the protocols hooks instance.
      this->network_priority_protocols_hooks_->init_hooks (this);
    }

  // As a last step perform initializations of the service callbacks
  this->services_callbacks_init ();

  // The ORB has been initialized, meaning that the ORB is no longer
  // in the shutdown state.
  this->has_shutdown_ = false;

  return 0;
}


int
TAO_ORB_Core::fini (void)
{
  try
    {
      // Shutdown the ORB and block until the shutdown is complete.
      this->shutdown (1);
    }
  catch (const ::CORBA::Exception& ex)
    {
      ACE_CString message =
        "Exception caught in trying to shutdown ";
      message += this->orbid_;
      message += "\n";

      ex._tao_print_exception (message.c_str ());
    }

  // Wait for any server threads, ignoring any failures.
  (void) this->thr_mgr ()->wait ();

  ::CORBA::release (this->typecode_factory_);

  ::CORBA::release (this->codec_factory_);

  ::CORBA::release (this->compression_manager_);

  ::CORBA::release (this->dynany_factory_);

  ::CORBA::release (this->ior_manip_factory_);

  ::CORBA::release (this->ior_table_);

  ::CORBA::release (this->async_ior_table_);

  ::CORBA::release (this->monitor_);

  if (TAO_debug_level > 2)
    {
      TAOLIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - Destroying ORB <%C>\n"),
                  this->orbid_));
    }

  // Finalize lane resources.
  //
  // @@ Do not call this->thread_lane_resources_manager().finalize().
  // this->thread_lane_manager_resources() can seg fault if the
  // factory method it invokes returns a zero pointer, which can
  // easily occur if the ORB is partially initialized due to a Service
  // Configurator initialization failure.  Instead check if the
  // cached pointer is non-zero and then finalize.
  //
  // @todo Fix potential seg fault in
  //       TAO_ORB_Core::thread_lane_resources_manager().
  if (this->thread_lane_resources_manager_ != 0)
    this->thread_lane_resources_manager_->finalize ();

  // Destroy the object_key table
  this->object_key_table_.destroy ();

  delete this;

  return 0;
}

void
TAO_ORB_Core::set_resource_factory (const char *resource_factory_name)
{
  TAO_ORB_Core_Static_Resources::instance ()->resource_factory_name_ =
    resource_factory_name;
}

void
TAO_ORB_Core::set_gui_resource_factory (TAO::GUIResource_Factory *gui_resource_factory)
{
  if (TAO_TSS_Resources::instance ()->gui_resource_factory_ != 0)
    {
      if (TAO_debug_level > 2)
        {
          TAOLIB_DEBUG ((LM_DEBUG,
                      "TAO (%P|%t) - Deleting old gui_resource_factory.\n"));
        }
      delete TAO_TSS_Resources::instance ()->gui_resource_factory_;
    }

  TAO_TSS_Resources::instance ()->gui_resource_factory_ = gui_resource_factory;
}

void
TAO_ORB_Core::dynamic_adapter_name (const char *name)
{
  TAO_ORB_Core_Static_Resources::instance ()->dynamic_adapter_name_ = name;
}

const char *
TAO_ORB_Core::dynamic_adapter_name (void)
{
  return TAO_ORB_Core_Static_Resources::instance ()->dynamic_adapter_name_.c_str();
}

void
TAO_ORB_Core::ifr_client_adapter_name (const char *name)
{
  TAO_ORB_Core_Static_Resources::instance ()->ifr_client_adapter_name_ = name;
}

const char *
TAO_ORB_Core::ifr_client_adapter_name (void)
{
  return TAO_ORB_Core_Static_Resources::instance ()->ifr_client_adapter_name_.c_str();
}

void
TAO_ORB_Core::typecodefactory_adapter_name (const char *name)
{
  TAO_ORB_Core_Static_Resources::instance ()->typecodefactory_adapter_name_ = name;
}

const char *
TAO_ORB_Core::typecodefactory_adapter_name (void)
{
  return TAO_ORB_Core_Static_Resources::instance ()->typecodefactory_adapter_name_.c_str();
}

void
TAO_ORB_Core::iorinterceptor_adapter_factory_name (const char *name)
{
  TAO_ORB_Core_Static_Resources::instance ()->iorinterceptor_adapter_factory_name_ = name;
}

const char *
TAO_ORB_Core::iorinterceptor_adapter_factory_name (void)
{
  return TAO_ORB_Core_Static_Resources::instance ()->iorinterceptor_adapter_factory_name_.c_str();
}

void
TAO_ORB_Core::valuetype_adapter_factory_name (const char *name)
{
  TAO_ORB_Core_Static_Resources::instance ()->valuetype_adapter_factory_name_ = name;
}

const char *
TAO_ORB_Core::valuetype_adapter_factory_name (void)
{
  return TAO_ORB_Core_Static_Resources::instance ()->valuetype_adapter_factory_name_.c_str();
}

TAO_Resource_Factory *
TAO_ORB_Core::resource_factory (void)
{
  // Check if there is a cached reference.
  if (this->resource_factory_ != 0)
    {
      return this->resource_factory_;
    }

  // Look in the service repository for an instance.
  ACE_CString &resource_factory_name =
    TAO_ORB_Core_Static_Resources::instance ()->resource_factory_name_;

  this->resource_factory_ =
    ACE_Dynamic_Service<TAO_Resource_Factory>::instance
      (this->configuration (),
       ACE_TEXT_CHAR_TO_TCHAR (resource_factory_name.c_str()));

  return this->resource_factory_;
}

TAO::GUIResource_Factory *
TAO_ORB_Core::gui_resource_factory (void)
{
  return TAO_TSS_Resources::instance ()->gui_resource_factory_;
}


TAO_Thread_Lane_Resources_Manager &
TAO_ORB_Core::thread_lane_resources_manager (void)
{
  // Check if there is a cached reference.
  if (this->thread_lane_resources_manager_ != 0)
    return *this->thread_lane_resources_manager_;

  // If not, lookup the corresponding factory and ask it to make one.
  const char *thread_lane_resources_manager_factory_name =
    this->orb_params ()->thread_lane_resources_manager_factory_name ();

  TAO_Thread_Lane_Resources_Manager_Factory *factory =
    ACE_Dynamic_Service<TAO_Thread_Lane_Resources_Manager_Factory>::instance
      (this->configuration (),
       ACE_TEXT_CHAR_TO_TCHAR (thread_lane_resources_manager_factory_name));

  this->thread_lane_resources_manager_ =
    factory->create_thread_lane_resources_manager (*this);

  return *this->thread_lane_resources_manager_;
}

TAO_Collocation_Resolver &
TAO_ORB_Core::collocation_resolver (void)
{
  // Check if there is a cached reference.
  if (this->collocation_resolver_ != 0)
    return *this->collocation_resolver_;

  // If not, lookup it up.
  this->collocation_resolver_ =
    ACE_Dynamic_Service<TAO_Collocation_Resolver>::instance
      (this->configuration (),
       ACE_TEXT_CHAR_TO_TCHAR (this->orb_params ()->collocation_resolver_name ()));

  return *this->collocation_resolver_;
}

TAO::PolicyFactory_Registry_Adapter *
TAO_ORB_Core::policy_factory_registry_i (void)
{

  TAO_PolicyFactory_Registry_Factory *loader =
    ACE_Dynamic_Service<TAO_PolicyFactory_Registry_Factory>::instance
      (this->configuration (),
       ACE_TEXT ("PolicyFactory_Loader"));

  if (loader == 0)
    {
      this->configuration ()->process_directive (
        ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE("PolicyFactory_Loader",
                                      "TAO_PI",
                                      TAO_VERSION,
                                      "_make_TAO_PolicyFactory_Loader",
                                      ""));
      loader =
        ACE_Dynamic_Service<TAO_PolicyFactory_Registry_Factory>::instance
          (this->configuration (),
           ACE_TEXT ("PolicyFactory_Loader"));
    }

  if (loader != 0)
    {
      this->policy_factory_registry_ =
        loader->create ();
    }

  return this->policy_factory_registry_;
}

TAO_ZIOP_Adapter *
TAO_ORB_Core::ziop_adapter_i (void)
{
  // Check if there is a cached reference.
  if (this->ziop_adapter_ != 0)
    return this->ziop_adapter_;

  this->ziop_adapter_ =
    ACE_Dynamic_Service<TAO_ZIOP_Adapter>::instance
      (this->configuration (),
       ACE_TEXT ("ZIOP_Loader"));

  return this->ziop_adapter_;
}

TAO::ORBInitializer_Registry_Adapter *
TAO_ORB_Core::orbinitializer_registry_i (void)
{
  // @todo The ORBInitializer_Registry is supposed to be a singleton.

  // If not, lookup it up.
  this->orbinitializer_registry_ =
    ACE_Dynamic_Service<TAO::ORBInitializer_Registry_Adapter>::instance
      (this->configuration (),
       ACE_TEXT ("ORBInitializer_Registry"));

#if !defined (TAO_AS_STATIC_LIBS) && !(defined (ACE_VXWORKS) && !defined (__RTP__))
      // In case we build shared, try to load the PI Client library, in a
      // static build we just can't do this, so don't try it, lower layers
      // output an error then.
  if (this->orbinitializer_registry_ == 0)
    {
      this->configuration ()->process_directive (
        ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE ("ORBInitializer_Registry",
                                       "TAO_PI",
                                       TAO_VERSION,
                                       "_make_ORBInitializer_Registry",
                                       ""));
      this->orbinitializer_registry_ =
        ACE_Dynamic_Service<TAO::ORBInitializer_Registry_Adapter>::instance
          (this->configuration (),
           ACE_TEXT ("ORBInitializer_Registry"));
    }
#endif /* !TAO_AS_STATIC_LIBS && !(ACE_VXWORKS && !__RTP__) */

  return this->orbinitializer_registry_;
}

TAO_Stub_Factory *
TAO_ORB_Core::stub_factory (void)
{
  // Check if there is a cached reference.
  if (this->stub_factory_ != 0)
    return this->stub_factory_;

  // If not, look in the service repository for an instance.
  const char *stub_factory_name =
    this->orb_params ()->stub_factory_name ();

  this->stub_factory_ =
    ACE_Dynamic_Service<TAO_Stub_Factory>::instance
      (this->configuration (),
       ACE_TEXT_CHAR_TO_TCHAR (stub_factory_name));

  return this->stub_factory_;
}

TAO_Endpoint_Selector_Factory *
TAO_ORB_Core::endpoint_selector_factory (void)
{
  // Check if there is a cached reference.
  if (this->endpoint_selector_factory_ != 0)
    return this->endpoint_selector_factory_;

  // If not, look in the service repository for an instance.
  const char* endpoint_selector_factory_name =
    this->orb_params ()->endpoint_selector_factory_name ();

  this->endpoint_selector_factory_ =
    ACE_Dynamic_Service<TAO_Endpoint_Selector_Factory>::instance
      (this->configuration (),
       ACE_TEXT_CHAR_TO_TCHAR (endpoint_selector_factory_name));

  return this->endpoint_selector_factory_;
}

void
TAO_ORB_Core::set_network_priority_protocols_hooks (
  const char *network_priority_protocols_hooks_name)
{
  // Is synchronization necessary?
  TAO_ORB_Core_Static_Resources::instance ()->
    network_priority_protocols_hooks_name_ =
      network_priority_protocols_hooks_name;
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

TAO::Invocation_Status
TAO_ORB_Core::service_raise_comm_failure (
    IOP::ServiceContextList &clist,
    TAO_Profile *profile)
{
  if (this->ft_service_.service_callback ())
    {
      return this->ft_service_.service_callback ()->
                 raise_comm_failure (clist, profile);
    }

  throw ::CORBA::COMM_FAILURE (
    CORBA::SystemException::_tao_minor_code (
      TAO_INVOCATION_RECV_REQUEST_MINOR_CODE,
      errno),
    CORBA::COMPLETED_MAYBE);
}


TAO::Invocation_Status
TAO_ORB_Core::service_raise_transient_failure (
    IOP::ServiceContextList &clist,
    TAO_Profile *profile)
{
  if (this->ft_service_.service_callback ())
    {
      return
        this->ft_service_.service_callback ()->raise_transient_failure (clist,
                                 profile);
    }

  return TAO::TAO_INVOKE_FAILURE;
}

TAO_Client_Strategy_Factory *
TAO_ORB_Core::client_factory (void)
{
  if (this->client_factory_ == 0)
    {
      // Look in the service repository for an instance.
      this->client_factory_ =
        ACE_Dynamic_Service<TAO_Client_Strategy_Factory>::instance
          (this->configuration (),
           ACE_TEXT ("Client_Strategy_Factory"));
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
        ACE_Dynamic_Service<TAO_Server_Strategy_Factory>::instance
          (this->configuration (),
           ACE_TEXT ("Server_Strategy_Factory"));
    }

  return this->server_factory_;
}

CORBA::Object_ptr
TAO_ORB_Core::root_poa (void)
{
  // DCL ..
  if (CORBA::is_nil (this->root_poa_.in ()))
    {
      // Making sure the initialization process in the current thread uses
      // the correct service repository (ours), instead of the global one.
      ACE_Service_Config_Guard scg (this->configuration ());

      TAO_Adapter_Factory *factory =
        ACE_Dynamic_Service<TAO_Adapter_Factory>::instance
          (this->configuration (),
          this->orb_params ()->poa_factory_name ());

      if (factory == 0)
        {
          this->configuration()->process_directive (
             this->orb_params ()->poa_factory_directive ());

          factory =
            ACE_Dynamic_Service<TAO_Adapter_Factory>::instance
              (this->configuration (),
              this->orb_params ()->poa_factory_name ());
        }

      if (factory == 0)
        {
          return CORBA::Object::_nil ();
        }

      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        monitor,
                        this->open_lock_,
                        0);

      if (CORBA::is_nil (this->root_poa_.in ()))
        {
          auto_ptr<TAO_Adapter> poa_adapter (factory->create (this));

          poa_adapter->open ();

          // @@ Not exception safe
          this->root_poa_ = poa_adapter->root ();

          this->adapter_registry_.insert (poa_adapter.get ());

          poa_adapter.release ();
        }
    }

  return CORBA::Object::_duplicate (this->root_poa_.in ());
}

TAO_Adapter *
TAO_ORB_Core::poa_adapter (void)
{
  if (this->poa_adapter_ == 0)
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);
      if (this->poa_adapter_ == 0)
        {
          this->poa_adapter_ =
            this->adapter_registry_.find_adapter ("RootPOA");
        }
    }
  return this->poa_adapter_;
}

TAO_Stub *
TAO_ORB_Core::create_stub (const char *repository_id,
                           const TAO_MProfile &profiles)
{
  TAO_Stub *retval =
    this->stub_factory ()->create_stub (repository_id, profiles, this);
  return retval;
}

void
TAO_ORB_Core::request_dispatcher (TAO_Request_Dispatcher *request_dispatcher)
{
  // Assume ownership of the request dispatcher.
  TAO_Request_Dispatcher *tmp = this->request_dispatcher_;
  this->request_dispatcher_ = request_dispatcher;
  delete tmp;
}

TAO_Stub *
TAO_ORB_Core::create_stub_object (TAO_MProfile &mprofile,
                                  const char *type_id,
                                  CORBA::PolicyList *policy_list)
{
  // Add the Polices contained in "policy_list" to each profile so
  // that those policies will be exposed to the client in the IOR.  In
  // particular each CORBA::Policy has to be converted in to
  // Messaging::PolicyValue, and then all the Messaging::PolicyValue
  // should be embedded inside a Messaging::PolicyValueSeq which
  // became in turns the "body" of the IOP::TaggedComponent. This
  // conversion is a responsability of the CORBA::Profile class.  (See
  // orbos\98-05-05.pdf Section 5.4)
  if (policy_list->length () != 0)
    {
      TAO_Profile * profile = 0;

      CORBA::ULong const count = mprofile.profile_count ();
      for (CORBA::ULong i = 0; i < count; ++i)
        {
          // Get the ith profile
          profile = mprofile.get_profile (i);
          profile->policies (policy_list);
        }
    }

  /// Initialize a TAO_Stub object with the mprofile thats passed.
  TAO_Stub *stub = this->create_stub (type_id, mprofile);

  stub->base_profiles ().policy_list (policy_list);

  return stub;
}

void
TAO_ORB_Core::load_policy_validators (TAO_Policy_Validator &validator)
{
  if (this->bidir_adapter_ == 0)
    {
      this->bidir_adapter_ =
        ACE_Dynamic_Service<TAO_BiDir_Adapter>::instance
          (this->configuration (), ACE_TEXT ("BiDirGIOP_Loader"));
    }

  // Call the BiDir library if it has been loaded
  if (this->bidir_adapter_)
    {
      this->bidir_adapter_->load_policy_validators (validator);
    }

  // Call the ZIOP library if it has been loaded
  if (this->ziop_adapter_)
    {
      this->ziop_adapter_->load_policy_validators (validator);
    }
}

CORBA::Object_ptr
TAO_ORB_Core::create_object (TAO_Stub *stub)
{
  // @@ What about forwarding.  With this approach we are never forwarded
  //    when we use collocation!
  const TAO_MProfile &mprofile = stub->base_profiles ();

  // @@ We should thow CORBA::NO_MEMORY in platforms with exceptions,
  // but we are stuck in platforms without exceptions!
  TAO_ORB_Core_Auto_Ptr collocated_orb_core;
  CORBA::Object_ptr x = 0;

  {
    // Lock the ORB_Table against concurrent modification while we
    // iterate through the ORBs.
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      TAO::ORB_Table::instance()->lock (),
                      CORBA::Object::_nil ());

    TAO::ORB_Table * const table = TAO::ORB_Table::instance ();
    TAO::ORB_Table::iterator const end = table->end ();
    for (TAO::ORB_Table::iterator i = table->begin (); i != end; ++i)
      {
        ::TAO_ORB_Core * const other_core = (*i).second.core ();

        if (this->is_collocation_enabled (other_core, mprofile))
          {
            other_core->_incr_refcnt();
             TAO_ORB_Core_Auto_Ptr tmp_auto_ptr (other_core);
             collocated_orb_core = tmp_auto_ptr;
            break;
          }
      }
  }

  if (collocated_orb_core.get ())
    {
      x = collocated_orb_core.get ()->adapter_registry ().create_collocated_object (stub, mprofile);
    }


  if (!x)
    {
      // The constructor sets the proxy broker as the
      // Remote one.
      ACE_NEW_RETURN (x,
                      CORBA::Object (stub, 0),
                      0);
    }

  return x;
}

CORBA::Long
TAO_ORB_Core::initialize_object (TAO_Stub *stub, CORBA::Object_ptr)
{
  // @@ What about forwarding.  With this approach we are never forwarded
  //    when we use collocation!
  const TAO_MProfile &mprofile =
    stub->base_profiles ();

  return initialize_object_i (stub, mprofile);
}

CORBA::Long
TAO_ORB_Core::reinitialize_object (TAO_Stub *stub)
{
  return initialize_object_i (stub, stub->forward_profiles ()
                                    ? *(stub->forward_profiles ())
                                    : stub->base_profiles ());
}

CORBA::Long
TAO_ORB_Core::initialize_object_i (TAO_Stub *stub, const TAO_MProfile &mprofile)

{
  CORBA::Long retval = 0;
  TAO_ORB_Core_Auto_Ptr collocated_orb_core;

  {
    // Lock the ORB_Table against concurrent modification while we
    // iterate through the ORBs.
    ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                              guard,
                              TAO::ORB_Table::instance()->lock (),
                              0));

    TAO::ORB_Table * const table = TAO::ORB_Table::instance ();
    TAO::ORB_Table::iterator const end = table->end ();
    for (TAO::ORB_Table::iterator i = table->begin (); i != end; ++i)
      {
        TAO_ORB_Core * const other_core = (*i).second.core ();

        if (this->is_collocation_enabled (other_core,
                                          mprofile))
          {
            other_core->_incr_refcnt ();
            TAO_ORB_Core_Auto_Ptr tmp_auto_ptr (other_core);
            collocated_orb_core = tmp_auto_ptr;
            break;
          }
      }
  }

  if (collocated_orb_core.get ())
    {
      retval = collocated_orb_core.get ()->adapter_registry ().initialize_collocated_object (stub);
    }
  else
    {
      stub->is_collocated (false);
    }

  return retval;
}

CORBA::Boolean
TAO_ORB_Core::is_collocation_enabled (TAO_ORB_Core *orb_core,
                                      const TAO_MProfile &mp)
{
  TAO_MProfile mp_temp;

  TAO_Profile* profile = 0;
  if (this->service_profile_selection(mp, profile) && profile)
  {
    if (mp_temp.add_profile(profile) == -1)
      {
        return false;
      }
  }

  if (!orb_core->optimize_collocation_objects ())
    return false;

  if (!orb_core->use_global_collocation () && orb_core != this)
    return false;

  if (!orb_core->is_collocated (profile ? mp_temp : mp))
    return false;

  return true;
}

int
TAO_ORB_Core::is_collocated (const TAO_MProfile& mprofile)
{
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

  return this->thread_lane_resources_manager ().is_collocated (mprofile);
}

// ****************************************************************

TAO_Leader_Follower &
TAO_ORB_Core::leader_follower (void)
{
  return this->lane_resources ().leader_follower ();
}

TAO_LF_Strategy &
TAO_ORB_Core::lf_strategy (void)
{
  return this->thread_lane_resources_manager ().lf_strategy ();
}

int
TAO_ORB_Core::run (ACE_Time_Value *tv, int perform_work)
{
  // ORB::run may be called from a thread, different from the one that
  // did the ORB_init, consequently we must establish the Service
  // Gestalt, this thread will consider "global"

  ACE_Service_Config_Guard use_orbs (this->configuration());

  if (TAO_debug_level > 10)
    {
      TAOLIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - ORB_Core::run, ")
                  ACE_TEXT ("start [%s]\n"),
                  perform_work?ACE_TEXT("perform_work"):ACE_TEXT("run")));
    }

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

  while (this->has_shutdown () == false)
    {
      // Every time we perform an interaction we have to become the
      // leader again, because it is possible that a client has
      // acquired the leader role...
      TAO_Leader_Follower &leader_follower = this->leader_follower ();
      TAO_LF_Strategy &lf_strategy = this->lf_strategy ();

      TAO_LF_Event_Loop_Thread_Helper helper (leader_follower, lf_strategy, tv);

      result = helper.event_loop_return ();
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

      if (TAO_debug_level > 10)
        {
          TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - ORB_Core::run, ")
                      ACE_TEXT ( "calling handle_events()\n")));
        }

      result = r->handle_events (tv);

      if (TAO_debug_level > 10)
        {
          TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - ORB_Core::run, ")
                      ACE_TEXT ("handle_events() returns %d\n"),
                      result));
        }

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

  // wait only in the parent thread.
  if (this->has_shutdown () == true &&
      (this->server_factory_->activate_server_connections () ||
       (this->tm_.task() == 0 && this->tm_.count_threads() > 0) ) )
    {
      this->tm_.wait ();
    }

  if (TAO_debug_level > 10)
    {
      TAOLIB_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - ORB_Core::run, ")
                  ACE_TEXT ("ends with result = %d\n"),
                  result));
    }

  return result;
}


void
TAO_ORB_Core::shutdown (CORBA::Boolean wait_for_completion)
{
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, monitor, this->lock_);

    if (this->has_shutdown () == true)
      return;

    // Check if we are on the right state, i.e. do not accept
    // shutdowns with the 'wait_for_completion' flag set in the middle
    // of an upcall (because those deadlock).
    this->adapter_registry_.check_close (wait_for_completion);

    // Set the 'has_shutdown' flag, so any further attempt to shutdown
    // becomes a noop.
    this->has_shutdown_ = true;

    // need to release the mutex, because some of the shutdown
    // operations invoke application code, that could (and in practice
    // does!) callback into ORB Core code.
  }

  this->adapter_registry_.close (wait_for_completion);

  // Shutdown reactor.
  this->thread_lane_resources_manager ().shutdown_reactor ();

  // Cleanup transports
  this->thread_lane_resources_manager ().close_all_transports ();

  // Grab the thread manager
  ACE_Thread_Manager *tm = this->thr_mgr ();

  // Try to cancel all the threads in the ORB.
  tm->cancel_all ();

  // If <wait_for_completion> is set, wait for all threads to exit.
  if (wait_for_completion == true)
    tm->wait ();

  // Explicitly destroy the valuetype adapter
  {
    ACE_GUARD (TAO_SYNCH_MUTEX, monitor, this->lock_);

    delete this->valuetype_adapter_;
    this->valuetype_adapter_ = 0;
  }

  // Explicitly destroy the object reference table since it
  // contains references to objects, which themselves may contain
  // reference to this ORB.
  this->object_ref_table_.destroy ();

  // Release implrepo_service_ if one existed. If everything went
  // fine then this must release reference from implrepo_service_
  // object to this orb core.
  ::CORBA::release (this->implrepo_service_);
  this->implrepo_service_ = CORBA::Object::_nil ();

#if (TAO_HAS_INTERCEPTORS == 1)
  CORBA::release (this->pi_current_);
  this->pi_current_ = CORBA::Object::_nil ();
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
}

void
TAO_ORB_Core::destroy (void)
{
  // All destroy() should do is (a) call shutdown() and (b) unbind()
  // from the ORB table.  Nothing else should really be added to this
  // method.  Everything else should go to the shutdown() method.
  // Remember when the ORB Core is finally removed from the ORB table,
  // the reference count goes to zero and fini() is called.  fini()
  // calls shutdown() and does not call destroy() since destroy() will
  // try to unbind from the ORB table again.  Additional code should
  // not be added to destroy() since there is no guarantee that
  // orb->destroy() will ever be called by the user.  Since TAO
  // guarantees that shutdown() will be called, all cleanup code
  // should go there.
  //

  // Shutdown the ORB and block until the shutdown is complete.
  this->shutdown (true);

  // Invoke Interceptor::destroy() on all registered interceptors.
  this->destroy_interceptors ();

  // Clean TSS resources. This cannot be done in shutdown() since the later
  // can be called during an upcall and once it's done it will remove
  // resources such as PICurrent that are required after the upcall. And this
  // cannot be postponed to TAO_ORB_Core's destructor as fini() needs access
  // to orb core and what is more important orb core can be destroyed too late
  // when some required libraries are already unloaded and we'll get
  // 'pure virtual method called' during cleanup.
  this->get_tss_resources ()->fini ();

  // Now remove it from the ORB table so that it's ORBid may be
  // reused.
  TAO::ORB_Table::instance ()->unbind (this->orbid_);
}

void
TAO_ORB_Core::check_shutdown (void)
{
  if (this->has_shutdown ())
    {
      // As defined by the CORBA 2.3 specification, throw a
      // CORBA::BAD_INV_ORDER exception with minor code 4 if the ORB
      // has shutdown by the time an ORB function is called.

      throw ::CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 4, CORBA::COMPLETED_NO);
    }
}

void
TAO_ORB_Core::destroy_interceptors (void)
{
  try
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, monitor, this->lock_);

#if TAO_HAS_INTERCEPTORS == 1
      if (this->client_request_interceptor_adapter_ != 0)
        {
          this->client_request_interceptor_adapter_->destroy_interceptors ();

          delete this->client_request_interceptor_adapter_;
          this->client_request_interceptor_adapter_ = 0;
        }

      if (this->server_request_interceptor_adapter_ != 0)
        {
          this->server_request_interceptor_adapter_->destroy_interceptors ();

          delete this->server_request_interceptor_adapter_;
          this->server_request_interceptor_adapter_ = 0;
        }

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

      if (this->ior_interceptor_adapter_ != 0)
        {
          this->ior_interceptor_adapter_->destroy_interceptors ();

          this->ior_interceptor_adapter_ = 0;
        }

    }
  catch (...)
    {
      // .. catch all the exceptions..
      if (TAO_debug_level > 3)
        {
          TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Exception in TAO_ORB_Core")
                      ACE_TEXT ("::destroy_interceptors ()\n")));
        }
    }

  return;
}

TAO_Thread_Lane_Resources &
TAO_ORB_Core::lane_resources (void)
{
  return this->thread_lane_resources_manager ().lane_resources ();
}

void
TAO_ORB_Core::resolve_typecodefactory_i (void)
{
  TAO_Object_Loader *loader =
    ACE_Dynamic_Service<TAO_Object_Loader>::instance
      (this->configuration (),
       ACE_TEXT ("TypeCodeFactory_Loader"));

  if (loader == 0)
    {
      this->configuration ()->process_directive
        (ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE("TypeCodeFactory",
                                       "TAO_TypeCodeFactory",
                                       TAO_VERSION,
                                       "_make_TAO_TypeCodeFactory_Loader",
                                       ""));
      loader =
        ACE_Dynamic_Service<TAO_Object_Loader>::instance
        (this->configuration (),
         ACE_TEXT ("TypeCodeFactory_Loader"));

      if (loader == 0)
        {
          TAOLIB_ERROR ((LM_ERROR,
                      ACE_TEXT ("TAO (%P|%t) Unable to instantiate ")
                      ACE_TEXT ("a TypeCodeFactory_Loader\n")));
          throw ::CORBA::ORB::InvalidName ();
        }
    }

  this->typecode_factory_ =
    loader->create_object (this->orb_, 0, 0);
}

void
TAO_ORB_Core::resolve_codecfactory_i (void)
{
  TAO_Object_Loader *loader =
    ACE_Dynamic_Service<TAO_Object_Loader>::instance
      (this->configuration (),
       ACE_TEXT ("CodecFactory_Loader"));

  if (loader == 0)
    {
      this->configuration()->process_directive
        (ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE("CodecFactory",
                                       "TAO_CodecFactory",
                                       TAO_VERSION,
                                       "_make_TAO_CodecFactory_Loader",
                                       ""));
      loader =
        ACE_Dynamic_Service<TAO_Object_Loader>::instance
          (this->configuration (), ACE_TEXT ("CodecFactory_Loader"));
    }

  if (loader != 0)
    {
      this->codec_factory_ =
        loader->create_object (this->orb_, 0, 0);
    }
}

void
TAO_ORB_Core::resolve_compression_manager_i (void)
{
  TAO_Object_Loader *loader =
    ACE_Dynamic_Service<TAO_Object_Loader>::instance
      (this->configuration (),
       ACE_TEXT ("Compression_Loader"));

  if (loader == 0)
    {
      this->configuration()->process_directive
        (ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE("Compression",
                                       "TAO_Compression",
                                       TAO_VERSION,
                                       "_make_TAO_Compression_Loader",
                                       ""));
      loader =
        ACE_Dynamic_Service<TAO_Object_Loader>::instance
          (this->configuration (), ACE_TEXT ("Compression_Loader"));
    }

  if (loader != 0)
    {
      this->compression_manager_ = loader->create_object (this->orb_, 0, 0);
    }
}

void
TAO_ORB_Core::resolve_poa_current_i (void)
{
  TAO_Object_Loader *loader =
    ACE_Dynamic_Service<TAO_Object_Loader>::instance
      (this->configuration(),
       ACE_TEXT ("TAO_POA_Current_Factory"));

  if (loader == 0)
    {
      this->configuration()->process_directive
        (ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE("TAO_POA_Current_Factory",
                                       "TAO_PortableServer",
                                       TAO_VERSION,
                                       "_make_TAO_POA_Current_Factory",
                                       ""));
      loader =
        ACE_Dynamic_Service<TAO_Object_Loader>::instance
          (this->configuration(), ACE_TEXT ("TAO_POA_Current_Factory"));
    }

  if (loader != 0)
    {
      this->poa_current_ = loader->create_object (this->orb_, 0, 0);
    }
}


#if TAO_HAS_INTERCEPTORS == 1

void
TAO_ORB_Core::resolve_picurrent_i (void)
{
  TAO_Object_Loader *loader =
    ACE_Dynamic_Service<TAO_Object_Loader>::instance
      (this->configuration (),
       ACE_TEXT ("PICurrent_Loader"));

  if (loader == 0)
    {
      this->configuration ()->process_directive
        (ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE("PICurrent_Loader",
                                       "TAO_PI",
                                       TAO_VERSION,
                                       "_make_TAO_PICurrent_Loader",
                                       ""));
      loader =
        ACE_Dynamic_Service<TAO_Object_Loader>::instance
          (this->configuration (), ACE_TEXT ("PICurrent_Loader"));
    }

  if (loader != 0)
    {
      CORBA::Object_ptr pi = loader->create_object (this->orb_, 0, 0);

      this->pi_current_ = pi;
    }
}

#endif /* TAO_HAS_INTERCEPTORS == 1 */


void
TAO_ORB_Core::resolve_dynanyfactory_i (void)
{
  TAO_Object_Loader *loader =
    ACE_Dynamic_Service<TAO_Object_Loader>::instance
      (this->configuration (),
       ACE_TEXT ("DynamicAny_Loader"));

  if (loader == 0)
    {
      this->configuration ()->process_directive
        (ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE("DynamicAny_Loader",
                                       "TAO_DynamicAny",
                                       TAO_VERSION,
                                       "_make_TAO_DynamicAny_Loader",
                                       ""));
      loader =
        ACE_Dynamic_Service<TAO_Object_Loader>::instance
        (this->configuration (),
         ACE_TEXT ("DynamicAny_Loader"));
    }

  if (loader != 0)
    {
      this->dynany_factory_ = loader->create_object (this->orb_, 0, 0);
    }
}

void
TAO_ORB_Core::resolve_iormanipulation_i (void)
{
  TAO_Object_Loader *loader =
    ACE_Dynamic_Service<TAO_Object_Loader>::instance
      (this->configuration (),
       ACE_TEXT ("IORManip_Loader"));

  if (loader == 0)
    {
      this->configuration()->process_directive
        (ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE("IORManip_Loader",
                                       "TAO_IORManip",
                                       TAO_VERSION,
                                       "_make_TAO_IORManip_Loader",
                                       ""));
      loader =
        ACE_Dynamic_Service<TAO_Object_Loader>::instance
          (this->configuration (), ACE_TEXT ("IORManip_Loader"));
    }

  if (loader != 0)
    {
      this->ior_manip_factory_ = loader->create_object (this->orb_, 0, 0);
    }
}

void
TAO_ORB_Core::resolve_ior_table_i (void)
{
  TAO_Adapter_Factory *factory = 0;
  ACE_Dynamic_Service<TAO_Adapter_Factory>::instance
    (this->configuration (), ACE_TEXT("TAO_IORTable"));
  if (factory == 0)
    {
      this->configuration ()->process_directive
        (ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE("TAO_IORTable",
                                                 "TAO_IORTable",
                                                 TAO_VERSION,
                                                 "_make_TAO_Table_Adapter_Factory",
                                                 ""));
      factory =
        ACE_Dynamic_Service<TAO_Adapter_Factory>::instance
        (this->configuration (), ACE_TEXT("TAO_IORTable"));
    }

  if (factory != 0)
    {
      ACE_Auto_Ptr <TAO_Adapter> iortable_adapter (factory->create (this));
      iortable_adapter->open ();

      CORBA::Object_var tmp_root = iortable_adapter->root ();

      this->adapter_registry_.insert (iortable_adapter.get ());

      // It is now (exception) safe to release ownership from the auto pointers
      this->ior_table_= tmp_root._retn ();
      iortable_adapter.release ();
    }
}

void
TAO_ORB_Core::resolve_async_ior_table_i (void)
{
  TAO_Adapter_Factory *factory = 0;
  ACE_Dynamic_Service<TAO_Adapter_Factory>::instance
    (this->configuration (), ACE_TEXT("TAO_Async_IORTable"));
  if (factory == 0)
    {
      this->configuration ()->process_directive
        (ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE("TAO_Async_IORTable",
                                                 "TAO_Async_IORTable",
                                                 TAO_VERSION,
                                                 "_make_TAO_Async_Table_Adapter_Factory",
                                                 ""));
      factory =
        ACE_Dynamic_Service<TAO_Adapter_Factory>::instance
        (this->configuration (), ACE_TEXT("TAO_Async_IORTable"));
    }

  if (factory != 0)
    {
      ACE_Auto_Ptr <TAO_Adapter> iortable_adapter (factory->create (this));
      iortable_adapter->open ();

      CORBA::Object_var tmp_root = iortable_adapter->root ();

      this->adapter_registry_.insert (iortable_adapter.get ());

      // It is now (exception) safe to release ownership from the auto pointers
      this->async_ior_table_= tmp_root._retn ();
      iortable_adapter.release ();
    }
}

void
TAO_ORB_Core::resolve_monitor_i (void)
{
  TAO_Object_Loader *loader =
    ACE_Dynamic_Service<TAO_Object_Loader>::instance
      (this->configuration (),
       ACE_TEXT ("Monitor_Init"));

  if (loader == 0)
    {
      this->configuration ()->process_directive
        (ACE_DYNAMIC_VERSIONED_SERVICE_DIRECTIVE("Monitor_Init",
                                       "TAO_Monitor",
                                       TAO_VERSION,
                                       "_make_TAO_Monitor_Init",
                                       ""));
      loader =
        ACE_Dynamic_Service<TAO_Object_Loader>::instance
        (this->configuration (),
         ACE_TEXT ("Monitor_Init"));
    }

  if (loader != 0)
    {
      this->monitor_ = loader->create_object (this->orb_, 0, 0);
    }
}

int
TAO_ORB_Core::set_endpoint_helper (const ACE_CString &lane,
                                   const ACE_CString &endpoints)
{
  if (this->orb_params ()->add_endpoints (lane, endpoints) != 0)
    {
      TAOLIB_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) ")
                  ACE_TEXT ("Invalid endpoint(s) specified: <%C>.\n"),
                  endpoints.c_str ()));
      throw ::CORBA::BAD_PARAM (
        CORBA::SystemException::_tao_minor_code (
          TAO_ORB_CORE_INIT_LOCATION_CODE,
          EINVAL),
        CORBA::COMPLETED_NO);
    }

  return 0;
}

CORBA::Object_ptr
TAO_ORB_Core::resolve_rir (const char *name)
{
  // Get the table of initial references specified through
  // -ORBInitRef.
  ACE_CString ior;
  ACE_CString object_id ((const char *) name);

  // Get the list of initial reference prefixes specified through
  // -ORBDefaultInitRef.
  CORBA::String_var default_init_ref =
    this->orb_params ()->default_init_ref ();

  // Check if a DefaultInitRef was specified.
  if (ACE_OS::strlen (default_init_ref.in ()) != 0)
    {
      static const char corbaloc_prefix[] = "corbaloc:";
      static const char mcast_prefix[] = "mcast:";
      char object_key_delimiter = 0;

      ACE_CString list_of_profiles (default_init_ref.in ());

      // Check if the protocol is corbaloc: or mcast:.
      // If it is, set the object_key_delimiter.
      if ((ACE_OS::strncmp (default_init_ref.in (),
                            corbaloc_prefix,
                            sizeof corbaloc_prefix -1) == 0) ||
          (ACE_OS::strncmp (default_init_ref.in (),
                            mcast_prefix,
                            sizeof mcast_prefix -1) == 0))
        {
          object_key_delimiter = '/';
        }
      else
        {
          TAO_Connector_Registry *conn_reg = this->connector_registry ();

          // Obtain the appropriate object key delimiter for the
          // specified protocol.
          object_key_delimiter =
            conn_reg->object_key_delimiter (list_of_profiles.c_str ());
        }

      // Make sure that the default initial reference doesn't end
      // with the object key delimiter character.
      if (list_of_profiles[list_of_profiles.length() - 1] !=
          object_key_delimiter)
            list_of_profiles += ACE_CString (object_key_delimiter);

      list_of_profiles += object_id;

      return this->orb ()->string_to_object (list_of_profiles.c_str ());
    }

  return CORBA::Object::_nil ();
}

CORBA::ORB::ObjectIdList *
TAO_ORB_Core::list_initial_references (void)
{
  // Unsupported initial services should NOT be included in the below list!
  static const char *initial_services[] = { TAO_LIST_OF_INITIAL_SERVICES };
  // Make sure the "terminating" zero is the last array element so
  // that there is a stop condition when iterating through the list.

  static const size_t initial_services_size =
    sizeof (initial_services) / sizeof (initial_services[0]);

  const size_t total_size =
    initial_services_size
    + this->init_ref_map_.size ()
    + this->object_ref_table_.current_size ();

  CORBA::ORB::ObjectIdList *tmp = 0;

  ACE_NEW_THROW_EX (tmp,
                    CORBA::ORB::ObjectIdList (
                      static_cast<CORBA::ULong> (total_size)),
                    CORBA::NO_MEMORY ());

  CORBA::ORB::ObjectIdList_var list (tmp);
  list->length (static_cast<CORBA::ULong> (total_size));

  CORBA::ULong index = 0;
  // Index for ObjectIdList members.

  // Iterate over the registered initial references.
  for (index = 0; index < initial_services_size; ++index)
    list[index] = initial_services[index];

  // Now iterate over the initial references created by the user and
  // add them to the sequence.

  // References registered via
  // ORBInitInfo::register_initial_reference().
  TAO_Object_Ref_Table::iterator const obj_ref_end =
    this->object_ref_table_.end ();

  for (TAO_Object_Ref_Table::iterator i = this->object_ref_table_.begin ();
       i != obj_ref_end;
       ++i, ++index)
    list[index] = CORBA::string_dup ((*i).first.in ());

  // References registered via INS.
  InitRefMap::iterator const end = this->init_ref_map_.end ();

  for (InitRefMap::iterator j = this-> init_ref_map_.begin ();
       j != end;
       ++j, ++index)
    list[index] = (*j).first.c_str ();

  return list._retn ();
}

// ****************************************************************
ACE_Allocator*
TAO_ORB_Core::input_cdr_dblock_allocator (void)
{
  return this->lane_resources ().input_cdr_dblock_allocator ();
}

ACE_Allocator*
TAO_ORB_Core::input_cdr_buffer_allocator (void)
{
  return this->lane_resources ().input_cdr_buffer_allocator ();
}

ACE_Allocator*
TAO_ORB_Core::input_cdr_msgblock_allocator (void)
{
  return this->lane_resources ().input_cdr_msgblock_allocator ();
}

ACE_Allocator*
TAO_ORB_Core::output_cdr_dblock_allocator (void)
{

  return this->lane_resources ().output_cdr_dblock_allocator ();
}

ACE_Allocator*
TAO_ORB_Core::output_cdr_buffer_allocator (void)
{
  return this->lane_resources ().output_cdr_buffer_allocator ();
}


ACE_Allocator*
TAO_ORB_Core::output_cdr_msgblock_allocator (void)
{
  return this->lane_resources ().output_cdr_msgblock_allocator ();
}


ACE_Allocator *
TAO_ORB_Core::transport_message_buffer_allocator (void)
{
  return this->lane_resources ().transport_message_buffer_allocator ();
}


ACE_Data_Block*
TAO_ORB_Core::create_input_cdr_data_block (size_t size)
{

  ACE_Allocator *dblock_allocator = 0;
  ACE_Allocator *buffer_allocator = 0;

  dblock_allocator =
    this->input_cdr_dblock_allocator ();
  buffer_allocator =
    this->input_cdr_buffer_allocator ();

  ACE_Lock* lock_strategy = 0;
  if (this->resource_factory ()->use_locked_data_blocks ())
    {
      lock_strategy = &this->data_block_lock_;
    }

  return this->create_data_block_i (size,
                                    buffer_allocator,
                                    dblock_allocator,
                                    lock_strategy);
}

ACE_Data_Block *
TAO_ORB_Core::create_data_block_i (size_t size,
                                   ACE_Allocator *buffer_allocator,
                                   ACE_Allocator *dblock_allocator,
                                   ACE_Lock *lock_strategy)
{
  ACE_Data_Block *nb = 0;

  ACE_NEW_MALLOC_RETURN (
                         nb,
                         static_cast<ACE_Data_Block*> (
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

TAO_Connector_Registry *
TAO_ORB_Core::connector_registry (void)
{
  TAO_Connector_Registry *conn =
    this->lane_resources ().connector_registry ();

  return conn;
}

auto_ptr<TAO_GIOP_Fragmentation_Strategy>
TAO_ORB_Core::fragmentation_strategy (TAO_Transport * transport)
{
  return
    this->resource_factory ()->create_fragmentation_strategy (
      transport,
      this->orb_params_.max_message_size ());
}

ACE_Reactor *
TAO_ORB_Core::reactor (void)
{
  return this->leader_follower ().reactor ();
}

CORBA::Object_ptr
TAO_ORB_Core::implrepo_service (void)
{
  if (!this->use_implrepo_)
    return CORBA::Object::_nil ();

  if (CORBA::is_nil (this->implrepo_service_))
    {

      try
        {
          CORBA::Object_var temp =
            this->orb_->resolve_initial_references ("ImplRepoService");

          ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, CORBA::Object::_nil ());

          // @@ Worry about assigning a different IOR? (brunsch)
          this->implrepo_service_ = temp._retn ();
        }
      catch (const ::CORBA::Exception&)
        {
          // Just make sure that we have a null pointer.  Ignore the exception
          // anyway.
          this->implrepo_service_ = CORBA::Object::_nil ();
        }
    }

  return CORBA::Object::_duplicate (this->implrepo_service_);
}

void
TAO_ORB_Core::default_sync_scope_hook (TAO_ORB_Core *,
                                       TAO_Stub *,
                                       bool &has_synchronization,
                                       Messaging::SyncScope &scope)
{
  has_synchronization = true;
  scope = Messaging::SYNC_WITH_TRANSPORT;
}

void
TAO_ORB_Core::call_sync_scope_hook (TAO_Stub *stub,
                                    bool &has_synchronization,
                                    Messaging::SyncScope &scope)
{
  Sync_Scope_Hook sync_scope_hook = this->sync_scope_hook_;

  if (sync_scope_hook == 0)
    {
      has_synchronization = false;
      return;
    }

  (*sync_scope_hook) (this, stub, has_synchronization, scope);
}

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

TAO::Transport_Queueing_Strategy *
TAO_ORB_Core::get_transport_queueing_strategy (TAO_Stub *,
                                               Messaging::SyncScope &scope)
{
  switch (scope)
  {
    case Messaging::SYNC_WITH_TRANSPORT:
    case Messaging::SYNC_WITH_SERVER:
    case Messaging::SYNC_WITH_TARGET:
    {
      return this->flush_transport_queueing_strategy_;
    }
    break;
    case Messaging::SYNC_NONE:
    {
      return this->eager_transport_queueing_strategy_;
    }
    break;
    case TAO::SYNC_DELAYED_BUFFERING:
    {
      return this->delayed_transport_queueing_strategy_;
    }
    break;
    default:
    {
      return 0;
    }
  }
}

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

int
TAO_ORB_Core::add_tss_cleanup_func (ACE_CLEANUP_FUNC cleanup, size_t &slot_id)
{
  return this->tss_cleanup_funcs_.register_cleanup_function (cleanup, slot_id);
}

void
TAO_ORB_Core::call_timeout_hook (TAO_Stub *stub,
                                 bool &has_timeout,
                                 ACE_Time_Value &time_value)
{
  Timeout_Hook timeout_hook = this->timeout_hook_;

  if (timeout_hook == 0)
    {
      has_timeout = false;
      return;
    }
  (*timeout_hook) (this, stub, has_timeout, time_value);
}

void
TAO_ORB_Core::connection_timeout (TAO_Stub *stub,
                                  bool &has_timeout,
                                  ACE_Time_Value &time_value)
{
  Timeout_Hook connection_timeout_hook =
    TAO_ORB_Core_Static_Resources::instance ()->connection_timeout_hook_;

  if (connection_timeout_hook == 0)
    {
      has_timeout = false;
      return;
    }

  (*connection_timeout_hook) (this, stub, has_timeout, time_value);

  Timeout_Hook alt_connection_timeout_hook =
    TAO_ORB_Core_Static_Resources::instance ()->alt_connection_timeout_hook_;

  if (alt_connection_timeout_hook == 0)
    return;

  if (!has_timeout || time_value == ACE_Time_Value::zero )
    {
      (*alt_connection_timeout_hook) (this, stub, has_timeout,time_value);
      return;
    }

  // At this point, both the primary and alternate hooks are defined, and
  // the primary did indeed set a value
  ACE_Time_Value tv1;
  bool ht1;
  (*alt_connection_timeout_hook) (this, stub, ht1,tv1);
  if (ht1 && tv1 > ACE_Time_Value::zero && tv1 < time_value)
    time_value = tv1;
}

void
TAO_ORB_Core::connection_timeout_hook (Timeout_Hook hook)
{
  // Saving the hook pointer so that we can use it later when needed.
  // For now there are only two entry points that may supply a connection
  // timeout hook. But there might be future entry points, so this should
  // probably be addressed by a more sophisticated mechanism.

#define TOCSRi TAO_ORB_Core_Static_Resources::instance ()

  // A consern was raised that since this function is called by two
  // different initializers there may be a race condition that might
  // require a lock. We are not using a lock at this time because of
  // two callers, one happens only during service directive processing
  // and the other only during ORB Initialization time. The former
  // happens when the OC_Endpoint_Selector_Factory is loaded, the
  // latter is part of the messaging library. The messaging library
  // calls this function as part of pre_init processing, and this call
  // happes for every ORB instance. This was the case before these The
  // latter call occurs when the messaging library is loaded. The
  // redundant calls occured then as well. Second, it isn't clear how
  // a lock in this static method would react in the face of windows
  // dlls, shared memory segments, etc. Therefore we are continuing to
  // keep this code lockless as it always was, assuming no
  // simultanious overwrite will occur.

  if (TOCSRi->connection_timeout_hook_ == 0)
    {
      if (TAO_debug_level > 2)
        {
          TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - Setting primary connection ")
                      ACE_TEXT("timeout hook\n")));
        }
      TOCSRi->connection_timeout_hook_ = hook;
    }
  else if (TOCSRi->connection_timeout_hook_ != hook &&
           TOCSRi->alt_connection_timeout_hook_ == 0)
    {
      if (TAO_debug_level > 2)
        {
          TAOLIB_DEBUG ((LM_DEBUG,
                      ACE_TEXT("TAO (%P|%t) - Setting alternate connection ")
                      ACE_TEXT("timeout hook\n")));
        }
      TOCSRi->alt_connection_timeout_hook_ = hook;
    }
  else
    if (TAO_debug_level > 2)
      {
        TAOLIB_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("TAO (%P|%t) - Not overwriting alternate ")
                    ACE_TEXT ("connection timeout hook. It is %@\n"),
                    TOCSRi->alt_connection_timeout_hook_));
      }

#undef TOCSRi
}

#if (TAO_HAS_CORBA_MESSAGING == 1)

CORBA::Policy_ptr
TAO_ORB_Core::get_policy (CORBA::PolicyType type)
{
  CORBA::Policy_var result;

  TAO_Policy_Manager *policy_manager = this->policy_manager ();
  if (policy_manager != 0)
    {
      result = policy_manager->get_policy (type);
    }

  if (CORBA::is_nil (result.in ()))
    {
      result = this->get_default_policies ()->get_policy (type);
    }

  return result._retn ();
}

CORBA::Policy_ptr
TAO_ORB_Core::get_policy_including_current (CORBA::PolicyType type)
{
  TAO_Policy_Current &policy_current = this->policy_current ();

  CORBA::Policy_var result = policy_current.get_policy (type);

  if (CORBA::is_nil (result.in ()))
    {
      result = this->get_policy (type);
    }

  return result._retn ();
}

CORBA::Policy_ptr
TAO_ORB_Core::get_cached_policy (TAO_Cached_Policy_Type type)
{
  CORBA::Policy_var result;

  TAO_Policy_Manager *policy_manager = this->policy_manager ();
  if (policy_manager != 0)
    {
      result = policy_manager->get_cached_policy (type);
    }

  if (CORBA::is_nil (result.in ()))
    {
      result = this->get_default_policies ()->get_cached_policy (type);
    }

  return result._retn ();
}

CORBA::Policy_ptr
TAO_ORB_Core::get_cached_policy_including_current (TAO_Cached_Policy_Type type)
{
  TAO_Policy_Current &policy_current = this->policy_current ();

  CORBA::Policy_var result = policy_current.get_cached_policy (type);

  if (CORBA::is_nil (result.in ()))
    {
      result = this->get_cached_policy (type);
    }

  return result._retn ();
}

#endif /* (TAO_HAS_CORBA_MESSAGING == 1) */

CORBA::Environment *
TAO_ORB_Core::default_environment (void) const
{
  return TAO_TSS_Resources::instance ()->default_environment_;
}

void
TAO_ORB_Core::default_environment (CORBA::Environment *env)
{
  TAO_TSS_Resources::instance ()->default_environment_ = env;
}

void
TAO_ORB_Core::add_interceptor (
   PortableInterceptor::IORInterceptor_ptr interceptor)
{
  if (this->ior_interceptor_adapter ())
    {
      this->ior_interceptor_adapter_->add_interceptor (interceptor);
    }
  else
    {
      TAOLIB_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - %p\n"),
                  ACE_TEXT ("ERROR: ORB Core unable to find the ")
                  ACE_TEXT ("IORInterceptor Adapter Factory instance")));

      throw ::CORBA::INTERNAL ();
    }
}

TAO_IORInterceptor_Adapter *
TAO_ORB_Core::ior_interceptor_adapter (void)
{
  if (this->ior_interceptor_adapter_ == 0)
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        ace_mon,
                        this->lock_,
                        0);

      if (this->ior_interceptor_adapter_ == 0)
        {
          try
            {
              TAO_IORInterceptor_Adapter_Factory * ior_ap_factory =
                ACE_Dynamic_Service<TAO_IORInterceptor_Adapter_Factory>::instance
                  (this->configuration (),
                   ACE_TEXT_CHAR_TO_TCHAR (TAO_ORB_Core::iorinterceptor_adapter_factory_name ()));

              if (ior_ap_factory)
                {
                  this->ior_interceptor_adapter_ = ior_ap_factory->create ();
                }
            }
          catch (const ::CORBA::Exception& ex)
            {
              ex._tao_print_exception (
                "Cannot initialize the ior_interceptor_adapter\n");
            }
        }
    }

  return this->ior_interceptor_adapter_;
}

#if TAO_HAS_INTERCEPTORS == 1

void
TAO_ORB_Core::add_interceptor (
   PortableInterceptor::ClientRequestInterceptor_ptr interceptor
   )
{
  if (this->clientrequestinterceptor_adapter_i ())
    {
      this->client_request_interceptor_adapter_->add_interceptor (interceptor);
    }
  else
    {
      TAOLIB_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - %p\n"),
                  ACE_TEXT ("ERROR: ORB Core unable to find the ")
                  ACE_TEXT ("Client Request Interceptor Adapter Factory ")
                  ACE_TEXT ("instance")));

      throw ::CORBA::INTERNAL ();
    }
}

TAO::ClientRequestInterceptor_Adapter *
TAO_ORB_Core::clientrequestinterceptor_adapter_i (void)
{
  if (this->client_request_interceptor_adapter_ == 0)
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        ace_mon,
                        this->lock_,
                        0);

      if (this->client_request_interceptor_adapter_ == 0)
        {
          TAO_ClientRequestInterceptor_Adapter_Factory *factory =
            ACE_Dynamic_Service<TAO_ClientRequestInterceptor_Adapter_Factory>::instance
              (this->configuration (),
               ACE_TEXT ("ClientRequestInterceptor_Adapter_Factory"));

          if (factory)
            {
              this->client_request_interceptor_adapter_ =
                factory->create ();
            }
        }
    }

  return this->client_request_interceptor_adapter_;
}

void
TAO_ORB_Core::add_interceptor (
   PortableInterceptor::ServerRequestInterceptor_ptr interceptor
   )
{
  if (this->serverrequestinterceptor_adapter_i ())
    {
      this->server_request_interceptor_adapter_->add_interceptor (interceptor);
    }
  else
    {
      TAOLIB_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - %p\n"),
                  ACE_TEXT ("ERROR: ORB Core unable to find the ")
                  ACE_TEXT ("Server Request Interceptor Adapter Factory ")
                  ACE_TEXT ("instance")));

      throw ::CORBA::INTERNAL ();
    }
}

void
TAO_ORB_Core::add_interceptor (
   PortableInterceptor::ClientRequestInterceptor_ptr interceptor,
   const CORBA::PolicyList& policies)
{
  if (this->clientrequestinterceptor_adapter_i ())
    {
      this->client_request_interceptor_adapter_->add_interceptor (
        interceptor,
        policies);

    }
  else
    {
      TAOLIB_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - %p\n"),
                  ACE_TEXT ("ERROR: ORB Core unable to find the ")
                  ACE_TEXT ("Client Request Interceptor Adapter Factory ")
                  ACE_TEXT ("instance")));

      throw ::CORBA::INTERNAL ();
    }
}

void
TAO_ORB_Core::add_interceptor (
   PortableInterceptor::ServerRequestInterceptor_ptr interceptor,
   const CORBA::PolicyList& policies)
{
  if (this->serverrequestinterceptor_adapter_i ())
    {
      this->server_request_interceptor_adapter_->add_interceptor (
        interceptor,
        policies);

    }
  else
    {
      TAOLIB_ERROR ((LM_ERROR,
                  ACE_TEXT ("TAO (%P|%t) - %p\n"),
                  ACE_TEXT ("ERROR: ORB Core unable to find the ")
                  ACE_TEXT ("Server Request Interceptor Adapter Factory ")
                  ACE_TEXT ("instance")));

      throw ::CORBA::INTERNAL ();
    }
}

TAO::ServerRequestInterceptor_Adapter *
TAO_ORB_Core::serverrequestinterceptor_adapter_i (void)
{
  if (this->server_request_interceptor_adapter_ == 0)
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        ace_mon,
                        this->lock_,
                        0);

      if (this->server_request_interceptor_adapter_ == 0)
        {
          TAO_ServerRequestInterceptor_Adapter_Factory *factory =
            ACE_Dynamic_Service<TAO_ServerRequestInterceptor_Adapter_Factory>::instance
              (this->configuration (),
               ACE_TEXT ("ServerRequestInterceptor_Adapter_Factory"));

          if (factory)
            {
              this->server_request_interceptor_adapter_ =
                factory->create ();
            }
        }
    }

  return this->server_request_interceptor_adapter_;
}

#endif  /* TAO_HAS_INTERCEPTORS == 1  */

TAO_Valuetype_Adapter *
TAO_ORB_Core::valuetype_adapter (void)
{
  if (this->valuetype_adapter_ == 0)
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        ace_mon,
                        this->lock_,
                        0);

      if (this->valuetype_adapter_ == 0)
        {
          try
            {
              TAO_Valuetype_Adapter_Factory * vt_ap_factory =
                ACE_Dynamic_Service<TAO_Valuetype_Adapter_Factory>::instance (
                    TAO_ORB_Core::valuetype_adapter_factory_name ()
                  );

              if (vt_ap_factory)
                {
                  this->valuetype_adapter_ = vt_ap_factory->create ();
                }
            }
          catch (const ::CORBA::Exception& ex)
            {
              ex._tao_print_exception (
                "Cannot initialize the valuetype_adapter\n");
            }
        }

      if (this->valuetype_adapter_ == 0)
        {
           throw ::CORBA::INTERNAL ();
        }
    }

  return this->valuetype_adapter_;
}

// *************************************************************
// Valuetype factory operations
// *************************************************************

#if !defined(CORBA_E_MICRO)
CORBA::ValueFactory
TAO_ORB_Core::register_value_factory (const char *repository_id,
                                      CORBA::ValueFactory factory)
{
  if (this->valuetype_adapter ())
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

      if (this->valuetype_adapter_ == 0)
        {
          return 0;
        }

      int const result =
        this->valuetype_adapter_->vf_map_rebind (repository_id, factory);

      if (result == 0) // No previous factory found
        {
          return 0;
        }

      if (result == -1)
        {
          // Error on bind.
          throw ::CORBA::MARSHAL ();
        }
    }

  return factory;    // previous factory was found
}
#endif

#if !defined(CORBA_E_MICRO)
void
TAO_ORB_Core::unregister_value_factory (const char *repository_id)
{
  if (this->valuetype_adapter ())
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

      if (this->valuetype_adapter_ == 0)
        {
          return;
        }

      // Dont care whther it was successful or not!
      (void) this->valuetype_adapter_->vf_map_unbind (repository_id);
    }
}
#endif

#if !defined(CORBA_E_MICRO)
CORBA::ValueFactory
TAO_ORB_Core::lookup_value_factory (const char *repository_id)
{
  if (this->valuetype_adapter ())
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, 0);

      if (this->valuetype_adapter_ == 0)
        {
          return 0;
        }

      return this->valuetype_adapter_->vf_map_find (repository_id);
    }

  return 0;
}
#endif

// ****************************************************************

TAO_Export TAO_ORB_Core *
TAO_ORB_Core_instance (void)
{
  // @@ This is a slight violation of layering, we should use
  //    TAO_ORB_Core_instance(), but that breaks during startup.
  TAO::ORB_Table * const orb_table = TAO::ORB_Table::instance ();
  if (orb_table->first_orb () == 0)
    {
      ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_RECURSIVE_MUTEX, guard,
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

          try
            {
              int argc = 0;
              ACE_TCHAR **const argv= 0;
              CORBA::ORB_var orb =
                CORBA::ORB_init (argc, argv);
            }
          catch (const ::CORBA::Exception&)
            {
              // @@ What should we do here?
            }
        }
    }

  return orb_table->first_orb ();
}

TAO_END_VERSIONED_NAMESPACE_DECL
