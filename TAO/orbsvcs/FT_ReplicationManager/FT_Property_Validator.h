// -*- C++ -*-

//=============================================================================
/**
 * @file FT_Property_Validator.h
 *
 * @author Curt Hibbs <hibbs_c@ociweb.com>
 */
//=============================================================================

#ifndef FT_PROPERTY_VALIDATOR_H
#define FT_PROPERTY_VALIDATOR_H

#include /**/ "ace/pre.h"

#include "orbsvcs/PortableGroup/PG_Default_Property_Validator.h"
#include "orbsvcs/PortableGroupC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
  * @class FT_Property_Validator
  *
  * @brief Default property validator implementation.
  *
  * This Property_Validator verifies that all properties defined in the
  * FT CORBA IDL are valid.
  */
  class FT_Property_Validator : public TAO_PG_Default_Property_Validator
  {
  public:
    /// Constructor.
    FT_Property_Validator ();

    /// Destructor.
    ~FT_Property_Validator () override;

    /// Validate the given properties.  Throw an exception when the
    /// first invalid property is encountered.  The remaining properties
    /// will not be validated.
    void validate_property (PortableGroup::Properties const & props) override;

    /// Validate the given properties/criteria.  All criteria
    /// will be validated regardless of whether or not an invalid
    /// property was encountered.
    void validate_criteria (PortableGroup::Properties const & criteria) override;

  private:
    FT_Property_Validator (FT_Property_Validator const &) = delete;
    FT_Property_Validator (FT_Property_Validator &&) = delete;
    FT_Property_Validator & operator= (FT_Property_Validator const &) = delete;
    FT_Property_Validator & operator= (FT_Property_Validator &&) = delete;

    /**
    * @name Pre-initialize property Names.
    *
    * These properties are pre-initialized once to reduce property
    * validation overhead.  Note that the following properties are
    * not validated since there are no restrictions for these values:
    *   InitialNumberMembers
    *   MinimumNumberMembers
    *   FaultMonitoringInterval
    *   CheckpointInterval
    */
    //@{
    PortableGroup::Name replication_style_;
    PortableGroup::Name membership_style_;
    PortableGroup::Name consistency_style_;
    PortableGroup::Name fault_monitoring_style_;
    PortableGroup::Name fault_monitoring_granularity_;
    PortableGroup::Name factories_;
    //@}

  };
} // namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* FT_PROPERTY_VALIDATOR_H */
