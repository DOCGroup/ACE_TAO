// This may look like C, but it's really -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO
//
// = FILENAME
//     orb_core.h
//
// = AUTHOR
//     Chris Cleeland
//
// ============================================================================

#if !defined (TAO_ORB_CORE_H)
#  define TAO_ORB_CORE_H

#  include "tao/corba.h"

class TAO_Collocation_Table_Lock : public ACE_Adaptive_Lock
{
  // TITLE
  //   This lock class determines the type underlying lock
  //   when it gets constructed.
public:
  TAO_Collocation_Table_Lock (void);
  ~TAO_Collocation_Table_Lock (void);
};

typedef ACE_Hash_Map_Manager<ACE_Hash_Addr<ACE_INET_Addr>, TAO_POA *, TAO_Collocation_Table_Lock>
        TAO_GLOBAL_Collocation_Table;

class TAO_Cached_Connector_Lock : public ACE_Adaptive_Lock
{
  // TITLE
  //   This lock class determines the type underlying lock
  //   when it gets constructed.
public:
  TAO_Cached_Connector_Lock (void);
  ~TAO_Cached_Connector_Lock (void);
};

typedef ACE_Strategy_Connector<TAO_Client_Connection_Handler, TAO_SOCK_CONNECTOR>
        TAO_CONNECTOR;

typedef ACE_Cached_Connect_Strategy<TAO_Client_Connection_Handler,
                                    TAO_SOCK_CONNECTOR,
                                    TAO_Cached_Connector_Lock>
        TAO_CACHED_CONNECT_STRATEGY;

typedef ACE_NOOP_Creation_Strategy<TAO_Client_Connection_Handler>
        TAO_NULL_CREATION_STRATEGY;

typedef ACE_NOOP_Concurrency_Strategy<TAO_Client_Connection_Handler>
        TAO_NULL_ACTIVATION_STRATEGY;


// Forward decl.
class TAO_Resource_Factory;

class TAO_Export TAO_ORB_Core
{
  // = TITLE
  //    Encapsulates the state of an ORB.
  //
  // = DESCRIPTION
  //    This class encapsulates the state of an ORB.  The motivation for
  //    this is so that the state can be accessed as a singleton.  The scope
  //    of the state can be regulated by utilizing singletons of different
  //    scope, e.g., <ACE_Singleton> will provide process-wide scope, while
  //    <ACE_TSS_Singleton> will provide thread-specific scope (which
  //    is useful for the "thread-per-rate" real-time concurrency
  //    model).
  friend class CORBA_ORB;
  friend CORBA::ORB_ptr CORBA::ORB_init (int &,
                                         char * const*,
                                         const char *,
                                         CORBA_Environment &_env = CORBA_Environment::default_environment ());
public:
  // = Initialization and termination methods.
  TAO_ORB_Core (void);
  // Constructor.

  ~TAO_ORB_Core (void);
  // Destructor

  TAO_OA_Parameters *oa_params (void);
  // Accessor for the Object Adapter parameters.

  TAO_ORB_Parameters *orb_params (void);
  // Accessor for the ORB parameters.

  TAO_POA_Current *poa_current (void);
  // Accessor which returns a pointer to a structure containing
  // context on the current POA upcall.

  TAO_POA_Current *poa_current (TAO_POA_Current *new_current);
  // Sets the thread-specific pointer to the new POA Current state,
  // returning a pointer to the existing POA Current state.

  // = Set/get the connector.
  TAO_CONNECTOR *connector (TAO_CONNECTOR *c);
  TAO_CONNECTOR *connector (void);

  // = Set/get the acceptor.
  TAO_ACCEPTOR *acceptor (TAO_ACCEPTOR *a);
  TAO_ACCEPTOR *acceptor (void);
  // Accessor which returns the acceptor.

  // = Set/get pointer to the ORB.
  CORBA::ORB_ptr orb (CORBA::ORB_ptr);
  CORBA_ORB_ptr orb (void);

  // = Set/get the <ACE_Reactor>.
  ACE_Reactor *reactor (ACE_Reactor *r);
  ACE_Reactor *reactor (void);

  // = Set/get the <ACE_Thread_Manager>.
  ACE_Thread_Manager *thr_mgr (ACE_Thread_Manager *tm);
  ACE_Thread_Manager *thr_mgr (void);

  // = Set/get <Acceptor> for the POA.
  TAO_POA *root_poa (TAO_POA *np);
  TAO_POA *root_poa (const char *adapter_name = TAO_DEFAULT_ROOTPOA_NAME,
                     TAO_POA_Manager *poa_manager = 0,
                     const TAO_POA_Policies *policies = 0,
                     TAO_Object_Table *active_object_map = 0);

  ACE_INET_Addr &addr (void);
  // Accessors for the local address on which we're listening.

  int inherit_from_parent_thread (TAO_ORB_Core *p);
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

  CORBA::Boolean using_collocation (void);
  // Check if we are optimizing collocation objects.

  CORBA::Boolean using_collocation (CORBA::Boolean);
  // Set if we want to use optimized collocation objects.

  int add_to_collocation_table (void);
  // Added this ORB into collocation table.

  TAO_POA *get_collocated_poa (ACE_INET_Addr &addr);
  // See if we have a collocated address, if yes, return the POA
  // associated with the address.

#if defined (TAO_ARL_USES_SAME_CONNECTOR_PORT)
  CORBA::Boolean arl_same_port_connect (void);
  // Access function to query whether we want this feature or not.
  // This is a specialization only for the ARL at Wash U.
  // This setting this flag will for the connect use the same port
  // that the server uses.
#endif /* TAO_ARL_USES_SAME_CONNECTOR_PORT */

  int leader_available (void);
  // returns the refcount on the leader

  int I_am_the_leader_thread (void);
  // returns 1 if we are the leader thread,
  // else 0

  void set_leader_thread (void) ;
  // sets the thread_available flag and the thread ID of the leader 
  // thread in the leader-follower model

  void set_leader_thread (ACE_thread_t thread_ID);
  // sets the thread ID of the leader thread in the leader-follower
  // model

  void unset_leader_thread (void);
  // sets the leader_available flag to false

  int unset_leader_wake_up_follower (void);
  // sets the leader_available flag to false
  // and wakes up a new follower

  ACE_SYNCH_MUTEX &leader_follower_lock (void);
  // returns the leader-follower lock

  int add_follower (ACE_SYNCH_CONDITION *follower_ptr);
  // adds the a follower to the set of followers in the leader-
  // follower model
  // returns 0 on success, -1 on failure

  int follower_available ();
  // checks for the availablity of a follower
  // returns 1 on available, 0 else

  int remove_follower (ACE_SYNCH_CONDITION *follower_ptr);
  // removes a follower from the leader-follower set
  // returns 0 on success, -1 on failure

  ACE_SYNCH_CONDITION *get_next_follower (void);
  // returns randomly a follower from the leader-follower set
  // returns follower on success, else 0

  ACE_Allocator *data_block_allocator (void);
  // This allocator is always TSS and has no locks. It is intended for
  // allocating the ACE_Data_Blocks used in *outgoing* CDR streams.

  ACE_Allocator *cdr_buffer_allocator (void);
  // This allocator is always TSS and has no locks. It is intended for
  // allocating the buffers used in *outgoing* CDR streams.

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
  // 

private:
  int init (int& argc, char ** argv);
  // Initialize the guts of the ORB Core.  It is intended that this be
  // called by <CORBA::ORB_init>.

  int fini (void);
  // Final termination hook, typically called by CORBA::ORB's DTOR.

  int preconnect (const char *preconnections);
  // Attempt to establish connections specified in <preconnections>.
  // Returns -1 in case of error, or the number of connections
  // actually established.

  void create_and_set_root_poa (const char *adapter_name,
                                TAO_POA_Manager *poa_manager,
                                const TAO_POA_Policies *policies,
                                TAO_Object_Table *active_object_map);
  // Initialize the root POA.

  // = Data members.
  ACE_Reactor *reactor_;
  // Used for responding to I/O reactively

  ACE_Thread_Manager *thr_mgr_;
  // Used to manage threads within the ORB

  TAO_CONNECTOR *connector_;
  // The connector actively initiating connection requests.

  CORBA::ORB_ptr orb_;
  // Pointer to the ORB.

  TAO_POA *root_poa_;
  // Pointer to the root POA.  It will eventually be the pointer
  // returned by calls to <CORBA::ORB::resolve_initial_references
  // ("RootPOA")>.

  TAO_OA_Parameters *oa_params_;
  // Parameters which should be used by OAs attached to this ORB.

  TAO_ORB_Parameters *orb_params_;
  // Parameters used by the ORB.

  ACE_INET_Addr *addr_;
  // The address of the endpoint on which we're listening for
  // connections and requests.

  TAO_ACCEPTOR *acceptor_;
  // The acceptor passively listening for connection requests.

  TAO_POA_Current *poa_current_;
  // Points to structure containing state for the current upcall
  // context in this thread.  Note that it does not come from the
  // resource factory because it must always be held in
  // thread-specific storage.  For now, since TAO_ORB_Core instances
  // are TSS singletons, we simply ride along and don't allocate
  // occupy another TSS slot since there are some platforms where
  // those are precious commodities (e.g., NT).

  TAO_Resource_Factory *resource_factory_;
  // Handle to the factory for resource information..

  CORBA::Boolean resource_factory_from_service_config_;
  // TRUE if <resource_factory_> was obtained from the Service
  // Configurator.

  TAO_Client_Strategy_Factory *client_factory_;
  // Handle to the factory for Client-side strategies.

  CORBA::Boolean client_factory_from_service_config_;
  // TRUE if <client_factory_> was obtained from the Service
  // Configurator.

  TAO_Server_Strategy_Factory *server_factory_;
  // Handle to the factory for Server-side strategies.

  CORBA::Boolean server_factory_from_service_config_;
  // TRUE if <server_factory_> was obtained from the Service
  // Configurator.

  CORBA::Boolean opt_for_collocation_;
  // TRUE if we want to take advantage of collocation optimization in
  // this ORB.

#if defined (TAO_ARL_USES_SAME_CONNECTOR_PORT)
  CORBA::Boolean arl_same_port_connect_;
  // This is a specialization only for the ARL at Wash U.
  // This setting this flag will for the connect use the same port
  // that the server uses.
#endif /* TAO_ARL_USES_SAME_CONNECTOR_PORT */

  char *preconnections_;
  // A string of comma-separated <{host}>:<{port}> pairs used to
  // pre-establish connections using <preconnect>.

  typedef ACE_Malloc<ACE_LOCAL_MEMORY_POOL,ACE_Null_Mutex> TSS_MALLOC;
  typedef ACE_Allocator_Adapter<TSS_MALLOC> TSS_ALLOCATOR;

  TSS_ALLOCATOR data_block_allocator_;
  // The Allocator for the ACE_Data_Blocks.
  
  TSS_ALLOCATOR cdr_buffer_allocator_;
  // The Allocator for the CDR buffers.

  CORBA_Environment* default_environment_;
  // The default environment for the thread.

  CORBA_Environment tss_environment_;
  // If the user (or library) provides no environment the ORB_Core
  // still holds one.
};

class TAO_Default_Reactor : public ACE_Reactor
{
  // = TITLE
  //
  //   Force TAO to use Select Reactor.
public:
  TAO_Default_Reactor (void);
  virtual ~TAO_Default_Reactor (void);
};

class TAO_Export TAO_Resource_Factory : public ACE_Service_Object
{
  // = TITLE
  //   Factory which manufacturers resources for use by the ORB Core.
  //
  // = DESCRIPTION
  //   This class is a factory/repository for critical ORB Core
  //   resources.  Using a <{resource source specifier}> as a
  //   discriminator, the factory can return resource instances which
  //   are, e.g., global, stored in thread-specific storage, stored in
  //   shared memory, etc.

public:
  // = Initialization and termination methods.
  TAO_Resource_Factory (void);
  virtual ~TAO_Resource_Factory (void);

  // = Service Configurator hooks.
  virtual int init (int argc, char *argv[]);

  virtual int parse_args (int argc, char *argv[]);
  // Arguments are in the form of -ORBxxx.  Valid arguments are:
  // <-ORBresources> <{which}> where <{which}> is one of <global> or
  // <tss>.

  // = Member Accessors
  enum
  {
    TAO_GLOBAL,
    TAO_TSS
  };

  // = Range of values for <{resource source specifier}>.
  virtual void resource_source (int which_source);
  // Set the resource source specifier.
  virtual int resource_source (void);
  // Get the resource source specifier.

  virtual void poa_source (int which_source);
  // Set the POA source specifier.
  virtual int poa_source (void);
  // Get the POA source specifier.

  // = Resource Retrieval
  //
  // Methods in this category return pointers to resources.  Based on
  // the resource source specifier value, these resources may be
  // either global, i.e., the same resource is used across all threads
  // throughout the process, or thread-specific, i.e., different
  // resources are used in each thread.  Currently, all resources are
  // either global or thread-specific; choosing on a per-resource
  // basis is not supported.

  virtual ACE_Reactor *get_reactor (void);
  // Return an <ACE_Reactor> to be utilized.

  virtual ACE_Thread_Manager *get_thr_mgr (void);
  // Return an <ACE_Thread_Manager> to be utilized.

  virtual TAO_CONNECTOR *get_connector (void);
  // Return an Connector to be utilized.

  virtual TAO_CACHED_CONNECT_STRATEGY *get_cached_connect_strategy (void);
  // Return an Cached Connect Strategy to be utilized.

  virtual TAO_NULL_CREATION_STRATEGY *get_null_creation_strategy (void);
  // This no-op creation strategy is necessary for using the
  // <Strategy_Connector> with the <Cached_Connect_Strategy>.

  virtual TAO_NULL_ACTIVATION_STRATEGY *get_null_activation_strategy (void);
  // This no-op activation strategy prevents the cached connector from
  // calling the service handler's <open> method multiple times.

  virtual TAO_ACCEPTOR *get_acceptor (void);
  // Return an Acceptor to be utilized.

  virtual TAO_ORB_Parameters *get_orb_params (void);
  // Return ORB parameters to be utilized.

  virtual TAO_OA_Parameters *get_oa_params (void);
  // Return ORB parameters to be utilized.

  virtual ACE_INET_Addr *get_addr (void);
  // Return an address to be used for the endpoint for connections and
  // requests.

  virtual CORBA::ORB_ptr get_orb (void);
  // Return an ORB ptr to be utilized.

  virtual TAO_POA *get_root_poa (void);
  // Return a root poa to be utilized.

  virtual ACE_Allocator *get_allocator (void);
  // Return a pointer to an ACE_Allocator used for allocating memory
  // within the ORB.

  virtual TAO_GLOBAL_Collocation_Table *get_global_collocation_table (void);
  // Get the global collocation table.  Return the pointer to the
  // global collocation table if we are using one, otherwise, return
  // 0.

  // = Modifiers

  virtual void set_allocator (ACE_Allocator *alloc);
  // Set the allocator pointer which will be returned by
  // <get_allocator()>.

  virtual void set_orb (CORBA::ORB_ptr op);
  // Set the ORB pointer which will be returned by <get_orb()>.  This
  // is necessary because the ORB is created in application space by
  // <CORBA::ORB_init()>, but needs to be available to stubs and
  // generated code.

  virtual void set_root_poa (TAO_POA *pp);
  // Set the Root POA pointer which will be returned by
  // <get_root_poa()>.  This is necessary because the Root POA is
  // created in application space by <CORBA::ORB_init()>, but needs to
  // be available to stubs and generated code.

  // @@ I suspect that putting these structs inside of this class is
  // going to break some compilers (e.g., HP/YUX) when you try to use
  // this stuff with the ACE_Singletons below.  I suggest you move
  // them out and rename them as TAO_Pre_Allocated, etc.
  //
  // Let's do this after we merge the poa branch and the main back
  // together.
  struct Pre_Allocated
    // = TITLE
    //   Structure containing resources which can be pre-allocated by
    //   the ORB Core without intervention from the application.
  {
    Pre_Allocated (void);
    // Constructor

    TAO_Default_Reactor r_;
    // The Reactor.

    ACE_Thread_Manager tm_;
    // The Thread Manager

    TAO_CONNECTOR c_;
    // The Connector

    TAO_CACHED_CONNECT_STRATEGY cached_connect_strategy_;
    // The Cached Connect Strategy

    TAO_NULL_CREATION_STRATEGY null_creation_strategy_;
    // This no-op creation strategy is necessary for using the
    // <Strategy_Connector> with the <Cached_Connect_Strategy>.

    TAO_NULL_ACTIVATION_STRATEGY null_activation_strategy_;
    // This no-op activation strategy prevents the cached connector from
    // calling the service handler's <open> method multiple times.

    TAO_ACCEPTOR a_;
    // The Acceptor

    TAO_ORB_Parameters orbparams_;
    // ORB Parameters

    TAO_OA_Parameters oaparams_;
    // OA Parameters (will go away with new POA impl)

    ACE_INET_Addr addr_;
    // Address for connection endpoint.
  };

  struct App_Allocated
    // = TITLE
    //   Structure containing resources which can only be allocated
    //   after obtaining information from the application such as
    //   arguments, etc.
  {
    App_Allocated (void): orb_(0), poa_(0), alloc_(0) { };
    // Constructor necessary because we have pointers.  It's inlined
    // here rather than in the .i file because it's easier than trying
    // to re-order header files in corba.h to eliminate the "used
    // before declared inline" warnings/errors on certain compilers.

    // = Resources

    // Note:  These should change to _var types when they are available.
    CORBA::ORB_ptr orb_;
    // Pointer to application-created ORB.

    TAO_POA *poa_;
    // Pointer to application-created POA.

    ACE_Allocator *alloc_;
    // Pointer to application-created ACE_Allocator.
  };

protected:

  int resource_source_;
  // Flag indicating whether resources should be global or
  // thread-specific.

  int poa_source_;
  // Flag indicating whether the POA should be global or
  // thread-specific.  If not set specifically, this takes on the
  // value of <resource_source_>.

  int collocation_table_source_;
  // Flag indicating whether the collocation table should be global
  // thread-specific.  It defaults to TAO_GLOBAL if not set
  // specifically.

  // = Typedefs for the singleton types used to store our orb core
  // information.
  typedef ACE_Singleton<Pre_Allocated, ACE_SYNCH_MUTEX>
          GLOBAL_PRE_ALLOCATED;
  typedef ACE_TSS_Singleton<Pre_Allocated, ACE_SYNCH_MUTEX>
          TSS_PRE_ALLOCATED;

  typedef ACE_Singleton<App_Allocated, ACE_SYNCH_MUTEX>
          GLOBAL_APP_ALLOCATED;
  typedef ACE_TSS_Singleton<App_Allocated, ACE_SYNCH_MUTEX>
          TSS_APP_ALLOCATED;
  typedef ACE_Singleton<TAO_GLOBAL_Collocation_Table, ACE_SYNCH_MUTEX>
          GLOBAL_Collocation_Table;
};

#if defined (__ACE_INLINE__)
# include "tao/ORB_Core.i"
#endif /* __ACE_INLINE__ */

ACE_FACTORY_DECLARE (TAO, TAO_Resource_Factory)

extern TAO_Export TAO_ORB_Core *TAO_ORB_Core_instance (void);

// Declare that the following method should be specialized.

#if defined (ACE_WIN32)
#pragma warning(disable:4231)
extern template size_t
ACE_Hash_Addr<ACE_INET_Addr>::hash_i (const ACE_INET_Addr &addr) const;
#endif /* ACE_WIN32 */

#endif /* TAO_ORB_CORE_H */
