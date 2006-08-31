// -*- C++ -*-

//=============================================================================
/**
 * @file    Operation_Measurement.h
 *
 * $Id$
 *
 * [insert description here]
 *
 * @author  James H. Hill <hillj@isis.vanderbilt.edu>
 */
//=============================================================================

#ifndef _CUTS_OPERATION_MEASUREMENT_H_
#define _CUTS_OPERATION_MEASUREMENT_H_

#include "cuts/Time_Measurement.h"

//=============================================================================
/**
 * @class CUTS_Operation_Measurement
 *
 * @brief Time measurements for a port operation.
 */
//=============================================================================

class CUTS_Export CUTS_Operation_Measurement
{
public:
  /// Constructor.
  CUTS_Operation_Measurement (void);

  /// Copy constructor.
  CUTS_Operation_Measurement (const CUTS_Operation_Measurement &om);

  /// Destructor.
  ~CUTS_Operation_Measurement (void);

  /// Assignment operator
  const CUTS_Operation_Measurement &operator = (const CUTS_Operation_Measurement &om);

  /// Reset all the measurement data.
  void reset (void);

  /// Collect the timing information for the number of repetitions.
  void operator () (size_t reps, const ACE_Time_Value & tv);

  /// Get the number of repetitions collected.
  size_t repetitions (void) const;

  /// Get the current timing measurements.
  const CUTS_Time_Measurement & time (void) const;

private:
  /// Number of repetitions measured.
  size_t reps_;

  /// Timing data for the operation.
  CUTS_Time_Measurement time_;
};

#if defined (__CUTS_INLINE__)
#include "cuts/Operation_Measurement.inl"
#endif

#endif  // !defined _CUTS_OPERATION_MEASUREMENT_H_
