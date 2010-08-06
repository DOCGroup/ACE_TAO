// -*- C++ -*-

//=============================================================================
/**
 *  @file    OptimizedWCRT.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef OPTIMIZED_WCRT_H_
#define OPTIMIZED_WCRT_H_

#include "Schedule.h"
#include <memory>

struct OptimizedWCRT : public std::binary_function <double,
                                                    TASKNAME_SET,
                                                    double>
{
public:
  OptimizedWCRT (const TASK_LIST & tasks);

  // returns the maximum of the newly computed and current worst-case
  // response time
  double operator () (double current_wcrt, 
                      const TASKNAME_SET & active_backups);
private:
  // basic list of tasks to be checked for completion time
  TASK_LIST tasks_;

  std::auto_ptr <CTT_Algorithm> ctt_;
};

#endif /* OPTIMIZED_WCRT_H_ */
