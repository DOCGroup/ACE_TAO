/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Naming_Utils.h
 *
 *  $Id$
 *
 *    Implement wrappers useful to Naming Service clients and servers.
 *
 *
 *  @author Nagarajan Surendran (naga@cs.wustl.edu)
 *  @author Matt Braun <mjb2@cs.wustl.edu>
 *  @author and Douglas C. Schmidt <schmidt@cs.wustl.edu>.
 */
//=============================================================================


#ifndef TAO_NAMING_UTILS_H
#define TAO_NAMING_UTILS_H
#include /**/ "ace/pre.h"

#include "tao/corba.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/IOR_Multicast.h"
#include "Naming_Context_Interface.h"
#include "nsconf.h"

// Forward decl;
class TAO_Persistent_Context_Index;
class TAO_Storable_Naming_Context_Activator;

/**
 * @class TAO_Naming_Server
 *
 * @brief Defines a wrapper class that holds the root Naming Context.
 *
 * This class either finds an existing Naming Service (if the
 * <resolve_for_existing_naming_service> flag is set) or creates
 * one (if <resolve_for_existing_naming_service> flag isn't set or
 * Naming Service was not found).  This class also defines the
 * operator-> so that  <NamingContext> functions like <bind>,
 * <unbind> .. can be called directly on a <TAO_Naming_Server>
 * object, and be forwareded to the root Naming Context.
 * This class is intended to simplify
 * programs that want to play the role of a Naming Service
 * server.  To simplify programs that want to play the role of
 * Naming Service clients, use <TAO_Naming_Client>.
 * If a Naming Service is created locally, a TAO_IOR_Multicast
 * event handler is created and installed on the ORB's reactor.
 * This event handler allows other clients on the network to
 * discover and use this Naming Service.
 * Event handler listens on a multicast port for messages from
 * clients looking for a Naming Service, and sends back the ior
 * of the root Naming Context.  For more information on how this
 * bootstraping through a multicast process works, check out
 * orbsvcs/orbsvcs/TAO_IOR_Multicast.*, implementation of
 * <resolve_initial_references>, and orbsvcs/Naming/README.
 */
class TAO_Naming_Export TAO_Naming_Server
{
public:
  /// Default constructor.
  TAO_Naming_Server (void);

  /**
   * Constructor.  Attempts to find an existing Naming Service if
   * <resolve_for_existing_naming_service> is set to true.  If it is
   * false, or no Naming Service was found during a <timeout> period,
   * create the Naming Service in this process.  If creating the
   * Naming Service locally, make the root context of size
   * <context_size>, register it under the <poa>, and make the Naming
   * Service persistent if <persistence_location> is not 0.
   * (<persistence_location> specifies name of the file to use for
   * persistent storage, <base_addr> specifies the address used for
   * memory mapping <persistent_location> file).
   * If <enable_multicast> is not zero then the service will respond
   * to multicast location queries.
   */
  TAO_Naming_Server (CORBA::ORB_ptr orb,
                     PortableServer::POA_ptr poa,
                     size_t context_size = ACE_DEFAULT_MAP_SIZE,
                     ACE_Time_Value *timeout = 0,
                     int resolve_for_existing_naming_service = 1,
                     const ACE_TCHAR *persistence_location = 0,
                     void *base_addr = TAO_NAMING_BASE_ADDR,
                     int enable_multicast = 1,
                     int use_storable_context = 0);

  /**
   * Initializer.  Attempts to find an existing Naming Service if
   * <resolve_for_existing_naming_service> is set to true.  If it is
   * false, or no Naming Service was found during a <timeout> period,
   * create the Naming Service in this process.  If creating the
   * Naming Service locally, make the root context of size
   * <context_size>, register it under the <poa>, and make the Naming
   * Service persistent if <persistence_location> is not 0.
   * (<persistence_location> specifies name of the file to use for
   * persistent storage, <base_addr> specifies the address used for
   * memory mapping <persistent_location> file).
   * If <enable_multicast> is not zero then the service will respond
   * to multicast location queries.
   */
  int init (CORBA::ORB_ptr orb,
            PortableServer::POA_ptr poa,
            size_t context_size = ACE_DEFAULT_MAP_SIZE,
            ACE_Time_Value *timeout = 0,
            int resolve_for_existing_naming_service = 1,
            const ACE_TCHAR *persistence_location = 0,
            void *base_addr = TAO_NAMING_BASE_ADDR,
            int enable_multicast = 1,
            int use_storable_context = 0);

  /// Initialize the Naming Service with the command line arguments and
  /// the ORB.
  int init_with_orb (int argc,
                     ACE_TCHAR *argv [],
                     CORBA::ORB_ptr orb);

  /// Destroy the child POA created in <init_with_orb>
  int fini (void);

  /// Destructor.
  ~TAO_Naming_Server (void);

  /// Returns the IOR of the naming service.
  char * naming_service_ior (void);

  /// Returns a <NamingContext_ptr> for the root Naming Context.
  CosNaming::NamingContext_ptr operator-> (void) const;

protected:
  /**
   * Helper method: create Naming Service locally.
   * Make the root context of size
   * <context_size>, register it under the <root_poa>, and make the Naming
   * Service persistent if <persistence_location> is not 0.
   * (<persistence_location> specifies name of the file to use for
   * persistent storage).
   * If <enable_multicast> is not zero then the service will respond
   * to multicast location queries.
   */
  int init_new_naming (CORBA::ORB_ptr orb,
                       PortableServer::POA_ptr root_poa,
                       const ACE_TCHAR *persistence_location,
                       void *base_addr,
                       size_t context_size,
                       int enable_multicast,
                       int use_storable_context);

  /// parses the arguments.
  int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Root NamingContext_ptr.
  CosNaming::NamingContext_var naming_context_;

  /// The ior_multicast event handler.
  TAO_IOR_Multicast *ior_multicast_;

  /// The IOR string of the root naming context.
  CORBA::String_var naming_service_ior_;

  /**
   * Pointer to the object used to create/initialize
   * the Naming Service when local persistent Naming Service is
   * desired.
   */
  TAO_Persistent_Context_Index *context_index_;

  /// The ORB
  CORBA::ORB_var orb_;

  /// The Root POA.
  PortableServer::POA_var root_poa_;

  /// The Naming Service POA.
  PortableServer::POA_var ns_poa_;

  /// File to output the Naming Service IOR.
  FILE *ior_output_file_;

  /// File to output the process id.
  const ACE_TCHAR *pid_file_name_;

  /**
   * Size of the hash_table allocated upon the creation of the Naming
   * Service context (if one is created).  Note: all the contexts
   * created under the given context will use the same size for their
   * initial hash table allocations.
   */
  size_t context_size_;

  /// Path to the file to be used to store/read in Naming Service
  /// persistent state.
  const ACE_TCHAR *persistence_file_name_;

  /// Address to be used for memory mapping Naming Service state file,
  /// identified by the <persistence_file_name_>.
  void *base_address_;

  /// After how long the server should stop listening to requests (in
  /// seconds).
  size_t time_;

  /// If not zero multicast is enabled.
  int multicast_;

  /// If not zero use flat file persistence
  int use_storable_context_;

  /**
   * If not zero use servant activator that uses flat file persistence.
   */
  int use_servant_activator_;

  /**
   * Need to retain the servant activator between calls to
   * init_with_orb() and init_new_naming().
   */
  TAO_Storable_Naming_Context_Activator *servant_activator_;

  /**
   * If not zero support redundant naming servers.
   */
  int use_redundancy_;

};

/**
 * @class TAO_Naming_Client
 *
 * @brief This class is intended to simplify programs that want to play
 * the role of Naming Service clients.
 *
 * Wrapper class that locates the root Naming Context.  It also
 * defines the operator-> so that <NamingContext> functions like
 * <resolve>, etc. can be directly called on a
 * <TAO_Naming_Client> object, and will be forwarded to the root
 * Naming Context.
 */
class TAO_Naming_Export TAO_Naming_Client
{
public:
  // = Initialization and termination methods.

  /// Default constructor.
  TAO_Naming_Client (void);

  /**
   * Look for a Naming Service for a period of <timeout> using
   * <resolve_initial_references>.  Return 0 if Naming Service is
   * successfully located, and -1 on failure.
   */
  int init (CORBA::ORB_ptr orb,
            ACE_Time_Value *timeout = 0);

  /// Destructor.
  ~TAO_Naming_Client (void);

  /// Returns a pointer to the root Naming Context.
  CosNaming::NamingContext_ptr operator-> (void) const;

  /**
   * Returns a pointer to the root Naming Context.  This ptr is
   * duplicated via <_duplicate> so that it can be stored into a
   * <*_var>.
   */
  CosNaming::NamingContext_ptr get_context (void) const;

protected:
  /// Reference to the root Naming Context.
  CosNaming::NamingContext_var naming_context_;
};

#include /**/ "ace/post.h"
#endif /* TAO_NAMING_UTILS_H */
