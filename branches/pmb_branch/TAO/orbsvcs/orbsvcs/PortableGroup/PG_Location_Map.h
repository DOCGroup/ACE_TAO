// -*- C++ -*-

//=======================================================================
/**
 *  @file     PG_Location_Map.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=======================================================================


#ifndef TAO_PG_LOCATION_MAP_H
#define TAO_PG_LOCATION_MAP_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PG_Location_Hash.h"
#include "PG_Location_Equal_To.h"

#include "orbsvcs/PortableGroupC.h"

#include "ace/Array_Base.h"
#include "ace/Hash_Map_Manager_T.h"


/// Forward declarations.
struct TAO_PG_ObjectGroup_Map_Entry;

typedef ACE_Array_Base<TAO_PG_ObjectGroup_Map_Entry *>
  TAO_PG_ObjectGroup_Array;

/// Location hash map.
typedef ACE_Hash_Map_Manager_Ex<
  PortableGroup::Location,
  TAO_PG_ObjectGroup_Array *,
  TAO_PG_Location_Hash,
  TAO_PG_Location_Equal_To,
  ACE_Null_Mutex> TAO_PG_Location_Map;

#include "ace/post.h"

#endif  /* TAO_PG_LOCATION_MAP_H */
