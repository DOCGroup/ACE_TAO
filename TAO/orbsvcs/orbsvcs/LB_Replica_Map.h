// -*- C++ -*-
//
// $Id$

// ============================================================================
//
// = LIBRARY
//     TAO_LoadBalancing
//
// = FILENAME
//     LB_Replica_Map.h
//
// = AUTHOR
//     Ossama Othman <ossama@uci.edu>
//
// ============================================================================

#ifndef TAO_LB_REPLICA_MAP_H
#define TAO_LB_REPLICA_MAP_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager.h"
#include "orbsvcs/LoadBalancingC.h"
#include "LB_Replica_Hash.h"
#include "LB_Replica_Equal_To.h"

class TAO_LB_Replica_Map
{
  // = TITLE
  //     Map of replica references to location and factory.
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
    //     We need a mapping from and to all of the following fields:
    //     <replica>, <location>, <factory>, and
    //     <factory_creation_id>.  Therefore, we keep all the fields
    //     together in the map.

    CORBA::Object_var replica;
    // Reference to the replica.

    TAO_LoadBalancer::FactoryInfo_var factory_info;
    // Pointer to FactoryInfo structure associated with the replica.

    TAO_LoadBalancer::FactoryCreationId factory_creation_id;
    // The FactoryCreationId corresponding to the replica created by
    // the above factory.  This factory creation ID is only valid if
    // the corresponding factory reference above is not nil.

    TAO_LoadBalancer::Location &location = factory_info.the_location;
    // The location where the replica resides.
  };

  typedef ACE_Hash_Map_Manager_Ex<
    CORBA::Object_ptr,
    Map_Entry *,
    TAO_LB_Replica_Hash,
    TAO_LB_Replica_Equal_To,
    ACE_SYNCH_MUTEX> replica_map;
  // replica hash map.

  typedef ACE_Hash_Map_Manager_Ex<
    TAO_LoadBalancer::Location,
    Map_Entry *,
    TAO_LB_Location_Hash,
    TAO_LB_Location_Equal_To,
    ACE_SYNCH_MUTEX> location_map;
  // location hash map.
};


#include "ace/post.h"

#endif  /* TAO_LB_REPLICA_MAP_H */
