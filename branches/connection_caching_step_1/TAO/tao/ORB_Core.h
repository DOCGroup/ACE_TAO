// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     ORB_Core.h
//
// = AUTHOR
//     Chris Cleeland
//
// ============================================================================

#ifndef TAO_ORB_CORE_H
#define TAO_ORB_CORE_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"
#include "tao/Environment.h"
#include "tao/Policy_Manager.h"
#include "tao/Resource_Factory.h"
#include "tao/params.h"
#include "tao/TAO_Singleton_Manager.h"
#include "tao/TAO_Singleton.h"
#include "tao/Adapter.h"
#include "tao/PolicyFactory_Registry.h"
#include "tao/Parser_Registry.h"
#include "tao/Service_Callbacks.h"
#include "tao/Fault_Tolerance_Service.h"

#if (TAO_HAS_INTERCEPTORS == 1)
// Interceptor definitions.
# include "tao/PortableInterceptorC.h"
# include "tao/Interceptor_List.h"
#endif  /* TAO_HAS_INTERCEPTORS */

#include "tao/Connection_Cache_Manager.h"


// Forward declarations
class TAO_Acceptor;
class TAO_Connector;
class TAO_Acceptor_Registry;
class TAO_Connector_Registry;

class TAO_Resource_Factory;
class TAO_Client_Strategy_Factory;
class TAO_Server_Strategy_Factory;
class TAO_Connection_Cache;

class TAO_TSS_Resources;
class TAO_Reactor_Registry;
class TAO_Leader_Follower;
class TAO_Priority_Mapping;
class TAO_Priority_Mapping_Manager;
class TAO_RT_ORB;
class TAO_RT_Current;
class TAO_MProfile;
class TAO_Profile;
class TAO_GIOP_Invocation;

class TAO_Endpoint_Selector_Factory;
class TAO_Invocation_Endpoint_Selector;
class TAO_Default_Endpoint_Selector;
class TAO_Priority_Endpoint_Selector;
class TAO_Protocol_Endpoint_Selector;
class TAO_Priority_Protocol_Selector;
class TAO_Client_Priority_Policy_Selector;

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

class TAO_Eager_Buffering_Sync_Strategy;
class TAO_Delayed_Buffering_Sync_Strategy;

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

class TAO_Transport_Sync_Strategy;
class TAO_Sync_Strategy;

// ****************************************************************

class TAO_Export TAO_ORB_Core_TSS_Resources
{
  // = TITLE
  //   The TSS resoures of an ORB core.
  //
  // = DESCRIPTION
  //   This class is used by the ORB_Core to store the resources
  //   potentially bound to a thread in TSS storage.
  //   The members are public because only the ORB Core is expected to
  //   access them.
  //
public:
  TAO_ORB_Core_TSS_Resources (void);
  // constructor

  ~TAO_ORB_Core_TSS_Resources (void);
  // destructor

private:
  // The ORB Core TSS resources should not be copied
  ACE_UNIMPLEMENTED_FUNC (TAO_ORB_Core_TSS_Resources (const TAO_ORB_Core_TSS_Resources&))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_ORB_Core_TSS_Resources&))

public:
  // = The rest of the resources are not currently in use, just a plan
  //   for the future...

  ACE_Allocator *output_cdr_dblock_allocator_;
  ACE_Allocator *output_cdr_buffer_allocator_;
  ACE_Allocator *output_cdr_msgblock_allocator_;
  // The allocators for the output CDR streams.

  ACE_Allocator *input_cdr_dblock_allocator_;
  ACE_Allocator *input_cdr_buffer_allocator_;
  // The allocators for the input CDR streams.

  TAO_Connection_Cache *connection_cache_;
  // This is is just a place holder, in the future the connection
  // cache will be separated from the connectors and it will be a
  // (potentially) TSS object.

  int event_loop_thread_;
  // Counter for how (nested) calls this thread has made to run the
  // event loop.

  int client_leader_thread_;
  // Counter for how many times this thread has become a client
  // leader.

  ACE_SYNCH_CONDITION* leader_follower_condition_variable_;
  // Condition variable for the leader follower model.

  TAO_Reactor_Registry *reactor_registry_;
  // The Reactor Holder that we should callback when destroying the
  // cookie.

  void *reactor_registry_cookie_;
  // A TSS magic cookie used by the Reactor_Registry
};

// ****************************************************************

class TAO_Export TAO_ORB_Core
{
  // = TITLE
  //    Encapsulates the state of an ORB.
  //
  // = DESCRIPTION
  //   This is the implementation class for the CORBA::ORB interface.
  //   The class also encapsulates the access to the ORB resources and
  //   its state.
  //   Some resources can be TSS or global, those resources are always
  //   accessed through a TSS interface, but are allocated using the
  //   Resource_Factory.  If the resource is really global the
  //   Resource_Factory will simply return a pointer to the global
  //   instance.
  //
  friend class TAO_ORB_Core_Auto_Ptr;
  friend class TAO_ORB_Table;
  friend CORBA::ORB_ptr CORBA::ORB_init (int &,
                                         char *argv[],
                                         const char *,
                                         CORBA_Environment &);

public:
  // = Initialization and termination methods.
  TAO_ORB_Core (const char* id);
  // Constructor.

  TAO_ORB_Parameters *orb_params (void);
  // Accessor for the ORB parameters.

  // @@ In the future this hook should change, instead of hardcoding
  //    the object we should add a "Resolver" to the ORB, so the
  //    "POACurrent" object returns a per-ORB object.
  //    Similarly, each ORB should implement the TSS pattern to put
  //    the POA_Current_Impl in a void* slot.
  //    The current approach *does* decouple the POA from the ORB, but
  //    it cannot add new adapters or other components transparently.
  CORBA::Object_ptr poa_current (void);
  void poa_current (CORBA::Object_ptr poa_current);
  // Accessor to the POA current.

  // = Get the connector registry
  TAO_Connector_Registry *connector_registry (void);

  // = Get the acceptor registry
  TAO_Acceptor_Registry  *acceptor_registry  (void);

  // = Get the IOR parser registry
  TAO_Parser_Registry *parser_registry (void);

  TAO_PolicyFactory_Registry *policy_factory_registry (void);
  ///< Return pointer to the policy factory registry associated with
  ///< this ORB core.

  // = Get the protocol factories
  TAO_ProtocolFactorySet *protocol_factories (void);

  // = Get pointer to the ORB.
  CORBA::ORB_ptr orb (void);

  ACE_Reactor *reactor (void);
  ACE_Reactor *reactor (TAO_Acceptor *acceptor);
  // Wrappers that forward the request to the concurrency strategy

  // = Get the ACE_Thread_Manager
  ACE_Thread_Manager *thr_mgr (void);

  CORBA::Object_ptr root_poa (CORBA::Environment &ACE_TRY_ENV);
  // Return the RootPOA, or try to load it if not initialized already.

  TAO_Adapter_Registry *adapter_registry (void);
  // Get the adapter registry

  // = Collocation strategies.
  enum
  {
    ORB_CONTROL,  // Indicate object should refer to ORB for either
                  // one of the following strategies.
    THRU_POA,     // Collocated calls will go thru POA.
    DIRECT        // Collocated calls invoke operation on Servant directly.
  };

  enum TAO_Collocation_Strategies
  {
    REMOTE_STRATEGY,    // i.e. no collocation.

    THRU_POA_STRATEGY,  // Calls to the collocated object are
                            // forwarded by the POA.

    DIRECT_STRATEGY,     // Calls to the collocated object are
                             // made directly to its servant.

    COLLOCATION_STRATEGIES_NUM  // This value should always be the
                                // last value in the enumeration.  It
                                // provides the count for the number
                                // of collocation strategies.
  };

  static TAO_Collocation_Strategies collocation_strategy (CORBA::Object_ptr object);
  // This methods give the right collocation strategy, if any,
  // to be used to perform a method invokation on the given object.
  // (Note that No-Collocation is a special case of collocation).


  // = Get the default codeset translators.
  //   In most configurations these are just <nil> objects, but they
  //   can be set to something different if the native character sets
  //   are not ISO8869 (aka Latin/1, UTF-8) and UNICODE (aka UTF-16).

  ACE_Char_Codeset_Translator *from_iso8859 (void) const;
  // Convert from ISO8859 to the native character set

  ACE_Char_Codeset_Translator *to_iso8859 (void) const;
  // Convert from the native character set to ISO8859

  ACE_WChar_Codeset_Translator *from_unicode (void) const;
  // Convert from UNICODE to the native wide character set

  ACE_WChar_Codeset_Translator *to_unicode (void) const;
  // Convert from the native wide character set to UNICODE

  // @@ This is just note on how could the translator database be
  //    implemented: use the service configurator to load the
  //    translator, and then use the CodesetId (an unsigned long) to
  //    translate the character set code into the Service Object
  //    name.
  //    The default resource factory could parse command line options
  //    like:
  //    -ORBcharcodeset 0x00010001=ISO8859
  //    -ORBcharcodeset 0x10020417=IBM1047
  //    -ORBwcharcodeset 0x00010106=ISOIEC10646
  //    that would let the user experiment with different translators
  //    and plug them in on demand.
  //
  //    We should also think about how translators will report
  //    conversion failures and how to simplify the implementation of
  //    char translators (it would seem like just a couple of arrays
  //    are needed, maybe the arrays should be dynamically loaded and
  //    the implementation would remain constant?  Just a thought

  // = Set/get the collocation flags
  void optimize_collocation_objects (CORBA::Boolean opt);
  CORBA::Boolean optimize_collocation_objects (void) const;

  void use_global_collocation (CORBA::Boolean opt);
  CORBA::Boolean use_global_collocation (void) const;

  CORBA::ULong get_collocation_strategy (void) const;

  TAO_Adapter *poa_adapter (void);
  // Get the adapter named "RootPOA" and cache the result, this is an
  // optimization for the POA.

  int inherit_from_parent_thread (TAO_ORB_Core_TSS_Resources *tss_resources);
  // A spawned thread needs to inherit some properties/objects from
  // the spawning thread in order to serve requests.  Return 0 if
  // it successfully inherits from the parent, -1 otherwise.

  // = Access to Factories.
  //
  // These factories are not thread-specific, and are presented here
  // in order to have one place to get useful information.  Often, the
  // instances to which the return pointers are stored in the Service
  // Repository.

  TAO_Resource_Factory *resource_factory (void);
  // Returns pointer to the resource factory.

  TAO_Client_Strategy_Factory *client_factory (void);
  // Returns pointer to the client factory.

  TAO_Server_Strategy_Factory *server_factory (void);
  // Returns pointer to the server factory.

  int is_collocated (const TAO_MProfile& mprofile);
  // See if we have a collocated address, if yes, return the POA
  // associated with the address.

  ACE_Allocator *output_cdr_dblock_allocator (void);
  // This allocator is always TSS and has no locks. It is intended for
  // allocating the ACE_Data_Blocks used in *outgoing* CDR streams.

  ACE_Allocator *output_cdr_buffer_allocator (void);
  // This allocator is always TSS and has no locks. It is intended for
  // allocating the buffers used in *outgoing* CDR streams.

  ACE_Allocator *input_cdr_dblock_allocator (void);
  // This allocator maybe TSS or global, may or may not have locks. It
  // is intended for allocating the ACE_Data_Blocks used in *outgoing*
  // CDR streams.

  ACE_Allocator *input_cdr_buffer_allocator (void);
  // This allocator is always TSS and has no locks. It is intended for
  // allocating the buffers used in *outgoing* CDR streams.

  ACE_Data_Block *create_input_cdr_data_block (size_t size);
  // The Message Blocks used for input CDRs must have appropiate
  // locking strategies.

  CORBA_Environment *default_environment (void) const;
  void default_environment (CORBA_Environment*);
  // The thread has a default environment to simplify porting between
  // platforms that support native C++ exceptions and those that
  // don't. This is a TSS resource (always), but with a twist: if the
  // user creates a new environment the old one is "pushed" (actually
  // the new one remembers it), eventually the new environment
  // destructor pops itself from the stack and we recover the old
  // environment.
  // This means that if the user create a new environment and somebody
  // calls a function using the default one the exception will still
  // be received in the environment created by the user.
  // The only drawback is that environments life time must nest
  // properly, this shouldn't be a problem because environments are
  // usually created on the stack, but, the spec allows their creation
  // on the heap and/or as class members; we need to investigate the
  // tradeoffs and take a decision.

  TAO_Endpoint_Selector_Factory *endpoint_selector_factory (void);
  //

  TAO_Default_Endpoint_Selector *default_endpoint_selector (void);
  //

#if (TAO_HAS_RT_CORBA == 1)

  TAO_Protocol_Endpoint_Selector *protocol_endpoint_selector (void);
  //

  TAO_Priority_Endpoint_Selector *priority_endpoint_selector (void);
  //

  TAO_Priority_Protocol_Selector *priority_protocol_selector (void);
  //

  TAO_Client_Priority_Policy_Selector *client_priority_policy_selector (void);
  //
#endif /* TAO_HAS_RT_CORBA == 1 */

#if (TAO_HAS_CORBA_MESSAGING == 1)

  TAO_Policy_Manager *policy_manager (void);
  // Return the Policy_Manager for this ORB.

  TAO_Policy_Current &policy_current (void);
  // Accesors to the policy current, this object should be kept in TSS
  // storage.  The POA has to reset the policy current object on every
  // upcall.

  CORBA::Policy_ptr get_default_policy (
      CORBA::PolicyType policy,
      CORBA::Environment &ACE_TRY_ENV);
  // Accesor to obtain the default policy for a particular policy
  // type.  If there is no default policy it returns
  // CORBA::Policy::_nil ()

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  CORBA::Policy *default_relative_roundtrip_timeout (void) const;

  CORBA::Policy *stubless_relative_roundtrip_timeout (void);
  // Access to the RoundtripTimeoutPolicy policy set on the thread or
  // on the ORB.  In this method, we do not consider the stub since we
  // do not have access to it.

  void call_timeout_hook (TAO_Stub *stub,
                          int &has_timeout,
                          ACE_Time_Value &time_value);

  typedef void (*Timeout_Hook) (TAO_ORB_Core *,
                                TAO_Stub *,
                                int&,
                                ACE_Time_Value&);

  static void set_timeout_hook (Timeout_Hook hook);

  static Timeout_Hook timeout_hook_;
  // The hook to be set for the RelativeRoundtripTimeoutPolicy

#if (TAO_HAS_CLIENT_PRIORITY_POLICY == 1)

  TAO_Client_Priority_Policy *default_client_priority (void) const;

#endif /* TAO_HAS_CLIENT_PRIORITY_POLICY == 1 */

  CORBA::Policy *default_sync_scope (void) const;

  void call_sync_scope_hook (TAO_Stub *stub,
                             int &has_synchronization,
                             int &scope);

  TAO_Sync_Strategy &get_sync_strategy (TAO_Stub *stub,
                                        int &scope);

  typedef void (*Sync_Scope_Hook) (TAO_ORB_Core *, TAO_Stub *, int&, int&);

  static void set_sync_scope_hook (Sync_Scope_Hook hook);

  void stubless_sync_scope (CORBA::Policy *&result);

  static Sync_Scope_Hook sync_scope_hook_;
  // The hook to be set for the SyncScopePolicy

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  TAO_Buffering_Constraint_Policy *default_buffering_constraint (void) const;

  // = This strategy will buffer messages.
  TAO_Eager_Buffering_Sync_Strategy &eager_buffering_sync_strategy (void);
  TAO_Delayed_Buffering_Sync_Strategy &delayed_buffering_sync_strategy (void);

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

  TAO_Transport_Sync_Strategy &transport_sync_strategy (void);
  // This strategy will sync with the transport.

#if (TAO_HAS_RT_CORBA == 1)

  TAO_RT_ORB *rt_orb (void);
  // Access the RTORB.

  TAO_RT_Current *rt_current (void);
  // Access the RT Current.

  TAO_Priority_Mapping_Manager *priority_mapping_manager (void);
  // Access the priority mapping manager class.  This is a TAO extension but
  // there is no standard for setting priority mapping either.

  // = Methods for obtaining ORB implementation default values for RT
  //   policies.

  TAO_PrivateConnectionPolicy *default_private_connection (void) const;

  TAO_PriorityBandedConnectionPolicy *
  default_priority_banded_connection (void) const;

  TAO_ClientProtocolPolicy *default_client_protocol (void) const;

  TAO_ServerProtocolPolicy *default_server_protocol (void) const;

  TAO_ThreadpoolPolicy *default_threadpool (void) const;

  TAO_PriorityModelPolicy *default_priority_model (void) const;

  // = Methods for obtaining effective ORB-level overrides for
  //   policies available only at the POA/ORB levels, and unavailable
  //   at Object/Current levels.
  //
  //   First check for an override at the ORB scope; if nothing there,
  //   check the ORB implementation default values.

  TAO_ThreadpoolPolicy *threadpool (void);

  TAO_PriorityModelPolicy *priority_model (void);

  TAO_ServerProtocolPolicy *server_protocol (void);

#endif /* TAO_HAS_RT_CORBA == 1 */

  int get_thread_priority (CORBA::Short &priority);
  int set_thread_priority (CORBA::Short  priority);
  // Accessor and modifier to the current thread priority, used to
  // implement the RTCORBA::Current interface, but it is faster for
  // some critical components.
  // If TAO_HAS_RT_CORBA == 0, the operations are noops.

  TAO_ORB_Core_TSS_Resources* get_tss_resources (void);
  // Obtain the TSS resources of this orb.

  TAO_Leader_Follower &leader_follower (void);
  // Get access to the leader_follower class.

  int run (ACE_Time_Value *tv,
           int perform_work,
           CORBA::Environment &ACE_TRY_ENV);
  // Run the event loop

  void shutdown (CORBA::Boolean wait_for_completion,
                 CORBA::Environment &ACE_TRY_ENV);
  // End the event loop

  int has_shutdown (void);
  // Get the shutdown flag value

  void destroy (CORBA::Environment &ACE_TRY_ENV);
  // Shutdown the ORB and free resources

  void check_shutdown (CORBA::Environment &ACE_TRY_ENV);
  // Check if ORB has shutdown.  If it has, throw an exception.

  int thread_per_connection_timeout (ACE_Time_Value &timeout) const;
  // Returns the <timeout> value used by the server threads to poll
  // the shutdown flag.
  // If the return value is zero then the server threads block forever.

  ACE_SYNCH_CONDITION* leader_follower_condition_variable (void);
  // Condition variable used in the Leader Follower Wait Strategy, on
  // which the follower thread blocks.

  TAO_Stub *create_stub_object (const TAO_ObjectKey &key,
                                const char *type_id,
                                CORBA::PolicyList *policy_list,
                                TAO_Acceptor_Filter *filter,
                                CORBA::Environment &ACE_TRY_ENV);
  // Makes sure that the ORB is open and then creates a TAO_Stub
  // based on the endpoint.

  CORBA::Object_ptr create_object (TAO_Stub *the_stub);
  // Create a new object, use the adapter registry to create a
  // collocated object, if not possible then create a regular object.

  const char *orbid (void) const;
  // Return ORBid string.

  CORBA::Object_ptr implrepo_service (void);
  void implrepo_service (const CORBA::Object_ptr ir);
  // Set/Get the IOR of the Implementation Repository service.

  CORBA::Object_ptr resolve_typecodefactory (CORBA::Environment &ACE_TRY_ENV);
  // Resolve the TypeCodeFactory DLL.

  CORBA::Object_ptr resolve_dynanyfactory (CORBA::Environment &ACE_TRY_ENV);
  // Resolve the Dynamic Any Factory

  CORBA::Object_ptr resolve_ior_manipulation (CORBA::Environment&);
  // Resolve the IOR Manipulation reference for this ORB.

  CORBA::Object_ptr resolve_ior_table (CORBA::Environment&);
  // Resolve the IOR Table reference for this ORB.

  CORBA::Object_ptr resolve_rir (const char *name,
                                 CORBA::Environment &);
  // Resolve an initial reference via the -ORBInitRef and
  // -ORBDefaultInitRef options

  CORBA_ORB_ObjectIdList_ptr list_initial_references (CORBA::Environment &);
  // List all the service known by the ORB

  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);
  // Reference counting...

  int register_handle (ACE_HANDLE handle);
  // Register the handle of an open connection with the ORB Core
  // handle set.  This handle set will be used to explicitly remove
  // corresponding event handlers from the reactor.

  int remove_handle (ACE_HANDLE handle);
  // Remove <handle> from the ORB Core's handle set so that it
  // isn't included in the set that is passed to the reactor upon ORB
  // destruction.

  // The following methods would represent the hooks in the ORB
  // Core. These hooks would be used to call back on the services or
  // other features that are dynamically loaded.

  CORBA::Boolean service_profile_selection (TAO_MProfile &mprofile,
                                            TAO_Profile  *&profile);
  // The loaded service in the ORB_Core would determine if the profile
  // selection is going to be made by the services or not. If the
  // services do make the selection they would return the selected
  // profile through <profile>

  CORBA::Boolean service_profile_reselection (TAO_Stub *stub,
                                              TAO_Profile *&profile);
  // The loaded service in the ORB_Core would determine if the profile
  // reselection is going to be made by the services or not. If the
  // services do make the reselection they would return the selected
  // profile through <profile>. The reselction is for the
  // multi-profile IORS.

  void reset_service_profile_flags (void);
  // Reset the flags in the loaded services.

  CORBA::Boolean object_is_nil (CORBA::Object_ptr object);
  // The loaded service would determineif the CORBA::Object_ptr is
  // actually nill or not. This would be useful to accomodate new
  // enhanced definitions as defined by the service specification.

  CORBA::Policy_ptr service_create_policy (CORBA::PolicyType policy,
                                           const CORBA::Any &val,
                                           CORBA::Environment &ACE_TRY_ENV);
  // The create_policy () method that is delegated to the service
  // layer. This method would call the loaded services to check
  // whether they can create the policy object requested by the
  // application.
  // @@ This method should go away in favor of the policy factory
  //    registration support provided by the Portable Interceptor
  //    spec.


  void service_context_list (TAO_Stub *&stub,
                             IOP::ServiceContextList &service_list,
                             CORBA::Boolean retstart,
                             CORBA::Environment &ACE_TRY_ENV);
  // Call the service layers with the IOP::ServiceContext to check
  // whether they would like to add something to the list.

  TAO_Fault_Tolerance_Service &fault_tolerance_service (void);
  // Return a reference to the Fault Tolerant service object

  int service_raise_comm_failure (TAO_GIOP_Invocation *invoke,
                                  TAO_Profile *profile,
                                  CORBA::Environment &ACE_TRY_ENV);
  // Raise a comm failure exception if a service is not loaded, else
  // delegate to the service to see what the service has to do for
  // this case.

  int service_raise_transient_failure (TAO_GIOP_Invocation *invoke,
                                       TAO_Profile *profile,
                                       CORBA::Environment &ACE_TRY_ENV);
  // Raise a transient failure exception if a service is not loaded, else
  // delegate to the service to see what the service has to do for
  // this case.

#if TAO_HAS_INTERCEPTORS == 1

  void add_interceptor (
    PortableInterceptor::ClientRequestInterceptor_ptr interceptor,
    CORBA_Environment &ACE_TRY_ENV);
  ///< Register a client request interceptor.

  void add_interceptor (
    PortableInterceptor::ServerRequestInterceptor_ptr interceptor,
    CORBA_Environment &ACE_TRY_ENV);
  ///< Register a server request interceptor.

  TAO_ClientRequestInterceptor_List::TYPE &
    client_request_interceptors (void);
  ///< Return the array of client-side interceptors specific to
  ///< this ORB.

  TAO_ServerRequestInterceptor_List::TYPE &
    server_request_interceptors (void);
  ///< Return the array of server-side interceptors specific to
  ///< this ORB.

#endif /* TAO_HAS_INTERCEPTORS */

  int open (CORBA::Environment &ACE_TRY_ENV);
  // Set up the ORB Core's acceptor to listen on the
  // previously-specified port for requests.  Returns -1 on failure,
  // else 0.

  TAO_Connection_Cache_Manager &connection_cache (void);
  // Return the underlying connection cache

protected:

  ~TAO_ORB_Core (void);
  // Destructor is protected since the ORB Core should only be
  // allocated on the heap.

  int init (int &argc, char **argv, CORBA::Environment &ACE_TRY_ENV);
  // Initialize the guts of the ORB Core.  It is intended that this be
  // called by <CORBA::ORB_init>.

  int fini (void);
  // Final termination hook, typically called by CORBA::ORB's DTOR.

  ACE_Allocator *input_cdr_dblock_allocator_i (TAO_ORB_Core_TSS_Resources *);
  ACE_Allocator *input_cdr_buffer_allocator_i (TAO_ORB_Core_TSS_Resources *);
  // Implement the input_cdr_*_allocator() routines using pre-fetched
  // TSS resources.  This minimizes the number of calls to them.

  int set_default_policies (void);
  // Set ORB-level policy defaults for this ORB.  Currently sets
  // default RTCORBA policies: ServerProtocolPolicy &
  // ClientProtocolPolicy.

  void resolve_typecodefactory_i (CORBA::Environment &ACE_TRY_ENV);
  // Obtain and cache the dynamic any factory object reference

  void resolve_dynanyfactory_i (CORBA::Environment &ACE_TRY_ENV);
  // Obtain and cache the dynamic any factory object reference

  void resolve_iormanipulation_i (CORBA::Environment &ACE_TRY_ENV);
  // Obtain and cache the IORManipulation factory object reference

  void services_callbacks_init (void);
  // Search the Dynamic service list for well known services that has
  // callbacks  which can be dynamically loaded.

private:

  // The ORB Core should not be copied.
  ACE_UNIMPLEMENTED_FUNC (TAO_ORB_Core(const TAO_ORB_Core&))
  ACE_UNIMPLEMENTED_FUNC (void operator=(const TAO_ORB_Core&))

  void resolve_ior_table_i (CORBA::Environment &ACE_TRY_ENV);
  // Obtain and cache the dynamic any factory object reference.

  CORBA::Object_ptr create_collocated_object (TAO_Stub *the_stub,
                                              TAO_ORB_Core *other_orb,
                                              const TAO_MProfile &mprofile);
  // Try to create a new collocated object, using <other_orb> as the
  // target ORB.  If not possible return 0.

protected:
  ACE_SYNCH_MUTEX lock_;
  // Synchronize internal state...

  // = Data members.

  TAO_Connector_Registry *connector_registry_;
  // The connector registry which all active connectors must register
  // themselves with.

  TAO_Acceptor_Registry *acceptor_registry_;
  // The registry which maintains a list of acceptor factories for each
  // loaded protocol.

  TAO_ProtocolFactorySet *protocol_factories_;
  // Pointer to the list of protocol loaded into this ORB instance.

  CORBA::Object_ptr implrepo_service_;
  // The cached IOR for the Implementation Repository.
  // @@ If this is a _var, where should it get deleted? (brunsch)

  int use_implrepo_;
  // Flag for whether the implrepo support is enabled or not.

  CORBA::Object_ptr typecode_factory_;
  // The cached IOR for the TypeCodeFactory DLL.

  CORBA::Object_ptr dynany_factory_;
  // The cached object reference for the DynAnyFactory.

  CORBA::Object_ptr ior_manip_factory_;
  // The cached object reference for the IORManipulataion.

  CORBA::Object_ptr ior_table_;
  // The cached object reference for the IORTable

  CORBA::ORB_var orb_;
  // @@ Should we keep a single ORB pointer? This is good because
  //    multiple calls to ORB_init() with the same ORBid can use the
  //    same object, but maybe don't want so much coupling.
  // Pointer to the ORB.

  CORBA::Object_var root_poa_;
  // Pointer to the root POA.  It will eventually be the pointer
  // returned by calls to <CORBA::ORB::resolve_initial_references
  // ("RootPOA")>.

  TAO_ORB_Parameters orb_params_;
  // Parameters used by the ORB.

  typedef ACE_Hash_Map_Manager<ACE_CString,ACE_CString,ACE_Null_Mutex> InitRefMap;
  InitRefMap init_ref_map_;

  char *orbid_;
  // The ORBid for this ORB.

  TAO_Resource_Factory *resource_factory_;
  // Handle to the factory for resource information..

  CORBA::Boolean resource_factory_from_service_config_;
  // TRUE if <resource_factory_> was obtained from the Service
  // Configurator.
  // @@ This is not needed since the default resource factory
  //    is staticaly added to the service configurator.

  TAO_Client_Strategy_Factory *client_factory_;
  // Handle to the factory for Client-side strategies.

  CORBA::Boolean client_factory_from_service_config_;
  // TRUE if <client_factory_> was obtained from the Service
  // Configurator.
  // @@ This is not needed since the client facotry factory
  //    is staticaly added to the service configurator.

  TAO_Server_Strategy_Factory *server_factory_;
  // Handle to the factory for Server-side strategies.

  CORBA::Boolean server_factory_from_service_config_;
  // TRUE if <server_factory_> was obtained from the Service
  // Configurator.
  // @@ This is not needed since the server factory factory
  //    is staticaly added to the service configurator.

  // Start of service level hooks

  TAO_Fault_Tolerance_Service ft_service_;
  // Fault Tolerant service hook.

  // End of Service level hooks

  CORBA::Boolean opt_for_collocation_;
  // TRUE if we want to take advantage of collocation optimization in
  // this ORB.

  CORBA::Boolean use_global_collocation_;
  // TRUE if we want to consider all ORBs in this address space
  // collocated.

  CORBA::ULong collocation_strategy_;
  // Default collocation policy.  This should never be ORB_CONTROL.

  #if (TAO_HAS_CORBA_MESSAGING == 1)

  TAO_Policy_Manager *policy_manager_;
  // The Policy_Manager for this ORB.

  TAO_Policy_Manager_Impl *default_policies_;
  // The default policies.

  TAO_Policy_Current *policy_current_;
  // Policy current.

  #endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  CORBA::Object_var poa_current_;
  // POA current.
  //
  // Note that this is a pointer in order to reduce the include file
  // dependencies.
  //

  TAO_Adapter_Registry adapter_registry_;
  // The list of Adapters used in this ORB

  TAO_Adapter *poa_adapter_;
  // An optimization for the POA

  ACE_Thread_Manager tm_;
  // The Thread Manager

  ACE_Lock_Adapter<ACE_SYNCH_MUTEX> data_block_lock_;
  // The data block reference counts are locked using this mutex

  ACE_Char_Codeset_Translator *from_iso8859_;
  ACE_Char_Codeset_Translator *to_iso8859_;
  ACE_WChar_Codeset_Translator *from_unicode_;
  ACE_WChar_Codeset_Translator *to_unicode_;
  // Codeset translators for simple implementations.

  int use_tss_resources_;
  // If 1 then this ORB uses thread-specific resources

  ACE_TSS_TYPE (TAO_ORB_Core_TSS_Resources) tss_resources_;
  // This is where the tss resources for this ORB are stored.

  TAO_ORB_Core_TSS_Resources orb_resources_;
  // If the resources are per-ORB (as opposed to per-ORB-per-thread)
  // then they are stored here...

  TAO_Reactor_Registry *reactor_registry_;
  // The server concurrency strategy

  ACE_Reactor *reactor_;
  // The reactor used for pure-clients, otherwise it comes from the
  // reactor_registry

  int has_shutdown_;
  // Flag which denotes that the ORB has been shutdown.

  int thread_per_connection_use_timeout_;
  ACE_Time_Value thread_per_connection_timeout_;
  // The value of the timeout if the flag above is not zero

  ACE_SYNCH_MUTEX open_lock_;
  // Mutual exclusion for calling open.

  int open_called_;
  // Flag which denotes that the open method was called.


  TAO_Endpoint_Selector_Factory *endpoint_selector_factory_;
  //

  TAO_Default_Endpoint_Selector* default_endpoint_selector_;
  //

#if (TAO_HAS_RT_CORBA == 1)

  TAO_Priority_Endpoint_Selector *priority_endpoint_selector_;
  //

  TAO_Protocol_Endpoint_Selector *protocol_endpoint_selector_;
  //

  TAO_Priority_Protocol_Selector *priority_protocol_selector_;
  //

  TAO_Client_Priority_Policy_Selector
  *client_priority_policy_selector_;
  //

  TAO_RT_ORB *rt_orb_;
  // Implementation of RTCORBA::RTORB interface.

  TAO_RT_Current *rt_current_;
  // Implementation of RTCORBA::RTCurrent interface.

  TAO_Priority_Mapping_Manager *priority_mapping_manager_;
  // Manager for setting priority mapping.

#endif /* TAO_HAS_RT_CORBA == 1 */

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  TAO_Eager_Buffering_Sync_Strategy *eager_buffering_sync_strategy_;
  // This strategy will buffer messages.

  TAO_Delayed_Buffering_Sync_Strategy *delayed_buffering_sync_strategy_;
  // This strategy will buffer messages.

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

  TAO_Transport_Sync_Strategy *transport_sync_strategy_;
  // This strategy will sync with the transport.

  int svc_config_argc_;
  // The number of arguments in the service configurator argument vector.

  char **svc_config_argv_;
  // The argument vector for the service configurator.

  CORBA::ULong refcount_;
  // Number of outstanding references to this object.

  TAO_PolicyFactory_Registry policy_factory_registry_;
  ///< Registry containing all registered policy factories.

#if (TAO_HAS_INTERCEPTORS == 1)
  TAO_ClientRequestInterceptor_List client_request_interceptors_;
  TAO_ServerRequestInterceptor_List server_request_interceptors_;
  ///< Interceptor registries.
#endif /* TAO_HAS_INTERCEPTORS */

  TAO_Parser_Registry parser_registry_;
  // The IOR parser registry

  TAO_Connection_Cache_Manager connection_cache_;
  // The connection cache for TAO
};

// ****************************************************************

// Define a TAO_ORB_Core auto_ptr class
class TAO_ORB_Core_Auto_Ptr
{
  // = TITLE
  //     Implements the draft C++ standard auto_ptr abstraction.
  //     This class allows one to work ORB_Core Objects *Only*!
public:
  // = Initialization and termination methods
  /* explicit */ TAO_ORB_Core_Auto_Ptr (TAO_ORB_Core *p = 0);
  TAO_ORB_Core_Auto_Ptr (TAO_ORB_Core_Auto_Ptr &ap);
  TAO_ORB_Core_Auto_Ptr &operator= (TAO_ORB_Core_Auto_Ptr &rhs);
  ~TAO_ORB_Core_Auto_Ptr (void);

  // = Accessor methods.
  TAO_ORB_Core &operator *() const;
  TAO_ORB_Core *get (void) const;
  TAO_ORB_Core *release (void);
  void reset (TAO_ORB_Core *p = 0);
  TAO_ORB_Core *operator-> () const;

protected:
  TAO_ORB_Core *p_;
};

// ****************************************************************

class TAO_Export TAO_TSS_Resources
{
  // = TITLE
  //   The TSS resoures shared by all the ORBs
  //
  // = DESCRIPTION
  //   This class is used by TAO to store the resources that are
  //   thread-specific but are *not* ORB specific...
  //   The members are public because only the ORB Core is expected to
  //   access them.
  //
public:
  TAO_TSS_Resources (void);
  // constructor

  ~TAO_TSS_Resources (void);
  // destructor

private:
  // Do not copy TSS resources
  ACE_UNIMPLEMENTED_FUNC (TAO_TSS_Resources(const TAO_TSS_Resources&))
  ACE_UNIMPLEMENTED_FUNC (void operator=(const TAO_TSS_Resources&))

public:

  void *poa_current_impl_;
  // Points to structure containing state for the current upcall
  // context in this thread.  Note that it does not come from the
  // resource factory because it must always be held in
  // thread-specific storage.  For now, since TAO_ORB_Core instances
  // are TSS singletons, we simply ride along and don't allocate
  // occupy another TSS slot since there are some platforms where
  // those are precious commodities (e.g., NT).

  CORBA_Environment* default_environment_;
  // The default environment for the thread.

  CORBA_Environment tss_environment_;
  // If the user (or library) provides no environment the ORB_Core
  // still holds one.

#if (TAO_HAS_CORBA_MESSAGING == 1)

  TAO_Policy_Current_Impl initial_policy_current_;
  // The initial PolicyCurrent for this thread. Should be a TSS
  // resource.

  TAO_Policy_Current_Impl *policy_current_;
  // This pointer is reset by the POA on each upcall.

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

};

// @@ Must go away....
typedef TAO_TSS_Singleton<TAO_TSS_Resources, ACE_SYNCH_MUTEX>
        TAO_TSS_RESOURCES;

// ****************************************************************

TAO_Export TAO_ORB_Core *TAO_ORB_Core_instance (void);

#if defined (__ACE_INLINE__)
# include "tao/ORB_Core.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_ORB_CORE_H */
