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
#include "tao/ORBInitializer_Registry.h"
#include "tao/Service_Callbacks.h"
#include "tao/Fault_Tolerance_Service.h"
#include "tao/Cleanup_Func_Registry.h"
#include "tao/Object_Ref_Table.h"
#include "tao/ObjectKey_Table.h"
#include "tao/Messaging_SyncScopeC.h"
#include "tao/Object.h"
#include "tao/Invocation_Utils.h"
#include "tao/Adapter_Registry.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/Interceptor_List.h"

namespace TAO
{
  typedef Interceptor_List< ::PortableInterceptor::ClientRequestInterceptor>
    ClientRequestInterceptor_List;

  typedef Interceptor_List< ::PortableInterceptor::ServerRequestInterceptor>
    ServerRequestInterceptor_List;
}

# include "PICurrent.h"
#endif  /* TAO_HAS_INTERCEPTORS == 1  */

#include "ace/Thread_Manager.h"
#include "ace/Lock_Adapter_T.h"

// Forward declarations
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
class TAO_RT_Current;
class TAO_MProfile;
class TAO_Profile;

class TAO_Endpoint_Selector_Factory;
class TAO_Message_State_Factory;
class TAO_Protocols_Hooks;
class TAO_BiDir_Adapter;

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

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

class TAO_Buffering_Constraint_Policy;
class TAO_Eager_Buffering_Sync_Strategy;
class TAO_Delayed_Buffering_Sync_Strategy;

#endif /* TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1 */

#if TAO_HAS_INTERCEPTORS == 1
class TAO_ClientRequestInfo;
#endif  /* TAO_HAS_INTERCEPTORS == 1  */


class TAO_Transport_Sync_Strategy;
class TAO_Sync_Strategy;
class TAO_Policy_Validator;
namespace TAO
{
  class GUIResource_Factory;
  class PolicyFactory_Registry_Adapter;
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
}
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

  // Set to true by the wait_on_lf_no_nested_upcall wait strategy
  // @CJC@  maybe we should use allocate_tss_slot_id() instead?
  bool upcalls_temporarily_suspended_on_this_thread_;

  /// Pointer to the ORB core.  Needed to get access to the TSS
  /// cleanup functions for the TSS objects stored in the TSS object
  /// array in this class.
  TAO_ORB_Core *orb_core_;

#if TAO_HAS_INTERCEPTORS == 1
  /// The thread-specific portion of the PICurrent object.
  TAO::PICurrent_Impl pi_current_;

  /// The PortableInterceptor::ClientRequestInfo object for the
  /// current thread.
  TAO_ClientRequestInfo *client_request_info_;
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
  friend CORBA::ORB_ptr CORBA::ORB_init (int &,
                                         char *argv[],
                                         const char *,
                                         CORBA::Environment &);
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
  TAO_Connector_Registry *connector_registry (ACE_ENV_SINGLE_ARG_DECL);

  ///Get the IOR parser registry
  TAO_Parser_Registry *parser_registry (void);

  /// Return pointer to the policy factory registry associated with
  /// this ORB core.
  TAO::PolicyFactory_Registry_Adapter *policy_factory_registry (void);

  /// Get the protocol factories
  TAO_ProtocolFactorySet *protocol_factories (void);

  /// Get pointer to the ORB.
  CORBA::ORB_ptr orb (void);

  /// Wrappers that forward the request to the concurrency strategy.
  ACE_Reactor *reactor (void);

  /// Get the ACE_Thread_Manager
  ACE_Thread_Manager *thr_mgr (void);

  /// Return the RootPOA, or try to load it if not initialized already.
  CORBA::Object_ptr root_poa (ACE_ENV_SINGLE_ARG_DECL);

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
  static
TAO::Collocation_Strategy collocation_strategy (CORBA::Object_ptr object
                                                  ACE_ENV_ARG_DECL);
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

  /// Sets the value of
  /// TAO_ORB_Core::thread_lane_resources_manager_factory_name_
  static void set_thread_lane_resources_manager_factory (const char *
    thread_lane_resources_manager_factory_name);

  /// Sets the value of TAO_ORB_Core::collocation_resolver_name_
  static void set_collocation_resolver (const char *collocation_resolver_name);

  /// Sets the value of TAO_ORB_Core::stub_factory_name_
  static void set_stub_factory (const char *stub_factory_name);

  /// Sets the value of TAO_ORB_Core::resource_factory_
  static void set_resource_factory (const char *resource_factory_name);

  /** Sets the value of TAO_ORB_Core::gui_resource_factory_.
   *
   *  Sets the value of gui_resource_factory in TSS. ORB_Core is responsible
   *  for releasing this factory if needed.
   */
  static void set_gui_resource_factory (TAO::GUIResource_Factory *gui_resource_factory);

  /// Sets the value of TAO_ORB_Core::protocols_hooks_
  static void set_protocols_hooks (const char *protocols_hooks_name);

  /// Sets the value of TAO_ORB_Core::endpoint_selector_factory_
  static void set_endpoint_selector_factory (
    const char *endpoint_selector_factory_name);

  /// Sets the name of the POA factory and the dynamic service
  /// configurator directive to load it if needed.
  static void set_poa_factory (const char *poa_factory_name,
                               const char *poa_factory_directive);

  /// Access the POA factory name.
  static const ACE_CString &poa_factory_name (void);

  /// Gets the value of TAO_ORB_Core::protocols_hooks__
  TAO_Protocols_Hooks * get_protocols_hooks (void);

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

  /// Sets the value of TAO_ORB_Core::valuetype_adapter_name.
  static void valuetype_adapter_name (const char *name);

  /// Gets the value of TAO_ORB_Core::valuetype_adapter_name.
  static const char *valuetype_adapter_name (void);


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
  CORBA::Policy_ptr get_policy (CORBA::PolicyType type
                                ACE_ENV_ARG_DECL);

  /// Get a policy.  First, check the thread current, then check the
  /// ORB-level Policy Manager, then check the ORB defaults.
  CORBA::Policy_ptr get_policy_including_current (CORBA::PolicyType type
                                                  ACE_ENV_ARG_DECL);

  /// Get a cached policy.  First, check the ORB-level Policy Manager,
  /// then check the ORB defaults.
  CORBA::Policy_ptr get_cached_policy (TAO_Cached_Policy_Type type
                                       ACE_ENV_ARG_DECL);

  /// Get a cached policy.  First, check the thread current, then
  /// check the ORB-level Policy Manager, then check the ORB defaults.
  CORBA::Policy_ptr get_cached_policy_including_current (
      TAO_Cached_Policy_Type type
      ACE_ENV_ARG_DECL);

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

  static void set_timeout_hook (Timeout_Hook hook);

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
  static void connection_timeout_hook (Timeout_Hook hook);

  void call_sync_scope_hook (TAO_Stub *stub,
                             bool &has_synchronization,
                             Messaging::SyncScope &scope);

  TAO_Sync_Strategy &get_sync_strategy (TAO_Stub *stub,
                                        Messaging::SyncScope &scope);
  typedef void (*Sync_Scope_Hook) (TAO_ORB_Core *,
                                   TAO_Stub *,
                                   bool &,
                                   Messaging::SyncScope &);

  static void set_sync_scope_hook (Sync_Scope_Hook hook);

#if (TAO_HAS_BUFFERING_CONSTRAINT_POLICY == 1)

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
           ACE_ENV_ARG_DECL);

  /// End the event loop
  void shutdown (CORBA::Boolean wait_for_completion
                 ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC (());

  /// Get the shutdown flag value
  int has_shutdown (void);

  /// Shutdown the ORB and free resources
  void destroy (ACE_ENV_SINGLE_ARG_DECL);

  /// Check if ORB has shutdown.  If it has, throw an exception.
  void check_shutdown (ACE_ENV_SINGLE_ARG_DECL);

  /// Returns the <timeout> value used by the server threads to poll
  /// the shutdown flag. If the return value is zero then the server
  /// threads block forever.
  int thread_per_connection_timeout (ACE_Time_Value &timeout) const;

  /// Makes sure that the ORB is open and then creates a TAO_Stub
  /// based on the endpoint.
  TAO_Stub *create_stub_object (TAO_MProfile &mprofile,
                                const char *type_id,
                                CORBA::PolicyList *policy_list
                                ACE_ENV_ARG_DECL);

  /// Factory method that create the "right" Stub depending on
  /// wheather RTCORBA is loaded or not. The factory used to create
  /// the stub, is loaded at ORB initialization, and its type depends
  /// on the fact that RTCORBA is being used or not.
  TAO_Stub *create_stub (const char *repository_id,
                         const TAO_MProfile &profiles
                         ACE_ENV_ARG_DECL);

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
  CORBA::Long initialize_object (TAO_Stub *the_stub,
                                 CORBA::Object_ptr obj);

  /// Return ORBid string.
  const char *orbid (void) const;

  /// Set/Get the IOR of the Implementation Repository service.
  //@{
  CORBA::Object_ptr implrepo_service (void);
  void implrepo_service (const CORBA::Object_ptr ir);
  //@}

  /// Do we put the ImR's endpoints into persistent object references we create
  CORBA::Boolean imr_endpoints_in_ior (void);

  /// Resolve the TypeCodeFactory DLL.
  CORBA::Object_ptr resolve_typecodefactory (ACE_ENV_SINGLE_ARG_DECL);

  /// Resolve the Dynamic Any Factory
  CORBA::Object_ptr resolve_dynanyfactory (ACE_ENV_SINGLE_ARG_DECL);

  /// Resolve the IOR Manipulation reference for this ORB.
  CORBA::Object_ptr resolve_ior_manipulation (ACE_ENV_SINGLE_ARG_DECL);

  /// Resolve the IOR Table reference for this ORB.
  CORBA::Object_ptr resolve_ior_table (ACE_ENV_SINGLE_ARG_DECL);

  /// Resolve an initial reference via the -ORBInitRef and
  // -ORBDefaultInitRef options.
  CORBA::Object_ptr resolve_rir (const char *name
                                 ACE_ENV_ARG_DECL);

  /// Resolve the RT ORB reference for this ORB.
  CORBA::Object_ptr resolve_rt_orb (ACE_ENV_SINGLE_ARG_DECL);

  /// Resolve the RT Current flyweight for this ORB.
  /// Return server_id string.
  const char *server_id (void) const;

  CORBA::Object_ptr resolve_rt_current (ACE_ENV_SINGLE_ARG_DECL);

  /// List all the service known by the ORB
  CORBA::ORB_ObjectIdList *list_initial_references (
      ACE_ENV_SINGLE_ARG_DECL_NOT_USED
    );

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
  CORBA::Boolean is_profile_equivalent (const TAO_Profile *this_p,
                                        const TAO_Profile *that_p);

  /// Hook for the services to determine the <hash> value of a
  /// profile.
  /**
   * For details on how this is used please see the FT service
   */
  CORBA::ULong hash_service (TAO_Profile *this_p,
                             CORBA::ULong max);

  /// Call the service layers with the Service Context to check
  /// whether they would like to add something to the list.
  void service_context_list (TAO_Stub *stub,
                             TAO_Service_Context &service_context,
                             CORBA::Boolean retstart
                             ACE_ENV_ARG_DECL);

  /// Return a reference to the Fault Tolerant service object.
  TAO_Fault_Tolerance_Service &fault_tolerance_service (void);

  /// Raise a comm failure exception if a service is not loaded, else
  /// delegate to the service to see what the service has to do for
  /// this case.
  TAO::Invocation_Status service_raise_comm_failure (
      IOP::ServiceContextList &clist,
      TAO_Profile *profile
      ACE_ENV_ARG_DECL);

  /// Raise a transient failure exception if a service is not loaded,
  /// else delegate to the service to see what the service has to do
  /// for this case.
  TAO::Invocation_Status service_raise_transient_failure (
      IOP::ServiceContextList &clist,
      TAO_Profile *profile
      ACE_ENV_ARG_DECL);

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
  TAO::PICurrent *pi_current (void);

  /// Set the pointer to the cached TAO::PICurrent object.
  void pi_current (TAO::PICurrent *current);

  /// Register a client request interceptor.
  void add_interceptor (
    PortableInterceptor::ClientRequestInterceptor_ptr interceptor
    ACE_ENV_ARG_DECL);

  /// Register a server request interceptor.
  void add_interceptor (
    PortableInterceptor::ServerRequestInterceptor_ptr interceptor
    ACE_ENV_ARG_DECL);

  /// Return the array of client-side interceptors specific to this
  /// ORB.
  TAO::ClientRequestInterceptor_List::TYPE &
    client_request_interceptors (void);

  /// Return the array of server-side interceptors specific to this
  /// ORB.
  TAO::ServerRequestInterceptor_List::TYPE &
    server_request_interceptors (void);

#endif /* TAO_HAS_INTERCEPTORS */

  /// Register an IOR interceptor.
  void add_interceptor (
    PortableInterceptor::IORInterceptor_ptr interceptor
    ACE_ENV_ARG_DECL);
  //@}

  /// Return the valuetype adapter
  TAO_Valuetype_Adapter *& valuetype_adapter (void);

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
  void load_policy_validators (TAO_Policy_Validator &validator
                               ACE_ENV_ARG_DECL);

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

  typedef ACE_Hash_Map_Manager_Ex<ACE_CString,
                                  ACE_CString,
                                  ACE_Hash<ACE_CString>,
                                  ACE_Equal_To<ACE_CString>,
                                  ACE_Null_Mutex>
    InitRefMap;

  /// Return a pointer to the -ORBInitRef map.
  InitRefMap * init_ref_map (void);

  /// Set the ORB related to the orb_id as the default ORB and not the
  /// ORB that is first binded in the ORB Table
  void set_default (const char *orb_id);

  /// Set to choose to be not a default ORB when there are more than
  /// one ORB.
  void not_default (const char *orb_id);

protected:

  /// Destructor is protected since the ORB Core is a reference
  /// counted object.
  ~TAO_ORB_Core (void);

  /// Initialize the guts of the ORB Core.  It is intended that this be
  /// called by <CORBA::ORB_init>.
  int init (int &argc, char **argv ACE_ENV_ARG_DECL);

  /// Final termination hook, typically called by CORBA::ORB's
  /// destructor.
  int fini (void);

  /// Routine that creates a ACE_Data_Block given the lock and allocators.
  ACE_Data_Block *create_data_block_i (size_t size,
                                       ACE_Allocator *buffer_allocator,
                                       ACE_Allocator *dblock_allocator,
                                       ACE_Lock *lock);

  /// Obtain and cache the dynamic any factory object reference.
  void resolve_typecodefactory_i (ACE_ENV_SINGLE_ARG_DECL);

  /// Obtain and cache the dynamic any factory object reference.
  void resolve_dynanyfactory_i (ACE_ENV_SINGLE_ARG_DECL);

  /// Obtain and cache the IORManipulation factory object reference.
  void resolve_iormanipulation_i (ACE_ENV_SINGLE_ARG_DECL);

  /// Search the Dynamic service list for BiDirectional options that
  /// can be dynamically loaded.
  int bidirectional_giop_init (ACE_ENV_SINGLE_ARG_DECL);

  /// Search the Dynamic service list for well known services that has
  /// callbacks  which can be dynamically loaded.
  void services_callbacks_init (void);

  /// Helper method that invokes Interceptor::destroy() on all
  /// registered interceptors when ORB::destroy() is called. Prevents
  /// exceptions from propagating up the call chain.
  void destroy_interceptors (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC (());

  /// Pointer to the list of protocol loaded into this ORB instance.
  /// Helper method to hold the common code part for -ORBEndpoint and
  /// -ORBListenEndpoint options.
  int set_endpoint_helper (const ACE_CString &lane,
                           const ACE_CString &endpoints
                           ACE_ENV_ARG_DECL);

private:

  /// The ORB Core should not be copied.
  //@{
  ACE_UNIMPLEMENTED_FUNC (TAO_ORB_Core(const TAO_ORB_Core&))
  ACE_UNIMPLEMENTED_FUNC (void operator=(const TAO_ORB_Core&))
  //@}

  /// Obtain and cache the dynamic any factory object reference.
  void resolve_ior_table_i (ACE_ENV_SINGLE_ARG_DECL);

  /// Checks to see whether collocation optimizations have to be
  /// applied on objects in the <other_orb>
  CORBA::Boolean is_collocation_enabled (TAO_ORB_Core *other_orb,
                                         const TAO_MProfile &mp);

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

  /// The cached object reference for the DynAnyFactory.
  CORBA::Object_ptr dynany_factory_;

  /// The cached object reference for the IORManipulataion.
  CORBA::Object_ptr ior_manip_factory_;

  /// The cached object reference for the IORTable.
  CORBA::Object_ptr ior_table_;

  /// The cached object reference for the RTCORBA::RTORB.
  CORBA::Object_var rt_orb_;

  /// The cached object reference for the RTCORBA::Current interface.
  CORBA::Object_var rt_current_;

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

  /// TSS Object cleanup functions.  These correspond to the TSS
  /// objects stored in TAO's TSS resources.
  TAO_Cleanup_Func_Registry tss_cleanup_funcs_;

  /// This is where the tss resources for this ORB are stored.
  ACE_TSS_TYPE (TAO_ORB_Core_TSS_Resources) tss_resources_;

  /// Flag which denotes that the ORB has been shutdown.
  int has_shutdown_;

  /// The value of the timeout if the flag above is not zero.
  //@{
  int thread_per_connection_use_timeout_;
  ACE_Time_Value thread_per_connection_timeout_;
  //@}

  /// Mutual exclusion for calling open.
  TAO_SYNCH_MUTEX open_lock_;

  /// Flag which denotes that the open method was called.
  int open_called_;

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
  TAO::PolicyFactory_Registry_Adapter *policy_factory_registry_;

#if (TAO_HAS_INTERCEPTORS == 1)
  /// Cached pointer/reference to the PICurrent object.
  /**
   * A pointer/reference to the PICurrent object is cached in the ORB
   * Core since it is accessed in the critical path (i.e. the request
   * invocation path).  Caching it prevents additional overhead to due
   * object resolution from occurring.
   */
  TAO::PICurrent *pi_current_;

  /// Client request interceptor registry.
  TAO::ClientRequestInterceptor_List client_request_interceptors_;

  /// Server request interceptor registry.
  TAO::ServerRequestInterceptor_List server_request_interceptors_;
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

  /// Hold the flushing strategy
  TAO_Flushing_Strategy *flushing_strategy_;

  /// Code Set Manager
  TAO_Codeset_Manager *codeset_manager_;

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
class TAO_Export TAO_ORB_Core_Static_Resources
{
public:

  /// Return the singleton instance.
  static TAO_ORB_Core_Static_Resources* instance (void);

public:
  // The hook to be set for the SyncScopePolicy
  TAO_ORB_Core::Sync_Scope_Hook sync_scope_hook_;

  /**
   * Name of the protocols_hooks that needs to be instantiated.
   * The default value is "Protocols_Hooks". If RTCORBA option is
   * set, its value will be set to be "RT_Protocols_Hooks".
   */
  ACE_CString protocols_hooks_name_;

  /// The hook to be set for the RelativeRoundtripTimeoutPolicy.
  TAO_ORB_Core::Timeout_Hook timeout_hook_;

  /// The hook to be set for the ConnectionTimeoutPolicy
  TAO_ORB_Core::Timeout_Hook connection_timeout_hook_;

  /**
   * Name of the endpoint selector factory that needs to be instantiated.
   * The default value is "Default_Endpoint_Selector_Factory". If
   * TAO_RTCORBA is linked, the set_endpoint_selector_factory will be
   * called to set the value to be "RT_Endpoint_Selector_Factory".
   */
  ACE_CString endpoint_selector_factory_name_;

  /**
   * Name of the thread lane resources manager that needs to be
   * instantiated.  The default value is
   * "Default_Thread_Lane_Resources_Manager_Factory". If TAO_RTCORBA
   * is linked, the set_thread_lane_resources_manager will be called
   * to set the value to be
   * "RT_Thread_Lane_Resources_Manager_Factory".
   */
  ACE_CString thread_lane_resources_manager_factory_name_;

  /**
   * Name of the collocation resolver that needs to be instantiated.
   * The default value is "Default_Collocation_Resolver". If
   * TAO_RTCORBA is linked, the set_collocation_resolver will be
   * called to set the value to be "RT_Collocation_Resolver".
   */
  ACE_CString collocation_resolver_name_;

  /**
   * Name of the stub factory that needs to be instantiated.
   * The default value is "Default_Stub_Factory". If TAO_RTCORBA is
   * linked, the set_stub_factory will be called to set the value
   * to be "RT_Stub_Factory".
   */
  ACE_CString stub_factory_name_;

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
   * Name of the service object used to adapt function calls on
   * the valuetype-related interfaces.
   * The default value is "Valuetype_Adapter". If the
   * Valuetype library is linked, the corresponding accessor
   * function valuetype_adapter_name() will be called to set
   * the value to "Concrete_Valuetype_Adapter".
   */
  ACE_CString valuetype_adapter_name_;

  /**
   * Name of the service object used to create the RootPOA.  The
   * default value is "TAO_POA".  If TAO_RTCORBA is loaded, this
   * will be changed to TAO_RT_POA so that a POA equipped with
   * realtime extensions will be returned.
   */
  ACE_CString poa_factory_name_;

  /**
   * The service configurator directive used to load
   * poa_factory_name_ dynamically.
   */
  ACE_CString poa_factory_directive_;

private:

  /// Constructor.
  TAO_ORB_Core_Static_Resources (void);

private:

  /// The singleton instance.
  static TAO_ORB_Core_Static_Resources* instance_;

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

#if defined (__ACE_INLINE__)
# include "ORB_Core.i"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif /* TAO_ORB_CORE_H */
