// -*- C++ -*-

//=======================================================================
/**
 *  @file     PG_Factory_Map.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=======================================================================


#ifndef TAO_PG_FACTORY_MAP_H
#define TAO_PG_FACTORY_MAP_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PG_Factory_Set.h"

#include "orbsvcs/PortableGroupC.h"

#include "ace/Hash_Map_Manager_T.h"


/// Factory hash map.
typedef ACE_Hash_Map_Manager_Ex<
  PortableGroup::ObjectGroupId,
  TAO_PG_Factory_Set,
  ACE_Hash<PortableGroup::ObjectGroupId>,
  ACE_Equal_To<PortableGroup::ObjectGroupId>,
  ACE_Null_Mutex> TAO_PG_Factory_Map;

#include /**/ "ace/post.h"

#endif  /* TAO_PG_FACTORY_MAP_H */
