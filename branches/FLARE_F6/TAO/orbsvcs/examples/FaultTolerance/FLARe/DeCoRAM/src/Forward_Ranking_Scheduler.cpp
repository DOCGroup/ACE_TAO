// -*- C++ -*-

//=============================================================================
/**
 *  @file    Foward_Ranking_Scheduler.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <numeric>
#include "Forward_Ranking_Scheduler.h"
#include "FailureAwareWCRT.h"
#include "Combination_T.h"

FailureMapFinder::FailureMapFinder (const REPLICA_GROUPS & rep_groups,
                                    const FAILURE_MAP & failure_map)
  : ReplicaFinder (rep_groups),
    failure_map_ (failure_map)
{
}

PROCESSOR_SET 
FailureMapFinder::operator () (const Task & task) const
{
  PROCESSOR_SET result;
  if (failure_map_.find (task.name.c_str (),
                         result) == 0)
    {
      TRACE (task);
      return result;
    }
  else
    {
      TRACE (task << "delegate");
      return this->ReplicaFinder::operator () (task);
    }
}

Forward_Ranking_Scheduler::Forward_Ranking_Scheduler (
  const PROCESSOR_LIST & processors,
  unsigned int max_failures)
  : Scheduler (processors, max_failures),
    replica_finder_ (replica_groups_,
                     failure_map_)
{
}

double
Forward_Ranking_Scheduler::schedule_task (const Task & task,
                                          const Processor & processor)
{
  TRACE ("(" << task << "," << processor << ")");

  // check if there is already a replica of this task on the processor
  if (this->check_for_existing_replicas (task, processor))
    {
      TRACE ("already contains replica of \"" << primary_name (task) << "\"");
      return .0;
    }

  // add task to local copy of the processor tasks
  TASK_LIST local_tasks = schedule_[processor];
  local_tasks.push_back (task);

  TRACE ("Tasks: " << local_tasks);

  // determine processors that need to fail necessarily to become active
  // this applies only for backups and depends on their rank
  PROCESSOR_SET fixed_failures = this->replica_processors (task);

  TRACE ("Fixed Failures: " << fixed_failures);

  // find other processors that affect the backups on this processor
  // if they fail
  PROCESSOR_SET additional_failures = 
    this->relevant_processors (local_tasks,
                               fixed_failures);

  TRACE ("Additional Failures: " << additional_failures);

  PROCESSOR_SETS failure_scenarios = 
    this->permute_processors (fixed_failures,
                              additional_failures,
                              max_failures_ - fixed_failures.size ());

  TRACE ("Relevant Failure Scenarios: " << failure_scenarios);

  double wcrt = this->accumulate_wcrt (local_tasks,
                                       failure_scenarios);

  TRACE ("Maximum wcrt: " << wcrt);

  return wcrt;
}

void
Forward_Ranking_Scheduler::update_schedule (const Task & task,
                                            const Processor & processor)
{
  this->Scheduler::update_schedule (task, processor);

  this->update_failure_map (task, processor);
}

void 
Forward_Ranking_Scheduler::update_failure_map (const Task & task,
                                               const Processor & /* processor */)
{
  PROCESSOR_SET proc_dependencies;

  REPLICA_GROUPS::iterator it =
    replica_groups_.find (primary_name (task));

  if (it != replica_groups_.end ())
    {
      for (TASK_POSITIONS::iterator tp_it = it->second.begin ();
           tp_it != it->second.begin () + task.rank;
           ++tp_it)
        {
          proc_dependencies.insert (tp_it->first);
        }
    }

  failure_map_.bind (task.name.c_str (), proc_dependencies);

  TRACE ("Failure Map: " << failure_map_);
}

bool 
Forward_Ranking_Scheduler::check_for_existing_replicas (
  const Task & task,
  const Processor & processor)
{
  TASK_POSITIONS replica_group = 
    replica_groups_[primary_name (task)];
      
  return std::accumulate (replica_group.begin (),
                          replica_group.end (),
                          false,
                          ProcessorNameComparison (processor));
}

PROCESSOR_SET
Forward_Ranking_Scheduler::replica_processors (const Task & task)
{
  return replica_finder_ (task);
}

class RelevantProcessorAccumulator : std::binary_function <PROCESSOR_SET,
                                                           Task,
                                                           PROCESSOR_SET>
{
public:
  RelevantProcessorAccumulator (const ReplicaFinder & rep_finder)
    : rep_finder_ (rep_finder)
  {
  }

  PROCESSOR_SET operator () (const PROCESSOR_SET & previous,
                             const Task & task)
  {
    PROCESSOR_SET result = rep_finder_ (task);

    result.insert (previous.begin (), previous.end ());

    return result;
  }

private:
  const ReplicaFinder & rep_finder_;
};

PROCESSOR_SET 
Forward_Ranking_Scheduler::relevant_processors (
  const TASK_LIST & tasks,
  const PROCESSOR_SET & ignored_processors)
{
  PROCESSOR_SET relevant =  
    std::accumulate (tasks.begin (),
                     tasks.end (),
                     PROCESSOR_SET (),
                     RelevantProcessorAccumulator (replica_finder_));

  PROCESSOR_SET result;

  // remove the processors from the result that should not be
  // permutated here
  std::set_difference (relevant.begin (),
                       relevant.end (),
                       ignored_processors.begin (),
                       ignored_processors.end (),
                       std::inserter (result,
                                      result.begin ()));

  return result;
}

PROCESSOR_SETS
Forward_Ranking_Scheduler::permute_processors (
  const PROCESSOR_SET & fixed,
  const PROCESSOR_SET & exchangeable,
  unsigned int failure_number)
{
  PROCESSOR_SETS failure_sets;
  PROCESSOR_LIST combination;
  unsigned int tupel_size = 
    std::min (failure_number,
              static_cast <unsigned int> (exchangeable.size ()));

  PROCESSOR_SET::iterator it = exchangeable.begin ();
  for (unsigned int c_index = 0; 
       c_index < tupel_size; 
       ++c_index, ++it)
    {
      combination.push_back (*it);
    }

  PROCESSOR_LIST failure_elements;
  std::copy (exchangeable.begin (),
             exchangeable.end (),
             std::inserter (failure_elements,
                            failure_elements.begin ()));

  do
    {
      PROCESSOR_SET set;
      // add a permutation of the relevant failures
      std::copy (combination.begin (),
                 combination.end (),
                 std::inserter (set,
                                set.begin ()));

      // add the fixed aspects
      std::copy (fixed.begin (),
                 fixed.end (),
                 std::inserter (set,
                                set.begin ()));

      failure_sets.push_back (set);
    }
  while (next_combination (failure_elements.begin (),
                           failure_elements.end (),
                           combination.begin (),
                           combination.end ()));

  return failure_sets;
}

double
Forward_Ranking_Scheduler::accumulate_wcrt (const TASK_LIST & tasks,
                                            const PROCESSOR_SETS & scenarios)
{
  return std::accumulate (scenarios.begin (),
                          scenarios.end (),
                          -1.0,
                          FailureAwareWCRT (tasks,
                                            replica_finder_));
}

std::ostream & operator<< (std::ostream & ostr, 
                           const FAILURE_MAP & fm)
{
  ostr << "{";
  for (FAILURE_MAP::const_iterator it = fm.begin ();
       it != fm.end ();
       ++it)
    {
      ostr << it->key ().c_str () << ": " << it->item () << ", ";
    }
  ostr << "}";

  return ostr;
}
