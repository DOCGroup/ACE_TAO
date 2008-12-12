// -*- C++ -*-

//=============================================================================
/**
 *  @file    RankedWCRT.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef RANKED_WCRT_H_
#define RANKED_WCRT_H_

#include "Schedule.h"

/// this map associates a task with all the possible combinations of
/// processor failures for which it becomes active
typedef std::map <Taskname, PROCESSOR_SET> FAILURE_MAP;

struct RankedWCRT : public std::unary_function <TASK_LIST,
                                                double>
{
public:
  // within the constructor the failure map will be created from the
  // schedule
  RankedWCRT (const SCHEDULE & schedule,
              unsigned int consistency_level);

  // returns the maximum of the newly computed and current worst-case
  // response time
  double operator () (const TASK_LIST & scheduled_tasks);

private:
  FAILURE_MAP failure_map_;
  std::auto_ptr <CTT_Algorithm> ctt_;
  unsigned int consistency_level_;
};

#endif /* RANKED_WCRT_H_ */
