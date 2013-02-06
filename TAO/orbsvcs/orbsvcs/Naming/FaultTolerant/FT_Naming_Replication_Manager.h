// -*- C++ -*-

//=============================================================================
/**
 * @file   FT_Naming_Replication_Manager.h
 *
 * $Id$
 *
 * @author Kevin Stanley <stanleyk@ociweb.com>
 */
//=============================================================================


#ifndef TAO_FT_NAMING_REPLICATION_MANAGER_H
#define TAO_FT_NAMING_REPLICATION_MANAGER_H

#include /**/ "ace/pre.h"

#include "orbsvcs/Naming/FaultTolerant/ftnaming_export.h"
#include "orbsvcs/FT_NamingReplicationS.h"
#include "ace/Recursive_Thread_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/FT_NamingReplicationC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_FT_Naming_Server;

/**
 * @class TAO_FT_Naming_Replication_Manager
 * @brief The class that implements the FT_Naming::ReplicationManager
 * interface.
 */
class TAO_FtNaming_Export TAO_FT_Naming_Replication_Manager
  : public virtual POA_FT_Naming::ReplicationManager
{
public:

  /// Create a Replication Manager and provide it with the naming server
  /// to be updated whenever notified by the peer replica
  TAO_FT_Naming_Replication_Manager(TAO_FT_Naming_Server *naming_svr,
                                    const char* repl_mgr_name);

  virtual ~TAO_FT_Naming_Replication_Manager(void);

  /// Initialize the naming manager. This will provide the poa to
  /// the naming manager and underlying components for use in
  /// managing the object groups.
  void initialize (CORBA::ORB_ptr orb,
                   PortableServer::POA_ptr root_poa);


  /// Implementation of the FT_Naming::ReplicationManager interface
  virtual ::FT_Naming::ReplicaInfo * register_replica (
    ::FT_Naming::ReplicationManager_ptr replica,
    const ::FT_Naming::ReplicaInfo & replica_info);

  /// This method implements the operation invoked by the peer replica when an
  /// object group is updated on the remote process.
  virtual void notify_updated_object_group (
    const FT_Naming::ObjectGroupUpdate & group_info);

  /// This method implements the operation invoked by the peer replica when an
  /// naming context is updated on the remote process.
  virtual void notify_updated_context (
    const FT_Naming::NamingContextUpdate & group_info);

  /// Retrieve the object reference for the peer naming service
  /// ReplicationManager.
  static FT_Naming::ReplicationManager_ptr peer_replica (void);

 /*
  * Utilities for implementing the FT_Naming::ReplicationManager
  */

  /// Stores the peer in the peer_replica_ data member and invokes the
  /// register_replica interface method with the peer. Returns 0 if
  /// successful and -1 if unable to contact the peer.
  int register_with_peer_replica (FT_Naming::ReplicationManager_ptr replica,
                                  CosNaming::NamingContext_ptr nc,
                                  FT_Naming::NamingManager_ptr rm);

  /// The object reference for this servant instance
  FT_Naming::ReplicationManager_ptr reference (void);

protected:

  // The object which implements the naming service and the object manager
  TAO_FT_Naming_Server *naming_svr_;

  // Store the reference to the replica object reference
  // For now only a single replica is supported.
  static FT_Naming::ReplicationManager_var peer_replica_;

  PortableServer::POA_var repl_mgr_poa_;

  ACE_CString repl_mgr_name_;

  FT_Naming::ReplicationManager_var reference_;

  /// Lock used to serialize access to fault tolerant extensions
  /// to Naming Service.
  TAO_SYNCH_MUTEX lock_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_FT_NAMING_REPLICATION_MANAGER_H */
