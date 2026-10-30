// -*- C++ -*-

//=============================================================================
/**
 * @file  PG_Property_Utils.h
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_PG_PROPERTY_UTILS_H
#define TAO_PG_PROPERTY_UTILS_H

#include /**/ "ace/pre.h"

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/PortableGroup/portablegroup_export.h"
#include "orbsvcs/PortableGroupC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO_PG
{
  /// Retrieve the value of the given property from the given property
  /// list.
  /**
   * @return true if successful, false otherwise
   */
  TAO_PortableGroup_Export CORBA::Boolean get_property_value (
    PortableGroup::Name const & property_name,
    PortableGroup::Properties const & properties,
    PortableGroup::Value & property_value);

  /// Override properties in the "properties" sequence with those in
  /// the "overrides" sequence.
  /**
   * If no property is overridden, the override in question will be
   * appended to the "properties" list.
   */
  TAO_PortableGroup_Export void override_properties (
    PortableGroup::Properties const & overrides,
    PortableGroup::Properties &properties);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif  /* TAO_PG_PROPERTY_UTILS_H */
