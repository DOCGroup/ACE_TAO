// -*- C++ -*-

//=============================================================================
/**
 *  @file    FTRMFF_Primary.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <sstream>
#include "FTRMFF_Primary.h"
#include <algorithm>

FTRMFF_Primary::~FTRMFF_Primary ()
{
}

FTRMFF_Output
FTRMFF_Primary::operator () (const FTRMFF_Input & input)
{
  FTRMFF_Primary_Algorithm algorithm (input.processors);

  FTRMFF_Output output;
  output.schedule = algorithm (input.tasks);
  output.unscheduled_tasks = algorithm.get_unschedulable ();

  DBG_OUT (algorithm.schedule ());

  return output;
}

FTRMFF_Primary_Algorithm::FTRMFF_Primary_Algorithm (
  const PROCESSOR_LIST & processors)
  : schedule_ (create_schedule (processors))
{
}

FTRMFF_Primary_Algorithm::~FTRMFF_Primary_Algorithm ()
{
}

SCHEDULING_MAP
FTRMFF_Primary_Algorithm::operator () (const TASK_LIST & tasks)
{
  // sort tasks based on their periods, which results in a priority
  // ordered list since we do rate monotonic scheduling
  TASK_LIST sorted_input = tasks;

  std::sort (sorted_input.begin (), 
	     sorted_input.end (), 
	     PeriodComparison <Task> ());

  for (TASK_LIST::iterator it = sorted_input.begin ();
       it != sorted_input.end ();
       ++it)
    {
      double wcrt = -1.0;

      for (SCHEDULE::iterator processor_it = schedule_.begin ();
           processor_it != schedule_.end ();
           ++processor_it)
        {
          TASK_LIST local_tasks = processor_it->second;
          local_tasks.push_back (*it);

          // run time completion test
          wcrt = ctt_ (local_tasks);

          if (wcrt > .0)
            {
              processor_it->second.push_back (*it);
              break;
            }
        }

      if (wcrt <= .0)
        {
          // could not schedule task
          ScheduleProgress pg = {*it, 1};
          unschedulable_.push_back (pg);
        }
    }

  return transform_schedule (schedule_);
}

SCHEDULE_PROGRESS_LIST
FTRMFF_Primary_Algorithm::get_unschedulable ()
{
  return unschedulable_;
}

const SCHEDULE & 
FTRMFF_Primary_Algorithm::schedule () const
{
  return schedule_;
}
