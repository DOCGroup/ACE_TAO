// -*- C++ -*-

//=============================================================================
/**
 *  @file    TAO_UTO.h
 *
 *  $Id$
 *
 *  This class implements the CosTime::UTO IDL interface.
 *
 *
 *  @author Vishal Kachroo  <vishal@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_UTO_H
#define TAO_UTO_H
#include "ace/pre.h"

#include "orbsvcs/TimeServiceS.h"
#include "time_export.h"

/**
 * @class TAO_UTO
 *
 * @brief Universal Time Object Implementation.
 *
 * This is an encapsulation of the time. It provides the following
 * operations on basic time.
 * - Construction of a UTO from piece parts, and extraction of the
 * piece parts from a UTO. The piece parts are the readonly
 * attributes :
 * time
 * inaccuracy
 * time displacement factor
 * structure with all the above.
 * - Comparison of time.
 * - Conversion from relative to absolute time, and conversion to
 * an interval (TIO).
 */
class TAO_Time_Export TAO_UTO : public POA_CosTime::UTO
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_UTO (TimeBase::TimeT time,
           TimeBase::InaccuracyT inaccuracy,
           TimeBase::TdfT tdf);

  /// Destructor.
  ~TAO_UTO (void);

  /// For the readonly attribute <time>.
  virtual TimeBase::TimeT time (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// For the readonly attribute <inaccuracy>.
  virtual TimeBase::InaccuracyT  inaccuracy (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// For the readonly attribute <tdf>, which is the "time displacement
  /// factor".
  virtual TimeBase::TdfT tdf (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// For the readonly attribute <utc_time>.
  virtual TimeBase::UtcT utc_time (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Absolute time = Relative time + Base time.
   * ?? Find out more about the Base Time, UTC and
   *    Distributed Time Sync. Algos. [3
   */
  CosTime::UTO_ptr absolute_time (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Compares the time contained in the object with the time in the
  /// supplied uto according to the supplied comparison type.
  CosTime::TimeComparison compare_time (CosTime::ComparisonType comparison_type,
                                        CosTime::UTO_ptr uto
                                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Returns a TIO representing the time interval between the time in
   * the object and the time in the UTO passed as a parameter. The
   * interval returned is the interval between the mid-points of the
   * two UTOs. Inaccuracies are ignored.  Note the result of this
   * operation is meaningless if the base times of UTOs are different.
   */
  CosTime::TIO_ptr time_to_interval (CosTime::UTO_ptr
                                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Returns a TIO object representing the error interval around the
  /// time value in the UTO.
  CosTime::TIO_ptr interval (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /**
   * The readonly attribute structure having the time, inaccuracy and
   * displacement.  The get methods for other readonly attributes
   * (time, inaccuracy, tdf) defined in the IDL use the members of
   * this structure and hence need not have separate member variables
   * for them.
   */
  TimeBase::UtcT attr_utc_time_;
};

#include "ace/post.h"
#endif /* TAO_UTO_H */
