// -*- C++ -*-

//=============================================================================
/**
 *  @file    Naming_Server.h
 *
 *  $Id$
 *
 *    Implement wrappers useful to Naming Service servers.
 *
 *
 *  @author Nagarajan Surendran (naga@cs.wustl.edu)
 *  @author Matt Braun <mjb2@cs.wustl.edu>
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>.
 */
//=============================================================================

#ifndef TAO_NAMING_SERVER_H
#define TAO_NAMING_SERVER_H
#include /**/ "ace/pre.h"

#include "tao/ORB.h"
#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/IOR_Multicast.h"
#include "orbsvcs/Naming/Naming_Context_Interface.h"
#include "orbsvcs/Naming/nsconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#if !defined (CORBA_E_MICRO)
// Forward decl;
class TAO_Persistent_Context_Index;
class TAO_Storable_Naming_Context_Activator;
#endif /* !CORBA_E_MICRO */

class TAO_Storable_Naming_Context_Factory;
class TAO_Persistent_Naming_Context_Factory;

/**
 * @class TAO_Naming_Server
 *
 * @brief Defines a wrapper class that holds the root Naming Context.
 *
 * This class either finds an existing Naming Service (if the
 * <resolve_for_existing_naming_service> flag is set) or creates
 * one (if @a resolve_for_existing_naming_service flag isn't set or
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
class TAO_Naming_Serv_Export TAO_Naming_Server
{
public:
  /// Default constructor.
  TAO_Naming_Server (void);

  /**
   * Constructor.  Attempts to find an existing Naming Service if
   * @a resolve_for_existing_naming_service is set to true.  If it is
   * false, or no Naming Service was found during a @a timeout period,
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
                     bool resolve_for_existing_naming_service = true,
                     const ACE_TCHAR *persistence_location = 0,
                     void *base_addr = TAO_NAMING_BASE_ADDR,
                     int enable_multicast = 1,
                     int use_storable_context = 0,
                     int round_trip_timeout = 0,
                     int use_round_trip_timeout = 0);

  /**
   * Initializer.  Attempts to find an existing Naming Service if
   * @a resolve_for_existing_naming_service is set to true.  If it is
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
            bool resolve_for_existing_naming_service = true,
            const ACE_TCHAR *persistence_location = 0,
            void *base_addr = TAO_NAMING_BASE_ADDR,
            int enable_multicast = 1,
            int use_storable_context = 0,
            int round_trip_timeout = 0,
            int use_round_trip_timeout = 0);

  /// Initialize the Naming Service with the command line arguments and
  /// the ORB.
  virtual int init_with_orb (int argc, ACE_TCHAR *argv [], CORBA::ORB_ptr orb);

  /// Destroy the child POA created in @c init_with_orb
  virtual int fini (void);

  /// Destructor.
  virtual ~TAO_Naming_Server (void);

  /// Returns the IOR of the naming service.
  char * naming_service_ior (void);

  /// Returns a NamingContext_ptr for the root Naming Context.
  CosNaming::NamingContext_ptr operator-> (void) const;

protected:
  /**
   * Helper method: create Naming Service locally. Can be specialized to
   * refine how Naming Service components are created and initialized
   * Make the root context of size
   * @a context_size, register it under the @a root_poa, and make the Naming
   * Service persistent if @a persistence_location is not 0.
   * (@a persistence_location specifies name of the file to use for
   * persistent storage).
   * If @a enable_multicast is not zero then the service will respond
   * to multicast location queries.
   */
  virtual int init_new_naming (CORBA::ORB_ptr orb,
                       PortableServer::POA_ptr root_poa,
                       const ACE_TCHAR *persistence_location,
                       void *base_addr,
                       size_t context_size,
                       int enable_multicast,
                       int use_storable_context,
                       int round_trip_timeout = 0,
                       int use_round_trip_timeout = 0);

  /// parses the arguments.
  virtual int parse_args (int argc, ACE_TCHAR *argv[]);

  /// Write the provided ior_string to the file. Return 0 if success.
  int write_ior_to_file (const char* ior_string,
                         const char* file_name);

  /* Factory method to create a naming context factory for use with
   * the -u and -r options.
   */
  virtual TAO_Storable_Naming_Context_Factory *
    storable_naming_context_factory (size_t context_size);

  /* Factory method to create a naming context factory for use with
   * the -f option.
   */
  virtual TAO_Persistent_Naming_Context_Factory *
    persistent_naming_context_factory (void);

  /// Root NamingContext_ptr.
  CosNaming::NamingContext_var naming_context_;

  /// The ior_multicast event handler.
  TAO_IOR_Multicast *ior_multicast_;

  /// The IOR string of the root naming context.
  CORBA::String_var naming_service_ior_;

  /// The ORB
  CORBA::ORB_var orb_;

  /// The Root POA.
  PortableServer::POA_var root_poa_;

  /// The Naming Service POA.
  PortableServer::POA_var ns_poa_;

  /// File to output the Naming Service IOR.
  const ACE_TCHAR *ior_file_name_;

  /// File to output the process id.
  const ACE_TCHAR *pid_file_name_;

  /**
   * Size of the hash_table allocated upon the creation of the Naming
   * Service context (if one is created).  Note: all the contexts
   * created under the given context will use the same size for their
   * initial hash table allocations.
   */
  size_t context_size_;

  /// If not zero multicast is enabled.
  int multicast_;

#if !defined (CORBA_E_MICRO)
  /**
   * Pointer to the object used to create/initialize
   * the Naming Service when local persistent Naming Service is
   * desired.
   */
  TAO_Persistent_Context_Index *context_index_;

  /// Path to the file to be used to store/read in Naming Service
  /// persistent state.
  const ACE_TCHAR *persistence_file_name_;

  /// Address to be used for memory mapping Naming Service state file,
  /// identified by the <persistence_file_name_>.
  void *base_address_;

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
#endif /* !CORBA_E_MICRO */

  /**
   * If not zero support redundant naming servers.
   */
  int use_redundancy_;

  /// If not zero use round trip timeout policy set to value specified
  int round_trip_timeout_;
  int use_round_trip_timeout_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_NAMING_SERVER_H */
