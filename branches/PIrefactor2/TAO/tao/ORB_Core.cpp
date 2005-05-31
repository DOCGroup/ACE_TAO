// $Id$

#include "ORB_Core.h"
#include "ORB_Table.h"
#include "TAO_Internal.h"
#include "default_server.h"
#include "Profile.h"
#include "Stub.h"
#include "LF_Follower.h"
#include "Leader_Follower.h"
#include "LF_Event_Loop_Thread_Helper.h"
#include "Connector_Registry.h"
#include "Sync_Strategies.h"
#include "Object_Loader.h"
#include "ObjectIdListC.h"
#include "BiDir_Adapter.h"
#include "Collocation_Resolver.h"
#include "Flushing_Strategy.h"
#include "Request_Dispatcher.h"
#include "Stub_Factory.h"
#include "Thread_Lane_Resources.h"
#include "Thread_Lane_Resources_Manager.h"
#include "TSS_Resources.h"
#include "Protocols_Hooks.h"
#include "IORInterceptor_Adapter.h"
#include "IORInterceptor_Adapter_Factory.h"
#include "debug.h"
#include "TAOC.h"
#include "Endpoint_Selector_Factory.h"
#include "Client_Strategy_Factory.h"
#include "Adapter_Factory.h"
#include "Adapter.h"
#include "GUIResource_Factory.h"
#include "PolicyFactory_Registry_Adapter.h"

#if (TAO_HAS_INTERCEPTORS == 1)
# include "ClientRequestInfo.h"
#endif  /* TAO_HAS_INTERCEPTORS == 1  */

#include "ace/Reactor.h"
#include "ace/Dynamic_Service.h"
#include "ace/Arg_Shifter.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/Static_Object_Lock.h"


#if !defined (ACE_LACKS_IOSTREAM_TOTALLY)
// Needed to set ACE_LOG_MSG::msg_ostream()
// FUZZ: disable check_for_streams_include
#  include "ace/streams.h"
#endif /* !ACE_LACKS_IOSTREAM_TOTALLY */

#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_string.h"

#if !defined (__ACE_INLINE__)
# include "ORB_Core.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID (tao,
           ORB_Core,
           "$Id$")

// ****************************************************************

CORBA::Environment&
TAO_default_environment (void)
{
  return *TAO_TSS_RESOURCES::instance ()->default_environment_;
}

// ****************************************************************

// Initialize instance_ to 0, since this is what we test for in the call
// to instance ().  Note that this does not require a constructor call, so
// it is always initialized by the time that instance () can be called.
TAO_ORB_Core_Static_Resources* TAO_ORB_Core_Static_Resources::instance_ = 0;

// Force an instance to be created at module initialization time,
// since we do not want to worry about double checked locking and
// the race condition to initialize the lock.
TAO_ORB_Core_Static_Resources* TAO_ORB_Core_Static_Resources::initialization_reference_ =
  TAO_ORB_Core_Static_Resources::instance ();

TAO_ORB_Core_Static_Resources*
TAO_ORB_Core_Static_Resources::instance (void)
{
  if (TAO_ORB_Core_Static_Resources::instance_ == 0)
    {
      // This new is never freed on purpose.  The data specified by
      // it needs to be around for the last shared library that references
      // this class.  This could occur in a destructor in a shared library
      // that is unloaded after this one.  One solution to avoid this
      // harmless memory leak would be to use reference counting.
      ACE_NEW_RETURN (TAO_ORB_Core_Static_Resources::instance_,
                      TAO_ORB_Core_Static_Resources (),
                      0);
    }

  return TAO_ORB_Core_Static_Resources::instance_;
}

TAO_ORB_Core_Static_Resources::TAO_ORB_Core_Static_Resources (void)
  : sync_scope_hook_ (0),
    protocols_hooks_name_ ("Protocols_Hooks"),
    timeout_hook_ (0),
    connection_timeout_hook_ (0),
    endpoint_selector_factory_name_ ("Default_Endpoint_Selector_Factory"),
    thread_lane_resources_manager_factory_name_ ("Default_Thread_Lane_Resources_Manager_Factory"),
    collocation_resolver_name_ ("Default_Collocation_Resolver"),
    stub_factory_name_ ("Default_Stub_Factory"),
    resource_factory_name_ ("Resource_Factory"),
    dynamic_adapter_name_ ("Dynamic_Adapter"),
    ifr_client_adapter_name_ ("IFR_Client_Adapter"),
    typecodefactory_adapter_name_ ("TypeCodeFactory_Adapter"),
    iorinterceptor_adapter_factory_name_ ("IORInterceptor_Adapter_Factory"),
    valuetype_adapter_name_ ("Valuetype_Adapter"),
    poa_factory_name_ ("TAO_Object_Adapter_Factory"),
    poa_factory_directive_ ("dynamic TAO_Object_Adapter_Factory Service_Object * TAO_PortableServer:_make_TAO_Object_Adapter_Factory()")
{
}

// ****************************************************************

TAO_ORB_Core::TAO_ORB_Core (const char *orbid)
  : protocols_hooks_ (0),
    lock_ (),
    thread_lane_resources_manager_ (0),
    collocation_resolver_ (0),
    stub_factory_ (0),
    protocol_factories_ (0),
    implrepo_service_ (CORBA::Object::_nil ()),
    use_implrepo_ (0),
    imr_endpoints_in_ior_ (1),
    typecode_factory_ (CORBA::Object::_nil ()),
    dynany_factory_ (CORBA::Object::_nil ()),
    ior_manip_factory_ (CORBA::Object::_nil ()),
    ior_table_ (CORBA::Object::_nil ()),
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
    opt_for_collocation_ (1),
    use_global_collocation_ (1),
    collocation_strategy_ (THRU_POA),

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
    has_shutdown_ (1),  // Start the ORB in a  "shutdown" state.  Only
                        // after CORBA::ORB_init() is called will the
                        // ORB no longer be shutdown.  This does not
                        // mean that the ORB can be reinitialized.  It
                        // can only be initialized once.
    thread_per_connection_use_timeout_ (1),
    open_lock_ (),
    endpoint_selector_factory_ (0),
#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)
    eager_buffering_sync_strategy_ (0),
    delayed_buffering_sync_strategy_ (0),
#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */
    transport_sync_strategy_ (0),
    refcount_ (1),
    policy_factory_registry_ (0),
#if (TAO_HAS_INTERCEPTORS == 1)
    pi_current_ (0),
    client_request_interceptors_ (),
    server_request_interceptors_ (),
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
    ior_interceptor_adapter_ (0),
    valuetype_adapter_ (0),
    parser_registry_ (),
    bidir_adapter_ (0),
    bidir_giop_policy_ (0),
    flushing_strategy_ (0),
    codeset_manager_ (0)
{
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
           TAO_Policy_Set (TAO_POLICY_ORB_SCOPE));

  ACE_NEW (this->policy_current_,
           TAO_Policy_Current);

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  ACE_NEW (this->transport_sync_strategy_,
           TAO_Transport_Sync_Strategy);

  // Initialize the default request dispatcher.
  ACE_NEW (this->request_dispatcher_,
           TAO_Request_Dispatcher);
}

TAO_ORB_Core::~TAO_ORB_Core (void)
{
  delete this->thread_lane_resources_manager_;

  delete this->flushing_strategy_;

  ACE_OS::free (this->orbid_);

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  delete this->eager_buffering_sync_strategy_;
  delete this->delayed_buffering_sync_strategy_;

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if (TAO_HAS_CORBA_MESSAGING == 1)

  CORBA::release (this->policy_manager_);
  delete this->default_policies_;
  CORBA::release (this->policy_current_);

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  delete this->transport_sync_strategy_;

  delete this->request_dispatcher_;

  CORBA::release (this->orb_);
}

int
TAO_ORB_Core::init (int &argc, char *argv[] ACE_ENV_ARG_DECL)
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
  long rcv_sock_size = -1;
  long snd_sock_size = -1;

  // Use TCP_NODELAY.
  int nodelay = 1;

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

  int linger = -1;

  // Copy command line parameter not to use original.
  ACE_Argv_Type_Converter command_line (argc, argv);

  ACE_Arg_Shifter arg_shifter (command_line.get_argc (),
                               command_line.get_TCHAR_argv ());

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    monitor,
                    this->lock_,
                    -1);

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
      if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBDottedDecimalAddresses"))))
        {
          // Use dotted decimal addresses
          // @@ this should be renamed.  See above comment. fredk
          dotted_decimal_addresses =
            ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBNameServicePort"))))
        {
          // Specify the port number for the NameService.
          // Unrelated to ORB Protocols, this is used for multicast.

          ns_port = (CORBA::UShort) ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBMulticastDiscoveryEndpoint"))))
        {
          // Specify mcast address:port@network_interface for the
          // Naming Service Multicast Discovery Protocol.
          // If there is no colon, its only the port no.
          // If there is a '@' also, it means that the network
          // interface name is specified.
          this->orb_params ()->mcast_discovery_endpoint (ACE_TEXT_ALWAYS_CHAR(current_arg));

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBNodelay"))))
        {
          // Use TCP_NODELAY or not.
          nodelay =
            ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBTradingServicePort"))))
        {
          // Specify the port number for the NameService.

          ts_port = (CORBA::UShort) ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBImplRepoServicePort"))))
        {
          // Specify the multicast port number for the Implementation
          // Repository.
          ir_port = (CORBA::UShort) ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBRcvSock"))))
        {
          // @@ All protocol implementation may not use sockets, so
          //    this can either be a generic I/O Buffer size or
          //    Buffer info can be a per protocol specification, fredk

          // Specify the size of the socket's receive buffer

          rcv_sock_size = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBSndSock"))))
        {
          // @@ All protocol implementation may not use sockets, so
          //    this can either be a generic I/O Buffer size or
          //    Buffer info can be a per protocol specification, fredk

          // Specify the size of the socket's send buffer
          snd_sock_size = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
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
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBCollocationStrategy"))))
        {
          // Specify which collocation policy we want to use.
          const ACE_TCHAR *opt = current_arg;
          if (ACE_OS::strcasecmp (opt, ACE_TEXT("thru_poa")) == 0)
            this->collocation_strategy_ = THRU_POA;
          else if (ACE_OS::strcasecmp (opt, ACE_TEXT("direct")) == 0)
            this->collocation_strategy_ = DIRECT;

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBCollocation"))))
        {
          // Specify whether we want to optimize against collocation
          // objects.  Valid arguments are: "yes" and "no".
          // Default is yes.

          int yes_implies_global = 0;

          const ACE_TCHAR *opt = current_arg;
          if (ACE_OS::strcasecmp (opt, ACE_TEXT("YES")) == 0)
            {
              yes_implies_global = 1;
              ACE_DEBUG ((LM_WARNING,
                          ACE_TEXT ("WARNING: using '-ORBCollocation YES' is obsolete ")
                          ACE_TEXT ("and implies '-ORBCollocation global'")
                          ACE_TEXT ("  Please use '-ORBCollocation global' instead.\n")));
            }

          if (yes_implies_global ||
              ACE_OS::strcasecmp (opt, ACE_TEXT("global")) == 0)
            {
              this->opt_for_collocation_ = 1;
              this->use_global_collocation_ = 1;
            }
          else if (ACE_OS::strcasecmp (opt, ACE_TEXT("NO")) == 0)
            {
              this->opt_for_collocation_ = 0;
              this->use_global_collocation_ = 0;
            }
          else if (ACE_OS::strcasecmp (opt, ACE_TEXT("per-orb")) == 0)
            {
              this->opt_for_collocation_ = 1;
              this->use_global_collocation_ = 0;
            }
          else
            {
              ACE_DEBUG ((LM_WARNING,
                          ACE_TEXT ("WARNING: Unknown option to ")
                          ACE_TEXT ("'-ORBCollocation': %s\n"), opt));
            }

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBGlobalCollocation"))))
        // Specify whether we want to use collocation across ORBs;
        // i.e. all the ORBs in the same address space use collocated
        // calls.
        {
          ACE_DEBUG ((LM_WARNING,
                      ACE_TEXT ("WARNING: -ORBGlobalCollocation option is obsolete.")
                      ACE_TEXT ("  Please use '-ORBCollocation global/per-orb/no'")
                      ACE_TEXT ("instead.\n")));

          const ACE_TCHAR *opt = current_arg;
          if (ACE_OS::strcasecmp (opt, ACE_TEXT("YES")) == 0)
            this->use_global_collocation_ = 1;
          else if (ACE_OS::strcasecmp (opt, ACE_TEXT("NO")) == 0)
            this->use_global_collocation_ = 0;

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBPreferredInterfaces"))))
        {
          if (this->orb_params ()->preferred_interfaces (
                ACE_TEXT_ALWAYS_CHAR (current_arg)) == false)
            ACE_THROW_RETURN (CORBA::INTERNAL (
                                  CORBA::SystemException::_tao_minor_code (
                                    TAO_ORB_CORE_INIT_LOCATION_CODE,
                                    0),
                                  CORBA::COMPLETED_NO),
                                -1);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBEnforcePreferredInterfaces"))))
        {
          if (ACE_OS::strcasecmp (current_arg,
                                  ACE_TEXT("YES")) == 0)
            this->orb_params ()->enforce_pref_interfaces (true);
          else if (ACE_OS::strcasecmp (current_arg,
                                       ACE_TEXT("NO")) == 0)
            this->orb_params ()->enforce_pref_interfaces (false);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBCDRTradeoff"))))
        {
          cdr_tradeoff = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }

      // A new <ObjectID>:<IOR> mapping has been specified. This will be
      // used by the resolve_initial_references ().

      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBInitRef"))))
        {
          const ACE_TCHAR *pos = ACE_OS::strchr (current_arg, '=');
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
          ACE_CString object_id (ACE_TEXT_ALWAYS_CHAR(current_arg),
                                 pos - current_arg);
          ACE_CString IOR (ACE_TEXT_ALWAYS_CHAR(pos + 1));
          if (this->init_ref_map_.bind (object_id, IOR) != 0)
            {
              ACE_ERROR ((LM_ERROR,
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
                (ACE_TEXT("-ORBDefaultInitRef"))))
        {
          // Set the list of prefixes from -ORBDefaultInitRef.
          this->orb_params ()->default_init_ref (ACE_TEXT_ALWAYS_CHAR(current_arg));

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBStdProfileComponents"))))
        {
          std_profile_components =
            ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBResources"))))
        {
          ACE_DEBUG ((LM_WARNING,
                      ACE_TEXT ("\"-ORBResources\" has been ")
                      ACE_TEXT ("deprecated.\n")));

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBLogFile"))))
        {
          // redirect all ACE_DEBUG and ACE_ERROR output to a file
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
          ACE_CHECK_RETURN (-1);

          output_stream->open (ACE_TEXT_ALWAYS_CHAR (file_name), ios::out | ios::app);

          if (!output_stream->bad ())
            {
              ACE_LOG_MSG->msg_ostream (output_stream, 1);
            }

#endif /* ACE_LACKS_IOSTREAM_TOTALLY */

          ACE_LOG_MSG->clr_flags (ACE_Log_Msg::STDERR | ACE_Log_Msg::LOGGER);
          ACE_LOG_MSG->set_flags (ACE_Log_Msg::OSTREAM);

        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBVerboseLogging"))))
        {
          unsigned long verbose_logging = ACE_OS::atoi (current_arg);

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
                (ACE_TEXT("-ORBUseIMR"))))
        {
          // Use IR or not.
          this->use_implrepo_ = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_LIB_TEXT("-ORBIMREndpointsInIOR"))))
        {
          this->imr_endpoints_in_ior_ = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
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
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBServerId"))))
        {
          // The this->server_id_ is to uniquely identify a server to
          // an IMR.
          // Fill in later.
          this->server_id_.set(ACE_TEXT_ALWAYS_CHAR(current_arg));

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
               (ACE_LIB_TEXT("-ORBLingerTimeout"))))
        {
          linger = ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
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
                                     ACE_TEXT_ALWAYS_CHAR (current_arg)
                                     ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBListenEndpoints"))))
        {
          // This option is similar to the -ORBEndPoint option. May be
          // ORBEndpoint option will be deprecated in future. But, for
          // now, I (Priyanka) am leaving so that both options can be
          // used.

          this->set_endpoint_helper (TAO_DEFAULT_LANE,
                                     ACE_TEXT_ALWAYS_CHAR (current_arg)
                                     ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBLaneEndpoint"))) ||
               (current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBLaneListenEndpoints"))))
        {
          // This option is similar to the -ORBEndPoint option but
          // specifies endpoints for each lane.

          if (arg_shifter.is_option_next ())
            return -1;

          ACE_CString lane (ACE_TEXT_ALWAYS_CHAR (current_arg));
          arg_shifter.consume_arg ();

          if(arg_shifter.is_option_next ())
            return -1;

          ACE_CString endpoints (ACE_TEXT_ALWAYS_CHAR (arg_shifter.get_current ()));
          arg_shifter.consume_arg ();

          this->set_endpoint_helper (lane,
                                     endpoints
                                     ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBNoProprietaryActivation"))))
        {
          // This option can be used to set to not use any proprietary
          // activation framework. The only TAO proprietary activation
          // framework is IMR. So, by setting this option in TAO, the
          // IMR shouldnt be used .. even if the ORBUseIMR option is
          // set.
          // Fill in later
          // @@ To do later: Priyanka.

          ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), -1);
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBSingleReadOptimization"))))
        {
          this->orb_params ()->single_read_optimization
            (ACE_OS::atoi (current_arg));

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter
                (ACE_TEXT("-ORBDisableRTCollocation"))))
        {
          const ACE_TCHAR *popt = current_arg;
          if (ACE_OS::strcasecmp (popt, ACE_TEXT("NO")) == 0)
            {
              this->orb_params ()->disable_rt_collocation_resolver (false);
            }
          else if (ACE_OS::strcasecmp (popt, ACE_TEXT("YES")) == 0)
            {
              this->orb_params ()->disable_rt_collocation_resolver (true);
            }
          else
            {
              // Should we print an error mesg?? Probably we
              // should. We will look into this after 1.4 and make all
              // the options consistent.
            }

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
              ACE_DEBUG ((LM_ERROR,
                          ACE_TEXT ("ERROR: Unknown \"-ORB\" option ")
                          ACE_TEXT ("<%s>.\n"),
                          ((current_arg == 0) ? ACE_TEXT("<NULL>") : current_arg)));
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

  const char *env_endpoint =
    ACE_OS::getenv ("TAO_ORBENDPOINT");

  if (env_endpoint != 0)
    {
      int result =
        this->orb_params ()->add_endpoints (TAO_DEFAULT_LANE,
                                            env_endpoint);

      if (result != 0)
        {
          if (TAO_debug_level > 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("ERROR: Environment variable TAO_ORBENDPOINT set to invalid value ")
                          ACE_TEXT ("<%s>.\n"),
                          env_endpoint));
            }

          ACE_THROW_RETURN (CORBA::BAD_PARAM (
                              CORBA::SystemException::_tao_minor_code (
                                TAO_ORB_CORE_INIT_LOCATION_CODE,
                                EINVAL),
                              CORBA::COMPLETED_NO),
                            -1);
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
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("ORB Core unable to find a ")
                  ACE_TEXT ("Resource Factory instance")));
      ACE_THROW_RETURN (CORBA::INTERNAL (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_ORB_CORE_INIT_LOCATION_CODE,
                            0),
                          CORBA::COMPLETED_NO),
                        -1);
    }

  // @@Phil: Could we add a -ORB option to prevent creation of codeset
  // manager. This adds to our runtime footprint. It would be awesome
  // if we can do away with this if the user doesnt want to. Does that
  // sound reasonable? Please let me know if this is not possible.

  this->codeset_manager_ = trf->get_codeset_manager();
  if (this->codeset_manager_ == 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("ORB Core unable to initialize codeset_manager")));
      ACE_THROW_RETURN (CORBA::INITIALIZE (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_ORB_CORE_INIT_LOCATION_CODE,
                            0),
                          CORBA::COMPLETED_NO),
                        -1);
    }

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

  // Open the ObjectKey_Table
  (void) this->object_key_table_.init (this);

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
  this->orb_ = CORBA::ORB::_tao_make_ORB (this);

  // This should probably move into the ORB Core someday rather then
  // being done at this level.
  this->orb_->_use_omg_ior_format (use_ior);

  // Set all kinds of orb parameters whose setting needed to be
  // deferred until after the service config entries had been
  // determined.

  this->orb_params ()->service_port (NAMESERVICE, ns_port);

  if (ns_port != 0)
    {
      char ns_port_char[256];

      ACE_OS::itoa (ns_port,
                    ns_port_char,
                    10);

      CORBA::String_var ns_port_ptr =
        CORBA::string_alloc (static_cast<CORBA::ULong> (
                               ACE_OS::strlen ((const char *) ns_port_char)));

      ns_port_ptr = (const char *) ns_port_char;

      const char prefix[] = "mcast://:";

      CORBA::String_var def_init_ref =
        CORBA::string_alloc (sizeof (prefix) +
                             static_cast<CORBA::ULong> (
                                    ACE_OS::strlen (ns_port_ptr.in ())) +
                             2);

      ACE_OS::strcpy (def_init_ref.inout (), prefix);
      ACE_OS::strcat (def_init_ref.inout (), ns_port_ptr.in ());
      ACE_OS::strcat (def_init_ref.inout (), "::");

      this->orb_params ()->default_init_ref (def_init_ref.in ());
    }

  this->orb_params ()->service_port (TRADINGSERVICE, ts_port);
  this->orb_params ()->service_port (IMPLREPOSERVICE, ir_port);

  this->orb_params ()->use_dotted_decimal_addresses (dotted_decimal_addresses);
  this->orb_params ()->linger (linger);
  this->orb_params ()->nodelay (nodelay);
  if (rcv_sock_size >= 0)
    this->orb_params ()->sock_rcvbuf_size (rcv_sock_size);
  if (snd_sock_size >= 0)
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

  // Initialize the flushing strategy
  this->flushing_strategy_ = trf->create_flushing_strategy ();

  // Look in the service repository for an instance of the Protocol Hooks.
  this->protocols_hooks_ =
    ACE_Dynamic_Service<TAO_Protocols_Hooks>::instance
    (TAO_ORB_Core_Static_Resources::instance ()->protocols_hooks_name_.c_str());

  // Must have valid protocol hooks.
  if (this->protocols_hooks_ == 0)
    ACE_THROW_RETURN (CORBA::INITIALIZE (
                        CORBA::SystemException::_tao_minor_code (
                          TAO_ORB_CORE_INIT_LOCATION_CODE,
                          0),
                        CORBA::COMPLETED_NO),
                      -1);

  // Initialize the protocols hooks instance.
  this->protocols_hooks_->init_hooks (this
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Look for BiDirectional library here. If the user has svc.conf
  // file, load the library at this point.
  int ret = this->bidirectional_giop_init (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (ret == -1)
    return -1;

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
  ACE_TRY_NEW_ENV
    {
      // Shutdown the ORB and block until the shutdown is complete.
      this->shutdown (1
                      ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_CString message =
        "Exception caught in trying to shutdown ";
      message += this->orbid_;
      message += "\n";

      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           message.c_str ());
    }
  ACE_ENDTRY;

  // Wait for any server threads, ignoring any failures.
  (void) this->thr_mgr ()->wait ();

  CORBA::release (this->implrepo_service_);

  CORBA::release (this->typecode_factory_);

  CORBA::release (this->dynany_factory_);

  CORBA::release (this->ior_manip_factory_);

  CORBA::release (this->ior_table_);

  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Destroying ORB <%s>\n"),
                  ACE_TEXT_CHAR_TO_TCHAR (this->orbid_)));
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

  (void) TAO::ORB::close_services ();

  // Destroy the object_key table
  this->object_key_table_.destroy ();

  delete this;

  return 0;
}

void
TAO_ORB_Core::set_thread_lane_resources_manager_factory (const char *thread_lane_resources_manager_factory_name)
{
  TAO_ORB_Core_Static_Resources::instance ()->thread_lane_resources_manager_factory_name_ =
    thread_lane_resources_manager_factory_name;
}

void
TAO_ORB_Core::set_collocation_resolver (const char *collocation_resolver_name)
{
  TAO_ORB_Core_Static_Resources::instance ()->collocation_resolver_name_ =
    collocation_resolver_name;
}

void
TAO_ORB_Core::set_stub_factory (const char *stub_factory_name)
{
  TAO_ORB_Core_Static_Resources::instance ()->stub_factory_name_ =
    stub_factory_name;
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
  if (TAO_TSS_RESOURCES::instance ()->gui_resource_factory_ != 0)
    {

      ACE_DEBUG ((LM_WARNING,
                  "TAO (%P|%t) - Deleting old gui_resource_factory.\n"));
      delete TAO_TSS_RESOURCES::instance ()->gui_resource_factory_;
    }

  TAO_TSS_RESOURCES::instance ()->gui_resource_factory_ = gui_resource_factory;
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
TAO_ORB_Core::valuetype_adapter_name (const char *name)
{
  TAO_ORB_Core_Static_Resources::instance ()->valuetype_adapter_name_ = name;
}

const char *
TAO_ORB_Core::valuetype_adapter_name (void)
{
  return TAO_ORB_Core_Static_Resources::instance ()->valuetype_adapter_name_.c_str();
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
  this->resource_factory_ =
    ACE_Dynamic_Service<TAO_Resource_Factory>::instance
    (TAO_ORB_Core_Static_Resources::instance ()->resource_factory_name_.c_str());

  return this->resource_factory_;
}

TAO::GUIResource_Factory *
TAO_ORB_Core::gui_resource_factory (void)
{
  return TAO_TSS_RESOURCES::instance ()->gui_resource_factory_;
}


TAO_Thread_Lane_Resources_Manager &
TAO_ORB_Core::thread_lane_resources_manager (void)
{
  // Check if there is a cached reference.
  if (this->thread_lane_resources_manager_ != 0)
    return *this->thread_lane_resources_manager_;

  // If not, lookup the corresponding factory and ask it to make one.
  TAO_Thread_Lane_Resources_Manager_Factory *factory =
    ACE_Dynamic_Service<TAO_Thread_Lane_Resources_Manager_Factory>::instance
    (TAO_ORB_Core_Static_Resources::instance ()->thread_lane_resources_manager_factory_name_.c_str());

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
    (TAO_ORB_Core_Static_Resources::instance ()->collocation_resolver_name_.c_str());

  return *this->collocation_resolver_;
}

TAO_Stub_Factory *
TAO_ORB_Core::stub_factory (void)
{
  // Check if there is a cached reference.
  if (this->stub_factory_ != 0)
    return this->stub_factory_;

  // If not, look in the service repository for an instance.
  this->stub_factory_ =
    ACE_Dynamic_Service<TAO_Stub_Factory>::instance
    (TAO_ORB_Core_Static_Resources::instance ()->stub_factory_name_.c_str());

  return this->stub_factory_;
}

void
TAO_ORB_Core::set_poa_factory (const char *poa_factory_name,
                               const char *poa_factory_directive)
{
  TAO_ORB_Core_Static_Resources::instance ()->poa_factory_name_ =
    poa_factory_name;
  TAO_ORB_Core_Static_Resources::instance ()->poa_factory_directive_ =
    poa_factory_directive;
}

const ACE_CString &
TAO_ORB_Core::poa_factory_name (void)
{
  return TAO_ORB_Core_Static_Resources::instance ()->poa_factory_name_;
}

void
TAO_ORB_Core::set_endpoint_selector_factory (const char *endpoint_selector_factory_name)
{
  TAO_ORB_Core_Static_Resources::instance ()->endpoint_selector_factory_name_ =
    endpoint_selector_factory_name;
}

TAO_Endpoint_Selector_Factory *
TAO_ORB_Core::endpoint_selector_factory (void)
{
  // Check if there is a cached reference.
  if (this->endpoint_selector_factory_ != 0)
    return this->endpoint_selector_factory_;

  // If not, look in the service repository for an instance.
  this->endpoint_selector_factory_ =
    ACE_Dynamic_Service<TAO_Endpoint_Selector_Factory>::instance
    (TAO_ORB_Core_Static_Resources::instance ()->endpoint_selector_factory_name_.c_str());

  return this->endpoint_selector_factory_;
}

void
TAO_ORB_Core::set_protocols_hooks (const char *protocols_hooks_name)
{
  // Is synchronization necessary?
  TAO_ORB_Core_Static_Resources::instance ()->protocols_hooks_name_ =
    protocols_hooks_name;
}

TAO_Protocols_Hooks *
TAO_ORB_Core::get_protocols_hooks (void)
{
  return this->protocols_hooks_;
}

int
TAO_ORB_Core::bidirectional_giop_init (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->bidir_adapter_ == 0)
    {
      this->bidir_adapter_ =
        ACE_Dynamic_Service<TAO_BiDir_Adapter>::instance ("BiDirGIOP_Loader");
    }

  if (this->bidir_adapter_)
    return this->bidir_adapter_->activate (this->orb_,
                                           0,
                                           0
                                           ACE_ENV_ARG_PARAMETER);
  else
    return 0;
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
    TAO_Profile *profile
    ACE_ENV_ARG_DECL)
{
  if (this->ft_service_.service_callback ())
    {
      return this->ft_service_.service_callback ()->
                 raise_comm_failure (clist,
                                     profile
                                     ACE_ENV_ARG_PARAMETER);
    }

  ACE_THROW_RETURN (CORBA::COMM_FAILURE (
      CORBA::SystemException::_tao_minor_code (
          TAO_INVOCATION_RECV_REQUEST_MINOR_CODE,
          errno),
      CORBA::COMPLETED_MAYBE),
      TAO::TAO_INVOKE_SYSTEM_EXCEPTION);
}


TAO::Invocation_Status
TAO_ORB_Core::service_raise_transient_failure (
    IOP::ServiceContextList &clist,
    TAO_Profile *profile
    ACE_ENV_ARG_DECL)
{
  if (this->ft_service_.service_callback ())
    {
      return
        this->ft_service_.service_callback ()->raise_transient_failure (clist,
                                 profile
                                 ACE_ENV_ARG_PARAMETER);
    }

  return TAO::TAO_INVOKE_FAILURE;
}

void
TAO_ORB_Core::service_context_list (
    TAO_Stub *stub,
    TAO_Service_Context &service_context,
    CORBA::Boolean restart
    ACE_ENV_ARG_DECL)
{
  // @NOTE: Can use Interceptors instead..
  this->protocols_hooks_->rt_service_context (stub,
                                              service_context,
                                              restart
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

TAO_Client_Strategy_Factory *
TAO_ORB_Core::client_factory (void)
{
  if (this->client_factory_ == 0)
    {
      // Look in the service repository for an instance.
      this->client_factory_ =
        ACE_Dynamic_Service<TAO_Client_Strategy_Factory>::instance ("Client_Strategy_Factory");
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
        ACE_Dynamic_Service<TAO_Server_Strategy_Factory>::instance ("Server_Strategy_Factory");
    }

  return this->server_factory_;
}

CORBA::Object_ptr
TAO_ORB_Core::root_poa (ACE_ENV_SINGLE_ARG_DECL)
{
  // DCL ..
  if (CORBA::is_nil (this->root_poa_.in ()))
    {
      TAO_ORB_Core_Static_Resources* static_resources =
        TAO_ORB_Core_Static_Resources::instance ();

      TAO_Adapter_Factory *factory =
        ACE_Dynamic_Service<TAO_Adapter_Factory>::instance (
          static_resources->poa_factory_name_.c_str());

      if (factory == 0)
        {
          ACE_Service_Config::process_directive (
           ACE_TEXT_CHAR_TO_TCHAR (
             static_resources->poa_factory_directive_.c_str()));
          factory =
            ACE_Dynamic_Service<TAO_Adapter_Factory>::instance (
              static_resources->poa_factory_name_.c_str());
        }

      if (factory == 0)
        {
          // It really failed, raise an exception!
          ACE_THROW_RETURN (CORBA::ORB::InvalidName (),
                            CORBA::Object::_nil ());
        }

      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        monitor,
                        this->open_lock_,
                        0);

      if (CORBA::is_nil (this->root_poa_.in ()))
        {
          // @@ Not exception safe
          TAO_Adapter *poa_adapter =
            factory->create (this);

          poa_adapter->open (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (CORBA::Object::_nil ());

          // @@ Not exception safe
          this->root_poa_ =
            poa_adapter->root ();

          this->adapter_registry_.insert (poa_adapter
                                          ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (CORBA::Object::_nil ());
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
                           const TAO_MProfile &profiles
                           ACE_ENV_ARG_DECL)
{
  TAO_Stub *retval =
    this->stub_factory ()->create_stub (repository_id,
                                        profiles,
                                        this
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN(0);
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
                                  CORBA::PolicyList *policy_list
                                  ACE_ENV_ARG_DECL)
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
      TAO_Profile * profile;

      const CORBA::ULong count = mprofile.profile_count ();
      for (CORBA::ULong i = 0; i < count; ++i)
        {
          // Get the ith profile
          profile = mprofile.get_profile (i);
          profile->policies (policy_list ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
        }
    }

  /// Initialize a TAO_Stub object with the mprofile thats passed.
  TAO_Stub *stub =
    this->create_stub (type_id, mprofile ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (stub);

  stub->base_profiles ().policy_list (policy_list);

  return stub;
}

void
TAO_ORB_Core::load_policy_validators (TAO_Policy_Validator &validator
                                      ACE_ENV_ARG_DECL)
{
  // Call the BiDir library if it has been loaded
  if (this->bidir_adapter_)
    this->bidir_adapter_->load_policy_validators (validator ACE_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_ORB_Core::create_object (TAO_Stub *stub)
{
  // @@ What about forwarding.  With this approach we are never forwarded
  //    when we use collocation!
  const TAO_MProfile &mprofile = stub->base_profiles ();

  // @@ We should thow CORBA::NO_MEMORY in platforms with exceptions,
  // but we are stuck in platforms without exceptions!
  CORBA::Object_ptr x;
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->lock_,
                      CORBA::Object::_nil ());

    TAO::ORB_Table * const table = TAO::ORB_Table::instance ();
    const TAO::ORB_Table::Iterator end = table->end ();
    for (TAO::ORB_Table::Iterator i = table->begin (); i != end; ++i)
      {
        TAO_ORB_Core *other_core = (*i).int_id_;

        if (this->is_collocation_enabled (other_core,
                                          mprofile))
          {
            TAO_Adapter_Registry *ar =
              other_core->adapter_registry ();

            return ar->create_collocated_object (stub,
                                                 mprofile);
          }
      }
  }

  // The constructor sets the proxy broker as the
  // Remote one.
  ACE_NEW_RETURN (x,
                  CORBA::Object (stub, 0),
                  0);
  return x;
}

CORBA::Long
TAO_ORB_Core::initialize_object (TAO_Stub *stub,
                                 CORBA::Object_ptr obj)
{
  // @@ What about forwarding.  With this approach we are never forwarded
  //    when we use collocation!
  const TAO_MProfile &mprofile =
    stub->base_profiles ();
  {
    // @@ Ossama: maybe we need another lock for the table, to
    //    reduce contention on the Static_Object_Lock below, if so
    //    then we need to use that lock in the ORB_init() function.
    ACE_MT (ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                              guard,
                              this->lock_,
                              0));

    TAO::ORB_Table * const table = TAO::ORB_Table::instance ();
    TAO::ORB_Table::Iterator const end = table->end ();
    for (TAO::ORB_Table::Iterator i = table->begin (); i != end; ++i)
      {
        TAO_ORB_Core * const other_core = (*i).int_id_;

        if (this->is_collocation_enabled (other_core,
                                          mprofile))
          {
            TAO_Adapter_Registry * const ar =
              other_core->adapter_registry ();

            return ar->initialize_collocated_object (stub,
                                                     obj);
          }
      }
  }

  return 0;
}

CORBA::Boolean
TAO_ORB_Core::is_collocation_enabled (TAO_ORB_Core *orb_core,
                                      const TAO_MProfile &mp)
{
  if (!orb_core->optimize_collocation_objects ())
    return 0;

  if (!orb_core->use_global_collocation () && orb_core != this)
    return 0;

  if (!orb_core->is_collocated (mp))
    return 0;

  return 1;
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
TAO_ORB_Core::run (ACE_Time_Value *tv,
                   int perform_work
                   ACE_ENV_ARG_DECL_NOT_USED)
{
  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
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

  while (this->has_shutdown () == 0)
    {
      // Every time we perform an interation we have to become the
      // leader again, because it is possible that a client has
      // acquired the leader role...
      TAO_Leader_Follower &leader_follower =
        this->leader_follower ();

      TAO_LF_Strategy &lf_strategy =
        this->lf_strategy ();

      TAO_LF_Event_Loop_Thread_Helper helper (leader_follower,
                                              lf_strategy,
                                              tv);
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

      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - ORB_Core::run, ")
                      ACE_TEXT ( "calling handle_events()\n")));
        }

      result = r->handle_events (tv);

      if (TAO_debug_level > 2)
        {
          ACE_DEBUG ((LM_DEBUG,
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

  if (this->has_shutdown () == 1 &&
      this->server_factory_->activate_server_connections ())
      this->tm_.wait ();

  if (TAO_debug_level > 2)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("TAO (%P|%t) - ORB_Core::run, ")
                  ACE_TEXT ("ends with result = %d\n"),
                  result));
    }

  return result;
}


void
TAO_ORB_Core::shutdown (CORBA::Boolean wait_for_completion
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC (())
{
  ACE_TRY
    {
      {
        ACE_GUARD (TAO_SYNCH_MUTEX, monitor, this->lock_);

        if (this->has_shutdown () != 0)
          return;

        // Check if we are on the right state, i.e. do not accept
        // shutdowns with the 'wait_for_completion' flag set in the middle
        // of an upcall (because those deadlock).
        this->adapter_registry_.check_close (wait_for_completion
                                             ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;

        // Set the 'has_shutdown' flag, so any further attempt to shutdown
        // becomes a noop.
        this->has_shutdown_ = 1;

        // need to release the mutex, because some of the shutdown
        // operations invoke application code, that could (and in practice
        // does!) callback into ORB Core code.
      }

      this->adapter_registry_.close (wait_for_completion
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Shutdown reactor.
      this->thread_lane_resources_manager ().shutdown_reactor ();

      // Cleanup transports that use the RW strategies
      this->thread_lane_resources_manager ().cleanup_rw_transports ();

      // Grab the thread manager
      ACE_Thread_Manager *tm = this->thr_mgr ();

      // Try to cancel all the threads in the ORB.
      tm->cancel_all ();

      // If <wait_for_completion> is set, wait for all threads to exit.
      if (wait_for_completion != 0)
        tm->wait ();

      // Explicitly destroy the object reference table since it
      // contains references to objects, which themselves may contain
      // reference to this ORB.
      this->object_ref_table_.destroy ();

#if (TAO_HAS_INTERCEPTORS == 1)
      this->pi_current_ = 0;  // For the sake of consistency.
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
    }
  ACE_CATCHALL
    {
      // Do not allow exceptions to escape.. So catch all the
      // exceptions.
      // @@ Not sure what to print here for the users..

    }
  ACE_ENDTRY;

  return;
}

void
TAO_ORB_Core::destroy (ACE_ENV_SINGLE_ARG_DECL)
{
  //
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
  this->shutdown (1
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Invoke Interceptor::destroy() on all registered interceptors.
  this->destroy_interceptors (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Now remove it from the ORB table so that it's ORBid may be
  // reused.
  TAO::ORB_Table::instance ()->unbind (this->orbid_);
}

void
TAO_ORB_Core::check_shutdown (ACE_ENV_SINGLE_ARG_DECL)
{
  if (this->has_shutdown ())
    {
      // As defined by the CORBA 2.3 specification, throw a
      // CORBA::BAD_INV_ORDER exception with minor code 4 if the ORB
      // has shutdown by the time an ORB function is called.

      ACE_THROW (CORBA::BAD_INV_ORDER (CORBA::OMGVMCID | 4,
                                       CORBA::COMPLETED_NO));
    }
}

void
TAO_ORB_Core::destroy_interceptors (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC (())
{

  ACE_TRY
    {
#if TAO_HAS_INTERCEPTORS == 1
      size_t len = 0;   // The length of the interceptor array.
      size_t ilen = 0;  // The incremental length of the interceptor array.

      TAO::ClientRequestInterceptor_List::TYPE &client_interceptors =
        this->client_request_interceptors_.interceptors ();

      len = client_interceptors.size ();
      ilen = len;

      for (size_t i = 0; i < len; ++i)
        {
          // Destroy the interceptors in reverse order in case the array
          // list is only partially destroyed and another invocation
          // occurs afterwards.
          --ilen;

          client_interceptors[ilen]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Since Interceptor::destroy() can throw an exception, decrease
          // the size of the interceptor array incrementally since some
          // interceptors may not have been destroyed yet.  Note that this
          // size reduction is fast since no memory is actually
          // deallocated.
          client_interceptors.size (ilen);
        }

      TAO::ServerRequestInterceptor_List::TYPE &server_interceptors =
        this->server_request_interceptors_.interceptors ();

      len = server_interceptors.size ();
      ilen = len;

      for (size_t j = 0; j < len; ++j)
        {
          // Destroy the interceptors in reverse order in case the array
          // list is only partially destroyed and another invocation
          // occurs afterwards.
          --ilen;

          server_interceptors[ilen]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          // Since Interceptor::destroy() can throw an exception, decrease
          // the size of the interceptor array incrementally since some
          // interceptors may not have been destroyed yet.  Note that this
          // size reduction is fast since no memory is actually
          // deallocated.
          server_interceptors.size (ilen);
        }

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

      if (this->ior_interceptor_adapter_ != 0)
        {
          this->ior_interceptor_adapter_->destroy_interceptors (
              ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

          this->ior_interceptor_adapter_ = 0;
        }

    }
  ACE_CATCHALL
    {
      // .. catch all the exceptions..
      if (TAO_debug_level > 3)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - Exception in TAO_ORB_Core")
                      ACE_TEXT ("::destroy_interceptors () \n")));
        }
    }
  ACE_ENDTRY;

  return;
}

TAO_Thread_Lane_Resources &
TAO_ORB_Core::lane_resources (void)
{
  return this->thread_lane_resources_manager ().lane_resources ();
}

void
TAO_ORB_Core::resolve_typecodefactory_i (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Object_Loader *loader =
    ACE_Dynamic_Service<TAO_Object_Loader>::instance ("TypeCodeFactory_Loader");
  if (loader == 0)
    {
      ACE_Service_Config::process_directive (ACE_TEXT("dynamic TypeCodeFactory Service_Object *")
                                             ACE_TEXT("TAO_TypeCodeFactory:_make_TAO_TypeCodeFactory_Loader()"));
      loader =
        ACE_Dynamic_Service<TAO_Object_Loader>::instance ("TypeCodeFactory_Loader");
      if (loader == 0)
        ACE_THROW (CORBA::ORB::InvalidName ());
    }
  this->typecode_factory_ =
    loader->create_object (this->orb_, 0, 0 ACE_ENV_ARG_PARAMETER);
}

void
TAO_ORB_Core::resolve_dynanyfactory_i (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Object_Loader *loader =
    ACE_Dynamic_Service<TAO_Object_Loader>::instance ("DynamicAny_Loader");
  if (loader == 0)
    {
      ACE_Service_Config::process_directive (ACE_TEXT("dynamic DynamicAny_Loader Service_Object *")
                                             ACE_TEXT("TAO_DynamicAny:_make_TAO_DynamicAny_Loader()"));
      loader =
        ACE_Dynamic_Service<TAO_Object_Loader>::instance ("DynamicAny_Loader");
      if (loader == 0)
        ACE_THROW (CORBA::ORB::InvalidName ());
    }
  this->dynany_factory_ =
    loader->create_object (this->orb_, 0, 0 ACE_ENV_ARG_PARAMETER);
}

void
TAO_ORB_Core::resolve_iormanipulation_i (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Object_Loader *loader =
    ACE_Dynamic_Service<TAO_Object_Loader>::instance ("IORManip_Loader");

  if (loader == 0)
    {
      ACE_Service_Config::process_directive (ACE_TEXT("dynamic IORManip_Loader Service_Object *")
                                             ACE_TEXT("TAO_IORManip:_make_TAO_IORManip_Loader()"));
      loader =
        ACE_Dynamic_Service<TAO_Object_Loader>::instance ("IORManip_Loader");
      if (loader == 0)
        ACE_THROW (CORBA::ORB::InvalidName ());
    }
  this->ior_manip_factory_ =
    loader->create_object (this->orb_, 0, 0 ACE_ENV_ARG_PARAMETER);
}

void
TAO_ORB_Core::resolve_ior_table_i (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Adapter_Factory *factory =
    ACE_Dynamic_Service<TAO_Adapter_Factory>::instance ("TAO_IORTable");
  if (factory == 0)
    {
      ACE_Service_Config::process_directive (ACE_TEXT("dynamic TAO_IORTable Service_Object *")
                                             ACE_TEXT("TAO_IORTable:_make_TAO_Table_Adapter_Factory()"));
      factory =
        ACE_Dynamic_Service<TAO_Adapter_Factory>::instance ("TAO_IORTable");
    }
  if (factory == 0)
    ACE_THROW (CORBA::ORB::InvalidName ());

  // @@ Not exception safe
  TAO_Adapter *iortable_adapter = factory->create (this);
  this->adapter_registry_.insert (iortable_adapter ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  iortable_adapter->open (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->ior_table_ = iortable_adapter->root ();
}

int
TAO_ORB_Core::set_endpoint_helper (const ACE_CString &lane,
                                   const ACE_CString &endpoints
                                   ACE_ENV_ARG_DECL)
{
  if (this->orb_params ()->add_endpoints (lane,
                                          endpoints) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t)\n")
                  ACE_TEXT ("Invalid endpoint(s) specified:\n%s\n"),
                  ACE_TEXT_CHAR_TO_TCHAR(endpoints.c_str ())));
      ACE_THROW_RETURN (CORBA::BAD_PARAM (
                           CORBA::SystemException::_tao_minor_code (
                              TAO_ORB_CORE_INIT_LOCATION_CODE,
                              EINVAL),
                           CORBA::COMPLETED_NO),
                        -1);
    }

  return 0;
}

CORBA::Object_ptr
TAO_ORB_Core::resolve_rir (const char *name
                           ACE_ENV_ARG_DECL)
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
          TAO_Connector_Registry *conn_reg =
            this->connector_registry (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);

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

      return this->orb ()->string_to_object (list_of_profiles.c_str ()
                                             ACE_ENV_ARG_PARAMETER);
    }

  return CORBA::Object::_nil ();
}

CORBA::ORB::ObjectIdList *
TAO_ORB_Core::list_initial_references (ACE_ENV_SINGLE_ARG_DECL)
{
  // Unsupported initial services should NOT be included in the below list!
  static const char *initial_services[] = { TAO_LIST_OF_INITIAL_SERVICES };
  // Make sure the "terminating" zero is the last array element so
  // that there is a stop condition when iterating through the list.

  static const size_t initial_services_size =
    sizeof (initial_services) / sizeof (initial_services[0]);

  const size_t total_size =
    initial_services_size
    + this->init_ref_map_.current_size ()
    + this->object_ref_table_.current_size ();

  CORBA::ORB::ObjectIdList *tmp = 0;

  ACE_NEW_THROW_EX (tmp,
                    CORBA::ORB::ObjectIdList (
                      static_cast<CORBA::ULong> (total_size)),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

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
  TAO_Object_Ref_Table::Iterator obj_ref_end =
    this->object_ref_table_.end ();

  for (TAO_Object_Ref_Table::Iterator i = this->object_ref_table_.begin ();
       i != obj_ref_end;
       ++i, ++index)
    list[index] = CORBA::string_dup ((*i).ext_id_);

  // References registered via INS.
  const InitRefMap::iterator end = this->init_ref_map_.end ();

  for (InitRefMap::iterator j = this-> init_ref_map_.begin ();
       j != end;
       ++j, ++index)
    list[index] = (*j).ext_id_.c_str ();

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

  ACE_Allocator *dblock_allocator;
  ACE_Allocator *buffer_allocator;

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
TAO_ORB_Core::connector_registry (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Connector_Registry *conn =
    this->lane_resources ().connector_registry (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return conn;
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

      ACE_TRY_NEW_ENV
        {
          CORBA::Object_var temp = this->orb_->resolve_initial_references ("ImplRepoService" ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->lock_, CORBA::Object::_nil ());

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
                                    bool &has_synchronization,
                                    Messaging::SyncScope &scope)
{
  Sync_Scope_Hook sync_scope_hook =
    TAO_ORB_Core_Static_Resources::instance ()->sync_scope_hook_;

  if (sync_scope_hook == 0)
    {
      has_synchronization = false;
      return;
    }

  (*sync_scope_hook) (this, stub, has_synchronization, scope);
}

TAO_Sync_Strategy &
TAO_ORB_Core::get_sync_strategy (TAO_Stub *,
                                 Messaging::SyncScope &scope)
{

  if (scope == Messaging::SYNC_WITH_TRANSPORT
      || scope == Messaging::SYNC_WITH_SERVER
      || scope == Messaging::SYNC_WITH_TARGET)
    {
      return this->transport_sync_strategy ();
    }

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  if (scope == Messaging::SYNC_NONE
      || scope == TAO::SYNC_EAGER_BUFFERING)
    {
      return this->eager_buffering_sync_strategy ();
    }

  if (scope == TAO::SYNC_DELAYED_BUFFERING)
    {
      return this->delayed_buffering_sync_strategy ();
    }

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

  return this->transport_sync_strategy ();
}

void
TAO_ORB_Core::set_sync_scope_hook (Sync_Scope_Hook hook)
{
  TAO_ORB_Core_Static_Resources::instance ()-> sync_scope_hook_ = hook;
}

void
TAO_ORB_Core::call_timeout_hook (TAO_Stub *stub,
                                 bool &has_timeout,
                                 ACE_Time_Value &time_value)
{
  Timeout_Hook timeout_hook =
    TAO_ORB_Core_Static_Resources::instance ()->timeout_hook_;

  if (timeout_hook == 0)
    {
      has_timeout = false;
      return;
    }
  (*timeout_hook) (this, stub, has_timeout, time_value);
}

void
TAO_ORB_Core::set_timeout_hook (Timeout_Hook hook)
{
  TAO_ORB_Core_Static_Resources::instance ()->timeout_hook_ = hook;
  // Saving the hook pointer so that we can use it later when needed.

  return;
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
}

void
TAO_ORB_Core::connection_timeout_hook (Timeout_Hook hook)
{
  // Saving the hook pointer so that we can use it later when needed.
  TAO_ORB_Core_Static_Resources::instance ()->connection_timeout_hook_ = hook;
}

#if (TAO_HAS_CORBA_MESSAGING == 1)

CORBA::Policy_ptr
TAO_ORB_Core::get_policy (CORBA::PolicyType type
                          ACE_ENV_ARG_DECL)
{
  CORBA::Policy_var result;

  TAO_Policy_Manager *policy_manager =
    this->policy_manager ();
  if (policy_manager != 0)
    {
      result = policy_manager->get_policy (type
                                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  if (CORBA::is_nil (result.in ()))
    {
      result =
        this->get_default_policies ()->get_policy (type
                                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  return result._retn ();
}

CORBA::Policy_ptr
TAO_ORB_Core::get_policy_including_current (CORBA::PolicyType type
                                            ACE_ENV_ARG_DECL)
{
  TAO_Policy_Current &policy_current =
    this->policy_current ();

  CORBA::Policy_var result =
    policy_current.get_policy (type
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Policy::_nil ());

  if (CORBA::is_nil (result.in ()))
    {
      result = this->get_policy (type
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  return result._retn ();
}

CORBA::Policy_ptr
TAO_ORB_Core::get_cached_policy (TAO_Cached_Policy_Type type
                                 ACE_ENV_ARG_DECL)
{
  CORBA::Policy_var result;

  TAO_Policy_Manager *policy_manager =
    this->policy_manager ();
  if (policy_manager != 0)
    {
      result =
        policy_manager->get_cached_policy (type
                                           ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  if (CORBA::is_nil (result.in ()))
    {
      result =
        this->get_default_policies ()->get_cached_policy (type
                                                          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  return result._retn ();
}

CORBA::Policy_ptr
TAO_ORB_Core::get_cached_policy_including_current (
    TAO_Cached_Policy_Type type
    ACE_ENV_ARG_DECL)
{
  TAO_Policy_Current &policy_current =
    this->policy_current ();

  CORBA::Policy_var result =
    policy_current.get_cached_policy (type
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK (CORBA::Policy::_nil ());

  if (CORBA::is_nil (result.in ()))
    {
      result =
        this->get_cached_policy (type
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Policy::_nil ());
    }

  return result._retn ();
}

#endif /* (TAO_HAS_CORBA_MESSAGING == 1) */

CORBA::Environment *
TAO_ORB_Core::default_environment (void) const
{
  return TAO_TSS_RESOURCES::instance ()->default_environment_;
}

void
TAO_ORB_Core::default_environment (CORBA::Environment *env)
{
  TAO_TSS_RESOURCES::instance ()->default_environment_ = env;
}

void
TAO_ORB_Core::add_interceptor (
   PortableInterceptor::IORInterceptor_ptr interceptor
   ACE_ENV_ARG_DECL)
{
  if (this->ior_interceptor_adapter ())
    {
      this->ior_interceptor_adapter_->add_interceptor (interceptor
                                                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) %p\n"),
                  ACE_TEXT ("ERROR: ORB Core unable to find the ")
                  ACE_TEXT ("IORInterceptor Adapter Factory instance")));

      ACE_THROW (CORBA::INTERNAL ());
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
          ACE_DECLARE_NEW_CORBA_ENV;
          ACE_TRY
            {
              TAO_IORInterceptor_Adapter_Factory * ior_ap_factory =
                ACE_Dynamic_Service<TAO_IORInterceptor_Adapter_Factory>::instance (
                    TAO_ORB_Core::iorinterceptor_adapter_factory_name ()
                  );

              if (ior_ap_factory)
                {
                  this->ior_interceptor_adapter_ =
                    ior_ap_factory->create (ACE_ENV_SINGLE_ARG_PARAMETER);
                  ACE_TRY_CHECK;
                }
            }
          ACE_CATCHANY
            {
              ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                                   "(%P|%t) Cannot initialize the "
                                   "ior_interceptor_adapter \n");
            }
          ACE_ENDTRY;
          ACE_CHECK_RETURN(0);
        }
    }

  return this->ior_interceptor_adapter_;
}

// ****************************************************************

TAO_ORB_Core_TSS_Resources::TAO_ORB_Core_TSS_Resources (void)
  : event_loop_thread_ (0)
  , client_leader_thread_ (0)
  , lane_ (0)
  , ts_objects_ ()
  , upcalls_temporarily_suspended_on_this_thread_ (false)
  , orb_core_ (0)
#if TAO_HAS_INTERCEPTORS == 1
    , pi_current_ ()
    , client_request_info_ (0)
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
{
#if TAO_HAS_INTERCEPTORS == 1
  ACE_NEW (this->client_request_info_,
           TAO_ClientRequestInfo);
#endif  /* TAO_HAS_INTERCEPTORS == 1 */
}

TAO_ORB_Core_TSS_Resources::~TAO_ORB_Core_TSS_Resources (void)
{

#if TAO_HAS_INTERCEPTORS == 1
  CORBA::release (this->client_request_info_);
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

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

          ACE_DECLARE_NEW_CORBA_ENV;
          ACE_TRY
            {
              int argc = 0;
              CORBA::ORB_var orb =
                CORBA::ORB_init (argc, 0, 0 ACE_ENV_ARG_PARAMETER);
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


TAO::Collocation_Strategy
TAO_ORB_Core::collocation_strategy (CORBA::Object_ptr object
                                    ACE_ENV_ARG_DECL)
{
  TAO_Stub *stub = object->_stubobj ();
  if (!CORBA::is_nil (stub->servant_orb_var ().in ()) &&
      stub->servant_orb_var ()->orb_core () != 0)
    {
      TAO_ORB_Core *orb_core =
        stub->servant_orb_var ()->orb_core ();

      const int collocated =
        orb_core->collocation_resolver ().is_collocated (object
                                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (TAO::TAO_CS_REMOTE_STRATEGY);

      if (collocated)
        {
          switch (stub->servant_orb_var ()->orb_core ()->get_collocation_strategy ())
            {
            case THRU_POA:
              return TAO::TAO_CS_THRU_POA_STRATEGY;

            case DIRECT:
              {
                /////////////////////////////////////////////////////////////
                // If the servant is null and you are collocated this means
                // that the POA policy NON-RETAIN is set, and with that policy
                // using the DIRECT collocation strategy is just insane.
                /////////////////////////////////////////////////////////////
                ACE_ASSERT (object->_servant () != 0);
                return TAO::TAO_CS_DIRECT_STRATEGY;
              }
            }
        }
    }

  // In this case the Object is a client.
  return TAO::TAO_CS_REMOTE_STRATEGY;
}

TAO_ORB_Core::InitRefMap *
TAO_ORB_Core::init_ref_map ()
{
  return &this->init_ref_map_;
}

void
TAO_ORB_Core::set_default (const char * orb_id)
{
  TAO::ORB_Table * const table = TAO::ORB_Table::instance ();
  table->set_default (orb_id);
}

void
TAO_ORB_Core::not_default (const char * orb_id)
{
  TAO::ORB_Table * const table = TAO::ORB_Table::instance ();
  table->not_default (orb_id);
}

/// Return the valuetype adapter
TAO_Valuetype_Adapter *&
TAO_ORB_Core::valuetype_adapter (void)
{
  return this->valuetype_adapter_;
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Lock_Adapter<ACE_Null_Mutex>;
#if defined (ACE_HAS_THREADS)
template class ACE_Lock_Adapter<TAO_SYNCH_RECURSIVE_MUTEX>;
template class ACE_Lock_Adapter<TAO_SYNCH_MUTEX>;
#endif /* ACE_HAS_THREADS */

template class ACE_Reverse_Lock<TAO_SYNCH_MUTEX>;
template class ACE_Guard<ACE_Reverse_Lock<TAO_SYNCH_MUTEX> >;

template class ACE_TSS<TAO_ORB_Core_TSS_Resources>;

template class ACE_Hash_Map_Manager_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<ACE_CString, ACE_CString>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>;

template class ACE_Array_Base<void *>;

template class ACE_Dynamic_Service<TAO_Server_Strategy_Factory>;
template class ACE_Dynamic_Service<TAO_Client_Strategy_Factory>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Lock_Adapter<ACE_Null_Mutex>
#if defined (ACE_HAS_THREADS)
#pragma instantiate ACE_Lock_Adapter<TAO_SYNCH_RECURSIVE_MUTEX>
#pragma instantiate ACE_Lock_Adapter<TAO_SYNCH_MUTEX>
#endif /* ACE_HAS_THREADS */

#pragma instantiate ACE_Reverse_Lock<TAO_SYNCH_MUTEX>
#pragma instantiate ACE_Guard<ACE_Reverse_Lock<TAO_SYNCH_MUTEX> >

#pragma instantiate ACE_TSS<TAO_ORB_Core_TSS_Resources>

#pragma instantiate ACE_Hash_Map_Manager_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<ACE_CString, ACE_CString>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ACE_CString, ACE_CString, ACE_Hash<ACE_CString>, ACE_Equal_To<ACE_CString>, ACE_Null_Mutex>

#pragma instantiate ACE_Array_Base<void *>

#pragma instantiate ACE_Dynamic_Service<TAO_Server_Strategy_Factory>
#pragma instantiate ACE_Dynamic_Service<TAO_Client_Strategy_Factory>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
