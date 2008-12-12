// -*- C++ -*-

//=============================================================================
/**
 *  @file    Multi_Failure_Scheduler.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef MULTI_FAILURE_SCHEDULER_H_
#define MULTI_FAILURE_SCHEDULER_H_

#include "Schedule.h"

typedef std::list <PROCESSOR_SET> FAILURE_SCENARIOS;
typedef std::list <TASKNAME_SET> FAILOVER_SCENARIOS;
typedef std::map <Processor, Taskname> BACKUP_PRIMARY_MAP;

/**
 *  @struct PrimaryFinder
 *
 *  @brief Functor to find the the processor the primary of a given 
 *         backup replica runs on.
 */
struct PrimaryFinder : public std::unary_function <Task,
                                                   std::pair <Processor,
                                                              Taskname> >
{
public:
  // ctor
  PrimaryFinder (SCHEDULE & schedule);

  // finds a task's priamry in a schedule and returns a pair of the
  // taskname and the processor name
  std::pair <Processor, Taskname> operator () (const Task & task);

private:
  SCHEDULE & schedule_;
};

struct Multi_Failure_Scheduler : public std::unary_function <Task, 
                                                       ScheduleResult>
{
public:
  /// default ctor
  Multi_Failure_Scheduler (SCHEDULE & current_schedule,
                           SCHEDULE & global_schedule,
                           unsigned int failure_number);

  // returns where the task has been placed and which WCRT it has.
  // If the WCRT response time is 0, it is not schedulable.
  ScheduleResult operator () (const Task & task);

  /// returns all backups running on a processor
  static TASK_LIST get_backups (const Processor & p,
                                SCHEDULE & schedule);

  /// returns all primaries running on a processor
  static TASK_LIST get_primaries (const Processor & p,
                                  SCHEDULE & schedule);

  /// returns all the processor where a list of tasks has primaries
  /// running on
  static BACKUP_PRIMARY_MAP 
  get_primary_processors (const TASK_LIST & tasks,
                          SCHEDULE & schedule);

private:
  /// creates possible sets of processor failures that are relevant
  /// for scheduling calculation
  FAILURE_SCENARIOS calculate_relevant_failure_scenarios (
                      const Processor & processor,
                      const Task & task,
                      unsigned int failure_number);

  /// this method returns all backup tasks that will run actively
  /// given a certain set of failed processors
  FAILOVER_SCENARIOS get_failover_scenarios (
                 const FAILURE_SCENARIOS & failed_processors,
                 const Processor & processor);

  SCHEDULE & current_schedule_;
  SCHEDULE & global_schedule_;

  // number of processor failures that the scheduler should provide
  // for
  unsigned int failure_number_;

  PrimaryFinder primary_finder_;
};

#endif /* MULTI_FAILURE_SCHEDULER_H_ */
