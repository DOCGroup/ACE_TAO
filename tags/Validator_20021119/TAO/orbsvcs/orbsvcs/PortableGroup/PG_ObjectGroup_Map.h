// -*- C++ -*-

//=======================================================================
/**
 *  @file    PG_ObjectGroup_Map.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=======================================================================


#ifndef TAO_PG_OBJECTGROUP_MAP_H
#define TAO_PG_OBJECTGROUP_MAP_H

#include "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "PG_MemberInfo.h"

#include "orbsvcs/PortableGroupC.h"

#include "tao/PortableServer/Key_Adapters.h"
#include "tao/PortableServer/PortableServerC.h"

#include "ace/Hash_Map_Manager_T.h"


/**
 * @class TAO_PG_ObjectGroup_Map_Entry
 *
 * @brief Value field of the ObjectGroup map.
 */
struct TAO_PG_ObjectGroup_Map_Entry
{
public:

  /// The RepositoryId corresponding to all Members in the
  /// ObjectGroup.
  CORBA::String_var type_id;

  /// This is the PortableGroup::ObjectGroupId.
  /**
   * The spec states that PortableGroup::ObjectGroupId is a
   * CORBA::ULongLong.  However, the over 4 billion group IDs that can
   * be represented by a CORBA::ULong should be plenty for any
   * application.
   */
  CORBA::ULong group_id;

  /// Reference to the ObjectGroup.
  PortableGroup::ObjectGroup_var object_group;

  /// Unbounded set containing member references and all related
  /// information for each member.
  TAO_PG_MemberInfo_Set member_infos;

  /// Properties used when creating this object group, in addition to
  /// those set dynamically after the creation.
  PortableGroup::Properties properties;

};

/// ObjectId hash map typedef.
typedef ACE_Hash_Map_Manager_Ex<
  PortableServer::ObjectId,
  TAO_PG_ObjectGroup_Map_Entry *,
  TAO_ObjectId_Hash,
  ACE_Equal_To<PortableServer::ObjectId>,
  ACE_Null_Mutex> TAO_PG_ObjectGroup_Map;


#include "ace/post.h"

#endif  /* TAO_PG_OBJECTGROUP_MAP_H */
