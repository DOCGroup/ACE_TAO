// -*- C++ -*-

//=============================================================================
/**
 *  @file    TAO_TIO.h
 *
 *  $Id$
 *
 *  This class implements the CosTime::TIO IDL interface.
 *
 *
 *  @author Vishal Kachroo  <vishal@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_TIO_H
#define TAO_TIO_H
#include "ace/pre.h"

#include "tao/PortableServer/ORB_Manager.h"
#include "orbsvcs/TimeServiceS.h"
#include "TAO_UTO.h"
#include "time_export.h"

/**
 * @class TAO_TIO
 *
 * @brief Time Interval Object Implementation.
 *
 * The TIO represents a time interval and has operations to
 * compare itself with a UTO or another TIO. It also has an
 * operation to create a UTO from the value of it's time
 * interval.
 */
class TAO_Time_Export TAO_TIO : public POA_CosTime::TIO
{
public:
  // = Initialization and termination methods.
  /// Constructor.
  TAO_TIO (TimeBase::TimeT lower,
           TimeBase::TimeT upper);

  /// Destructor.
  ~TAO_TIO (void);

  /// This is the get method for the attribute time interval.
  virtual TimeBase::IntervalT  time_interval (
      ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * This operation returns a value of type OverlapType depending on
   * how the interval in the object and the time range represented by
   * the parameter UTO overlap.  If OverlapType is not OTNoOverlap,
   * then the out parameter overlap contains the overlap interval,
   * otherwise the out parameter contains the gap between the two
   * intervals.
   */
  virtual CosTime::OverlapType spans (CosTime::UTO_ptr time,
                                      CosTime::TIO_out overlap
                                      ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * This operation returns a value of type OverlapType depending on
   * how the interval in the object and interval in the parameter TIO
   * overlap. If OverlapType is not OTNoOverlap, then the out
   * parameter overlap contains the overlap interval, otherwise the
   * out parameter contains the gap between the two intervals.
   */
  virtual CosTime::OverlapType overlaps (CosTime::TIO_ptr interval,
                                         CosTime::TIO_out overlap
                                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /**
   * Returns a UTO in which the inaccuracy interval is equal to the
   * time interval in the TIO and time value is the midpoint of the
   * interval.
   */
  virtual CosTime::UTO_ptr time (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

private:
  /// This attribute returns an IntervalT structure with the values of
  /// its fields filled in with the corresponding values from the TIO.
  TimeBase::IntervalT attr_time_interval;
};

#include "ace/post.h"
#endif /* TAO_TIO_H */
