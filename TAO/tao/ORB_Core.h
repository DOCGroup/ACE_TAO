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
#include "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ORB.h"
#include "Environment.h"
#include "Policy_Manager.h"
#include "Resource_Factory.h"
#include "params.h"
#include "TAO_Singleton_Manager.h"
#include "TAO_Singleton.h"
#include "Adapter.h"
#include "PolicyFactory_Registry.h"
#include "Parser_Registry.h"
#include "Service_Callbacks.h"
#include "Fault_Tolerance_Service.h"
#include "Cleanup_Func_Registry.h"
#include "Object_Ref_Table.h"

// Interceptor definitions.
#include "PortableInterceptorC.h"
#include "Interceptor_List.h"
#include "PICurrent.h"

#include "Protocols_Hooks.h"

#include "ace/Hash_Map_Manager.h"
#include "ace/Thread_Manager.h"

// Forward declarations
class TAO_Acceptor;
class TAO_Connector;
class TAO_Connector_Registry;

class TAO_Resource_Factory;
class TAO_Client_Strategy_Factory;
class TAO_Server_Strategy_Factory;
class TAO_Transport_Cache_Manager;

class TAO_TSS_Resources;
class TAO_Leader_Follower;
class TAO_LF_Strategy;
class TAO_RT_ORB;
class TAO_RT_Current;
class TAO_MProfile;
class TAO_Profile;
class TAO_GIOP_Invocation;

class TAO_Endpoint_Selector_Factory;
class TAO_Message_State_Factory;
class TAO_ServerRequest;
class TAO_Protocols_Hooks;
class TAO_BiDir_Adapter;

class TAO_Flushing_Strategy;

class TAO_Thread_Lane_Resources_Manager;
class TAO_Collocation_Resolver;
class TAO_Thread_Lane_Resources;
class TAO_Stub_Factory;
class TAO_Endpoint_Selector_Factory;
class TAO_Service_Context;
class TAO_POA_PortableGroup_Hooks;
class TAO_Request_Dispatcher;

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

class TAO_Buffering_Constraint_Policy;
class TAO_Eager_Buffering_Sync_Strategy;
class TAO_Delayed_Buffering_Sync_Strategy;

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

class TAO_Transport_Sync_Strategy;
class TAO_Sync_Strategy;

// ****************************************************************

/**
 * @class TAO_ORB_Core_TSS_Resources
 *
 * @brief The TSS resoures of an ORB core.
 *
 * This class is used by the ORB_Core to store the resources
 * potentially bound to a thread in TSS storage. The members are public
 * because only the ORB Core is expected to access them.
 */
class TAO_Export TAO_ORB_Core_TSS_Resources
{
public:

  /// Constructor
  TAO_ORB_Core_TSS_Resources (void);

  /// destructor
  ~TAO_ORB_Core_TSS_Resources (void);

private:

  /// The ORB Core TSS resources should not be copied
  ACE_UNIMPLEMENTED_FUNC (TAO_ORB_Core_TSS_Resources (const TAO_ORB_Core_TSS_Resources&))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const TAO_ORB_Core_TSS_Resources&))

public:
  /**
   * @todo
   * The rest of the resources are not currently in use, just a plan
   * for the future...
   */

  /// The allocators for the output CDR streams.
  //@{
  ACE_Allocator *output_cdr_dblock_allocator_;
  ACE_Allocator *output_cdr_buffer_allocator_;
  ACE_Allocator *output_cdr_msgblock_allocator_;
  //@}

  /// The allocators for the input CDR streams.
  //@{
  ACE_Allocator *input_cdr_dblock_allocator_;
  ACE_Allocator *input_cdr_buffer_allocator_;
  ACE_Allocator *input_cdr_msgblock_allocator_;
  //@}

  /// Counter for how (nested) calls this thread has made to run the
  /// event loop.
  int event_loop_thread_;

  /// Counter for how many times this thread has become a client
  /// leader.
  int client_leader_thread_;

  /// Lane for this thread.
  void *lane_;

  /// Generic container for thread-specific objects.
  ACE_Array_Base<void *> ts_objects_;

  /// Pointer to the ORB core.  Needed to get access to the TSS
  /// cleanup functions for the TSS objects stored in the TSS object
  /// array in this class.
  TAO_ORB_Core *orb_core_;

#if TAO_HAS_INTERCEPTORS == 1
  /// The thread-specific portion of the PICurrent object.
  TAO_PICurrent_Impl pi_current_;
#endif  /* TAO_HAS_INTERCEPTORS == 1 */


};

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
  friend class TAO_ORB_Table;
  friend CORBA::ORB_ptr CORBA::ORB_init (int &,
                                         char *argv[],
                                         const char *,
                                         CORBA_Environment &);
  
public:

  /// Constructor.
  TAO_ORB_Core (const char* id);

  /// Accessor for the ORB parameters.
  TAO_ORB_Parameters *orb_params (void);

  /**
   * @todo
   * In the future this hook should change, instead of hardcoding the
   * object we should add a "Resolver" to the ORB, so the "POACurrent"
   * object returns a per-ORB object.
   * @par
   * Similarly, each ORB should implement the TSS pattern to put   the
   * POA_Current_Impl in a void* slot.  The current approach *does*
   * decouple the POA from the ORB, but it cannot add new adapters or
   * other components transparently.
   */
  /// Accessor to the POA current.
  //@{
  CORBA::Object_ptr poa_current(void);
  void poa_current (CORBA::Object_ptr poa_current);
  //@}

  ///Get the connector registry
  TAO_Connector_Registry *connector_registry (void);

  ///Get the IOR parser registry
  TAO_Parser_Registry *parser_registry (void);

  /// Return pointer to the policy factory registry associated with
  /// this ORB core.
  TAO_PolicyFactory_Registry *policy_factory_registry (void);

  /// Get the protocol factories
  TAO_ProtocolFactorySet *protocol_factories (void);

  /// Get pointer to the ORB.
  CORBA::ORB_ptr orb (void);

  /// Wrappers that forward the request to the concurrency strategy.
  ACE_Reactor *reactor (void);

  /// Get the ACE_Thread_Manager
  ACE_Thread_Manager *thr_mgr (void);

  /// Return the RootPOA, or try to load it if not initialized already.
  CORBA::Object_ptr root_poa (TAO_ENV_SINGLE_ARG_DECL);

  /// Get the adapter registry
  TAO_Adapter_Registry *adapter_registry (void);

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
  static int collocation_strategy (CORBA::Object_ptr object
                                   TAO_ENV_ARG_DECL);
  //@}

  /**
   * @name Default Code Set Translators
   *
   * Get the default codeset translators.
   *
   * @par
   * In most configurations these are just <nil> objects, but they can
   * be set to something different if the native character sets are
   * not ISO8869 (aka Latin/1, UTF-8) and UNICODE (aka UTF-16).
   *
   * @note
   * This is just note on how the translator database coule be
   * implemented: use the Service Configurator to load the translator,
   * and then use the CodesetId (an unsigned long) to translate the
   * character set code into the Service Object name.
   * @par
   * The default resource factory could parse command line options
   * like:
   *   - -ORBcharcodeset 0x00010001=ISO8859
   *   - -ORBcharcodeset 0x10020417=IBM1047
   *   - -ORBwcharcodeset 0x00010106=ISOIEC10646
   * that would let the user experiment with different translators
   * and plug them in on demand.
   *@par
   *
   * We should also think about how translators will report conversion
   * failures and how to simplify the implementation of char
   * translators (it would seem like just a couple of arrays are
   * needed, maybe the arrays should be dynamically loaded and the
   * implementation would remain constant?  Just a thought.
   *
   */
  //@{
  /// Convert from ISO8859 to the native character set
  ACE_Char_Codeset_Translator *from_iso8859 (void) const;

  /// Convert from the native character set to ISO8859
  ACE_Char_Codeset_Translator *to_iso8859 (void) const;

  /// Convert from UNICODE to the native wide character set
  ACE_WChar_Codeset_Translator *from_unicode (void) const;

  /// Convert from the native wide character set to UNICODE
  ACE_WChar_Codeset_Translator *to_unicode (void) const;
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

  /// A spawned thread needs to inherit some properties/objects from
  /// the spawning thread in order to serve requests.  Return 0 if
  /// it successfully inherits from the parent, -1 otherwise.
  int inherit_from_parent_thread (TAO_ORB_Core_TSS_Resources *tss_resources);

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

  /// Returns pointer to the client factory.
  TAO_Client_Strategy_Factory *client_factory (void);

  /// Returns pointer to the server factory.
  TAO_Server_Strategy_Factory *server_factory (void);

  /// Returns pointer to the Protocol_Hooks.
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

  /// Sets the value of TAO_ORB_Core::thread_lane_resources_manager_factory_name_
  static void set_thread_lane_resources_manager_factory (const char *thread_lane_resources_manager_factory_name);

  /// Sets the value of TAO_ORB_Core::collocation_resolver_name_
  static void set_collocation_resolver (const char *collocation_resolver_name);

  /// Sets the value of TAO_ORB_Core::stub_factory_name_
  static void set_stub_factory (const char *stub_factory_name);

  /// Sets the value of TAO_ORB_Core::resource_factory_
  static void set_resource_factory (const char *resource_factory_name);

  /// Sets the value of TAO_ORB_Core::protocols_hooks_
  static void set_protocols_hooks (const char *protocols_hooks_name);

  /// Sets the value of TAO_ORB_Core::endpoint_selector_factory_
  static void set_endpoint_selector_factory (
    const char *endpoint_selector_factory_name);

  /// Sets the name of the POA factory and the dynamic service
  /// configurator directive to load it if needed.
  static void set_poa_factory (
                    const char *poa_factory_name,
                    const char *poa_factory_directive);

  /// Gets the value of TAO_ORB_Core::protocols_hooks__
  TAO_Protocols_Hooks * get_protocols_hooks (TAO_ENV_SINGLE_ARG_DECL);

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

  /// This allocator maybe TSS or global, may or may not have
  /// locks. It is intended for allocating the ACE_Data_Blocks used in
  /// *outgoing* / CDR streams.
  ACE_Allocator *input_cdr_dblock_allocator (void);

  /// This allocator is always TSS and has no locks. It is intended
  /// for allocating the buffers used in *outgoing* CDR streams.
  ACE_Allocator *input_cdr_buffer_allocator (void);

  /// This allocator is always TSS and has no locks. It is intended
  /// for allocating the buffers used in *outgoing* CDR streams.
  ACE_Allocator *input_cdr_msgblock_allocator (void);

  /// This allocator is global, may or may not have locks. It is
  /// intended for ACE_Data_Blocks used in message blocks or CDR
  /// streams that have no relation with the life of threads,
  /// something like used in a class on a per connection basis
  ACE_Allocator *message_block_dblock_allocator (void);

  /// This allocator is global, may or may not have locks. It is
  /// intended for ACE_Data_Blocks used in message blocks or CDR
  /// streams that have no relation with the life of threads,
  /// something like used in a class on a per connection basis
  ACE_Allocator *message_block_buffer_allocator (void);

  /// This allocator is global, may or may not have locks. It is
  /// intended for ACE_Data_Blocks used in message blocks or CDR
  /// streams that have no relation with the life of threads,
  /// something like used in a class on a per connection basis
  ACE_Allocator *message_block_msgblock_allocator (void);

  /// The Message Blocks used for input CDRs must have appropiate
  /// locking strategies.
  ACE_Data_Block *create_input_cdr_data_block (size_t size);


  /// Return the locking strategy used for the data blocks.
  ACE_Lock *locking_strategy (void);

  /// The data blocks returned have memeory from the global pool. Will
  /// not get anything from the TSS even if it is available.
  ACE_Data_Block *data_block_for_message_block (size_t size);


#if (TAO_HAS_CORBA_MESSAGING == 1)

  /// Accessor method for the default_policies_
  TAO_Policy_Set *get_default_policies (void);

  /// Get a cached policy.  First, check the ORB-level Policy
  /// Manager, and then check the ORB defaults.
  CORBA::Policy *get_cached_policy (TAO_Cached_Policy_Type type);

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
  CORBA_Environment *default_environment (void) const;
  void default_environment (CORBA_Environment*);
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
                          int &has_timeout,
                          ACE_Time_Value &time_value);

  /// Define the Timeout_Hook signature
  typedef void (*Timeout_Hook) (TAO_ORB_Core *,
                                TAO_Stub *,
                                int&,
                                ACE_Time_Value&);

  static void set_timeout_hook (Timeout_Hook hook);

  /// Access to the RoundtripTimeoutPolicy policy set on the thread or
  /// on the ORB.  In this method, we do not consider the stub since
  /// we do not have access to it.
  CORBA::Policy *stubless_relative_roundtrip_timeout (void);

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

  CORBA::Policy *default_buffering_constraint (void) const;

  /// This strategy will buffer messages.
  //@{
  TAO_Eager_Buffering_Sync_Strategy &eager_buffering_sync_strategy (void);
  TAO_Delayed_Buffering_Sync_Strategy &delayed_buffering_sync_strategy (void);
  //@}

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

  /// This strategy will sync with the transport.
  TAO_Transport_Sync_Strategy &transport_sync_strategy (void);

  /// Handle to the factory for protocols_hooks_..
  TAO_Protocols_Hooks *protocols_hooks_;

  // Name of the protocols_hooks that needs to be instantiated.
  // The default value is "Protocols_Hooks". If RTCORBA option is
  // set, its value will be set to
  // be "RT_Protocols_Hooks".
  static const char *protocols_hooks_name_;

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
  int add_tss_cleanup_func (ACE_CLEANUP_FUNC cleanup,
                            size_t &slot_id);

  /// Return the underlying TSS cleanup function registry.
  TAO_Cleanup_Func_Registry *tss_cleanup_funcs (void);

  /// Get access to the leader_follower class.
  TAO_Leader_Follower &leader_follower (void);

  /// Get access to the leader follower strategy.
  TAO_LF_Strategy &lf_strategy (void);

  /// Get access to the thread lane resources.
  TAO_Thread_Lane_Resources &lane_resources (void);

  /// Run the event loop.
  int run (ACE_Time_Value *tv,
           int perform_work
           TAO_ENV_ARG_DECL);

  /// End the event loop
  void shutdown (CORBA::Boolean wait_for_completion
                 TAO_ENV_ARG_DECL);

  /// Get the shutdown flag value
  int has_shutdown (void);

  /// Shutdown the ORB and free resources
  void destroy (TAO_ENV_SINGLE_ARG_DECL);

  /// Check if ORB has shutdown.  If it has, throw an exception.
  void check_shutdown (TAO_ENV_SINGLE_ARG_DECL);

  /// Returns the <timeout> value used by the server threads to poll
  /// the shutdown flag. If the return value is zero then the server
  /// threads block forever.
  int thread_per_connection_timeout (ACE_Time_Value &timeout) const;

  /// Makes sure that the ORB is open and then creates a TAO_Stub
  /// based on the endpoint.
  TAO_Stub *create_stub_object (TAO_MProfile &mprofile,
                                const char *type_id,
                                CORBA::PolicyList *policy_list
                                TAO_ENV_ARG_DECL);

  /// Factory method that create the "right" Stub depending on
  /// wheather RTCORBA is loaded or not. The factory used to create
  /// the stub, is loaded at ORB initialization, and its type depends
  /// on the fact that RTCORBA is being used or not.
  TAO_Stub *create_stub (const char *repository_id,
                         const TAO_MProfile &profiles
                         TAO_ENV_ARG_DECL);


  /// Give each registered IOR interceptor the opportunity to add
  /// tagged components to profiles of each created servant.
  void establish_components (TAO_MProfile &mp,
                             CORBA::PolicyList *policy_list
                             TAO_ENV_ARG_DECL);

  /// Create a new object, use the adapter registry to create a
  /// collocated object, if not possible then create a regular
  /// object.
  CORBA::Object_ptr create_object (TAO_Stub *the_stub);

  /// Return ORBid string.
  const char *orbid (void) const;

  /// Set/Get the IOR of the Implementation Repository service.
  //@{
  CORBA::Object_ptr implrepo_service (void);
  void implrepo_service (const CORBA::Object_ptr ir);
  //@}

  /// Resolve the TypeCodeFactory DLL.
  CORBA::Object_ptr resolve_typecodefactory (TAO_ENV_SINGLE_ARG_DECL);

  /// Resolve the Dynamic Any Factory
  CORBA::Object_ptr resolve_dynanyfactory (TAO_ENV_SINGLE_ARG_DECL);

  /// Resolve the IOR Manipulation reference for this ORB.
  CORBA::Object_ptr resolve_ior_manipulation (TAO_ENV_SINGLE_ARG_DECL_NOT_USED);

  /// Resolve the IOR Table reference for this ORB.
  CORBA::Object_ptr resolve_ior_table (TAO_ENV_SINGLE_ARG_DECL_NOT_USED);

  /// Resolve an initial reference via the -ORBInitRef and
  // -ORBDefaultInitRef options.
  CORBA::Object_ptr resolve_rir (const char *name
                                 TAO_ENV_ARG_DECL_NOT_USED);

  /// Resolve the RT ORB reference for this ORB.
  CORBA::Object_ptr resolve_rt_orb (TAO_ENV_SINGLE_ARG_DECL);

  /// Resolve the RT Current flyweight for this ORB.
  CORBA::Object_ptr resolve_rt_current (TAO_ENV_SINGLE_ARG_DECL);

#if (TAO_HAS_MIOP == 1)
  /// Set/Get the current PortableGroup POA hooks.
  TAO_POA_PortableGroup_Hooks *portable_group_poa_hooks (void) const;
  void portable_group_poa_hooks(TAO_POA_PortableGroup_Hooks *poa_hooks);
#endif /* TAO_HAS_MIOP == 1 */

  /// List all the service known by the ORB
  CORBA_ORB_ObjectIdList_ptr list_initial_references (TAO_ENV_SINGLE_ARG_DECL_NOT_USED);

  /// Reference counting...
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  /// Register the handle of an open connection with the ORB Core
  /// handle set.  This handle set will be used to explicitly remove
  /// corresponding event handlers from the reactor.
  int register_handle (ACE_HANDLE handle);

  /// Remove <handle> from the ORB Core's handle set so that it
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
  CORBA::Boolean service_profile_selection (TAO_MProfile &mprofile,
                                            TAO_Profile  *&profile);

  /**
   * The loaded service in the ORB_Core would determine if the profile
   * reselection is going to be made by the services or not. If the
   * services do make the reselection they would return the selected
   * profile through <profile>. The reselction is for the
   * multi-profile IORS.
   */
  CORBA::Boolean service_profile_reselection (TAO_Stub *stub,
                                              TAO_Profile *&profile);

  /// Reset the flags in the loaded services.
  void reset_service_profile_flags (void);

  /**
   * The loaded service would determineif the CORBA::Object_ptr is
   * actually nill or not. This would be useful to accomodate new
   * enhanced definitions as defined by the service specification.
   */
  CORBA::Boolean object_is_nil (CORBA::Object_ptr object);

  /// Call the service layers with the Service Context to check
  /// whether they would like to add something to the list.
  void service_context_list (TAO_Stub *stub,
                             TAO_Service_Context &service_context,
                             CORBA::Boolean retstart
                             TAO_ENV_ARG_DECL);

  /// Return a reference to the Fault Tolerant service object.
  TAO_Fault_Tolerance_Service &fault_tolerance_service (void);

  /// Raise a comm failure exception if a service is not loaded, else
  /// delegate to the service to see what the service has to do for
  /// this case.
  int service_raise_comm_failure (TAO_GIOP_Invocation *invoke,
                                  TAO_Profile *profile
                                  TAO_ENV_ARG_DECL);

  /// Raise a transient failure exception if a service is not loaded,
  /// else delegate to the service to see what the service has to do
  /// for this case.
  int service_raise_transient_failure (TAO_GIOP_Invocation *invoke,
                                       TAO_Profile *profile
                                       TAO_ENV_ARG_DECL);

  /// Hook for logging of messages by the Logging & Recovery service
  /// of an FT service.
  void services_log_msg_rcv (TAO_Message_State_Factory &state);

  /// Hook for logging of messages by the Logging & Recovery service
  /// of an FT service.
  void services_log_msg_pre_upcall (TAO_ServerRequest &req);

  /// Hook for logging of messages by the Logging & Recovery service
  /// of an FT service.
  void services_log_msg_post_upcall (TAO_ServerRequest &req);
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

  /// Return a pointer to the cached TAO_PICurrent object.
  TAO_PICurrent *pi_current (void);

  /// Set the pointer to the cached TAO_PICurrent object.
  void pi_current (TAO_PICurrent *current);

  /// Register a client request interceptor.
  void add_interceptor (
    PortableInterceptor::ClientRequestInterceptor_ptr interceptor
    TAO_ENV_ARG_DECL);

  /// Register a server request interceptor.
  void add_interceptor (
    PortableInterceptor::ServerRequestInterceptor_ptr interceptor
    TAO_ENV_ARG_DECL);

  /// Return the array of client-side interceptors specific to this
  /// ORB.
  TAO_ClientRequestInterceptor_List::TYPE &
    client_request_interceptors (void);

  /// Return the array of server-side interceptors specific to this
  /// ORB.
  TAO_ServerRequestInterceptor_List::TYPE &
    server_request_interceptors (void);

#endif /* TAO_HAS_INTERCEPTORS */

  /// Register an IOR interceptor.
  void add_interceptor (
    PortableInterceptor::IORInterceptor_ptr interceptor
    TAO_ENV_ARG_DECL);

  /// Return the array of IOR interceptors specific to this ORB.
  TAO_IORInterceptor_List::TYPE & ior_interceptors (void);
  //@}

  /// Call the bidir_giop library to parse the policy.
  int parse_bidir_policy (CORBA::Policy_ptr policy
                          TAO_ENV_ARG_DECL);

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

  /// Return the current request dispatcher strategy.
  TAO_Request_Dispatcher *request_dispatcher (void);

  /// Set a new request dispatcher.  The ORB Core retains ownership
  /// of the request dispatcher once it gets it.  Currently, this only
  /// gets called at initialization.
  void request_dispatcher (TAO_Request_Dispatcher *rd);

  /// Return the flushing strategy
  /**
   * The flushing strategy is created by the resource factory, and it
   * is used by the ORB to control the mechanism used to flush the
   * outgoing data queues.
   * The flushing strategies are stateless, therefore, there is only
   * one per ORB.
   */
  TAO_Flushing_Strategy *flushing_strategy (void);

  typedef ACE_Hash_Map_Manager<ACE_CString,ACE_CString,ACE_Null_Mutex> InitRefMap;
  InitRefMap *init_ref_map ();

protected:

  /// Destructor is protected since the ORB Core is a reference
  /// counted object.
  ~TAO_ORB_Core (void);

  /// Initialize the guts of the ORB Core.  It is intended that this be
  /// called by <CORBA::ORB_init>.
  int init (int &argc, char **argv TAO_ENV_ARG_DECL);

  /// Final termination hook, typically called by CORBA::ORB's
  /// destructor.
  int fini (void);

  /// Implement the input_cdr_*_allocator() routines using pre-fetched
  /// TSS resources.  This minimizes the number of calls to them.
  //@{
  ACE_Allocator *input_cdr_dblock_allocator_i (TAO_ORB_Core_TSS_Resources *);
  ACE_Allocator *input_cdr_buffer_allocator_i (TAO_ORB_Core_TSS_Resources *);
  ACE_Allocator *input_cdr_msgblock_allocator_i (TAO_ORB_Core_TSS_Resources *);
  //@}

  /// Routine that creates a ACE_Data_Block given the lock and allocators.
  ACE_Data_Block *create_data_block_i (size_t size,
                                       ACE_Allocator *buffer_allocator,
                                       ACE_Allocator *dblock_allocator,
                                       ACE_Lock *lock);

  /// Obtain and cache the dynamic any factory object reference.
  void resolve_typecodefactory_i (TAO_ENV_SINGLE_ARG_DECL);

  /// Obtain and cache the dynamic any factory object reference.
  void resolve_dynanyfactory_i (TAO_ENV_SINGLE_ARG_DECL);

  /// Obtain and cache the IORManipulation factory object reference.
  void resolve_iormanipulation_i (TAO_ENV_SINGLE_ARG_DECL);

  /// Search the Dynamic service list for BiDirectional options that
  /// can be dynamically loaded.
  int bidirectional_giop_init (TAO_ENV_SINGLE_ARG_DECL);

  /// Search the Dynamic service list for well known services that has
  /// callbacks  which can be dynamically loaded.
  void services_callbacks_init (void);

  /// Helper method that invokes Interceptor::destroy() on all
  /// registered interceptors when ORB::destroy() is called.
  void destroy_interceptors (TAO_ENV_SINGLE_ARG_DECL);

private:

  /// The ORB Core should not be copied.
  //@{
  ACE_UNIMPLEMENTED_FUNC (TAO_ORB_Core(const TAO_ORB_Core&))
  ACE_UNIMPLEMENTED_FUNC (void operator=(const TAO_ORB_Core&))
  //@}

  /// Obtain and cache the dynamic any factory object reference.
  void resolve_ior_table_i (TAO_ENV_SINGLE_ARG_DECL);

  /// Try to create a new collocated object, using <other_orb> as the
  /// target ORB.  If not possible return 0.
  CORBA::Object_ptr create_collocated_object (TAO_Stub *the_stub,
                                              TAO_ORB_Core *other_orb,
                                              const TAO_MProfile &mprofile);

  /// The hook to be set for the RelativeRoundtripTimeoutPolicy.
  static Timeout_Hook timeout_hook_;

protected:

  /// Synchronize internal state...
  TAO_SYNCH_MUTEX lock_;

  /// The connector registry which all active connectors must register
  /// themselves with.
  TAO_Connector_Registry *connector_registry_;

  TAO_Thread_Lane_Resources_Manager *thread_lane_resources_manager_;

  TAO_Collocation_Resolver *collocation_resolver_;

  TAO_Stub_Factory *stub_factory_;

  /// Pointer to the list of protocol loaded into this ORB instance.
  TAO_ProtocolFactorySet *protocol_factories_;

  /// The cached IOR for the Implementation Repository.
  // @@ If this is a _var, where should it get deleted? (brunsch)
  CORBA::Object_ptr implrepo_service_;

  /// Flag for whether the implrepo support is enabled or not.
  int use_implrepo_;

  /// The cached IOR for the TypeCodeFactory DLL.
  CORBA::Object_ptr typecode_factory_;

  /// The cached object reference for the DynAnyFactory.
  CORBA::Object_ptr dynany_factory_;

  /// The cached object reference for the IORManipulataion.
  CORBA::Object_ptr ior_manip_factory_;

  /// The cached object reference for the IORTable.
  CORBA::Object_ptr ior_table_;

  /// The cached object reference for the RTCORBA::RTORB.
  CORBA::Object_var rt_orb_;

  /// The cached object reference for the RTCORBA::RTCurrent interface.
  CORBA::Object_var rt_current_;

  /**
   * @note
   * Should we keep a single ORB pointer? This is good because
   * multiple calls to ORB_init() with the same ORBid can use the same
   * object, but maybe don't want so much coupling.
   *
   * Pointer to the ORB.
   */
  CORBA::ORB_var orb_;

  /// Object reference to the root POA.  It will eventually be the
  /// object reference returned by calls to
  ///   CORBA::ORB::resolve_initial_references ("RootPOA").
  CORBA::Object_var root_poa_;

#if (TAO_HAS_MIOP == 1)
  // Hold a pointer for the POA if it needs to use any of the Portable 
  // group hooks.
  TAO_POA_PortableGroup_Hooks *portable_group_poa_hooks_;
#endif /* TAO_HAS_MIOP == 1 */

  /// Parameters used by the ORB.
  TAO_ORB_Parameters orb_params_;

  /// Return InitRefMap to find if a particular object id is present.
  InitRefMap init_ref_map_;

  /// Table that maps object key/name to (de-stringified) object
  /// reference.  It is needed for supporting local objects in the
  /// resolve_initial_references() mechanism.
  TAO_Object_Ref_Table object_ref_table_;

  /// The ORBid for this ORB.
  char *orbid_;

  /// Handle to the factory for resource information..
  TAO_Resource_Factory *resource_factory_;

  /// The allocators for the message blocks
  //@{
  ACE_Allocator *message_block_dblock_allocator_;
  ACE_Allocator *message_block_buffer_allocator_;
  ACE_Allocator *message_block_msgblock_allocator_;
  //@}

  // Name of the endpoint selector factory that needs to be instantiated.
  // The default value is "Default_Endpoint_Selector_Factory". If
  // TAO_RTCORBA is linked, the set_endpoint_selector_factory will be
  // called to set the value to be "RT_Endpoint_Selector_Factory".
  static const char *endpoint_selector_factory_name_;

  // Name of the thread lane resources manager that needs to be
  // instantiated.  The default value is
  // "Default_Thread_Lane_Resources_Manager_Factory". If TAO_RTCORBA
  // is linked, the set_thread_lane_resources_manager will be called
  // to set the value to be
  // "RT_Thread_Lane_Resources_Manager_Factory".
  static const char *thread_lane_resources_manager_factory_name_;

  // Name of the collocation resolver that needs to be instantiated.
  // The default value is "Default_Collocation_Resolver". If
  // TAO_RTCORBA is linked, the set_collocation_resolver will be
  // called to set the value to be "RT_Collocation_Resolver".
  static const char *collocation_resolver_name_;

  // Name of the stub factory that needs to be instantiated.
  // The default value is "Default_Stub_Factory". If TAO_RTCORBA is
  // linked, the set_stub_factory will be called to set the value
  // to be "RT_Stub_Factory".
  static const char *stub_factory_name_;

  // Name of the resource factory that needs to be instantiated.
  // The default value is "Resource_Factory". If TAO_Strategies is
  // linked, the set_resource_factory will be called to set the value
  // to be "Advanced_Resource_Factory".
  static const char *resource_factory_name_;

  // Name of the service object for DII request creation that needs
  // to be instantiated. The default value is "Dynamic_Adaper". If
  // TAO_DynamicInterface is linked, dynamic_adapter_name() will be
  // called to set the value to "Concrete_Dynamic_Adapter".
  static const char *dynamic_adapter_name_;

  // Name of the service object for functions that make calls on
  // the Interface Repository. The default value is "IFR_Client_Adaper".
  // If TAO_IFR_CLient is linked, ifr_client_adapter_name() will be
  // called to set the value to "Concrete_IFR_Client_Adapter".
  static const char *ifr_client_adapter_name_;

  // Name of the service object used by the ORB create_*_tc functions.
  // The default value is "TypeCodeFactory_Adapter". If the
  // TypeCodeFactory library is linked, the corresponding accessor
  // function typecodefactory_adapter_name() will be called to set
  // the value to "Concrete_TypeCodeFactory_Adapter".
  static const char *typecodefactory_adapter_name_;

  // Name of the service object used to create the RootPOA.  The
  // default value is "TAO_POA".  If TAO_RTCORBA is loaded, this
  // will be changed to TAO_RT_POA so that a POA equipped with
  // realtime extensions will be returned.
  static const char *poa_factory_name_;

  // The service configurator directive used to load
  // poa_factory_name_ dynamically.
  static const char *poa_factory_directive_;

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
   *
   * @note
   * This is a pointer in order to reduce the include file
   * dependencies.
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


  /// Codeset translators for simple implementations.
  //@{
  ACE_Char_Codeset_Translator *from_iso8859_;
  ACE_Char_Codeset_Translator *to_iso8859_;
  ACE_WChar_Codeset_Translator *from_unicode_;
  ACE_WChar_Codeset_Translator *to_unicode_;
  //@}

  /// TSS Object cleanup functions.  These correspond to the TSS
  /// objects stored in TAO's TSS resources.
  TAO_Cleanup_Func_Registry tss_cleanup_funcs_;

  /// If 1 then this ORB uses thread-specific resources
  int use_tss_resources_;

  /// This is where the tss resources for this ORB are stored.
  ACE_TSS_TYPE (TAO_ORB_Core_TSS_Resources) tss_resources_;

  /// If the resources are per-ORB (as opposed to per-ORB-per-thread)
  /// then they are stored here...
  TAO_ORB_Core_TSS_Resources orb_resources_;

  /// Flag which denotes that the ORB has been shutdown.
  int has_shutdown_;

  /// The value of the timeout if the flag above is not zero.
  //@{
  int thread_per_connection_use_timeout_;
  ACE_Time_Value thread_per_connection_timeout_;
  //@}

  TAO_Endpoint_Selector_Factory *endpoint_selector_factory_;

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

  /// This strategy will buffer messages.
  TAO_Eager_Buffering_Sync_Strategy *eager_buffering_sync_strategy_;

  /// This strategy will buffer messages.
  TAO_Delayed_Buffering_Sync_Strategy *delayed_buffering_sync_strategy_;

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

  /// This strategy will sync with the transport.
  TAO_Transport_Sync_Strategy *transport_sync_strategy_;

  /// Number of outstanding references to this object.
  CORBA::ULong refcount_;

  /// Registry containing all registered policy factories.
  TAO_PolicyFactory_Registry policy_factory_registry_;

#if (TAO_HAS_INTERCEPTORS == 1)
  /// Cached pointer/reference to the PICurrent object.
  /**
   * A pointer/reference to the PICurrent object is cached in the ORB
   * Core since it is accessed in the critical path (i.e. the request
   * invocation path).  Caching it prevents additional overhead to due
   * object resolution from occurring.
   */
  TAO_PICurrent *pi_current_;

  /// Client request interceptor registry.
  TAO_ClientRequestInterceptor_List client_request_interceptors_;

  /// Server request interceptor registry.
  TAO_ServerRequestInterceptor_List server_request_interceptors_;
#endif /* TAO_HAS_INTERCEPTORS */

  /// IOR interceptor registry.
  TAO_IORInterceptor_List ior_interceptors_;

  /// The IOR parser registry.
  TAO_Parser_Registry parser_registry_;

  /// BiDirectional GIOP factory
  TAO_BiDir_Adapter *bidir_adapter_;

  /// Bir Dir GIOP policy value
  CORBA::Boolean bidir_giop_policy_;

  /// Hold the flushing strategy
  TAO_Flushing_Strategy *flushing_strategy_;
};

// ****************************************************************

/**
 * @class TAO_ORB_Core_Auto_Ptr
 *
 * @brief Define a TAO_ORB_Core auto_ptr class.
 *
 * This class is used as an aid to make ORB initialization exception
 * safe.  It ensures that the ORB core is deallocated if an exception
 * is thrown.
 *
 * @todo
 * TAO_ORB_Core_Auto_Ptr should be renamed to TAO_ORB_Core_var
 * since the ORB Core is reference counted.
 */
class TAO_Export TAO_ORB_Core_Auto_Ptr
{
public:

  /// Initialization and termination methods
  //@{
  /* explicit */ TAO_ORB_Core_Auto_Ptr (TAO_ORB_Core *p = 0);
  TAO_ORB_Core_Auto_Ptr (TAO_ORB_Core_Auto_Ptr &ap);
  TAO_ORB_Core_Auto_Ptr &operator= (TAO_ORB_Core_Auto_Ptr &rhs);
  ~TAO_ORB_Core_Auto_Ptr (void);
  //@}

  /// Accessor methods.
  //@{
  TAO_ORB_Core &operator *() const;
  TAO_ORB_Core *get (void) const;
  TAO_ORB_Core *release (void);
  void reset (TAO_ORB_Core *p = 0);
  TAO_ORB_Core *operator-> () const;
  //@}

protected:

  TAO_ORB_Core *p_;

};

// ****************************************************************
/**
 * @class TAO_TSS_Resources
 *
 * @brief The TSS resoures shared by all the ORBs
 *
 * This class is used by TAO to store the resources that are
 * thread-specific but are *not* ORB specific. The members are public
 * because only the ORB Core is expected to access them.
 */
class TAO_Export TAO_TSS_Resources
{
public:

  /// Constructor
  TAO_TSS_Resources (void);

  /// Destructor
  ~TAO_TSS_Resources (void);

private:

  /// Do not copy TSS resources
  //@{
  ACE_UNIMPLEMENTED_FUNC (TAO_TSS_Resources(const TAO_TSS_Resources&))
  ACE_UNIMPLEMENTED_FUNC (void operator=(const TAO_TSS_Resources&))
  //@}

public:

  /**
   * Points to structure containing state for the current upcall
   * context in this thread.  Note that it does not come from the
   * resource factory because it must always be held in
   * thread-specific storage.  For now, since TAO_ORB_Core instances
   * are TSS singletons, we simply ride along and don't allocate
   * occupy another TSS slot since there are some platforms where
   * those are precious commodities (e.g., NT).
   */
  void *poa_current_impl_;

  /// The default environment for the thread.
  CORBA_Environment *default_environment_;

  /// If the user (or library) provides no environment the ORB_Core
  /// still holds one.
  CORBA_Environment tss_environment_;

#if (TAO_HAS_CORBA_MESSAGING == 1)

  /// The initial PolicyCurrent for this thread. Should be a TSS
  /// resource.
  TAO_Policy_Current_Impl initial_policy_current_;

  /// This pointer is reset by the POA on each upcall.
  TAO_Policy_Current_Impl *policy_current_;

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

};

/**
 * @todo TAO_TSS_RESOURCES singleton typedef should go away.
 */
typedef TAO_TSS_Singleton<TAO_TSS_Resources, TAO_SYNCH_MUTEX>
        TAO_TSS_RESOURCES;

// ****************************************************************

/// Obtain an instance of the first ORB core registered in the ORB
/// table.
TAO_Export TAO_ORB_Core *TAO_ORB_Core_instance (void);

#if defined (__ACE_INLINE__)
# include "ORB_Core.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_ORB_CORE_H */
