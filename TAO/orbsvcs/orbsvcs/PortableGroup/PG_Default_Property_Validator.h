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

#include "ace/pre.h"

#include "orbsvcs/PortableGroupC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


/**
 * @class TAO_PG_Default_Property_Validator
 *
 * @brief Default property validator implementation.
 *
 * This Property_Validator verifies that all properties defined in the
 * PortableGroup IDL module are valid.
 */
class TAO_PortableGroup_Export TAO_PG_Default_Property_Validator
{
public:

  /// Constructor.
  TAO_PG_Default_Property_Validator (void);

  /// Validate the given properties.  Throw an exception when the
  /// first invalid property is encountered.  The remaining properties
  /// will not be validated.
  void validate_property (const PortableGroup::Properties & props
                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::InvalidProperty,
                     PortableGroup::UnsupportedProperty));

  /// Validate the given properties/criteria.  All criteria
  /// will be validated regardless of whether or not an invalid
  /// property was encountered.
  void validate_criteria (const PortableGroup::Properties & criteria
                          ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableGroup::InvalidCriteria,
                     PortableGroup::CannotMeetCriteria));

private:

  /**
   * @name Pre-initialize property Names.
   *
   * These properties are pre-initialized once to reduce property
   * validation overhead.  Note that the InitialNumberMembers and
   * MinimumNumberMembers properties are not validated since there are
   * no restrictions imposed by TAO's PortableGroup implementation
   * regarding the number of such members.
   */
  //@{
  PortableGroup::Name membership_;
  PortableGroup::Name factories_;
  //@}

};

#include "ace/post.h"

#endif  /* TAO_PG_PROPERTY_VALIDATOR_H */
