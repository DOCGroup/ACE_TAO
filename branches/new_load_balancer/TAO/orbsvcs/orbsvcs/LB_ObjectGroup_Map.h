// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO_LoadBalancing
//
// = FILENAME
//     LB_ObjectGroup_Map.h
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef TAO_LB_OBJECTGROUP_MAP_H
#define TAO_LB_OBJECTGROUP_MAP_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager.h"
#include "LB_Replica_Map.h"
#include "LB_ObjectGroup_Hash.h"
#include "LB_ObjectGroup_Equal_To.h"
#include "orbsvcs/LoadBalancingC.h"

class TAO_LB_ObjectGroup_Map
{
  // = TITLE
  //     Map of RepositoryId to ObjectGroup reference,
  //     FactoryCreationId, replicas belonging to the ObjectGroup and
  //     corresponding replica-specific information.
  //
  // = DESCRIPTION
  //     Implementation to be used by the Load Balancer
  //     ReplicationManager.

public:

  struct Map_Entry
  {
    // = TITLE
    //     Value field of the replica map.
    //
    // = DESCRIPTION
    //     Mapping from and to all of the following fields:
    //     <object_group>, <factory_creation_id>, <factory>, and
    //     <factory_creation_id>.  Therefore, we keep all the fields
    //     together in the map.

    CORBA::String_var type_id;
    // The RepositoryId corresponding to all Replicas in the
    // ObjectGroup.

    TAO_LoadBalancing::ObjectGroup_var object_group;
    // Reference to the ObjectGroup.

    TAO_LoadBalancing::FactoryCreationId factory_creation_id;
    // The FactoryCreationId corresponding to the ObjectGroup created by
    // the Load Balancer GenericFactory.

    TAO_LB_Replica_Map replica_map;
    // Hash map containing replica references and all related
    // information for each replica.
  };

  typedef ACE_Hash_Map_Manager_Ex<
    const char *,
    Map_Entry *,
    ACE_Hash<const char *>,
    ACE_Equal_To<const char *>,
    ACE_SYNCH_MUTEX> type_id_map;
  // type_id hash map.

  typedef ACE_Hash_Map_Manager_Ex<
    TAO_LoadBalancing::ObjectGroup_ptr,
    Map_Entry *,
    TAO_LB_ObjectGroup_Hash,
    TAO_LB_ObjectGroup_Equal_To,
    ACE_SYNCH_MUTEX> ObjectGroup_map;
  // ObjectGroup hash map.

  typedef ACE_Hash_Map_Manager_Ex<
    TAO_LoadBalancing::FactoryCreationId,
    Map_Entry *,
    TAO_LB_FactoryCreationId_Hash,
    TAO_LB_FactoryCreationId_Equal_To,
    ACE_SYNCH_MUTEX> FactoryCreationId_map;
  // FactoryCreationId hash map.

  int bind (const char *type_id,
            TAO_LoadBalancing::ObjectGroup_ptr object_group,
            TAO_LoadBalancing::FactoryCreationId &factory_creation_id);
};

#include "ace/post.h"

#endif  /* TAO_LB_OBJECTGROUP_MAP_H */
