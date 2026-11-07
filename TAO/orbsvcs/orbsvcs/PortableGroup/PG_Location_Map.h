// -*- C++ -*-

//=======================================================================
/**
 *  @file     PG_Location_Map.h
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=======================================================================


#ifndef TAO_PG_LOCATION_MAP_H
#define TAO_PG_LOCATION_MAP_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroup/PG_Location_Hash.h"
#include "orbsvcs/PortableGroup/PG_Location_Equal_To.h"

#include "orbsvcs/PortableGroupC.h"

#include "ace/Hash_Map_Manager_T.h"
#include "ace/Null_Mutex.h"

#include <vector>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Forward declarations.
struct TAO_PG_ObjectGroup_Map_Entry;

using TAO_PG_ObjectGroup_Array = std::vector<TAO_PG_ObjectGroup_Map_Entry *>;

/// Location hash map.
using TAO_PG_Location_Map = ACE_Hash_Map_Manager_Ex<
  PortableGroup::Location,
  TAO_PG_ObjectGroup_Array *,
  TAO_PG_Location_Hash,
  TAO_PG_Location_Equal_To,
  ACE_Null_Mutex>;

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_PG_LOCATION_MAP_H */
