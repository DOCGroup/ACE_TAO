// -*- C++ -*-

//=============================================================================
/**
 *  @file    FT_Naming_Server.h
 *
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
#include "orbsvcs/FT_NamingReplicationC.h"
#include "ace/Recursive_Thread_Mutex.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_FT_Naming_Server;

class TAO_FTNS_Notifier : public ACE_Event_Handler
{
public:
  TAO_FTNS_Notifier (TAO_FT_Naming_Server &owner, bool iors);

  virtual int handle_exception (ACE_HANDLE );

  TAO_FT_Naming_Server &owner_;
  bool iors_;
};


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
  friend class TAO_FTNS_Notifier;

  TAO_FT_Naming_Server (void);

  /// Initialize the Naming Service and Object Group Manager with the command line
  /// arguments and the ORB. Overrridden from TAO_Naming_Server
  virtual int init_with_orb (int , ACE_TCHAR * [], CORBA::ORB_ptr orb);

  /**
   * Accessors and mutators for object references.
   */
  /// Returns a <NamingContext_ptr> for the root Naming Context.
  CosNaming::NamingContext_ptr my_root_context (void) const;
  CosNaming::NamingContext_ptr ft_root_context (void) const;

  /// Returns the reference for this servers local naming manager servant.
  ::FT_Naming::NamingManager_ptr my_naming_manager (void) const;
  ::FT_Naming::NamingManager_ptr ft_naming_manager (void) const;

  /// Initialize the naming manager with the ORB.
  int init_naming_manager_with_orb (int , ACE_TCHAR * [], CORBA::ORB_ptr orb);

  int init_replicator ();

  ACE_TString replica_ior_filename (bool ) const;

/// Setup pairing with peer replica.
  /// Returns 0 on successful pairing with peer.
  /// Returns 1 if peer IOR file is present, but peer is not responding
  /// Returns -1 if pairing is not possible.
  void init_replication_pairing (void);

  void no_replica (void);

  //  TAO_FTNS_Notifier &notifier (void);

  /// Overridden parse operation. Only allows options supported by the FT_Naming_Server
  /// and adds options for the object group manager
  virtual int parse_args (int argc,
                          ACE_TCHAR *argv[]);

  /// Factory method to create a naming context factory for use with
  /// the -u and -r options.
  virtual TAO_Storable_Naming_Context_Factory *
    storable_naming_context_factory (size_t context_size);

  /// Factory method to create a naming context factory for use with
  /// the -f option.
  virtual TAO_Persistent_Naming_Context_Factory *
    persistent_naming_context_factory (void);

  /// Returns the IOR of the replication manager.
  char* replicator_ior (void);

  /// Returns the IOR of the naming manager.
  char * naming_manager_ior (void);

  virtual int update_object_group (
    const ::FT_Naming::ObjectGroupUpdate & group_info);

  virtual int update_naming_context (
    const ::FT_Naming::NamingContextUpdate & naming_context);

  void update_ior (FT_Naming::EntityKind, const CORBA::Object_ptr);
  void combine_iors (FT_Naming::EntityKind, const CORBA::Object_ptr);

  const ACE_CString & ft_endpoint (void);
  const ACE_Time_Value & ft_update_delay (void);

  /// Destroy the child POAs created in @c init_with_orb,
  /// @c init_naming_manager_with_orb, and
  /// @c init_replication_manager_with_orb
  virtual int fini (void);

  /// Destructor.
  virtual ~TAO_FT_Naming_Server (void);

  void update_info (FT_Naming::UpdateInfoSeq &infos);
  void update_iors (const FT_Naming::ReplicaInfo & iors);
  void update_info_i (void);
  void update_iors_i (void);

protected:

  enum FT_IOR_Indexes {
    PEER_ROOT = 1,
    FT_ROOT,
    GROUP,
    PEER_GROUP,
    FT_GROUP,
    REPLICATOR,
    IOR_ARRAY_SIZE
  };

  int recover_iors (void);

  /// The object that implements the ObjectGroupManager, PropertyManager,
  /// and GenericFactory interfaces.
  TAO_FT_Naming_Manager naming_manager_;

  /// The object that implements the FT_Naming::Replication_Manager
  /// interface.
  TAO_FT_Naming_Replication_Manager* replicator_;

  /// The Object Group Manager POA.
  PortableServer::POA_var naming_manager_poa_;

  int use_object_group_persistence_;
  ACE_TString object_group_dir_;

  /// The role this server is supporting in the dual redundant
  /// replication scheme.
  enum ServerRole { PRIMARY, BACKUP, STANDALONE };

  /// The role this server is to take on. Controlled by the
  /// --primary or --backup command line arguments.  Defaults
  /// to STANDALONE if no role is provided in command line.
  ServerRole server_role_;

  /// Lock used to serialize access to fault tolerant extensions
  /// to Naming Service.
  TAO_SYNCH_RECURSIVE_MUTEX lock_;

  /// The endpoint used by the replicator ORB, otherwise it
  /// just picks one
  ACE_CString ft_endpoint_;

  /// The minimum amount of time to wait before issuing a new
  /// update notice to the peer. Heavily loaded systems may want
  /// to use this option to avoid overloading the peer with lots
  /// of small, possibly contradictory changes. During the delay
  /// time, the replicator will hold update notices, removing
  /// earlier ones in favor of later updates.
  ACE_Time_Value ft_update_delay_;

  bool registered_;

  TAO_FTNS_Notifier ior_notifier_;
  TAO_FTNS_Notifier info_notifier_;

  ACE_Unbounded_Queue<FT_Naming::ReplicaInfo> u_iors_;
  ACE_Unbounded_Queue<FT_Naming::UpdateInfoSeq*> u_infos_;
  TAO_SYNCH_MUTEX ior_lock_;
  TAO_SYNCH_MUTEX info_lock_;


 };

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_FT_NAMING_SERVER_H */
