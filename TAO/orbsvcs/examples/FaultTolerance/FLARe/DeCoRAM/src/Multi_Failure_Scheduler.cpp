// -*- C++ -*-

//=============================================================================
/**
 *  @file    Multi_Failure_Scheduler.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <numeric>
#include "Multi_Failure_Scheduler.h"
#include "Combination_T.h"
#include "OptimizedWCRT.h"

const std::string PROCESSOR_NOT_FOUND ("NO PROCESSOR");

//-----------------------------------------------------------------------------

PrimaryFinder::PrimaryFinder (SCHEDULE & schedule)
  : schedule_ (schedule)
{
}

BACKUP_PRIMARY_MAP::value_type
PrimaryFinder::operator () (const Task & task)
{
  Processor processor;
  TaskNamePredicate base_name_predicate (
    task.name.substr (0, task.name.find_first_of ('_')));

  TASK_LIST::iterator find_result;

  for (SCHEDULE::iterator it = schedule_.begin ();
       it != schedule_.end ();
       ++it)
    {
      find_result = std::find_if (it->second.begin (),
                                  it->second.end (),
                                  base_name_predicate);

      if (find_result != it->second.end ())
        {
          processor = it->first;
          break;
        }
      else
        {
          processor = PROCESSOR_NOT_FOUND;
        }
    }

  return std::pair <Taskname, Processor> (task.name, processor);
}

//-----------------------------------------------------------------------------

Multi_Failure_Scheduler::Multi_Failure_Scheduler (SCHEDULE & current_schedule,
                                                  const SCHEDULE & global_schedule,
                                                  unsigned int failure_number)
  : current_schedule_ (current_schedule),
    global_schedule_ (global_schedule),
    failure_number_ (failure_number),
    primary_finder_ (global_schedule_)
{
}

ScheduleResult 
Multi_Failure_Scheduler::operator () (const Task & task)
{
  TRACE (" scheduling " << task << "  = " << task.role);

  ScheduleResult result;
  result.task = task;
  result.processor = "NO PROCESSOR";
  result.wcrt = .0;

  for (SCHEDULE::iterator processor_it = current_schedule_.begin ();
       processor_it != current_schedule_.end ();
       ++processor_it)
    {
      // iterate through all possible failure cases that might affect
      // this processor, based on its tasks
      TASK_LIST local_tasks = processor_it->second;
      
      FAILOVER_SCENARIOS failover_scenarios = 
        this->get_failover_scenarios (
          this->calculate_relevant_failure_scenarios (processor_it->first,
                                                      task,
                                                      failure_number_), 
          processor_it->first,
          task);

      // add task name itself to each failure scenario and always
      // schedule it as active
      Task copy = task;
      copy.role = PRIMARY;
      local_tasks.push_back (task);

      // this will go over all the possible combinations of active
      // backups on this processor and return the maximum worst-case
      // response time.
      OptimizedWCRT wcrt_algorithm (local_tasks);

      double wcrt = std::accumulate (failover_scenarios.begin (),
                                     failover_scenarios.end (),
                                     -1.0, // this value is just for
                                           // initialization, and cannot
                                           // be 0 since this would mean
                                           // that there is an
                                           // unschedulable task
                                     wcrt_algorithm);

      TRACE ("wcrt = " << wcrt);

      // if the previous step found no valid schedule wcrt will be 0
      // and we have to proceed with the next available processor
      if (wcrt > .0)
        {
          // take result value and exit if we find a match
          result.wcrt = wcrt;
          result.processor = processor_it->first;

          // remove this processor from the schedule so that
          // following backups are not scheduled on the same processor
          current_schedule_.erase (processor_it);

          // add result to local copy of the global schedule
          global_schedule_[result.processor].push_back (result.task);

          break;
        }
    }

  return result;
}

//-----------------------------------------------------------------------------

struct NoBackupPredicate : public std::unary_function <Task,
                                                       bool>
{
  bool operator () (const Task & task)
  {
    return task.role != BACKUP;
  }
};

TASK_LIST 
Multi_Failure_Scheduler::get_backups (const Processor & p,
                                SCHEDULE & schedule)
{
  TASK_LIST backups;

  std::remove_copy_if (schedule[p].begin (),
                       schedule[p].end (),
                       std::inserter (backups,
                                      backups.begin ()),
                       NoBackupPredicate ());

  return backups;
}

//-----------------------------------------------------------------------------

struct NoPrimaryPredicate : public std::unary_function <Task,
                                                       bool>
{
  bool operator () (const Task & task)
  {
    return task.role != PRIMARY;
  }
};

TASK_LIST
Multi_Failure_Scheduler::get_primaries (const Processor & p,
                                  SCHEDULE & schedule)
{
  TASK_LIST primaries;

  std::remove_copy_if (schedule[p].begin (),
                       schedule[p].end (),
                       std::inserter (primaries,
                                      primaries.begin ()),
                       NoPrimaryPredicate ());

  return primaries;
}

//-----------------------------------------------------------------------------

BACKUP_PRIMARY_MAP
Multi_Failure_Scheduler::get_primary_processors (const TASK_LIST & tasks,
                                                 SCHEDULE & schedule)
{
  BACKUP_PRIMARY_MAP processors;

  std::transform (tasks.begin (),
                  tasks.end (),
                  std::inserter (processors,
                                 processors.begin ()),
                  PrimaryFinder (schedule));

  // make sure that we remove all entries that are not validx
  processors.erase (PROCESSOR_NOT_FOUND);

  return processors;
}

//-----------------------------------------------------------------------------

struct AddProcessor : public std::unary_function <BACKUP_PRIMARY_MAP::value_type,
                                                  Processor>
{
  Processor operator () (const BACKUP_PRIMARY_MAP::value_type & entry)
  {
    return entry.second;
  }
};

struct BackupSelector : public std::unary_function <Processor,
                                                    Taskname>
{
public:
  BackupSelector (BACKUP_PRIMARY_MAP & bp)
    : bp_ (bp)
  {
  }

  Taskname operator () (const Processor & proc)
  {
    return bp_[proc];
  }

private:
  BACKUP_PRIMARY_MAP & bp_;
};

struct ActiveBackupCalculator : public std::unary_function <PROCESSOR_SET,
                                                            TASKNAME_SET>
{
  ActiveBackupCalculator (const Processor & processor,
                          SCHEDULE & schedule,
                          const Task & task)
    : processor_ (processor),
      schedule_ (schedule),
      task_ (task)
  {
  }

  TASKNAME_SET 
  operator () (const PROCESSOR_SET & failed_processors)
  {
    // this method actually assumes that a backup can always gets
    // active when its primary failed. Since this is only the case for
    // one of the backups, we have to optimize this algorithm later by
    // taking the backup rank into account.

    // only consider the backup tasks on this processor
    TASK_LIST backups = Multi_Failure_Scheduler::get_backups (processor_,
                                                              schedule_);

    // if the currently scheduled task is a backup, we have to
    // consider it active as well to get the worst-case scenario
    if (task_.role == BACKUP)
      backups.push_back (task_);

    BACKUP_PRIMARY_MAP backups_to_primary_processors =
      Multi_Failure_Scheduler::get_primary_processors (backups,
                                                       schedule_);

    // create a set of processors that contain primaries for these
    // backups
    PROCESSOR_SET primary_processors;
    std::transform (backups_to_primary_processors.begin (),
                    backups_to_primary_processors.end (),
                    std::inserter (primary_processors,
                                   primary_processors.begin ()),
                    AddProcessor ());

    // find all processors that have failed and contained primaries of
    // the backups we look at.

    PROCESSOR_SET relevant_primary_processors;

    std::set_intersection (failed_processors.begin (),
                           failed_processors.end (),
                           primary_processors.begin (),
                           primary_processors.end (),
                           std::inserter (
                             relevant_primary_processors,
                             relevant_primary_processors.begin ()));

    // select all the backups as active that have relevant primaries
    TASKNAME_SET active_backups;
    for (TASK_LIST::iterator it = backups.begin ();
         it != backups.end ();
         ++it)
      {
        Processor primary = backups_to_primary_processors[it->name];

        if (relevant_primary_processors.find (primary) != 
            relevant_primary_processors.end ())
          active_backups.insert (it->name);
      }

    return active_backups;
  }
 
private:
  Processor processor_;
  SCHEDULE & schedule_;
  Task task_;
};

//-----------------------------------------------------------------------------

FAILURE_SCENARIOS 
Multi_Failure_Scheduler::calculate_relevant_failure_scenarios (
    const Processor & processor,
    const Task & task,
    unsigned int failure_number)
{
  TRACE ("(" << processor << ")");

  FAILURE_SCENARIOS result;

  // only consider the backup tasks on this processor
  TASK_LIST backups = this->get_backups (processor,
                                         global_schedule_);

  TRACE ("TL=" << backups);

  BACKUP_PRIMARY_MAP backups_to_primary_processors =
    this->get_primary_processors (backups,
                                  global_schedule_);

  // if the task to schedule is a backup, we can assume that its
  // primary processor failed in any case
  unsigned int failing_backups = 0;
  Processor primary_proc;
  if (task.role == BACKUP)
    {
      failing_backups = failure_number - 1;
      primary_proc = primary_finder_ (task).second;
      backups_to_primary_processors.erase (primary_proc);
    }
  else
    {
      failing_backups = failure_number;
    }
  
  // create a set of processors that contain primaries for these
  // backups
  PROCESSOR_SET relevant_processors_set;
  std::transform (backups_to_primary_processors.begin (),
                  backups_to_primary_processors.end (),
                  std::inserter (relevant_processors_set,
                                 relevant_processors_set.begin ()),
                  AddProcessor ());
  
  TRACE ("PP=" << relevant_processors_set);
  PROCESSOR_LIST relevant_processors;
  std::copy (relevant_processors_set.begin (),
             relevant_processors_set.end (),
             std::inserter (relevant_processors,
                            relevant_processors.begin ()));

  // initialize the list of combinations of processors If we have more
  // processors in here than the number of failures, we account for,
  // we will look at all failure scenarios with the maximum numbers of
  // failures. If there are less, we look at all possible combinations
  // of those processors failing.
  PROCESSOR_LIST combination;

  unsigned int combination_size = 
    std::min <unsigned int> (backups_to_primary_processors.size (), 
                             failing_backups);

  BACKUP_PRIMARY_MAP::iterator it = 
    backups_to_primary_processors.begin ();

  for (unsigned int c_index = 0; 
       c_index < combination_size; 
       ++c_index, ++it)
    {
      combination.push_back (it->second);
    }

  // here we will add all possible failure scenarios from zero
  // failures up to the maximum number of failures == consistency
  // level
  do 
    {
      PROCESSOR_SET set;
      std::copy (combination.begin (),
                 combination.end (),
                 std::inserter (set,
                                set.begin ()));
      
      if (task.role == BACKUP)
        set.insert (primary_proc);

      // always add a failed primary if this is a backup,
      // since this is the worst case
      result.push_back (set);
      
      TRACE ("result +=" << set);
    }
  while (next_combination (relevant_processors.begin (),
                           relevant_processors.end (),
                           combination.begin (),
                           combination.end ()));
  
  return result;
}

//-----------------------------------------------------------------------------

FAILOVER_SCENARIOS
Multi_Failure_Scheduler::get_failover_scenarios (
  const FAILURE_SCENARIOS & failed_processors,
  const Processor & processor,
  const Task & task)
{
  TRACE ("(" << processor << ")");

  FAILOVER_SCENARIOS scenarios;

  std::transform (failed_processors.begin (),
                  failed_processors.end (),
                  std::inserter (scenarios,
                                 scenarios.begin ()),
                  ActiveBackupCalculator (processor,
                                          global_schedule_,
                                          task));

  return scenarios;
}

//-----------------------------------------------------------------------------
