// -*- C++ -*-

//=============================================================================
/**
 * @file PG_Null_Property_Validator.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef TAO_PG_NULL_PROPERTY_VALIDATOR_H
#define TAO_PG_NULL_PROPERTY_VALIDATOR_H

#include /**/ "ace/pre.h"

#include "orbsvcs/PortableGroup/portablegroup_export.h"
#include "orbsvcs/PortableGroupC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_PG_Null_Property_Validator
 *
 * @brief No-op property validator.
 *
 * This property validator performs no validation whatsoever.
 */
class TAO_PortableGroup_Export TAO_PG_Null_Property_Validator
{
public:

  /// Validate the given properties.
  /**
   * This particular implementation performs no validation
   * whatsoever.
   */
  void validate_property (const PortableGroup::Properties & props);

  /// Validate the given criteria.
  /**
   * This particular implementation performs no validation
   * whatsoever.
   */
  void validate_criteria (const PortableGroup::Properties & criteria);

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_PG_PROPERTY_VALIDATOR_H */
