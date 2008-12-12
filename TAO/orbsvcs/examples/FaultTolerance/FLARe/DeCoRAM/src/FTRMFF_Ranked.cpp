// -*- C++ -*-

//=============================================================================
/**
 *  @file    FTRMFF_Ranked.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <sstream>
#include "FTRMFF_Ranked.h"
#include "Ranked_Scheduler.h"

FTRMFF_Ranked::~FTRMFF_Ranked ()
{
}

FTRMFF_Output
FTRMFF_Ranked::operator () (const FTRMFF_Input & input)
{
  FTRMFF_Ranked_Algorithm algorithm (input.processors,
                                     input.backup_count);

  FTRMFF_Output output;
  output.schedule = algorithm (input.tasks);
  output.unscheduled_tasks = algorithm.get_unschedulable ();

  return output;
}

FTRMFF_Ranked_Algorithm::FTRMFF_Ranked_Algorithm (
  const PROCESSOR_LIST & processors,
  unsigned int consistency_level)
  : consistency_level_ (consistency_level)
{
  for (PROCESSOR_LIST::const_iterator it = processors.begin ();
       it != processors.end ();
       ++it)
    {
      schedule_[*it] = Schedule_Entry ();
    }
}

FTRMFF_Ranked_Algorithm::~FTRMFF_Ranked_Algorithm ()
{
}

SCHEDULING_MAP
FTRMFF_Ranked_Algorithm::operator () (const TASK_LIST & tasks)
{
  double_equal equals;
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
      // use a copy of the original schedule since we will modify it
      SCHEDULE temp_schedule = schedule_;

      RankedWCRT wcrt_algorithm (schedule_,
                                 consistency_level_);

      Ranked_Scheduler scheduler (temp_schedule,
                                  wcrt_algorithm);

      // schedule primary
      Task primary = *it;
      primary.role = PRIMARY;

      ScheduleResult primary_schedule = scheduler (primary);

      if (equals (primary_schedule.wcrt, .0))
        {
          ScheduleProgress pg = { *it, 0 };
          unschedulable_.push_back (pg);
          continue;
        }

      // create the right amount of backup replica tasks
      TASK_LIST task_group = create_tasks (*it, consistency_level_);

      // schedule the backup tasks of one application
      SCHEDULE_RESULT_LIST results;
      std::transform (task_group.begin (),
                      task_group.end (),
                      std::inserter (results,
                                     results.begin ()),
                      scheduler);
          
      // rank backups according to their wcrt
      unsigned int scheduled_backups = rank_backups (results);
      if (scheduled_backups < results.size ())
        {
          // could not schedule all backups
          ScheduleProgress pg = {*it, 1 + scheduled_backups};
          unschedulable_.push_back (pg);
          continue;
        }

      // if we reach this code, we can add all tasks to the schedule
      results.push_back (primary_schedule);
      add_schedule_results (results, schedule_);
    }

  return transform_schedule (schedule_);
}

SCHEDULE_PROGRESS_LIST
FTRMFF_Ranked_Algorithm::get_unschedulable ()
{
  return unschedulable_;
}
