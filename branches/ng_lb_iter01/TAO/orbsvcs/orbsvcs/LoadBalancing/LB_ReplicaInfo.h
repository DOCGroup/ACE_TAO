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

class TAO_LB_ReplicaInfo;
typedef ACE_Unbounded_Set<TAO_LB_ReplicaInfo *> TAO_LB_ReplicaInfo_Set;
typedef ACE_Unbounded_Set_Iterator<TAO_LB_ReplicaInfo *> TAO_LB_ReplicaInfo_Set_Iterator;


/**
 * @class TAO_LB_ReplicaInfo
 *
 * @brief Class that contains all replica-specific information.
 */
class TAO_LB_ReplicaInfo
{
public:

  /// Reference to the replica.
  CORBA::Object_var replica;

  /// FactoryInfo used when creating the replica.
  LoadBalancing::FactoryInfo factory_info;

  /// FactoryCreationId assigned to this replica.  This
  /// FactoryCreationId_var will contain a zero pointer if the replica
  /// was not created using a GenericFactory.
  LoadBalancing::GenericFactory::FactoryCreationId_var factory_creation_id;

  /// Reference to the load monitor residing at the same location the
  /// replica resides.
  LoadBalancing::LoadMonitor_var load_monitor;

  /// Average load(s) on the replica
  LoadBalancing::LoadList_var load_list;

};

#include "ace/post.h"

#endif  /* TAO_LB_REPLICA_INFO_H */
