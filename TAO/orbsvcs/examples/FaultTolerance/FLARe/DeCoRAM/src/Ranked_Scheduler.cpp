// -*- C++ -*-

//=============================================================================
/**
 *  @file    Ranked_Scheduler.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include "Ranked_Scheduler.h"

Ranked_Scheduler::Ranked_Scheduler (SCHEDULE & current_schedule,
                                    RankedWCRT & wcrt_algorithm)
  : current_schedule_ (current_schedule),
    wcrt_algorithm_ (wcrt_algorithm)
{
}

ScheduleResult
Ranked_Scheduler::operator () (const Task & task)
{
  ScheduleResult result;
  result.task = task;
  result.processor = "NO PROCESSOR";
  result.wcrt = .0;

  for (SCHEDULE::iterator processor_it = current_schedule_.begin ();
       processor_it != current_schedule_.end ();
       ++processor_it)
    {
      TASK_LIST local_tasks = processor_it->second;

      local_tasks.push_back (task);

      double wcrt = wcrt_algorithm_ (local_tasks);

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

          break;
        }
    } // end for
    
  return result;  
}
