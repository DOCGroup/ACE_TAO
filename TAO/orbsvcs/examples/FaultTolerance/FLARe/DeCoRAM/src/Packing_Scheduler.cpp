// -*- C++ -*-

//=============================================================================
/**
 *  @file    Packing_Scheduler.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <numeric>
#include "Packing_Scheduler.h"
#include "CTT_Basic.h"
#include "CTT_Enhanced.h"

Packing_Scheduler::Packing_Scheduler (const PROCESSOR_LIST & processors,
                                      unsigned int max_failures)
  : Scheduler (processors, max_failures)
{
}

class TaskListBackupAccumulator : public std::binary_function <bool,
                                                               TASK_LIST,
                                                               bool>
{
public:
  TaskListBackupAccumulator (const Task & task,
                             const Processor & primary,
                             REPLICA_GROUPS & rep_groups)
    : task_ (task),
      primary_ (primary),
      rep_groups_ (rep_groups) {}

  bool operator () (bool found, TASK_LIST & tasks)
  {
    bool result = found;

    if (!found)
      {
        Processor p = rep_groups_[primary_name (
                                    *tasks.begin ())].begin ()->first;

        if (p.compare (primary_) == 0)
          {
            result = true;
            tasks.push_back (task_);
          }
      }

    return result;
  }
private:
  Task task_;
  Processor primary_;
  REPLICA_GROUPS & rep_groups_;
};

class RankAccumulator : public std::binary_function <unsigned int,
                                                     TASK_LIST,
                                                     unsigned int>
{
public:
  RankAccumulator (REPLICA_GROUPS & rep_groups)
    : rep_groups_ (rep_groups)
  {
  }

  unsigned int operator () (unsigned int value, const TASK_LIST & tasks)
  {
    bool found = false;

    // get first task
    Task t = *tasks.begin ();

    // take rank of the first entry
    const TASK_POSITIONS & replicas = 
      rep_groups_[primary_name (t)];

    // find position of task
    unsigned int rank = 0;
    for (; rank < replicas.size (); ++rank)
      if (t.name.compare (replicas[rank].second.name) == 0)
        {
          found = true;
          ++rank;
          break;
        }

    if (!found)
      rank = 0;

    return value + rank;
  }
private:
  REPLICA_GROUPS & rep_groups_;
};

void
Packing_Scheduler::update_schedule (const ScheduleResult & result)
{
  this->Scheduler::update_schedule (result);

  this->update_task_groups (result);
}

double
Packing_Scheduler::schedule_task (const Task & task, 
                                  const Processor & processor)
{
  CTT_Basic ctt;

  if (task.rank == 0)
    {
      // do ordinary wcrt for primary
      TASK_LIST local_tasks = schedule_[processor];
      local_tasks.push_back (task);
      return ctt (local_tasks);
    }
  else
    {
      // check whether this processor already contains a replica of
      // this application
      TASK_POSITIONS replica_group = 
        replica_groups_[primary_name (task)];
      
      if (std::accumulate (replica_group.begin (),
                           replica_group.end (),
                           false,
                           ProcessorNameComparison (
                             processor)))
        return .0;

      // check if the processors contains primaries
      if (schedule_[processor].begin ()->rank == 0)
        return .0;

      // determine groups of tasks that reside on the same processors
      TASK_LISTS local_groups = task_groups_[processor];

      // add task to group
      this->add_backup (task, local_groups);

      // if this is the first entry
      if (local_groups.size () == 1)
        {
          // schedule using the basic algorithm
          TASK_LIST local_tasks = schedule_[processor];
          local_tasks.push_back (task);
          return ctt (local_tasks);
        }
      else
        {
          if (!(this->rank_check (local_groups)))
            return .0;
          
          TRACE ("found: " << local_groups);
          
          // check if tasks are schedulable in any processor failure
          // case
          if (this->merge_check (local_groups))
            {
              CTT_Enhanced ctt_enh;
              return ctt_enh (this->merge_lists (local_groups));
            }
        }
    }

  return .0;
}

class TaskListPrimaryAccumulator : public std::binary_function <bool,
                                                                TASK_LIST,
                                                                bool>
{
public:
  TaskListPrimaryAccumulator (const Task & task)
    : task_ (task) {}

  bool operator () (bool found, TASK_LIST & tasks)
  {
    bool result = found;

    if (!found)
      {
        if (tasks.begin ()->rank == 0)
          {
            result = true;
            tasks.push_back (task_);
          }
      }

    return result;
  }

private:
  Task task_;
};

void
Packing_Scheduler::update_task_groups (const ScheduleResult & result)
{
  // add task to existing task groups
  TASK_LISTS & local_groups = task_groups_[result.processor];

  if (result.task.rank == 0)
    {
      // if this is a primary check wether there are other
      // primaries on this processors
      if (!std::accumulate (local_groups.begin (),
                            local_groups.end (),
                            false,
                            TaskListPrimaryAccumulator (result.task)))
        {
          // create a new group if there is no primary yet
          TASK_LIST new_list;
          new_list.push_back (result.task);
          local_groups.push_back (new_list);
        }
    }
  else // if the task is a backup task
    {
      this->add_backup (result.task, local_groups);
    }

  TRACE (task_groups_);
}

void
Packing_Scheduler::add_backup (const Task & task,
                               TASK_LISTS & tl)
{
  // first find the primary processor
  Processor pp = 
    replica_groups_[primary_name (task)].begin ()->first;

  // check wether there is a task list with the same
  // primary processor and add it if this is the case
  if (!std::accumulate (tl.begin (),
                        tl.end (),
                        false,
                        TaskListBackupAccumulator (task,
                                                   pp,
                                                   replica_groups_)))
    {
      // create a new group if there is no group yet
      TASK_LIST new_list;
      new_list.push_back (task);
      tl.push_back (new_list);
    }  
}

bool 
Packing_Scheduler::rank_check (const TASK_LISTS & list)
{
  for (TASK_LISTS::const_iterator it1 = list.begin ();
       it1 != list.end ();
       ++it1)
    {
      for (TASK_LISTS::const_iterator it2 = it1 + 1;
           it2 != list.end ();
           ++it2)
        {
          if (this->paired_rank_check (*it1, *it2) <= max_failures_)
            {          
              return false;
            }
        }
    }

  return true;
}

unsigned int 
Packing_Scheduler::paired_rank_check (const TASK_LIST & list1,
                                      const TASK_LIST & list2)
{
  TASK_LISTS group;
  group.push_back (list1);
  group.push_back (list2);

  // check whether k = consistency_level numbers of failures would
  // affect more than one task group to become active.
  return std::accumulate (group.begin (),
                          group.end (),
                          (unsigned int) 0,
                          RankAccumulator (replica_groups_));
}

bool
Packing_Scheduler::merge_check (const TASK_LISTS & lists)
{
  bool schedulable = true;
  CTT_Enhanced ctt;

  for (size_t i = 0;
       i < lists.size ();
       ++i)
    {
      TASK_LISTS tl = lists;
      TASK_LIST active_tasks;
      
      std::transform (tl[i].begin (),
                      tl[i].end (),
                      std::inserter (active_tasks,
                                     active_tasks.begin ()),
                      PrimaryConversion ());

      tl[i] = active_tasks;

      if (.0 >= ctt (this->merge_lists (tl)))
        {
          schedulable = false;
          break;
        }
    }

  return schedulable;
}

TASK_LIST
Packing_Scheduler::merge_lists (const TASK_LISTS & lists)
{
  TASK_LIST result;

  // merge all lists
  for (TASK_LISTS::const_iterator it = lists.begin ();
       it != lists.end ();
       ++it)
    {
      std::copy (it->begin (),
                 it->end (),
                 std::inserter (result,
                                result.begin ()));
    }

  // sort list by period
  std::sort (result.begin (),
             result.end (),
             PeriodComparison<Task> ());

  return result;
}

std::ostream & operator<< (std::ostream & ostr, 
                           const TASK_LISTS & tl)
{
  ostr << "<";
  for (TASK_LISTS::const_iterator it = tl.begin ();
       it != tl.end ();
       ++it)
    {
      ostr << *it << "|";
    }
  ostr << ">";

  return ostr;
}

std::ostream & operator<< (std::ostream & ostr, 
                           const TASK_GROUPS & tg)
{
  ostr << "TASK_GROUPS:";
  for (TASK_GROUPS::const_iterator it = tg.begin ();
       it != tg.end ();
       ++it)
    {
      ostr  << std::endl << it->first << ": " << it->second;
    }

  return ostr;
}
