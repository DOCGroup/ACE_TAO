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
#include <ace/Hash_Map_Manager_T.h>
#include <ace/Null_Mutex.h>

typedef ACE_Hash_Map_Manager_Ex <ACE_CString, 
                                 PROCESSOR_SET,
                                 ACE_Hash <ACE_CString>,
                                 ACE_Equal_To <ACE_CString>,
                                 ACE_Null_Mutex> FAILURE_MAP;

/**
 * @class FailureMapFinder
 *
 * @brief Functor that uses the replica group to determine the
 *        processors that need to fail in order for a given backup
 *        task to become active
 */
class FailureMapFinder : public ReplicaFinder
{
public:
  FailureMapFinder (const REPLICA_GROUPS & rep_groups,
                    const FAILURE_MAP & failure_map);

  virtual PROCESSOR_SET operator () (const Task & task) const;

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

  double accumulate_wcrt (const TASK_LIST & tasks,
                          const PROCESSOR_SETS & scenarios);

private:
  FAILURE_MAP failure_map_;
  FailureMapFinder replica_finder_;
};

std::ostream & operator<< (std::ostream & ostr, 
                           const FAILURE_MAP & fm);

#endif /* FORWARD_RANKING_SCHEDULER_H_ */
