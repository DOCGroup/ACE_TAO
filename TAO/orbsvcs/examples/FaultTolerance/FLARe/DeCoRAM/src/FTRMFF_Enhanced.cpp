// -*- C++ -*-

//=============================================================================
/**
 *  @file    FTRMFF_Enhanced.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <sstream>
#include "FTRMFF_Enhanced.h"
#include "Multi_Failure_Scheduler.h"
#include "Simple_Ranking.h"
#include <algorithm>

FTRMFF_Enhanced::~FTRMFF_Enhanced ()
{
}

FTRMFF_Output
FTRMFF_Enhanced::operator () (const FTRMFF_Input & input)
{
  FTRMFF_Enhanced_Algorithm algorithm (input.processors,
                                       input.backup_count);

  FTRMFF_Output output;
  output.schedule = algorithm (input.tasks);
  output.unscheduled_tasks = algorithm.get_unschedulable ();

  DBG_OUT (algorithm.schedule ());

  return output;
}

FTRMFF_Enhanced_Algorithm::FTRMFF_Enhanced_Algorithm (
  const PROCESSOR_LIST & processors,
  unsigned int consistency_level)
  : schedule_ (create_schedule (processors)),
    consistency_level_ (consistency_level),
    ranking_algorithm_ (new Simple_Ranking ())
{
}

FTRMFF_Enhanced_Algorithm::~FTRMFF_Enhanced_Algorithm ()
{
}

SCHEDULING_MAP
FTRMFF_Enhanced_Algorithm::operator () (const TASK_LIST & tasks)
{
  TRACE ("begin");
  
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

      // the scheduler needs both schedules. The local copy is used to
      // evaluate one stage of processing, while the global one is
      // used find possible failure scenarios.
      Multi_Failure_Scheduler scheduler (temp_schedule,
                                         schedule_,
                                         consistency_level_);

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
      unsigned int scheduled_backups = 
        (*ranking_algorithm_) (results,
                              schedule_);

      if (scheduled_backups < results.size ())
        {
          // could not schedule all backups
          ScheduleProgress pg = {*it, 1 + scheduled_backups};
          unschedulable_.push_back (pg);
          continue;
        }

      // if we reach this code, we can add all tasks to the schedule
      add_schedule_results (results, schedule_);
    }

  return transform_schedule (schedule_);
}

SCHEDULE_PROGRESS_LIST
FTRMFF_Enhanced_Algorithm::get_unschedulable ()
{
  return unschedulable_;
}

const SCHEDULE &
FTRMFF_Enhanced_Algorithm::schedule () const
{
  return schedule_;
}
