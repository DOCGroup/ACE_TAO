/**
 * @file RT_Class.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#ifndef TAO_PERF_RTEC_RT_CLASS_H
#define TAO_PERF_RTEC_RT_CLASS_H

#include "rtec_perf_export.h"
#include "ace/Sched_Params.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class RT_Class
 *
 * @brief Simplify the initialization of performance tests.
 *
 */
class TAO_RTEC_Perf_Export RT_Class
{
public:
  /// Constructor
  RT_Class (void);

  /** @name Return the test scheduling parameters
   */
  //@{
  /// Return the process scheduling class selected for the test
  int prc_sched_class (void) const;

  /// Return the thread scheduling class selected for the test
  int thr_sched_class (void) const;

  /// Return the process priority selected for the test
  int priority_process (void) const;

  /// Return the (thread, native) priority for the high priority
  /// task(s) in the test
  int priority_high (void) const;

  /// Return the (thread, native) priority for the low priority
  /// task(s) in the test
  int priority_low (void) const;

  //@}

private:
  /// The process scheduling class
  int prc_sched_class_;

  /// The thread scheduling class
  int thr_sched_class_;

  /// The process priority
  int priority_process_;

  /// The high priority task(s) priority
  int priority_high_;

  /// The low priority task(s) priority
  int priority_low_;
};

#if defined(__ACE_INLINE__)
#include "RT_Class.inl"
#endif /* __ACE_INLINE__ */

#endif /* TAO_PERF_RTEC_RT_CLASS_H */
