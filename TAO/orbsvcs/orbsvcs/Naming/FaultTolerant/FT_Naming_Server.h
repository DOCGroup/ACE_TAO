// -*- C++ -*-

//=============================================================================
/**
 *  @file    FT_Naming_Server.h
 *
 * $Id$
 *
 *    Implement functionality necessary for fault tolerant naming service.
 *    Adds support for Object Group Manager interfaces as well.  This class
 *    extends the TAO_Naming_Server.
 *
 *  @author Kevin Stanley
 */
//=============================================================================
//

#ifndef TAO_FT_NAMING_SERVER_H
#define TAO_FT_NAMING_SERVER_H

#include "orbsvcs/Naming/Naming_Server.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Manager.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Replication_Manager.h"
#include "orbsvcs/Naming/FaultTolerant/ftnaming_export.h"
#include "tao/IORManipulation/IORManip_Loader.h"
#include "ace/Recursive_Thread_Mutex.h"

/**
 * @class TAO_FT_Naming_Server
 *
 * @brief Defines a class derived from the TAO_Naming_Server to extend
   the functionality to support an ObjectGroupManager interface and a
   load balancing capability for objects that are bound within an object
   group within the naming service. The Naming Service will extend the
   resolve and resolve_str operations to perform load balancing on the
   objects within the object group using a specified load balancing
   strategy.
 */
class TAO_FtNaming_Export TAO_FT_Naming_Server : public TAO_Naming_Server
{
public:
  /// Default Constructor.
  TAO_FT_Naming_Server (void);

  /**
   * Helper method: Derived from TAO_Naming Server to
   * Should create & initialize Naming Service components
   * Make the root context of size
   * <context_size>, register it under the <root_poa>, and make the Naming
   * Service persistent if <persistence_location> is not 0.
   * (<persistence_location> specifies name of the file to use for
   * persistent storage).
   * If <enable_multicast> is not zero then the service will respond
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

  /// Initialize the Naming Service and Object Group Manager with the command line
  /// arguments and the ORB. Overrridden from TAO_Naming_Server
  virtual int init_with_orb (int argc, ACE_TCHAR *argv [], CORBA::ORB_ptr orb);

  /**
   * Accessors and mutators for object references.
   */
  void peer_root_context (CosNaming::NamingContext_ptr peer_cxt);
  CosNaming::NamingContext_ptr peer_root_context (void);

  void peer_naming_manager (FT::NamingManager_ptr peer_nm);
  FT::NamingManager_ptr peer_naming_manager (void);

  /// Returns a <NamingContext_ptr> for the root Naming Context.
  CosNaming::NamingContext_ptr my_root_context (void) const;

  FT::NamingManager_ptr my_naming_manager (void) const;

  /// Initialize the naming manager with the ORB.
  int init_naming_manager_with_orb (int argc, ACE_TCHAR *argv [], CORBA::ORB_ptr orb);

  /// Initialize the replication manager with the ORB.
  int init_replication_manager_with_orb (int argc, ACE_TCHAR *argv [], CORBA::ORB_ptr orb);

  /// Overridden parse operation. Only allows options supported by the FT_Naming_Server
  /// and adds options for the object group manager
  virtual int parse_args (int argc,
                          ACE_TCHAR *argv[]);

  /// Shut down the TAO_Naming_Service; you must still call fini().
  void shutdown (void);

  /// Returns the IOR of the naming manager.
  char* replication_manager_ior (void);

  /// Returns the IOR of the naming manager.
  char * naming_manager_ior (void);

  virtual int update_object_group (
    const FT_Naming::ObjectGroupUpdate & group_info);

  virtual int update_naming_context (
    const FT_Naming::NamingContextUpdate & naming_context);

  /// Destructor.
  virtual ~TAO_FT_Naming_Server (void);

protected:

  /// Read the replica from the specified file
  int read_reference_from_file (const char* replica_file_name,
                                CORBA::Object_out obj_ref);

  /// Export the NameService and NameManager combined object references
  /// to the file names provided
  int export_ft_naming_references (void);

  const ACE_TCHAR * replica_id_;

  /// The object that implements the ObjectGroupManager, PropertyManager,
  /// and GenericFactory interfaces.
  TAO_FT_Naming_Manager naming_manager_;

  FT::NamingManager_var my_naming_manager_;
  FT::NamingManager_var peer_naming_manager_;
  CosNaming::NamingContext_var peer_root_context_;

  /// The object that implements the FT_Naming::Replication_Manager
  /// interface
  TAO_FT_Naming_Replication_Manager* replication_manager_;

  /// File to output the Object Group Manager IOR.
  const ACE_TCHAR *naming_manager_ior_file_name_;

  /// Path to the file to be used to store/read in Object Group Manager
  /// persistent state.
  const ACE_TCHAR *naming_manager_persistence_file_name_;

  /// The IOR string of the object group manager.
  CORBA::String_var naming_manager_ior_;

  /// The IOR string of the object group manager.
  CORBA::String_var replication_manager_ior_;

  /// The IOR string of the peer replica.
  CORBA::String_var replica_peer_ior_;

  /// The Object Group Manager POA.
  PortableServer::POA_var naming_manager_poa_;

  PortableServer::POA_var replication_manager_poa_;

  int use_object_group_persistence_;
  ACE_CString object_group_dir_;

  bool is_primary_;

  /// Lock used to serialize access to fault tolerant extensions
  /// to Naming Service.
  TAO_SYNCH_RECURSIVE_MUTEX lock_;

private:
  static const ACE_TCHAR* primary_replica_ior_filename;
  static const ACE_TCHAR* backup_replica_ior_filename;

 };

#endif /* TAO_FT_NAMING_SERVER_H */
