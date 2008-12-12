// -*- C++ -*-

//=============================================================================
/**
 *  @file    Task_Scheduler.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include "Task_Scheduler.h"

Task_Scheduler::Task_Scheduler (SCHEDULE & schedule,
                                CTT_Algorithm & ctt)
  : schedule_ (schedule),
    ctt_ (ctt)
{  
}

ScheduleResult 
Task_Scheduler::operator () (const Task & task)
{
  ScheduleResult result;
  result.task = task;
  result.processor = "NO PROCESSOR";
  result.wcrt = .0;

  for (SCHEDULE::iterator processor_it = schedule_.begin ();
       processor_it != schedule_.end ();
       ++processor_it)
    {
      TASK_LIST local_tasks = processor_it->second;
      
      local_tasks.push_back (task);

      // run time completion test
      double wcrt = ctt_ (local_tasks);
      if (wcrt > .0)
        {
          // take result value and exit if we find a match
          result.wcrt = wcrt;
          result.processor = processor_it->first;

          // remove this processor from the schedule so that
          // following backups are not scheduled on the same processor
          schedule_.erase (processor_it);
          
          break;
        }
    }
    
  return result;
}
