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
#include "Combination_T.h"

Forward_Ranking_Scheduler::Forward_Ranking_Scheduler (
  const PROCESSOR_LIST & processors,
  unsigned int max_failures)
  : Scheduler (processors, max_failures)
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

  TASK_SCENARIOS activation_scenarios = 
    this->activate_tasks (local_tasks,
                          failure_scenarios);

  TRACE ("Task Scenarios: " << activation_scenarios);

  double wcrt = this->accumulate_wcrt (activation_scenarios);

  TRACE ("Maximum wcrt: " << wcrt);

  return wcrt;
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

class ReplicaFinder : public std::unary_function <Task,
                                                  PROCESSOR_SET>
{
public:
  ReplicaFinder (const REPLICA_GROUPS & rep_groups)
    : rep_groups_ (rep_groups) {}

  PROCESSOR_SET operator () (const Task & task)
  {
    PROCESSOR_SET result;

    REPLICA_GROUPS::const_iterator replicas = 
      rep_groups_.find (primary_name (task));
 
    if (replicas != rep_groups_.end ())
      {
        std::transform (replicas->second.begin (),
                        replicas->second.begin () + task.rank,
                        std::inserter (result,
                                       result.begin ()),
                        ProcessorPicker ());
      }

    return result;
  }

private:
  const REPLICA_GROUPS rep_groups_;
};

PROCESSOR_SET
Forward_Ranking_Scheduler::replica_processors (const Task & task)
{
  ReplicaFinder finder (replica_groups_);

  return finder (task);
}

class RelevantProcessorAccumulator : std::binary_function <PROCESSOR_SET,
                                                           Task,
                                                           PROCESSOR_SET>
{
public:
  RelevantProcessorAccumulator (const REPLICA_GROUPS & rep_groups)
    : rep_groups_ (rep_groups)
  {
  }

  PROCESSOR_SET operator () (const PROCESSOR_SET & previous,
                             const Task & task)
  {
    PROCESSOR_SET result = previous;
    
    std::transform (rep_groups_.find (primary_name (task))->second.begin (),
                    rep_groups_.find (primary_name (task))->second.begin () + task.rank,
                    std::inserter (result,
                                   result.begin ()),
                    ProcessorPicker ());

    return result;
  }

private:
  const REPLICA_GROUPS & rep_groups_;
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
                     RelevantProcessorAccumulator (replica_groups_));

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

class ActivateFailedTask : public std::unary_function <PROCESSOR_SET,
                                                       Task>
{
public:
  ActivateFailedTask (const PROCESSOR_SET & failures,
                      const REPLICA_GROUPS & rep_groups)
    : failures_ (failures),
      rep_groups_ (rep_groups),
      find_replicas_ (rep_groups_) {}

  Task operator () (const Task & task)
  {
    // check in the replica map if all previous tasks get activated
    // and switch the task to primary if this is the case
    PROCESSOR_SET necessary_failures = find_replicas_ (task);

    PROCESSOR_SET difference;
    std::set_intersection (failures_.begin (),
                           failures_.end (),
                           necessary_failures.begin (),
                           necessary_failures.end (),
                           std::inserter (difference,
                                          difference.begin ()));

    // If all necessary processors are failing, make the task primary.
    // This is the case if all necessary_failures are contained in the
    // difference set.
    if (difference.size () == necessary_failures.size ())
      return convert_ (task);

    return task;
  }

private:
  const PROCESSOR_SET & failures_;
  const REPLICA_GROUPS & rep_groups_;  
  PrimaryConversion convert_;
  ReplicaFinder find_replicas_;
};

class ActivateFailedTasks : public std::unary_function <PROCESSOR_SET,
                                                        TASK_LIST>
{
public:
  ActivateFailedTasks (const TASK_LIST & tasks,
                       const REPLICA_GROUPS & rep_groups)
    : tasks_ (tasks),
      rep_groups_ (rep_groups) {}

  TASK_LIST operator () (const PROCESSOR_SET & scenario)
  {
    TASK_LIST result;

    // for each backup check whether it is activated
    std::transform (tasks_.begin (),
                    tasks_.end (),
                    std::inserter (result,
                                   result.begin ()),
                    ActivateFailedTask (scenario,
                                        rep_groups_));

    return result;
  }

private:
  const TASK_LIST & tasks_;
  const REPLICA_GROUPS & rep_groups_;
};

TASK_SCENARIOS 
Forward_Ranking_Scheduler::activate_tasks (const TASK_LIST & tasks,
                                           const PROCESSOR_SETS & failures)
{
  TASK_SCENARIOS result;

  // add an empty one for the primary case
  result.push_back (tasks);

  // add a case for each failure scenario
  std::transform (failures.begin (),
                  failures.end (),
                  std::inserter (result,
                                 result.begin ()),
                  ActivateFailedTasks (tasks,
                                       replica_groups_));

  return result;
}

class WCRTAccumulator : public std::binary_function <double,
                                                     TASK_LIST,
                                                     double>
{
public:
  double operator () (double previous,
                      const TASK_LIST & tasks)
  {
    double result = ctt_ (tasks);

    if ((result > .0) && (previous != .0))
      return std::max(result,
                      previous);
    else
      return .0;
  }

private:
  CTT_Enhanced ctt_;
};

double
Forward_Ranking_Scheduler::accumulate_wcrt (const TASK_SCENARIOS & scenarios)
{
  return std::accumulate (scenarios.begin (),
                          scenarios.end (),
                          -1.0,
                          WCRTAccumulator ());
}

std::ostream & operator<< (std::ostream & ostr, 
                           const PROCESSOR_SETS & ps)
{
  ostr << "{";
  for (PROCESSOR_SETS::const_iterator it = ps.begin ();
       it != ps.end ();
       ++it)
    {
      ostr << *it << "| ";
    }
  ostr << "}";

  return ostr;
}

std::ostream & operator<< (std::ostream & ostr, 
                           const TASK_SCENARIOS & ts)
{
  ostr << "{";
  for (TASK_SCENARIOS::const_iterator it = ts.begin ();
       it != ts.end ();
       ++it)
    {
      ostr << *it << "| ";
    }
  ostr << "}";

  return ostr;  
}
