// -*- C++ -*-

// ===================================================================
/**
 *  @file   ORB_Core.h
 *
 *  $Id$
 *
 *  @author DOC Center - Washington University at St. Louis
 *  @author DOC Laboratory - University of California at Irvine
 */
// ===================================================================

#ifndef TAO_ORB_CORE_H
#define TAO_ORB_CORE_H

#include /**/ "ace/pre.h"

#include "tao/Resource_Factory.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Collocation_Strategy.h"
#include "tao/params.h"
#include "tao/ORB_Constants.h"
#include "tao/Parser_Registry.h"
#include "tao/Service_Callbacks.h"
#include "tao/Fault_Tolerance_Service.h"
#include "tao/Cleanup_Func_Registry.h"
#include "tao/Object_Ref_Table.h"
#include "tao/ObjectKey_Table.h"
#include "tao/Messaging_SyncScopeC.h"
#include "tao/Object.h"
#include "tao/Invocation_Utils.h"
#include "tao/Adapter_Registry.h"
#include "tao/ORB_Core_TSS_Resources.h"
#include "tao/Service_Context_Handler_Registry.h"
#include "ace/Array_Map.h"

#include "ace/Thread_Manager.h"
#include "ace/Lock_Adapter_T.h"
#include "ace/TSS_T.h"

#include "ace/Service_Config.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Data_Block;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#if TAO_HAS_INTERCEPTORS == 1

namespace TAO
{
  class ClientRequestInterceptor_Adapter;
  class ServerRequestInterceptor_Adapter;
}

#endif  /* TAO_HAS_INTERCEPTORS == 1  */

class TAO_Adapter;
class TAO_Acceptor;
class TAO_Connector;
class TAO_Connector_Registry;

class TAO_Resource_Factory;
class TAO_Client_Strategy_Factory;
class TAO_Server_Strategy_Factory;

class TAO_TSS_Resources;
class TAO_Leader_Follower;
class TAO_LF_Strategy;
class TAO_RT_ORB;
class TAO_MProfile;
class TAO_Profile;

class TAO_Endpoint_Selector_Factory;
class TAO_Message_State_Factory;
class TAO_Protocols_Hooks;
class TAO_Network_Priority_Protocols_Hooks;
class TAO_BiDir_Adapter;
class TAO_ZIOP_Adapter;

class TAO_Flushing_Strategy;

class TAO_Thread_Lane_Resources_Manager;
class TAO_Collocation_Resolver;
class TAO_Thread_Lane_Resources;
class TAO_Stub_Factory;
class TAO_Endpoint_Selector_Factory;
class TAO_Service_Context;
class TAO_Request_Dispatcher;
class TAO_Policy_Set;
class TAO_Policy_Manager;
class TAO_Policy_Current;

class TAO_Codeset_Manager;
class TAO_IORInterceptor_Adapter;
class TAO_Valuetype_Adapter;

class TAO_Policy_Validator;

namespace TAO
{
  class GUIResource_Factory;
  class PolicyFactory_Registry_Adapter;
  class ORBInitializer_Registry_Adapter;
  class Transport_Queueing_Strategy;
}

namespace CORBA
{
  class ORB_ObjectIdList;  // CORBA::ORB::ObjectIdList
  class ORB;
  typedef ORB *ORB_ptr;

  class PolicyList;
}

namespace IOP
{
  class ServiceContextList;
}

namespace PortableInterceptor
{
  class IORInterceptor;
  typedef IORInterceptor *IORInterceptor_ptr;

#if TAO_HAS_INTERCEPTORS == 1

  class ClientRequestInterceptor;
  typedef ClientRequestInterceptor *ClientRequestInterceptor_ptr;

  class ServerRequestInterceptor;
  typedef ServerRequestInterceptor *ServerRequestInterceptor_ptr;

#endif  /* TAO_HAS_INTERCEPTORS == 1  */
}

// ****************************************************************
/**
 * @class TAO_ORB_Core
 *
 * @brief Encapsulates the state of an ORB.
 *
 * This is the implementation class for the CORBA::ORB interface.  The
 * class also encapsulates the access to the ORB resources and its
 * state.
 * @par
 * Some resources can be TSS or global, those resources are always
 * accessed through a TSS interface, but are allocated using the
 * Resource_Factory.  If the resource is really global the
 * Resource_Factory will simply return a pointer to the global
 * instance.
 */
class TAO_Export TAO_ORB_Core
{
  friend class TAO_ORB_Core_Auto_Ptr;
  friend TAO_Export CORBA::ORB_ptr CORBA::ORB_init (int &,
                                         ACE_TCHAR *argv[],
                                         const char *);
public:

  /// Constructor.
  TAO_ORB_Core (const char* id, ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> g);

  /// Accessor for the ORB parameters.
  TAO_ORB_Parameters *orb_params (void);

  /**
   * @todo
   * In the future this hook should change, instead of hardcoding the
   * object we should add a "Resolver" to the ORB, so the "POACurrent"
   * object returns a per-ORB object.
   * @par
   * Similarly, each ORB should implement the TSS pattern to put the
   * POA_Current_Impl in a void* slot.  The current approach *does*
   * decouple the POA from the ORB, but it cannot add new adapters or
   * other components transparently.
   */
  /// Accessor to the POA current.
  //@{
  CORBA::Object_ptr poa_current(void);
  //@}

  /// Get the connector registry
  TAO_Connector_Registry *connector_registry (void);

  /// Get the IOR parser registry
  TAO_Parser_Registry *parser_registry (void);

  /// Return pointer to the policy factory registry associated with
  /// this ORB core.
  TAO::PolicyFactory_Registry_Adapter *policy_factory_registry (void);

  /// Return pointer to the orb initializer registry associated with
  /// this ORB core. Tries to load the PI library if it is not loaded
  /// yet
  TAO::ORBInitializer_Registry_Adapter *orbinitializer_registry (void);

  TAO_ZIOP_Adapter *ziop_adapter () const;
  void ziop_adapter (TAO_ZIOP_Adapter *adapter);

  TAO_Service_Context_Registry &service_context_registry (void);

  /// Get the protocol factories
  TAO_ProtocolFactorySet *protocol_factories (void);

  /// Get pointer to the ORB.
  CORBA::ORB_ptr orb (void);

  /// Wrappers that forward the request to the concurrency strategy.
  ACE_Reactor *reactor (void);

  /// Get the ACE_Thread_Manager
  ACE_Thread_Manager *thr_mgr (void);

  /// Return the RootPOA, or try to load it if not initialized already.
  CORBA::Object_ptr root_poa (void);

  /// Get the adapter registry
  TAO_Adapter_Registry &adapter_registry (void);

  /// @name Collocation Strategies
  //@{
  enum
  {
    /// Indicate object should refer to ORB for either one of the
    /// following strategies.
    ORB_CONTROL,

    /// Collocated calls will go thru POA.
    THRU_POA,

    /// Collocated calls invoke operation on Servant directly.
    DIRECT
  };

  /**
   * This method returns the right collocation strategy, if any,
   * to be used to perform a method invocation on the given object.
   *
   * @note
   * No-Collocation is a special case of collocation.
   */
  static
  TAO::Collocation_Strategy collocation_strategy (CORBA::Object_ptr object);
  //@}

  /// Set/get the collocation flags
  //@{
  void optimize_collocation_objects (CORBA::Boolean opt);
  CORBA::Boolean optimize_collocation_objects (void) const;

  void use_global_collocation (CORBA::Boolean opt);
  CORBA::Boolean use_global_collocation (void) const;

  CORBA::ULong get_collocation_strategy (void) const;
  //@}

  /// Get the adapter named "RootPOA" and cache the result, this is an
  /// optimization for the POA.
  TAO_Adapter *poa_adapter (void);

  /**
   * @name Access to Factories
   *
   * These factories are not thread-specific, and are presented here
   * in order to have one place to get useful information.  Often, the
   * instances to which the return pointers are stored in the Service
   * Repository.
   */
  //@{
  /// Returns pointer to the resource factory.
  TAO_Resource_Factory *resource_factory (void);

  /// Returns pointer to the factory for creating gui resources
  TAO::GUIResource_Factory *gui_resource_factory (void);

  /// Returns pointer to the client factory.
  TAO_Client_Strategy_Factory *client_factory (void);

  /// Returns pointer to the server factory.
  TAO_Server_Strategy_Factory *server_factory (void);

  /// Returns pointer to the Protocols_Hooks.
  TAO_Protocols_Hooks *protocols_hooks (void);

  /// Returns a pointer to the Thread Lane Resources Manager.
  TAO_Thread_Lane_Resources_Manager &thread_lane_resources_manager (void);

  /// Returns a pointer to the Collocation Resolver.
  TAO_Collocation_Resolver &collocation_resolver (void);

  /// Returns a pointer to the Stub factory.
  TAO_Stub_Factory *stub_factory (void);

  /// Returns a pointer to the endpoint selector factory.
  TAO_Endpoint_Selector_Factory *endpoint_selector_factory (void);

  //@}

  /// Sets the value of TAO_ORB_Core::resource_factory_
  static void set_resource_factory (const char *resource_factory_name);

  /** Sets the value of TAO_ORB_Core::gui_resource_factory_.
   *
   *  Sets the value of gui_resource_factory in TSS. ORB_Core is responsible
   *  for releasing this factory if needed.
   */
  static void set_gui_resource_factory (TAO::GUIResource_Factory *gui_factory);

  /// Sets the value of TAO_ORB_Core::network_priority_protocols_hooks_
  static void set_network_priority_protocols_hooks (
    const char *network_priority_protocols_hooks_name);

  /// Gets the value of TAO_ORB_Core::protocols_hooks__
  TAO_Protocols_Hooks * get_protocols_hooks (void);

  /// Gets the value of TAO_ORB_Core::network_priority_protocols_hooks__
  TAO_Network_Priority_Protocols_Hooks *
    get_network_priority_protocols_hooks (void);

  /// Sets the value of TAO_ORB_Core::dynamic_adapter_name_.
  static void dynamic_adapter_name (const char *name);

  /// Gets the value of TAO_ORB_Core::dynamic_adapter_name_.
  static const char *dynamic_adapter_name (void);

  /// Sets the value of TAO_ORB_Core::ifr_client_adapter_name_.
  static void ifr_client_adapter_name (const char *name);

  /// Gets the value of TAO_ORB_Core::ifr_client_adapter_name_.
  static const char *ifr_client_adapter_name (void);

  /// Sets the value of TAO_ORB_Core::typecodefactory_adapter_name_.
  static void typecodefactory_adapter_name (const char *name);

  /// Gets the value of TAO_ORB_Core::typecodefactory_adapter_name_.
  static const char *typecodefactory_adapter_name (void);

  /// Sets the value of TAO_ORB_Core::iorinterceptor_adapter_factory_name_.
  static void iorinterceptor_adapter_factory_name (const char *name);

  /// Gets the value of TAO_ORB_Core::iorinterceptor_adapter_factory_name_.
  static const char *iorinterceptor_adapter_factory_name (void);

  /// Sets the value of TAO_ORB_Core::valuetype_adapter_factory_name.
  static void valuetype_adapter_factory_name (const char *name);

  /// Gets the value of TAO_ORB_Core::valuetype_adapter_factory_name.
  static const char *valuetype_adapter_factory_name (void);

  /// See if we have a collocated address, if yes, return the POA
  /// associated with the address.
  int is_collocated (const TAO_MProfile& mprofile);

  /// This allocator is always TSS and has no locks. It is intended
  /// for allocating the ACE_Data_Blocks used in *outgoing* CDR
  /// streams.
  ACE_Allocator *output_cdr_dblock_allocator (void);

  /// This allocator is always TSS and has no locks. It is intended
  /// for allocating the buffers used in *outgoing* CDR streams.
  ACE_Allocator *output_cdr_buffer_allocator (void);

  /// This allocator is always TSS and has no locks. It is intended
  /// for allocating the ACE_Data_Blocks used in *outgoing* CDR
  /// streams.
  ACE_Allocator *output_cdr_msgblock_allocator (void);

  /// This allocator is global, may or may not have locks. It is
  /// intended for allocating the ACE_Data_Blocks used in *incoming*
  /// CDR streams.
  ACE_Allocator *input_cdr_dblock_allocator (void);

  /// This allocator is always global and has no locks. It is intended
  /// for allocating the buffers used in *incoming* CDR streams.
  ACE_Allocator *input_cdr_buffer_allocator (void);

  /// This allocator is always global and has no locks. It is intended
  /// for allocating the buffers used in *incoming* CDR streams.
  ACE_Allocator *input_cdr_msgblock_allocator (void);

  /// This allocator is always global and has no locks. It is intended
  /// for allocating the buffers used to queue messages in
  /// transports.
  ACE_Allocator *transport_message_buffer_allocator (void);

  /// The Message Blocks used for input CDRs must have appropiate
  /// locking strategies.
  ACE_Data_Block *create_input_cdr_data_block (size_t size);

  /// Return the locking strategy used for the data blocks.
  ACE_Lock *locking_strategy (void);

#if (TAO_HAS_CORBA_MESSAGING == 1)

  /// Accessor method for the default_policies_
  TAO_Policy_Set *get_default_policies (void);

  /// Get a policy.  First, check the ORB-level Policy Manager, then
  /// check the ORB defaults.
  CORBA::Policy_ptr get_policy (CORBA::PolicyType type);

  /// Get a policy.  First, check the thread current, then check the
  /// ORB-level Policy Manager, then check the ORB defaults.
  CORBA::Policy_ptr get_policy_including_current (CORBA::PolicyType type);

  /// Get a cached policy.  First, check the ORB-level Policy Manager,
  /// then check the ORB defaults.
  CORBA::Policy_ptr get_cached_policy (TAO_Cached_Policy_Type type);

  /// Get a cached policy.  First, check the thread current, then
  /// check the ORB-level Policy Manager, then check the ORB defaults.
  CORBA::Policy_ptr get_cached_policy_including_current (
      TAO_Cached_Policy_Type type);

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  /**
   * The thread has a default environment to simplify porting between
   * platforms that support native C++ exceptions and those that
   * don't. This is a TSS resource (always), but with a twist: if the
   * user creates a new environment the old one is "pushed" (actually
   * the new one remembers it), eventually the new environment
   * destructor pops itself from the stack and we recover the old
   * environment.
   * @par
   * This means that if the user create a new environment and somebody
   * calls a function using the default one the exception will still
   * be received in the environment created by the user.
   * The only drawback is that environments life time must nest
   * properly, this shouldn't be a problem because environments are
   * usually created on the stack, but, the spec allows their creation
   * on the heap and/or as class members; we need to investigate the
   * tradeoffs and take a decision.
   */
  //@{
  CORBA::Environment *default_environment (void) const;
  void default_environment (CORBA::Environment*);
  //@}

#if (TAO_HAS_CORBA_MESSAGING == 1)

  /// Return the Policy_Manager for this ORB.
  TAO_Policy_Manager *policy_manager (void);

  /// Accesors to the policy current, this object should be kept in
  /// TSS storage.  The POA has to reset the policy current object on
  /// every upcall.
  TAO_Policy_Current &policy_current (void);

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  /// Invoke the timeout hook if present.
  /**
   * The timeout hook is used to determine if the timeout policy is
   * set and with what value.  If the ORB is compiled without support
   * for Messaging this feature does not take effect
   * \param has_timeout returns 0 if there is no timeout policy set.
   * \param time_value returns the timeout value in effect for the object,
   * thread and current ORB.
   */
  void call_timeout_hook (TAO_Stub *stub,
                          bool &has_timeout,
                          ACE_Time_Value &time_value);

  /// Define the Timeout_Hook signature
  typedef void (*Timeout_Hook) (TAO_ORB_Core *,
                                TAO_Stub *,
                                bool&,
                                ACE_Time_Value&);

  void set_timeout_hook (Timeout_Hook hook);

  /// Invoke the timeout hook if present.
  /**
   * The timeout hook is used to determine if the timeout policy is
   * set and with what value.  If the ORB is compiled without support
   * for Messaging this feature does not take effect
   * \param has_timeout returns 0 if there is no timeout policy set.
   * \param time_value returns the timeout value in effect for the object,
   * thread and current ORB.
   */
  void connection_timeout (TAO_Stub *stub,
                           bool &has_timeout,
                           ACE_Time_Value &time_value);

  /// Define the Timeout_Hook signature
  /**
   * The connection timeout hook was originally defined to allow the
   * TAO Messaging code to be factored out of the core TAO library and
   * placed in to an optional library. Since then, a new invocation
   * endpoint selector, the optimised connection endpoint selector
   * (see Strategies/OC_Endpoint_Selector.h) reused this connection
   * timeout hook. However, this set up a problem when both the
   * Messaging library and OCES are being used in the same
   * application.
   *
   * The solution was to add a new connection timeout hook attribute
   * (see alt_connection_timeout_hook_ below). This method now checks
   * to see if the connection timeout hook is already set, and if so
   * assigns the supplied hook value to the alternate connection
   * timeout hook.  This functionality has a side-effect of assuming
   * that hooks are NEVER unloaded or actively replaced. IOW, no one
   * will call this method with a 0 or some other pointer value to
   * replace an existing hook.
   *
   * If such functionality as unloading a hook pointer is required,
   * then this method must be extended to give some kind of identity
   * for the hook. Additional changes to the definition of the hook
   * will also be necessary to support such identity and manipulation.
   */
  static void connection_timeout_hook (Timeout_Hook hook);

  void call_sync_scope_hook (TAO_Stub *stub,
                             bool &has_synchronization,
                             Messaging::SyncScope &scope);

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  TAO::Transport_Queueing_Strategy *get_transport_queueing_strategy (
    TAO_Stub *stub,
    Messaging::SyncScope &scope);

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

  typedef void (*Sync_Scope_Hook) (TAO_ORB_Core *,
                                   TAO_Stub *,
                                   bool &,
                                   Messaging::SyncScope &);

  void set_sync_scope_hook (Sync_Scope_Hook hook);

  /// Default Sync_Scope_Hook.
  static void default_sync_scope_hook (TAO_ORB_Core *,
                                       TAO_Stub *,
                                       bool &has_synchronization,
                                       Messaging::SyncScope &scope);

  /// Handle to the factory for protocols_hooks_..
  TAO_Protocols_Hooks *protocols_hooks_;

  /// Handle to the factory for network_priority_protocols_hooks_..
  TAO_Network_Priority_Protocols_Hooks *network_priority_protocols_hooks_;

  /// Obtain the TSS resources of this orb.
  TAO_ORB_Core_TSS_Resources* get_tss_resources (void);

  /// Obtain the TSS resource in the given slot.
  void* get_tss_resource (size_t slot_id);

  /// Set the TSS resource at the given slot.
  /// Returns 0 on success, and -1 on failure.
  int set_tss_resource (size_t slot_id, void *);

  /// Register a TSS cleanup function.  The slot ID for the
  /// corresponding ORB core TSS resource is returned by the reference
  /// argument.  This method return 0 on success, and -1 on failure.
  int add_tss_cleanup_func (ACE_CLEANUP_FUNC cleanup, size_t &slot_id);

  /// Cleans up ts_objects using the underlying TSS cleanup function registry.
  void tss_cleanup (ACE_Array_Base<void *> &ts_objects);

  /// Get access to the leader_follower class.
  TAO_Leader_Follower &leader_follower (void);

  /// Get access to the leader follower strategy.
  TAO_LF_Strategy &lf_strategy (void);

  /// Get access to the thread lane resources.
  TAO_Thread_Lane_Resources &lane_resources (void);

  /// Run the event loop.
  int run (ACE_Time_Value *tv, int perform_work);

  /// End the event loop
  void shutdown (CORBA::Boolean wait_for_completion);

  /// Get the shutdown flag value
  bool has_shutdown (void) const;

  /// Shutdown the ORB and free resources
  void destroy (void);

  /// Check if ORB has shutdown.  If it has, throw an exception.
  void check_shutdown (void);

  /// Returns the @a timeout value used by the server threads to poll
  /// the shutdown flag. If the return value is zero then the server
  /// threads block forever.
  int thread_per_connection_timeout (ACE_Time_Value &timeout) const;

  /// Makes sure that the ORB is open and then creates a TAO_Stub
  /// based on the endpoint.
  TAO_Stub *create_stub_object (TAO_MProfile &mprofile,
                                const char *type_id,
                                CORBA::PolicyList *policy_list);

  /// Factory method that create the "right" Stub depending on
  /// wheather RTCORBA is loaded or not. The factory used to create
  /// the stub, is loaded at ORB initialization, and its type depends
  /// on the fact that RTCORBA is being used or not.
  TAO_Stub *create_stub (const char *repository_id,
                         const TAO_MProfile &profiles);

  /// Create a new object, use the adapter registry to create a
  /// collocated object, if not possible then create a regular
  /// object.
  CORBA::Object_ptr create_object (TAO_Stub *the_stub);

  /// Initialize a new object, use the adapter registry to initialize a
  /// collocated object, if not possible then initialize a regular
  /// object.
  /// NOTE: Why would this method be required? The answer is if the
  /// user decides to use lazy initialization of CORBA object, then
  /// this is the route that we have to take to do the
  /// initialization.
  CORBA::Long initialize_object (TAO_Stub *the_stub, CORBA::Object_ptr obj);

  /// Reinitialise a stub after the effective profile has changed.
  /// This will happen after a location forward has been received
  /// or if a location forward supplied new target subsequently fails.
  CORBA::Long reinitialize_object (TAO_Stub *stub);

  /// Return ORBid string.
  const char *orbid (void) const;

  /// Set/Get the IOR of the Implementation Repository service.
  //@{
  CORBA::Object_ptr implrepo_service (void);
  void implrepo_service (const CORBA::Object_ptr ir);
  //@}

  /// Do we attempt to register with the Implementation Repository
  CORBA::Boolean use_implrepo (void);

  /// Do we put the ImR's endpoints into persistent object references we create
  CORBA::Boolean imr_endpoints_in_ior (void);

  /// Resolve the TypeCodeFactory DLL.
  CORBA::Object_ptr resolve_typecodefactory (void);

#if TAO_HAS_INTERCEPTORS == 1
  /// Resolve the PICurrent.
  CORBA::Object_ptr resolve_picurrent (void);
#endif  /* TAO_HAS_INTERCEPTORS == 1 */

  /// Resolve POA Current.
  CORBA::Object_ptr resolve_poa_current (void);

  /// Resolve the CodecFactory DLL.
  CORBA::Object_ptr resolve_codecfactory (void);

  /// Resolve the Compression DLL.
  CORBA::Object_ptr resolve_compression_manager (void);

  /// Resolve the Dynamic Any Factory
  CORBA::Object_ptr resolve_dynanyfactory (void);

  /// Resolve the IOR Manipulation reference for this ORB.
  CORBA::Object_ptr resolve_ior_manipulation (void);

  TAO_ZIOP_Adapter* ziop_adapter_i (void);

  /// Resolve the IOR Table reference for this ORB.
  CORBA::Object_ptr resolve_ior_table (void);

  /// Resolve the Monitor reference for this ORB.
  CORBA::Object_ptr resolve_monitor (void);

  /// Resolve an initial reference via the -ORBInitRef and
  // -ORBDefaultInitRef options.
  CORBA::Object_ptr resolve_rir (const char *name);

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  /// Resolve the RT ORB reference for this ORB.
  CORBA::Object_ptr resolve_rt_orb (void);
#endif

  /// Resolve the RT Current flyweight for this ORB.
  /// Return server_id string.
  const char *server_id (void) const;

  /// List all the service known by the ORB
  CORBA::ORB_ObjectIdList *list_initial_references (void);

  /// Reference counting...
  unsigned long _incr_refcnt (void);
  unsigned long _decr_refcnt (void);
  unsigned long _refcnt (void) const;

  /// Register the handle of an open connection with the ORB Core
  /// handle set.  This handle set will be used to explicitly remove
  /// corresponding event handlers from the reactor.
  int register_handle (ACE_HANDLE handle);

  /// Remove @a handle from the ORB Core's handle set so that it
  /// isn't included in the set that is passed to the reactor upon ORB
  /// destruction.
  int remove_handle (ACE_HANDLE handle);

  /**
   * @name ORB Core Service Hooks
   *
   * These methods would represent the hooks in the ORB Core. These
   * hooks would be used to call back on the services or other
   * features that are dynamically loaded.
   */
  //@{
  /**
   * The loaded service in the ORB_Core would determine if the profile
   * selection is going to be made by the services or not. If the
   * services do make the selection they would return the selected
   * profile through <profile>.
   */
  CORBA::Boolean service_profile_selection (const TAO_MProfile &mprofile,
                                            TAO_Profile  *&profile);

  /**
   * The loaded service would determine if the CORBA::Object_ptr is
   * actually nil or not. This would be useful to accomodate new
   * enhanced definitions as defined by the service specification.
   */
  CORBA::Boolean object_is_nil (CORBA::Object_ptr object);

  /// Hook for the services to determine whether the profiles are
  /// equivalent or not.
  /**
   * For details on how this is used please see the FT service
   */
  TAO_Service_Callbacks::Profile_Equivalence is_profile_equivalent (
                                        const TAO_Profile *this_p,
                                        const TAO_Profile *that_p);

  /// Hook for the services to determine the <hash> value of a
  /// profile.
  /**
   * For details on how this is used please see the FT service
   */
  CORBA::ULong hash_service (TAO_Profile *this_p, CORBA::ULong max);

  /// Return a reference to the Fault Tolerant service object.
  TAO_Fault_Tolerance_Service &fault_tolerance_service (void);

  /// Transmit the entire TAG_FT_GROUP component data from a server IOGR as
  /// a FT_GROUP_VERSION tagged service context on requests to that IOGR instead
  /// of just the group version.
  CORBA::Boolean ft_send_extended_sc (void);

  /// Raise a comm failure exception if a service is not loaded, else
  /// delegate to the service to see what the service has to do for
  /// this case.
  TAO::Invocation_Status service_raise_comm_failure (
      IOP::ServiceContextList &clist,
      TAO_Profile *profile);

  /// Raise a transient failure exception if a service is not loaded,
  /// else delegate to the service to see what the service has to do
  /// for this case.
  TAO::Invocation_Status service_raise_transient_failure (
      IOP::ServiceContextList &clist,
      TAO_Profile *profile);

  //@}

  /**
   * @name Portable Interceptor Related Methods
   *
   * These are support methods for interceptor registration and
   * interceptor set (an array) access, in addition to PICurrent
   * access.
   */
  //@{
#if TAO_HAS_INTERCEPTORS == 1

  /// Return a pointer to the cached TAO::PICurrent object.
  CORBA::Object_ptr pi_current (void);

  /// Set the pointer to the cached TAO::PICurrent object.
  void pi_current (CORBA::Object_ptr current);

  /// Register a client request interceptor.
  void add_interceptor (
    PortableInterceptor::ClientRequestInterceptor_ptr interceptor);

  /// Register a server request interceptor.
  void add_interceptor (
    PortableInterceptor::ServerRequestInterceptor_ptr interceptor);

  /// Register a client request interceptor with policies.
  void add_interceptor (
    PortableInterceptor::ClientRequestInterceptor_ptr interceptor,
    const CORBA::PolicyList& policies);

  /// Register a server request interceptor with policies.
  void add_interceptor (
    PortableInterceptor::ServerRequestInterceptor_ptr interceptor,
    const CORBA::PolicyList& policies);

  /// Get the Client Request Interceptor adapter.
  /// Will not create a new one if not available yet.
  TAO::ClientRequestInterceptor_Adapter *clientrequestinterceptor_adapter (void);

  /// Get the Server Request Interceptor adapter.
  /// Will not create a new one if not available yet.
  TAO::ServerRequestInterceptor_Adapter *serverrequestinterceptor_adapter (void);

#endif /* TAO_HAS_INTERCEPTORS */

  /// Register an IOR interceptor.
  void add_interceptor (PortableInterceptor::IORInterceptor_ptr interceptor);
  //@}

  /// Return the valuetype adapter
  TAO_Valuetype_Adapter *valuetype_adapter (void);

#if !defined(CORBA_E_MICRO)
  // Value factory operations  (CORBA 2.3 ptc/98-10-05 Ch. 4.2 p.4-7)
  CORBA::ValueFactory register_value_factory (
    const char *repository_id,
    CORBA::ValueFactory factory);
#endif

#if !defined(CORBA_E_MICRO)
  void unregister_value_factory (const char * repository_id);
#endif

#if !defined(CORBA_E_MICRO)
  CORBA::ValueFactory lookup_value_factory (const char *repository_id);
#endif

  /// Get the IOR Interceptor adapter. If not created, this method will try
  /// to create one.
  TAO_IORInterceptor_Adapter *ior_interceptor_adapter (void);

  /// Set and Get methods to indicate whether a BiDir IIOP policy has
  /// been set in the POA.
  /// @note At present, the value will be true even if one of the POA's
  ///       is set with the Bi Dir GIOP policy.
  CORBA::Boolean bidir_giop_policy (void);
  void bidir_giop_policy (CORBA::Boolean);

  /// Return the table that maps object key/name to de-stringified
  /// object reference.  It is needed for supporting local objects in
  /// the resolve_initial_references() mechanism.
  TAO_Object_Ref_Table &object_ref_table (void);

  /// Acceessor to the table that stores the object_keys.
  TAO::ObjectKey_Table &object_key_table (void);

  /// Return the current request dispatcher strategy.
  TAO_Request_Dispatcher *request_dispatcher (void);

  /// Set a new request dispatcher.  The ORB Core retains ownership
  /// of the request dispatcher once it gets it.  Currently, this only
  /// gets called at initialization.
  void request_dispatcher (TAO_Request_Dispatcher *rd);

  /// Call the libraries to handover the validators if they havent
  /// registered yet with the list of validators.
  void load_policy_validators (TAO_Policy_Validator &validator);

  /// Return the flushing strategy
  /**
   * The flushing strategy is created by the resource factory, and it
   * is used by the ORB to control the mechanism used to flush the
   * outgoing data queues.
   * The flushing strategies are stateless, therefore, there is only
   * one per ORB.
   */
  TAO_Flushing_Strategy *flushing_strategy (void);

  /// Get Code Set Manager
  TAO_Codeset_Manager *codeset_manager (void);

  typedef ACE_Array_Map<ACE_CString, ACE_CString> InitRefMap;

  /// Return a pointer to the -ORBInitRef map.
  InitRefMap * init_ref_map (void);

  /// Set the ORB related to the orb_id as the default ORB and not the
  /// ORB that is first binded in the ORB Table
  void set_default (const char * orb_id);

  /// Choose to be not a default ORB when there is more than one ORB.
  void not_default (const char * orb_id);

  /// Verify condition for  permanent forward is given,
  /// both parameters must provide group attributes.
  CORBA::Boolean is_permanent_forward_condition
    (const CORBA::Object_ptr obj,
    const TAO_Service_Context &service_context);

  /// Configuration accessor method
  ACE_Service_Gestalt* configuration () const;

  /// Get outgoing fragmentation strategy.
  auto_ptr<TAO_GIOP_Fragmentation_Strategy>
  fragmentation_strategy (TAO_Transport * transport);

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)
 void set_eager_transport_queueing_strategy (
  TAO::Transport_Queueing_Strategy* strategy);

 void set_delayed_transport_queueing_strategy (
  TAO::Transport_Queueing_Strategy* strategy);
#endif

protected:

  /// Destructor is protected since the ORB Core is a reference
  /// counted object.
  ~TAO_ORB_Core (void);

  /// Initialize the guts of the ORB Core.  It is intended that this be
  /// called by <CORBA::ORB_init>.
  int init (int &argc, char *argv[]);

  /// Final termination hook, typically called by CORBA::ORB's
  /// destructor.
  int fini (void);

  /// Routine that creates a ACE_Data_Block given the lock and allocators.
  ACE_Data_Block *create_data_block_i (size_t size,
                                       ACE_Allocator *buffer_allocator,
                                       ACE_Allocator *dblock_allocator,
                                       ACE_Lock *lock);

  /// Obtain and cache the typecode factory object reference.
  void resolve_typecodefactory_i (void);

  /// Obtain and cache the poa current.
  void resolve_poa_current_i (void);

#if TAO_HAS_INTERCEPTORS == 1
  /// Obtain and cache the picurrent factory object reference.
  void resolve_picurrent_i (void);

  /// Get the Client Request Interceptor adapter.
  /// If not created, this method will try to create one if needed.
  TAO::ClientRequestInterceptor_Adapter *clientrequestinterceptor_adapter_i (void);

  /// Get the Server Request Interceptor adapter.
  /// If not created, this method will try to create one if needed.
  TAO::ServerRequestInterceptor_Adapter *serverrequestinterceptor_adapter_i (void);

#endif /* TAO_HAS_INTERCEPTORS */

  /// Obtain and cache the codec factory object reference.
  void resolve_codecfactory_i (void);

  /// Obtain and cache the compression manager object reference.
  void resolve_compression_manager_i (void);

  /// Obtain and cache the dynamic any factory object reference.
  void resolve_dynanyfactory_i (void);

  /// Obtain and cache the IORManipulation factory object reference.
  void resolve_iormanipulation_i (void);

  /// Obtain and cache the Monitor object reference.
  void resolve_monitor_i (void);

  /// Search the Dynamic service list for well known services that has
  /// callbacks  which can be dynamically loaded.
  void services_callbacks_init (void);

  /// Helper method that invokes Interceptor::destroy() on all
  /// registered interceptors when ORB::destroy() is called. Prevents
  /// exceptions from propagating up the call chain.
  void destroy_interceptors (void);

  /// Pointer to the list of protocol loaded into this ORB instance.
  /// Helper method to hold the common code part for -ORBEndpoint and
  /// -ORBListenEndpoints options.
  int set_endpoint_helper (const ACE_CString &lane,
                           const ACE_CString &endpoints);

  /// Return pointer to the policy factory registry associated with
  /// this ORB core.
  TAO::PolicyFactory_Registry_Adapter *policy_factory_registry_i (void);

  /// Return pointer to the orb initializer registry associated with
  /// this ORB core.
  TAO::ORBInitializer_Registry_Adapter *orbinitializer_registry_i (void);

  /// Common code from ::initialize_object and ::reinitialize_object
  CORBA::Long initialize_object_i (TAO_Stub *the_stub,
                                   const TAO_MProfile& mprofile);

private:

  /// The ORB Core should not be copied.
  //@{
  TAO_ORB_Core(const TAO_ORB_Core&);
  void operator=(const TAO_ORB_Core&);
  //@}

  /// Obtain and cache the dynamic any factory object reference.
  void resolve_ior_table_i (void);

  /// Checks to see whether collocation optimizations have to be
  /// applied on objects in the @a other_orb
  CORBA::Boolean is_collocation_enabled (TAO_ORB_Core *other_orb,
                                         const TAO_MProfile &mp);

  bool use_local_memory_pool_;

protected:

  /// Synchronize internal state...
  TAO_SYNCH_MUTEX lock_;

  TAO_Thread_Lane_Resources_Manager *thread_lane_resources_manager_;

  TAO_Collocation_Resolver *collocation_resolver_;

  TAO_Stub_Factory *stub_factory_;

  TAO_ProtocolFactorySet *protocol_factories_;

  /// The cached IOR for the Implementation Repository.
  // @@ If this is a _var, where should it get deleted? (brunsch)
  CORBA::Object_ptr implrepo_service_;

  /// Flag for whether the implrepo support is enabled or not.
  int use_implrepo_;

  /// Flag for whether to put the ImR endpoints into our object refs.
  int imr_endpoints_in_ior_;

  /// The cached IOR for the TypeCodeFactory DLL.
  CORBA::Object_ptr typecode_factory_;

  /// The cached IOR for the CodecFactory DLL.
  CORBA::Object_ptr codec_factory_;

  /// The cached IOR for the Compression DLL.
  CORBA::Object_ptr compression_manager_;

  /// The cached object reference for the DynAnyFactory.
  CORBA::Object_ptr dynany_factory_;

  /// The cached object reference for the IORManipulataion.
  CORBA::Object_ptr ior_manip_factory_;

  /// The cached object reference for the IORTable.
  CORBA::Object_ptr ior_table_;

  /// The cached object reference for the Monitor.
  CORBA::Object_ptr monitor_;

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0
  /// The cached object reference for the RTCORBA::RTORB.
  CORBA::Object_var rt_orb_;

  /// The cached object reference for the RTCORBA::Current interface.
  CORBA::Object_var rt_current_;
#endif

  /**
   * @note
   * Should we keep a single ORB pointer? This is good because
   * multiple calls to ORB_init() with the same ORBid can use the same
   * object, but maybe don't want so much coupling.
   *
   * Pointer to the ORB.
   */
  CORBA::ORB_ptr orb_;

  /// Object reference to the root POA.  It will eventually be the
  /// object reference returned by calls to
  ///   CORBA::ORB::resolve_initial_references ("RootPOA").
  CORBA::Object_var root_poa_;

  /// Parameters used by the ORB.
  TAO_ORB_Parameters orb_params_;

  /// Return InitRefMap to find if a particular object id is present.
  InitRefMap init_ref_map_;

  /// Table that maps object key/name to (de-stringified) object
  /// reference.  It is needed for supporting local objects in the
  /// resolve_initial_references() mechanism.
  TAO_Object_Ref_Table object_ref_table_;

  /// Table that stores the object key instead of caching one per-profile.
  TAO::ObjectKey_Table object_key_table_;

  /// The ORBid for this ORB.
  char *orbid_;

  /// Handle to the factory for resource information..
  TAO_Resource_Factory *resource_factory_;

  /// The server_id_ that was passed via -ORBServerId option
  ACE_CString server_id_;

  /// Handle to the factory for Client-side strategies.
  TAO_Client_Strategy_Factory *client_factory_;

  /// Handle to the factory for Server-side strategies.
  TAO_Server_Strategy_Factory *server_factory_;

  /**
   * @name Service Level Hooks
   */
  //@{

  /// Fault Tolerant service hook.
  TAO_Fault_Tolerance_Service ft_service_;

  //@}

  /// TRUE transmits a full TAG_FT_GROUP component in place of the FT_GROUP_VERSION
  /// context on FT requests. This is contrary to the spec but enables more effective
  /// fault tolerant stuff. @todo reference to OMG issue. Default false.
  CORBA::Boolean ft_send_extended_sc_;

  /// TRUE if we want to take advantage of collocation optimization in
  /// this ORB.
  CORBA::Boolean opt_for_collocation_;

  /// TRUE if we want to consider all ORBs in this address space
  /// collocated.
  CORBA::Boolean use_global_collocation_;

  /// Default collocation policy.  This should never be ORB_CONTROL.
  CORBA::ULong collocation_strategy_;

#if (TAO_HAS_CORBA_MESSAGING == 1)

  /// The Policy_Manager for this ORB.
  TAO_Policy_Manager *policy_manager_;

  /// The default policies.
  TAO_Policy_Set *default_policies_;

  /// Policy current.
  TAO_Policy_Current *policy_current_;

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

    /// The request dispatching strategy.
  TAO_Request_Dispatcher *request_dispatcher_;

  /**
   * POA current.
   */
  CORBA::Object_var poa_current_;

  /// The list of Adapters used in this ORB.
  TAO_Adapter_Registry adapter_registry_;

  /// An optimization for the POA.
  TAO_Adapter *poa_adapter_;

  /// The Thread Manager
  ACE_Thread_Manager tm_;

  /// The data block reference counts are locked using this mutex
  ACE_Lock_Adapter<TAO_SYNCH_MUTEX> data_block_lock_;

  /// TSS Object cleanup functions.  These correspond to the TSS
  /// objects stored in TAO's TSS resources.
  TAO_Cleanup_Func_Registry tss_cleanup_funcs_;

  /// This is where the tss resources for this ORB are stored.
  ACE_TSS_TYPE (TAO_ORB_Core_TSS_Resources) tss_resources_;

  /// Flag which denotes that the ORB has been shutdown.
  bool has_shutdown_;

  /// The value of the timeout if the flag above is not zero.
  //@{
  int thread_per_connection_use_timeout_;
  ACE_Time_Value thread_per_connection_timeout_;
  //@}

  /// Mutual exclusion for calling open.
  TAO_SYNCH_MUTEX open_lock_;

  TAO_Endpoint_Selector_Factory *endpoint_selector_factory_;

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  /// This strategy will buffer messages.
  TAO::Transport_Queueing_Strategy *eager_transport_queueing_strategy_;

  /// This strategy will buffer messages.
  TAO::Transport_Queueing_Strategy *delayed_transport_queueing_strategy_;

  /// This strategy will not queue by default, but will flush the queue
  /// each time
  TAO::Transport_Queueing_Strategy *flush_transport_queueing_strategy_;

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

  /// Number of outstanding references to this object.
  ACE_Atomic_Op<TAO_SYNCH_MUTEX, unsigned long> refcount_;

  /// Registry containing all registered policy factories.
  TAO::PolicyFactory_Registry_Adapter *policy_factory_registry_;

  /// Registry containing all orb initializers
  TAO::ORBInitializer_Registry_Adapter *orbinitializer_registry_;

  /// Registry containing all service context handlers
  TAO_Service_Context_Registry service_context_registry_;

#if (TAO_HAS_INTERCEPTORS == 1)
  /// Cached pointer/reference to the PICurrent object.
  /**
   * A pointer/reference to the PICurrent object is cached in the ORB
   * Core since it is accessed in the critical path (i.e. the request
   * invocation path).  Caching it prevents additional overhead to due
   * object resolution from occurring.
   */
  CORBA::Object_ptr pi_current_;

  /// The adapter for handling client request interceptors
  TAO::ClientRequestInterceptor_Adapter *client_request_interceptor_adapter_;

  /// The adapter for handling server request interceptors
  TAO::ServerRequestInterceptor_Adapter *server_request_interceptor_adapter_;

#endif /* TAO_HAS_INTERCEPTORS */

  /// IORInterceptor adapter.
  TAO_IORInterceptor_Adapter *ior_interceptor_adapter_;

  /// Pointer to the valuetype adapter.
  TAO_Valuetype_Adapter *valuetype_adapter_;

  /// The IOR parser registry.
  TAO_Parser_Registry parser_registry_;

  /// BiDirectional GIOP factory
  TAO_BiDir_Adapter *bidir_adapter_;

  /// Bir Dir GIOP policy value
  CORBA::Boolean bidir_giop_policy_;

  /// ZIOP Adapter
  TAO_ZIOP_Adapter *ziop_adapter_;

  /// ZIOP enabled or not
  CORBA::Boolean ziop_enabled_;

  /// Hold the flushing strategy
  TAO_Flushing_Strategy *flushing_strategy_;

  /// Code Set Manager, received from the Resource Factory
  TAO_Codeset_Manager *codeset_manager_;

  /// ORB's service configuration
  ACE_Intrusive_Auto_Ptr<ACE_Service_Gestalt> config_;

  /// The hook to be set for the SyncScopePolicy
  Sync_Scope_Hook sync_scope_hook_;

  /// The hook to be set for the RelativeRoundtripTimeoutPolicy.
  Timeout_Hook timeout_hook_;
};

// ****************************************************************

/**
 * @class TAO_ORB_Core_Static_Resources
 *
 * @brief The static (global) resoures of all ORB cores.
 *
 * This class is used by the ORB_Core to store the resources global to
 * all ORB_Cores.  All instance variables that would have been
 * declared "static" in TAO_ORB_Core, should be declared in this class
 * to avoid the "static initialization order fiasco" as described in
 * http://www.parashift.com/c++-faq-lite/ctors.html#faq-10.11.
 * Briefly, this is the problem that occurs if any static initializers
 * in any other code call into set static members of TAO_ORB_Core.
 * Since the order in which these initializers execute is unspecified,
 * uninitialized members can be accessed.
 */
class TAO_Export TAO_ORB_Core_Static_Resources : public ACE_Service_Object
{
public:

  /// Return the context-specific singleton instance.
  static TAO_ORB_Core_Static_Resources* instance (void);

public:
  /**
   * Name of the network_priority_protocols_hooks that needs to be instantiated.
  */
  ACE_CString network_priority_protocols_hooks_name_;

  /// The hook to be set for the ConnectionTimeoutPolicy
  TAO_ORB_Core::Timeout_Hook connection_timeout_hook_;

  /**
   * Name of the resource factory that needs to be instantiated.
   * The default value is "Resource_Factory". If TAO_Strategies is
   * linked, the set_resource_factory will be called to set the value
   * to be "Advanced_Resource_Factory".
   */
  ACE_CString resource_factory_name_;

  /**
   * Name of the service object for DII request creation that needs
   * to be instantiated. The default value is "Dynamic_Adaper". If
   * TAO_DynamicInterface is linked, dynamic_adapter_name() will be
   * called to set the value to "Concrete_Dynamic_Adapter".
   */
  ACE_CString dynamic_adapter_name_;

  /**
   * Name of the service object for functions that make calls on
   * the Interface Repository. The default value is "IFR_Client_Adaper".
   * If TAO_IFR_CLient is linked, ifr_client_adapter_name() will be
   * called to set the value to "Concrete_IFR_Client_Adapter".
   */
  ACE_CString ifr_client_adapter_name_;

  /**
   * Name of the service object used by the ORB create_*_tc functions.
   * The default value is "TypeCodeFactory_Adapter". If the
   * TypeCodeFactory library is linked, the corresponding accessor
   * function typecodefactory_adapter_name() will be called to set
   * the value to "Concrete_TypeCodeFactory_Adapter".
   */
  ACE_CString typecodefactory_adapter_name_;

  /**
   * Name of the factory object used to adapt function calls on
   * the PortableInterceptor interfaces IORInfo and IORInterceptor.
   * The default value is "IORInterceptor_Adapter_Factory". If the
   * IORInterceptor library is linked, the corresponding accessor
   * function iorinterceptor_adapter_factory_name() will be called to set
   * the value to "Concrete_IORInterceptor_Adapter_Factory".
   */
  ACE_CString iorinterceptor_adapter_factory_name_;

  /**
   * Name of the factory object used to adapt function calls on
           * the valuetype-related interfaces.
   * The default value is "Valuetype_Adapter_Factory". If the
   * Valuetype library is linked, the corresponding accessor
   * function valuetype_adapter_factory_name() will be called to set
   * the value to "Concrete_Valuetype_Adapter_Factory".
   */
  ACE_CString valuetype_adapter_factory_name_;

  /// An alternative hook to be set for the ConnectionTimeoutPolicy
  TAO_ORB_Core::Timeout_Hook alt_connection_timeout_hook_;

//private:

  /// Constructor.
  TAO_ORB_Core_Static_Resources (void);
  TAO_ORB_Core_Static_Resources& operator=
      (const TAO_ORB_Core_Static_Resources&);

private:

  /// Mostly unused variable whose sole purpose is to enforce
  /// the instantiation of a TAO_ORB_Core_Static_Resources instance
  /// at initialization time.
  static TAO_ORB_Core_Static_Resources* initialization_reference_;
};

// ****************************************************************

/// Obtain an instance of the first ORB core registered in the ORB
/// table.
TAO_Export TAO_ORB_Core * TAO_ORB_Core_instance (void);

// ****************************************************************

ACE_STATIC_SVC_DECLARE_EXPORT (TAO, TAO_ORB_Core_Static_Resources)
ACE_FACTORY_DECLARE (TAO, TAO_ORB_Core_Static_Resources)

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/ORB_Core.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_ORB_CORE_H */
