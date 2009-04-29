// -*- C++ -*-

//=============================================================================
/**
 *  @file    Foward_Ranking_Scheduler.h
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#ifndef FORWARD_RANKING_SCHEDULER_H_
#define FORWARD_RANKING_SCHEDULER_H_

#include "Scheduler.h"
#include "CTT_Enhanced.h"

typedef std::list<PROCESSOR_SET> PROCESSOR_SETS;

typedef std::list<TASK_LIST> TASK_SCENARIOS;

class Forward_Ranking_Scheduler : public Scheduler
{
public:
  /// default ctor
  Forward_Ranking_Scheduler (const PROCESSOR_LIST & processors,
                             unsigned int max_failures);

  virtual double schedule_task (const Task & task,
                                const Processor & processor);
private:
  bool check_for_existing_replicas (const Task & task,
                                     const Processor & processor);

  PROCESSOR_SET replica_processors (const Task & task);

  PROCESSOR_SET relevant_processors (const TASK_LIST & tasks,
                                     const PROCESSOR_SET & ignored_processors);

  PROCESSOR_SETS permute_processors (const PROCESSOR_SET & fixed,
                                     const PROCESSOR_SET & exchangeable,
                                     unsigned int failure_number);

  TASK_SCENARIOS activate_tasks (const TASK_LIST & tasks,
                                 const PROCESSOR_SETS & failures);

  double accumulate_wcrt (const TASK_SCENARIOS & scenarios);

private:
  CTT_Enhanced ctt_;
};

// streaming operators for data structures defined in this header
std::ostream & operator<< (std::ostream & ostr, 
                           const PROCESSOR_SETS & ps);

std::ostream & operator<< (std::ostream & ostr, 
                           const TASK_SCENARIOS & ts);

#endif /* FORWARD_RANKING_SCHEDULER_H_ */
