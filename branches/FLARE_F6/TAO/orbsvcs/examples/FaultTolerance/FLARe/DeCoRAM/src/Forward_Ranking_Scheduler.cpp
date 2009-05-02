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
                              additional_failures);

  TRACE ("Relevant Failure Scenarios: " << failure_scenarios);

  double wcrt = this->accumulate_wcrt (local_tasks,
                                       failure_scenarios);

  TRACE ("Maximum wcrt: " << wcrt);

  std::cout << "WCRT of task " << task.name << " in pro " << processor << " is " << wcrt << " with fixed pros " << fixed_failures << " and exc pros " << additional_failures << " and failure sets " << failure_scenarios << std::endl;

  return wcrt;
}

void
Forward_Ranking_Scheduler::update_schedule (const ScheduleResult & result)
{
  this->Scheduler::update_schedule (result);

  this->update_failure_map (result);
}

void 
Forward_Ranking_Scheduler::update_failure_map (const ScheduleResult & result)
{
  PROCESSOR_SET proc_dependencies;

  REPLICA_GROUPS::iterator it =
    replica_groups_.find (primary_name (result.task));

  if (it != replica_groups_.end ())
    {
      for (TASK_POSITIONS::iterator tp_it = it->second.begin ();
           tp_it != it->second.begin () + result.task.rank;
           ++tp_it)
        {
          proc_dependencies.insert (tp_it->first);
        }
    }

  failure_map_.bind (result.task.name.c_str (), proc_dependencies);

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
  const PROCESSOR_SET & exchangeable)
{
  PROCESSOR_SETS failure_sets;

  if (exchangeable.size () > 0)
    {
      // there are multiple backup replicas that are already hosted
      // on the processor we are trying to allocate a primary or a backup
      // replica
      if (fixed.size () == 0)
        {
          // we are adding a primary replica to a processor that
          // already has multiple backup replicas
          if (exchangeable.size () <= max_failures_)
            {
              // we have less than or equal to 'K' processors to check
              // we just check them. We do not need any permutations
              failure_sets.push_back (exchangeable);
            }
          else if (exchangeable.size () > max_failures_)
            {
              // We have more than 'k' processors to check.
              // We need permutations of 'k' size each
              PROCESSOR_LIST failure_combination;
              PROCESSOR_SET::iterator failure_it = exchangeable.begin ();
              for (unsigned int exchange_index = 0;
                   exchange_index < max_failures_;
                   ++exchange_index, ++failure_it)
                {
                  failure_combination.push_back (*failure_it);
                }
              PROCESSOR_LIST exchangeable_elements;
              std::copy (exchangeable.begin (),
                         exchangeable.end (),
                         std::inserter (exchangeable_elements,
                                        exchangeable_elements.begin ()));
              do
                {
                  PROCESSOR_SET proc_fail_set;
                  std::copy (failure_combination.begin (),
                             failure_combination.end (),
                             std::inserter (proc_fail_set,
                                            proc_fail_set.begin ()));
                  failure_sets.push_back (proc_fail_set);
                }
              while (next_combination (exchangeable_elements.begin (),
                                       exchangeable_elements.end (),
                                       failure_combination.begin (),
                                       failure_combination.end ()));
            }
        }
      else if (fixed.size () > 0)
        {
          // we are adding a backup replica to a processor that already
          // has multiple backup replicas
          unsigned int total_proc_size =
            exchangeable.size () + fixed.size ();

          PROCESSOR_SET fixed_exchangeable_set;
          std::copy (exchangeable.begin (),
                     exchangeable.end (),
                     std::inserter (fixed_exchangeable_set,
                                    fixed_exchangeable_set.begin ()));
          std::copy (fixed.begin (),
                     fixed.end (),
                     std::inserter (fixed_exchangeable_set,
                                    fixed_exchangeable_set.begin ()));

          if (total_proc_size <= max_failures_)
            {
              // We have less than or equal to 'k' processors to check
              // We do not need any permutations.
              failure_sets.push_back (fixed_exchangeable_set);
            }
          else if (total_proc_size > max_failures_)
            {
              // We have more than 'k' processors to check.
              // We need permutations of 'k' size each
              PROCESSOR_LIST combination;
              PROCESSOR_SET::iterator it = fixed_exchangeable_set.begin ();
              for (unsigned int comb_index = 0;
                   comb_index < max_failures_; ++comb_index, ++it)
                {
                  combination.push_back (*it);
                }
              PROCESSOR_LIST failure_elements;
              std::copy (fixed_exchangeable_set.begin (),
                         fixed_exchangeable_set.end (),
                         std::inserter (failure_elements,
                                        failure_elements.begin ()));
              do
                {
                  PROCESSOR_SET set;
                  std::copy (combination.begin (),
                             combination.end (),
                             std::inserter (set,
                                            set.begin ()));
                  failure_sets.push_back (set);
                }
              while (next_combination (failure_elements.begin (),
                                       failure_elements.end (),
                                       combination.begin (),
                                       combination.end ()));
            }
        }
    }
  else if (exchangeable.size () == 0)
    {
      // we are adding a backup replica or a primary replica to a procssor
      // that does not have any backup replicas already hosted
      // So we do not need to check any failure scenarios
      if (fixed.size () == 0)
        {
          failure_sets.push_back (exchangeable);
        }
      else if (fixed.size () > 0)
        {
          failure_sets.push_back (fixed);
        }
    }

  return failure_sets;

/*

  if (fixed.size () == max_failures_)
    {
      failure_sets.push_back (fixed);
    }
  else if (fixed.size () < max_failures_)
    {
      unsigned int tupel_size = max_failures_ - fixed.size ();

      if (fixed.size () == 0)
        {
          if (exchangeable.size () <= tupel_size)
            {
              failure_sets.push_back (exchangeable);
            }
          else
            {
              PROCESSOR_LIST failure_combination;
              PROCESSOR_SET::iterator fail_it = exchangeable.begin ();
              for (unsigned int c_index = 0;c_index < tupel_size;
                   ++c_index, ++fail_it)
                {
                  failure_combination.push_back (*fail_it);
                }
              PROCESSOR_LIST fail_elements;
              std::copy (exchangeable.begin (),
                         exchangeable.end (),
                         std::inserter (fail_elements,
                                        fail_elements.begin ()));
              do
                {
                  PROCESSOR_SET fail_set;
                  std::copy (failure_combination.begin (),
                             failure_combination.end (),
                             std::inserter (fail_set,
                                            fail_set.begin ()));
                  std::copy (fixed.begin (),
                             fixed.end (),
                             std::inserter (fail_set,
                                            fail_set.begin ()));

                  failure_sets.push_back (fail_set);
                }
              while (next_combination (fail_elements.begin (),
                                       fail_elements.end (),
                                       failure_combination.begin (),
                                       failure_combination.end ()));
            }
        }
      else if (fixed.size () > 0)
        {
          if (exchangeable.size () <= tupel_size)
            {
              PROCESSOR_SET fixed_exchangeable_set;

              // add the fixed elements
              std::copy (fixed.begin (),
                         fixed.end (),
                         std::inserter (fixed_exchangeable_set,
                                        fixed_exchangeable_set.begin ()));
              
              // add the exchangeable elements
              std::copy (exchangeable.begin (),
                         exchangeable.end (),
                         std::inserter (fixed_exchangeable_set,
                                        fixed_exchangeable_set.begin ()));

              failure_sets.push_back (fixed_exchangeable_set);
            }
          else
            {
              PROCESSOR_LIST combination;
              PROCESSOR_SET::iterator it = exchangeable.begin ();
              for (unsigned int comb_index = 0;comb_index < tupel_size;
                   ++comb_index, ++it)
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
                  std::copy (combination.begin (),
                             combination.end (),
                             std::inserter (set,
                                            set.begin ()));
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
            }
        }
    }

  return failure_sets;

*/

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
