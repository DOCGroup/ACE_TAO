// -*- C++ -*-

//=======================================================================
/**
 *  @file     LB_Location_Map.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=======================================================================


#ifndef TAO_LB_LOCATION_MAP_H
#define TAO_LB_LOCATION_MAP_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager_T.h"
#include "orbsvcs/LoadBalancingC.h"

#include "LB_ReplicaInfo.h"


/// Location hash map.
typedef ACE_Hash_Map_Manager_Ex<
  LoadBalancing::Location,
  TAO_LB_ReplicaInfo_Set,
  ACE_Hash<LoadBalancing::Location>,
  ACE_Equal_To<LoadBalancing::Location>,
  TAO_SYNCH_MUTEX> TAO_LB_ObjectGroup_Map;

#include "ace/post.h"

#endif  /* TAO_LB_LOCATION_MAP_H */
