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
#include "LB_Location_Hash.h"
#include "LB_Location_Equal_To.h"


/// Location hash map.
typedef ACE_Hash_Map_Manager_Ex<
  LoadBalancing::Location,
  TAO_LB_ReplicaInfo_Set,
  TAO_LB_Location_Hash,
  TAO_LB_Location_Equal_To,
  TAO_SYNCH_MUTEX> TAO_LB_Location_Map;

#include "ace/post.h"

#endif  /* TAO_LB_LOCATION_MAP_H */
