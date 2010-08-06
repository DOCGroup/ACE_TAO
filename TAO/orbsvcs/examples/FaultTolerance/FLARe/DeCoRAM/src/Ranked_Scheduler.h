// -*- C++ -*-

//=============================================================================
/**
 *  @file    Ranked_Scheduler.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef RANKED_SCHEDULER_H_
#define RANKED_SCHEDULER_H_

#include "Schedule.h"
#include "RankedWCRT.h"

struct Ranked_Scheduler : public std::unary_function <Task, 
                                                      ScheduleResult>
{
public:
  /// default ctor
  Ranked_Scheduler (SCHEDULE & current_schedule,
                    RankedWCRT & wcrt_algorithm);

  // returns where the task has been placed and which WCRT it has.
  // If the WCRT response time is 0, it is not schedulable.
  ScheduleResult operator () (const Task & task);

private:
  SCHEDULE & current_schedule_;
  RankedWCRT & wcrt_algorithm_;
};

#endif /* RANKED_SCHEDULER_H_ */
