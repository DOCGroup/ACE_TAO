// -*- C++ -*-

//=============================================================================
/**
 *  @file    FailureAwareWCRT.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef FAILURE_AWARE_WCRT_H_
#define FAILURE_AWARE_WCRT_H_

#include "CTT_Enhanced.h"
#include "Scheduler.h"

class FailureAwareWCRT : public std::binary_function <double,
  PROCESSOR_SET,
  double>
{
public:
  FailureAwareWCRT (const TASK_LIST & tasks,
                    const ReplicaFinder & replica_finder);

  double operator () (double previous,
                      const PROCESSOR_SET & failures);

private:
  CTT_Enhanced ctt_;
  TASK_LIST tasks_;
  const ReplicaFinder & replica_finder_;
};

#endif /* FAILURE_AWARE_WCRT_H_ */
