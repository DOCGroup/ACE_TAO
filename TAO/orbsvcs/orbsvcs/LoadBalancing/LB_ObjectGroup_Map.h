// -*- C++ -*-

//=======================================================================
/**
 *  @file     LB_ObjectGroup_Map.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=======================================================================


#ifndef TAO_LB_OBJECTGROUP_MAP_H
#define TAO_LB_OBJECTGROUP_MAP_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager_T.h"
#include "orbsvcs/LoadBalancingC.h"

#include "LB_ReplicaInfo_Set.h"


/**
 * @class TAO_LB_ObjectGroup_Map
 *
 * @brief Map of FactoryCreationId to ObjectGroup reference.
 *
 * Implementation to be used by the Load Balancer ReplicationManager.
 */
class TAO_LB_ObjectGroup_Map
{
public:

  /**
   * @class Map_Entry
   *
   * @brief Value field of the ObjectGroup map.
   *
   * Mapping from and to all of the following fields:
   * @param object_group, @param factory_creation_id, @param factory,
   * and @param factory_creation_id.  Therefore, we keep all the
   * fields together in the map.
   */
  struct Map_Entry
  {
    /// The RepositoryId corresponding to all Replicas in the
    /// ObjectGroup.
    CORBA::String_var type_id;

    /// Reference to the ObjectGroup.
    LoadBalancing::ObjectGroup_var object_group;

    /// Unbounded set containing replica references and all related
    /// information for each replica.
    TAO_LB_ReplicaInfo_Set replica_infos;

    /// Lock used to synchronize access to the ReplicaInfo set.
    TAO_SYNCH_MUTEX lock;
  };

  /// FactoryCreationId hash map.  A FactoryCreationId is represented
  /// internally as a CORBA::ULong.
  typedef ACE_Hash_Map_Manager_Ex<
    ACE_UINT32,
    Map_Entry *,
    ACE_Hash<ACE_UINT32>,
    ACE_Equal_To<ACE_UINT32>,
    TAO_SYNCH_MUTEX> Table;

  int bind (CORBA::ULong factory_creation_id,
            TAO_LB_ObjectGroup_Map::Map_Entry *object_group_entry);


};

#include "ace/post.h"

#endif  /* TAO_LB_OBJECTGROUP_MAP_H */
