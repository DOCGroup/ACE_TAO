// -*- C++ -*-

//=======================================================================
/**
 *  @file    LB_LoadAlertMap.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=======================================================================


#ifndef TAO_LB_LOAD_ALERT_MAP_H
#define TAO_LB_LOAD_ALERT_MAP_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "LB_LoadAlertInfo.h"

#include "orbsvcs/PortableGroup/PG_Location_Hash.h"
#include "orbsvcs/PortableGroup/PG_Location_Equal_To.h"

#include "ace/Hash_Map_Manager_T.h"


/// LoadAlertInfo hash map typedef.
typedef ACE_Hash_Map_Manager_Ex<
  PortableGroup::Location,
  TAO_LB_LoadAlertInfo,
  TAO_PG_Location_Hash,
  TAO_PG_Location_Equal_To,
  ACE_Null_Mutex> TAO_LB_LoadAlertMap;


#include /**/ "ace/post.h"

#endif  /* TAO_LB_LOAD_ALERT_MAP_H */
