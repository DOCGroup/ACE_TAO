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

          // add the task to the schedule
          this->update_schedule (task, processor_it->first);

          break;
        }
    } // end for
    
  return result;  
}

void
Scheduler::update_schedule (const Task & task,
                            const Processor & processor)
{
  schedule_[processor].push_back (task);

  this->update_replica_groups (task, processor);
}

void
Scheduler::update_replica_groups (const Task & task,
                                  const Processor & processor)
{
  // create entry
  TASK_POSITION tp (processor, 
                    task);

  // add entry to respective replica group
  if (task.rank == 0)
    {
      // create a new group
      TASK_POSITIONS group;
      group.push_back (tp);
      replica_groups_[primary_name (task)] = group;
    }
  else
    {
      replica_groups_[primary_name (task)].push_back (tp);
    }

  TRACE (replica_groups_);
}

SCHEDULE
Scheduler::schedule (void) const
{
  return schedule_;
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
