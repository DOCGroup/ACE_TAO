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
#include "tao/PortableServer/Key_Adapters.h"

#include "LB_ReplicaInfo.h"


/**
 * @class Map_Entry
 *
 * @brief Value field of the ObjectGroup map.
 */
struct TAO_LB_ObjectGroup_Map_Entry
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

/// ObjectId hash map.
typedef ACE_Hash_Map_Manager_Ex<
  PortableServer::ObjectId,
  TAO_LB_ObjectGroup_Map_Entry *,
  TAO_ObjectId_Hash,
  ACE_Equal_To<PortableServer::ObjectId>,
  TAO_SYNCH_MUTEX> TAO_LB_ObjectGroup_Map;

#include "ace/post.h"

#endif  /* TAO_LB_OBJECTGROUP_MAP_H */
