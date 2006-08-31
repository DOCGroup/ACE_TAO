#ifndef _CUTS_WORKER_MEASUREMENT_H_
#define _CUTS_WORKER_MEASUREMENT_H_

#include "cuts/Operation_Measurement.h"
#include <map>

//=============================================================================
/**
 * @class CUTS_Worker_Measurement
 */
//=============================================================================

class CUTS_Export CUTS_Worker_Measurement
{
public:
  /// Type definition for mapping worker operations to timing measurements.
  typedef std::map <long, CUTS_Operation_Measurement> Operation_Map;

  /// Constructor.
  CUTS_Worker_Measurement (void);

  /// Copy constructor.
  CUTS_Worker_Measurement (const CUTS_Worker_Measurement & src);

  /// Destructor.
  virtual ~CUTS_Worker_Measurement (void);

  /// Record an operation time measurement.
  void record_operation (size_t reps, long opid, const ACE_Time_Value &tm);

  /// Get the collection of operations.
  const Operation_Map & operations (void) const;

  /// Get the collection of operations.
  Operation_Map & operations (void);

  /// Assignment operator.
  const CUTS_Worker_Measurement & operator = (
    const CUTS_Worker_Measurement & src);

private:
  /// Mapping of worker operation to timing measurements.
  Operation_Map operations_;
};

#if defined (__CUTS_INLINE__)
#include "cuts/Worker_Measurement.inl"
#endif

#endif  // !defined _CUTS_WORKER_MEASUREMENT_H_
