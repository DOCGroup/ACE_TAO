// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
//     ORB_Core.h
//
// = AUTHOR
//     Chris Cleeland
//
// ============================================================================

#ifndef TAO_ORB_CORE_H
#define TAO_ORB_CORE_H

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/ORB.h"
#include "tao/Environment.h"
#include "tao/Policy_Manager.h"
#include "tao/Resource_Factory.h"
#include "tao/params.h"
#include "tao/POAC.h"
#include "tao/GIOP.h"

#include "ace/Map_Manager.h"
#include "ace/Singleton.h"

// Forward declarations
class TAO_POA;
class TAO_POA_Current;
class TAO_POA_Current_Impl;
class TAO_POA_Manager;
class TAO_POA_Policies;
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

#if (TAO_HAS_CORBA_MESSAGING == 1)

class TAO_None_Sync_Strategy;

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

class TAO_Transport_Sync_Strategy;

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

  int is_server_thread_;
  // Is this thread a server for this ORB?

  int is_leader_thread_;
  // Is this thread a leader for this ORB?

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

  ~TAO_ORB_Core (void);
  // Destructor

  TAO_ORB_Parameters *orb_params (void);
  // Accessor for the ORB parameters.

  TAO_POA_Current &poa_current (void) const;
  // Accessor to the POA current.

  // = Get the connector registry
  TAO_Connector_Registry *connector_registry (void);

  // = Get the acceptor registry
  TAO_Acceptor_Registry  *acceptor_registry  (void);

  // = Get the protocol factories
  TAO_ProtocolFactorySet *protocol_factories (void);

  // = Set/get pointer to the ORB.
  CORBA::ORB_ptr orb (void);

  ACE_Reactor *reactor (void);
  ACE_Reactor *reactor (TAO_Acceptor *acceptor);
  // Wrappers that forward the request to the concurrency strategy

  // = Get the ACE_Thread_Manager
  ACE_Thread_Manager *thr_mgr (void);

  // = Get the rootPOA
  TAO_POA *root_poa (CORBA::Environment &ACE_TRY_ENV =
                           TAO_default_environment (),
                     const char *adapter_name = TAO_DEFAULT_ROOTPOA_NAME,
                     TAO_POA_Manager *poa_manager = 0,
                     const TAO_POA_Policies *policies = 0);
  PortableServer::POA_ptr root_poa_reference (
      CORBA::Environment &ACE_TRY_ENV = TAO_default_environment (),
      const char *adapter_name = TAO_DEFAULT_ROOTPOA_NAME,
      TAO_POA_Manager *poa_manager = 0,
      const TAO_POA_Policies *policies = 0);

  // = Collocation strategies.
  enum
  {
    ORB_CONTROL,  // Indicate object should refer to ORB for either
                  // one of the following strategies.
    THRU_POA,     // Collocated calls will go thru POA.
    DIRECT        // Collocated calls invoke operation on Servant directly.
  };

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

  TAO_Object_Adapter *object_adapter (void);
  // Get <Object Adapter>.

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

#if (TAO_HAS_CORBA_MESSAGING == 1)

  TAO_Policy_Manager *policy_manager (void);
  // Return the Policy_Manager for this ORB.

  TAO_Policy_Current &policy_current (void);
  // Accesors to the policy current, this object should be kept in TSS
  // storage.  The POA has to reset the policy current object on every
  // upcall.

  CORBA::Policy_ptr get_default_policy (
      CORBA::PolicyType policy,
      CORBA::Environment &ACE_TRY_ENV =
        TAO_default_environment ());
  // Accesor to obtain the default policy for a particular policy
  // type.
  // If there is no default policy it returns CORBA::Policy::_nil ()

  TAO_RelativeRoundtripTimeoutPolicy *default_relative_roundtrip_timeout (void) const;

  TAO_Client_Priority_Policy *default_client_priority (void) const;

  TAO_Sync_Scope_Policy *default_sync_scope (void) const;

  TAO_Buffering_Constraint_Policy *default_buffering_constraint (void) const;

  TAO_None_Sync_Strategy &none_sync_strategy (void);
  // This strategy will buffer messages.

  TAO_RelativeRoundtripTimeoutPolicy *stubless_relative_roundtrip_timeout (void);
  // Access to the RoundtripTimeoutPolicy policy set on the thread or
  // on the ORB.  In this method, we do not consider the stub since we
  // do not have access to it.

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  TAO_Transport_Sync_Strategy &transport_sync_strategy (void);
  // This strategy will sync with the transport.

#if (TAO_HAS_RT_CORBA == 1)

  TAO_Priority_Mapping *priority_mapping (void);
  // Access the priority mapping class, this is a TAO extension but
  // there is no standard way to get to it either.

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
           int break_on_timeouts,
           CORBA::Environment &ACE_TRY_ENV);
  // Run the event loop

  void shutdown (CORBA::Boolean wait_for_completion,
                 CORBA::Environment &ACE_TRY_ENV =
                     TAO_default_environment ());
  // End the event loop

  int has_shutdown (void);
  // Get the shutdown flag value

  void destroy (CORBA::Environment &ACE_TRY_ENV =
                    TAO_default_environment ());
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
                                CORBA::Environment &ACE_TRY_ENV =
                                    TAO_default_environment ());
  // Makes sure that the ORB is open and then creates a TAO_Stub
  // based on the endpoint.

  const char *orbid (void) const;
  // Return ORBid string.

  CORBA::Object_ptr implrepo_service (void);
  void implrepo_service (const CORBA::Object_ptr ir);
  // Set/Get the IOR of the Implementation Repository service.

protected:
  int set_iiop_endpoint (int dotted_decimal_addresses,
                         CORBA::UShort port,
                         ACE_CString &host,
                         ACE_CString &iiop_endpoint);
  // Set the endpoint

  int init (int &argc, char **argv, CORBA::Environment &ACE_TRY_ENV);
  // Initialize the guts of the ORB Core.  It is intended that this be
  // called by <CORBA::ORB_init>.

  int fini (void);
  // Final termination hook, typically called by CORBA::ORB's DTOR.

  void create_and_set_root_poa (const char *adapter_name,
                                TAO_POA_Manager *poa_manager,
                                const TAO_POA_Policies *policies,
                                CORBA::Environment &ACE_TRY_ENV);
  // Initialize the root POA.

  TAO_Object_Adapter *object_adapter_i (void);
  // Get <Object Adapter>, assume the lock is held...

  ACE_Allocator *input_cdr_dblock_allocator_i (TAO_ORB_Core_TSS_Resources *);
  ACE_Allocator *input_cdr_buffer_allocator_i (TAO_ORB_Core_TSS_Resources *);
  // Implement the input_cdr_*_allocator() routines using pre-fetched
  // TSS resources.  This minimizes the number of calls to them.

  int open (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ());
  // Set up the ORB Core's acceptor to listen on the
  // previously-specified port for requests.  Returns -1 on failure,
  // else 0.

protected:
  ACE_SYNCH_MUTEX lock_;
  // Synchronize internal state...

  // = Data members.

  TAO_Connector_Registry *connector_registry_;
  // The connector registry which all active connecters must register
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

  CORBA::ORB_var orb_;
  // @@ Should we keep a single ORB pointer? This is good because
  //    multiple calls to ORB_init() with the same ORBid can use the
  //    same object, but maybe don't want so much coupling.
  // Pointer to the ORB.

  TAO_POA *root_poa_;
  // Pointer to the root POA.  It will eventually be the pointer
  // returned by calls to <CORBA::ORB::resolve_initial_references
  // ("RootPOA")>.

  PortableServer::POA_var root_poa_reference_;
  // Cached POA reference

  TAO_ORB_Parameters orb_params_;
  // Parameters used by the ORB.

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

  TAO_POA_Current *poa_current_;
  // POA current.
  //
  // Note that this is a pointer in order to reduce the include file
  // dependencies.
  //

  TAO_Object_Adapter *object_adapter_;
  // Object Adapter.

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

  TAO_Priority_Mapping *priority_mapping_;
  // The priority mapping.

#if (TAO_HAS_CORBA_MESSAGING == 1)

  TAO_None_Sync_Strategy *none_sync_strategy_;
  // This strategy will buffer messages.

#endif /* TAO_HAS_CORBA_MESSAGING == 1 */

  TAO_Transport_Sync_Strategy *transport_sync_strategy_;
  // This strategy will sync with the transport.

  int svc_config_argc_;
  // The number of arguments in the service configurator argument vector.

  char **svc_config_argv_;
  // The argument vector for the service configurator.
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

  TAO_POA_Current_Impl *poa_current_impl_;
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
typedef ACE_TSS_Singleton<TAO_TSS_Resources, ACE_SYNCH_MUTEX>
        TAO_TSS_RESOURCES;

// ****************************************************************

class TAO_Export TAO_ORB_Table
{
  // = TITLE
  //   Keep a table with all the ORBs in the system.
  //
  // = DESCRIPTION
  //   CORBA::ORB_init() is supposed to return the same ORB if the
  //   user specifies the same ORBid, either in the ORB_init()
  //   parameter or in the -ORBid option.
  //   This class is used to implement that feature.
  //   It is also useful when trying to determine if an object
  //   reference is collocated or not.
  //
public:
  ~TAO_ORB_Table (void);
  // destructor

  // @@ Ossama, we may use a Hash_Map_Manager and use "const char*"
  //    instead of ACE_CString to speed things up.
  typedef ACE_Map_Manager<ACE_CString,TAO_ORB_Core*,ACE_Null_Mutex> Table;
  typedef Table::iterator Iterator;

  Iterator begin (void);
  Iterator end (void);
  int bind (const char* orb_id, TAO_ORB_Core* orb_core);
  TAO_ORB_Core* find (const char* orb_id);
  int unbind (const char* orb_id);
  // The canonical ACE_Map methods.

  TAO_ORB_Core *first_orb (void);
  // Obtain the first ORB for the ORB_Core_instance() implementation

  static TAO_ORB_Table* instance (void);
  // Return a unique instance

protected:
  friend class ACE_Singleton<TAO_ORB_Table,ACE_SYNCH_MUTEX>;
  TAO_ORB_Table (void);
  // Constructor

private:
  Table table_;
  // The implementation.

  TAO_ORB_Core *first_orb_;
  // The first ORB created by the user
};

// ****************************************************************

TAO_Export TAO_ORB_Core *TAO_ORB_Core_instance (void);

#if defined (__ACE_INLINE__)
# include "tao/ORB_Core.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_ORB_CORE_H */
