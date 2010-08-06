// -*- C++ -*-

//=============================================================================
/**
 *  @file    Task_Scheduler.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef TASK_SCHEDULER_H_
#define TASK_SCHEDULER_H_

#include "Schedule.h"

struct Task_Scheduler : public std::unary_function <Task, 
                                                    ScheduleResult>
{
public:
  /// default ctor
  Task_Scheduler (SCHEDULE & schedule,
                  CTT_Algorithm & ctt);

  // returns where the task has been placed and which WCRT it has.
  // If the WCRT response time is 0, it is not schedulable.
  ScheduleResult operator () (const Task & task);

private:
  SCHEDULE & schedule_;
  CTT_Algorithm & ctt_;
};

#endif /* TASK_SCHEDULER_H_ */
