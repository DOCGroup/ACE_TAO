// -*- C++ -*-

//=============================================================================
/**
 * @file  PG_Property_Utils.h
 *
 * $Id$
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

#include "orbsvcs/PortableGroupC.h"


namespace TAO_PG
{
  /// Retrieve the value of the given property from the given property
  /// list.
  /**
   * @return true if successful, false otherwise
   */
  TAO_PortableGroup_Export CORBA::Boolean get_property_value (
    const PortableGroup::Name & property_name,
    const PortableGroup::Properties & properties,
    PortableGroup::Value & property_value);

  /// Override properties in the "properties" sequence with those in
  /// the "overrides" sequence.
  /**
   * If no property is overridden, the override in question will be
   * appended to the "properties" list.
   */
  TAO_PortableGroup_Export void override_properties (
    const PortableGroup::Properties & overrides,
    PortableGroup::Properties &properties);

}

#include /**/ "ace/post.h"
#endif  /* TAO_PG_PROPERTY_UTILS_H */
