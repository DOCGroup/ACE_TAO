// -*- C++ -*-

//=============================================================================
/**
 * @file PG_Default_Property_Validator.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_PG_DEFAULT_PROPERTY_VALIDATOR_H
#define TAO_PG_DEFAULT_PROPERTY_VALIDATOR_H

#include /**/ "ace/pre.h"

#include "orbsvcs/PortableGroup/portablegroup_export.h"
#include "orbsvcs/PortableGroupC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_PG_Default_Property_Validator
 *
 * @brief Default property validator implementation.
 *
 * This Property_Validator verifies that all properties defined in the
 * PortableGroup IDL module are valid. This property validator can be
 * subclassed to validate a different property set.
 */
class TAO_PortableGroup_Export TAO_PG_Default_Property_Validator
{
public:

  /// Constructor.
  TAO_PG_Default_Property_Validator (void);

  /// Destructor
  virtual ~TAO_PG_Default_Property_Validator (void);

  /// Validate the given properties.  Throw an exception when the
  /// first invalid property is encountered.  The remaining properties
  /// will not be validated.
  virtual void validate_property (const PortableGroup::Properties & props);

  /// Validate the given properties/criteria.  All criteria
  /// will be validated regardless of whether or not an invalid
  /// property was encountered.
  virtual void validate_criteria (const PortableGroup::Properties & criteria);

private:

  /**
   * @name Pre-initialize property Names.
   *
   * These properties are pre-initialized once to reduce property
   * validation overhead.  Note that the InitialNumberReplicas and
   * MinimumNumberReplicas properties are not validated since there are
   * no restrictions imposed by TAO's PortableGroup implementation
   * regarding the number of such members.
   */
  //@{
  PortableGroup::Name membership_;
  PortableGroup::Name factories_;
  //@}

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_PG_PROPERTY_VALIDATOR_H */
