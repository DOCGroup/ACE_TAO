// -*- C++ -*-

//=======================================================================
/**
 *  @file    LB_ReplicaInfo.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=======================================================================


#ifndef TAO_LB_REPLICA_INFO_H
#define TAO_LB_REPLICA_INFO_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


#include "orbsvcs/LoadBalancingC.h"

// Forward declarations.
struct TAO_LB_Location_Map_Entry;
struct TAO_LB_ObjectGroup_Map_Entry;

struct TAO_LB_ReplicaInfo;
typedef ACE_Unbounded_Set<TAO_LB_ReplicaInfo *> TAO_LB_ReplicaInfo_Set;

/**
 * @class TAO_LB_ReplicaInfo
 *
 * @brief Class that contains all replica-specific information.
 *
 *
 */
struct TAO_LB_ReplicaInfo
{

  /// Reference to the replica.
  CORBA::Object_var replica;

  /// FactoryInfo used when creating the replica.
  PortableGroup::FactoryInfo factory_info;

  /// FactoryCreationId assigned to this replica.  This
  /// FactoryCreationId_var will contain a zero pointer if the replica
  /// was not created using a GenericFactory.
  PortableGroup::GenericFactory::FactoryCreationId_var factory_creation_id;

  /// Pointer to the object group map entry corresponding to the
  /// object group to which this replica member in this structure
  /// belongs.
  TAO_LB_ObjectGroup_Map_Entry *object_group_entry;

  /// Pointer to the location map entry containing the information
  /// related to the location the replica corresponding to this
  /// TAO_LB_ReplicaInfo structure.
  TAO_LB_Location_Map_Entry *location_entry;

};

#include "ace/post.h"

#endif  /* TAO_LB_REPLICA_INFO_H */
