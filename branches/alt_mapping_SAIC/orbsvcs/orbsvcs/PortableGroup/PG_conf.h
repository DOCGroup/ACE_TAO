// -*- C++ -*-

//=============================================================================
/**
 * @file PG_conf.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_PG_CONF_H
#define TAO_PG_CONF_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroupC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

#ifndef TAO_PG_MAX_OBJECT_GROUPS  // @@ why create arbitrary limits?
/// The maximum number of object groups to be managed by the
/// ObjectGroupManager.  This number is also equal to the number of
/// factory sets managed by the GenericFactory.
const size_t TAO_PG_MAX_OBJECT_GROUPS = 1024;
#endif  /* TAO_PG_MAX_NUMBER_OF_OBJECT_GROUPS */

#ifndef TAO_PG_MAX_LOCATIONS      // @@ why create arbitrary limits?
/// The maximum number of locations to be managed by the
/// ObjectGroupManager.
const size_t TAO_PG_MAX_LOCATIONS = 1024;
#endif  /* TAO_PG_MAX_LOCATIONS */

#ifndef TAO_PG_MEMBERSHIP_STYLE
/// The default object group membership style.
const PortableGroup::MembershipStyleValue TAO_PG_MEMBERSHIP_STYLE =
  PortableGroup::MEMB_INF_CTRL;
#endif  /* TAO_PG_INITIAL_NUMBER_MEMBERS */

#ifndef TAO_PG_INITIAL_NUMBER_MEMBERS
/// The default initial number of object group members.
const PortableGroup::InitialNumberMembersValue TAO_PG_INITIAL_NUMBER_MEMBERS =
  2;
#endif  /* TAO_PG_INITIAL_NUMBER_MEMBERS */

#ifndef TAO_PG_MINIMUM_NUMBER_MEMBERS
/// The default minimum number of object group members.
const PortableGroup::MinimumNumberMembersValue TAO_PG_MINIMUM_NUMBER_MEMBERS =
  TAO_PG_INITIAL_NUMBER_MEMBERS;
#endif  /* TAO_PG_MINIMUM_NUMBER_MEMBERS */

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_PG_CONF_H */
