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

typedef std::map <Taskname, PROCESSOR_SET> FAILURE_MAP;

/**
 * @class ReplicaFinder
 *
 * @brief Functor that uses the replica group to determine the
 *        processors that need to fail in order for a given backup
 *        task to become active
 */
class ReplicaFinder : public std::unary_function <Task,
                                                  PROCESSOR_SET>
{
public:
  ReplicaFinder (const REPLICA_GROUPS & rep_groups);

  PROCESSOR_SET operator () (const Task & task);

private:
  const REPLICA_GROUPS rep_groups_;
  ProcessorPicker processor_picker_;
};

/**
 * @class FailureMapFinder
 *
 * @brief Functor that uses the replica group to determine the
 *        processors that need to fail in order for a given backup
 *        task to become active
 */
class FailureMapFinder : public std::unary_function <Task,
                                                     PROCESSOR_SET>
{
public:
  FailureMapFinder (const FAILURE_MAP & failure_map);

  PROCESSOR_SET operator () (const Task & task);

private:
  const FAILURE_MAP & failure_map_;
};

class Forward_Ranking_Scheduler : public Scheduler
{
public:
  /// default ctor
  Forward_Ranking_Scheduler (const PROCESSOR_LIST & processors,
                             unsigned int max_failures);

  virtual double schedule_task (const Task & task,
                                const Processor & processor);

protected:
  virtual void update_schedule (const Task & task,
                                const Processor & processor);

private:
  void update_failure_map (const Task & task,
                           const Processor & processor);

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
  FAILURE_MAP failure_map_;
};

std::ostream & operator<< (std::ostream & ostr, 
                           const FAILURE_MAP & fm);

#endif /* FORWARD_RANKING_SCHEDULER_H_ */
