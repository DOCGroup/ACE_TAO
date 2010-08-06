// -*- C++ -*-

//=============================================================================
/**
 *  @file    Scheduler.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include "Scheduler.h"
#include <algorithm>

Scheduler::Scheduler (const PROCESSOR_LIST & processors,
                      unsigned int max_failures)
  : schedule_ (create_schedule (processors)),
    max_failures_ (max_failures)
{
}

Scheduler::~Scheduler (void)
{
}

ScheduleResult
Scheduler::operator () (const Task & task)
{
  ScheduleResult result;
  result.task = task;
  result.processor = "NO PROCESSOR";
  result.wcrt = .0;

  for (SCHEDULE::iterator processor_it = schedule_.begin ();
       processor_it != schedule_.end ();
       ++processor_it)
    {
      double wcrt = this->schedule_task (task, processor_it->first);

      if (wcrt > .0)
        {
          // take result value and exit if we find a match
          result.wcrt = wcrt;
          result.processor = processor_it->first;

          break;
        }
    } // end for
    
  return result;  
}

ScheduleResult 
Scheduler::operator () (const Task & task,
                        const Processor & processor)
{
  ScheduleResult result;
  result.task = task;
  result.processor = processor;
  result.wcrt = this->schedule_task (task, processor);
    
  return result;
}


void
Scheduler::update_schedule (const ScheduleResult & result)
{
  schedule_[result.processor].push_back (result.task);

  this->update_replica_groups (result);
}

void
Scheduler::update_replica_groups (const ScheduleResult & result)
{
  // create entry
  TASK_POSITION tp (result.processor, 
                    result.task);

  // add entry to respective replica group
  if (result.task.rank == 0)
    {
      // create a new group
      TASK_POSITIONS group;
      group.push_back (tp);
      replica_groups_[primary_name (result.task)] = group;
    }
  else
    {
      replica_groups_[primary_name (result.task)].push_back (tp);
    }

  TRACE (replica_groups_);
}

const SCHEDULE &
Scheduler::schedule (void) const
{
  return schedule_;
}

ProcessorNameComparison::ProcessorNameComparison (const Processor & p)
  :p_ (p)
{
}

bool
ProcessorNameComparison::operator () (bool equal, 
                                      const TASK_POSITIONS::value_type & pos)
{
  return (equal || (pos.first.compare (p_) == 0));
}

Processor 
ProcessorPicker::operator () (const TASK_POSITIONS::value_type & entry)
{
  return entry.first;
}

ReplicaFinder::ReplicaFinder (const REPLICA_GROUPS & rep_groups)
  : rep_groups_ (rep_groups) 
{
}

ReplicaFinder::~ReplicaFinder (void)
{
}

PROCESSOR_SET 
ReplicaFinder::operator () (const Task & task) const
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
                      processor_picker_);
    }
  
  return result;
}

std::ostream & operator<< (std::ostream & ostr, 
                           const TASK_POSITION & tp)
{
  ostr << tp.second << "@" << tp.first;

  return ostr;
}

std::ostream & operator<< (std::ostream & ostr, 
                           const TASK_POSITIONS & tps)
{
  ostr << "[";
  for (TASK_POSITIONS::const_iterator it = tps.begin ();
       it != tps.end ();
       ++it)
    {
      ostr << *it << ", ";
    }
  ostr << "]";

  return ostr;
}

std::ostream & operator<< (std::ostream & ostr, 
                           const REPLICA_GROUPS & rg)
{
  ostr << "REPLICA_GROUPS:";
  for (REPLICA_GROUPS::const_iterator it = rg.begin ();
       it != rg.end ();
       ++it)
    {
      ostr << std::endl << it->first << ": " << it->second;
    }
  
  return ostr;
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
