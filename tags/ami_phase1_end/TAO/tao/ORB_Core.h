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

#ifndef TAO_ORB_CORE_H
#define TAO_ORB_CORE_H

#include "ace/Singleton.h"
#include "tao/corbafwd.h"
#include "tao/Environment.h"
#include "tao/Policy_Manager.h"
#include "tao/Resource_Factory.h"
#include "tao/params.h"
#include "tao/POAC.h"

class TAO_Client_Connection_Handler;
class TAO_POA;
class TAO_POA_Current;
class TAO_POA_Manager;
class TAO_POA_Policies;
class TAO_Acceptor;
class TAO_Connector;
class TAO_Connector_Registry;

class TAO_Resource_Factory;
class TAO_Client_Strategy_Factory;
class TAO_Server_Strategy_Factory;
class TAO_Connection_Cache;

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
  friend class CORBA_ORB;
  friend CORBA::ORB_ptr CORBA::ORB_init (int &,
                                         char * const*,
                                         const char *,
                                         CORBA_Environment &);

public:
  // = Initialization and termination methods.
  TAO_ORB_Core (void);
  // Constructor.

  ~TAO_ORB_Core (void);
  // Destructor

  TAO_ORB_Parameters *orb_params (void);
  // Accessor for the ORB parameters.

  TAO_POA_Current *poa_current (void);
  // Accessor which returns a pointer to a structure containing
  // context on the current POA upcall.

  TAO_POA_Current *poa_current (TAO_POA_Current *new_current);
  // Sets the thread-specific pointer to the new POA Current state,
  // returning a pointer to the existing POA Current state.

  // = Set/get the connector registry - used to just be the connector.
  TAO_Connector_Registry *connector_registry (TAO_Connector_Registry *c);
  TAO_Connector_Registry *connector_registry (void);

  // = Set/get the acceptor.
  TAO_Acceptor *acceptor (TAO_Acceptor *a);
  TAO_Acceptor *acceptor (void);
  // Accessor which returns the acceptor.

  // = Set/get pointer to the ORB.
  CORBA::ORB_ptr orb (CORBA::ORB_ptr);
  CORBA::ORB_ptr orb (void);

  // = Set/get the <ACE_Reactor>.
  ACE_Reactor *reactor (ACE_Reactor *r);
  ACE_Reactor *reactor (void);

  // = Set/get the <ACE_Thread_Manager>.
  ACE_Thread_Manager *thr_mgr (ACE_Thread_Manager *tm);
  ACE_Thread_Manager *thr_mgr (void);

  // = Set/get <rootPOA>.
  TAO_POA *root_poa (TAO_POA *np);
  TAO_POA *root_poa (const char *adapter_name = TAO_DEFAULT_ROOTPOA_NAME,
                     TAO_POA_Manager *poa_manager = 0,
                     const TAO_POA_Policies *policies = 0);
  PortableServer::POA_ptr root_poa_reference (CORBA::Environment &TAO_IN_ENV = CORBA::default_environment (),
                                              const char *adapter_name = TAO_DEFAULT_ROOTPOA_NAME,
                                              TAO_POA_Manager *poa_manager = 0,
                                              const TAO_POA_Policies *policies = 0);

  TAO_Object_Adapter *object_adapter (void);
  // Get <Object Adapter>.

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

  TAO_Object_Adapter *get_collocated_object_adapter (const ACE_INET_Addr &addr);
  // See if we have a collocated address, if yes, return the POA
  // associated with the address.

  int add_to_ior_table (ACE_CString init_ref, TAO_IOR_LookupTable &table);
  // Add the init_ref (objectID->IOR) to the Lookup Table.

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

#if defined (TAO_HAS_CORBA_MESSAGING)
  TAO_Policy_Manager *policy_manager (void);
  // Return the Policy_Manager for this ORB.

  TAO_Policy_Current *policy_current (void) const;
  TAO_Policy_Current *policy_current (TAO_Policy_Current *);
  // Accesors to the policy current, this object should be kept in TSS
  // storage.
  // The POA has to reset the policy current object on every upcall.

  CORBA::Policy_ptr get_default_policy (
      CORBA::PolicyType policy,
      CORBA::Environment &ACE_TRY_ENV =
        CORBA::default_environment ());
  // Accesor to obtain the default policy for a particular policy
  // type.
  // If there is no default policy it returns CORBA::Policy::_nil ()

#endif /* TAO_HAS_CORBA_MESSAGING */

protected:
  int set_endpoint (int dotted_decimal_addresses,
                    CORBA::UShort port,
                    ACE_CString &host,
                    ACE_INET_Addr &rendezvous);
  // Set the endpoint

  int init (int& argc, char ** argv);
  // Initialize the guts of the ORB Core.  It is intended that this be
  // called by <CORBA::ORB_init>.

  int fini (void);
  // Final termination hook, typically called by CORBA::ORB's DTOR.

  // int preconnect (const char *preconnections);
  // @@ Now defined in IIOP_Connector
  // Attempt to establish connections specified in <preconnections>.
  // Returns -1 in case of error, or the number of connections
  // actually established.

  void create_and_set_root_poa (const char *adapter_name,
                                TAO_POA_Manager *poa_manager,
                                const TAO_POA_Policies *policies);
  // Initialize the root POA.

protected:
  // = Data members.
  ACE_Thread_Manager *thr_mgr_;
  // Used to manage threads within the ORB

  TAO_Connector_Registry *connector_registry_;
  // The connector registry which all active connecters must register
  // themselves with.

  CORBA::ORB_ptr orb_;
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

  TAO_ORB_Parameters *orb_params_;
  // Parameters used by the ORB.

  // @@ Depricated!
  ACE_INET_Addr *addr_;
  // The address of the endpoint on which we're listening for
  // connections and requests.

  TAO_Acceptor *acceptor_;
  // The acceptor passively listening for connection requests.

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

#if defined (TAO_HAS_CORBA_MESSAGING)
  TAO_Policy_Manager policy_manager_;
  // The Policy_Manager for this ORB.

  TAO_Policy_Manager_Impl default_policies_;
  // The default policies.
#endif /* TAO_HAS_CORBA_MESSAGING */
};

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

  ACE_Reactor *reactor_;
  // Used for responding to I/O reactively

  TAO_POA_Current *poa_current_;
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

#if defined (TAO_HAS_CORBA_MESSAGING)
  TAO_Policy_Current initial_policy_current_;
  // The initial PolicyCurrent for this thread. Should be a TSS
  // resource.

  TAO_Policy_Current* policy_current_;
  // This pointer is reset by the POA on each upcall.
#endif /* TAO_HAS_CORBA_MESSAGING */

  ACE_Allocator *output_cdr_dblock_allocator_;
  ACE_Allocator *output_cdr_buffer_allocator_;
  // The allocators for the output CDR streams.

  TAO_Connection_Cache *connection_cache_;
  // This is is just a place holder, in the future the connection
  // cache will be separated from the connectors and it will be a
  // (potentially) TSS object.
};

// ****************************************************************

extern TAO_Export TAO_ORB_Core *TAO_ORB_Core_instance (void);

#if defined (__ACE_INLINE__)
# include "tao/ORB_Core.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_ORB_CORE_H */
