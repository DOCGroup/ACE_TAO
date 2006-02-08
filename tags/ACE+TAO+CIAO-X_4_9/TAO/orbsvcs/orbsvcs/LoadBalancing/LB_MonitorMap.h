// -*- C++ -*-

//=============================================================================
/**
 * @file   LB_MonitorMap.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_MONITOR_MAP_H
#define TAO_LB_MONITOR_MAP_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosLoadBalancingC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroup/PG_Location_Hash.h"
#include "orbsvcs/PortableGroup/PG_Location_Equal_To.h"

#include "ace/Hash_Map_Manager_T.h"


/// Load monitor hash map.
typedef ACE_Hash_Map_Manager_Ex<
  PortableGroup::Location,
  CosLoadBalancing::LoadMonitor_var,
  TAO_PG_Location_Hash,
  TAO_PG_Location_Equal_To,
  ACE_Null_Mutex> TAO_LB_MonitorMap;


#include /**/ "ace/post.h"

#endif  /* TAO_LB_MONITOR_MAP_H */
