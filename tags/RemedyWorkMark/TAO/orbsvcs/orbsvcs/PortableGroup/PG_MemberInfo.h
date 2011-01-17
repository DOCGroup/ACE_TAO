// -*- C++ -*-

//=======================================================================
/**
 *  @file    PG_MemberInfo.h
 *
 *  $Id$
 *
 *  @author  Ossama Othman <ossama@uci.edu>
 */
//=======================================================================


#ifndef TAO_PG_MEMBER_INFO_H
#define TAO_PG_MEMBER_INFO_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroupC.h"
#include "ace/Unbounded_Set.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_PG_MemberInfo
 *
 * @brief Structure that contains all member-specific information.
 *
 * Structure that contains all member-specific information.
 */

struct TAO_PG_MemberInfo
{
  PortableGroup::ObjectGroup_var group;

  /// Reference to the member.
  CORBA::Object_var member;

  /// The location at which the member resides.
  PortableGroup::Location location;

  /// Flag indicates that the member is alive.
  bool is_alive;

  /// Equality operator.
  /**
   * For the purposes of the member info set, only the location is
   * important.  In particular, if a member already resides at the
   * given location, this equality operator will return true.
   */
  bool operator== (const TAO_PG_MemberInfo & rhs) const;

};

typedef ACE_Unbounded_Set<TAO_PG_MemberInfo> TAO_PG_MemberInfo_Set;

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_PG_MEMBER_INFO_H */
