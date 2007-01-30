// -*- C++ -*-

//=======================================================================
/**
 *  @file    LB_Location_Index_Map.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=======================================================================


#ifndef TAO_LB_LOCATION_INDEX_MAP_H
#define TAO_LB_LOCATION_INDEX_MAP_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosLoadBalancingC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroupC.h"

#include "ace/Functor.h"
#include "ace/Hash_Map_Manager_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Location index map.
typedef ACE_Hash_Map_Manager_Ex<
  PortableGroup::ObjectGroupId,
  CORBA::ULong,
  ACE_Hash<ACE_UINT64>,
  ACE_Equal_To<ACE_UINT64>,
  ACE_Null_Mutex> TAO_LB_Location_Index_Map;

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_LB_LOCATION_INDEX_MAP_H */
