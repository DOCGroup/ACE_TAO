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
#include "Simple_Ranking.h"
#include "Utilization_Ranking.h"
#include <algorithm>

FTRMFF_Ranked::~FTRMFF_Ranked ()
{
}

FTRMFF_Output
FTRMFF_Ranked::operator () (const FTRMFF_Input & input)
{
  FTRMFF_Ranked_Algorithm algorithm (input.processors,
                                     input.backup_count,
                                     "utilization");

  FTRMFF_Output output;
  output.schedule = algorithm (input.tasks);
  output.unscheduled_tasks = algorithm.get_unschedulable ();

  DBG_OUT (algorithm.schedule ());

  return output;
}

FTRMFF_Ranked_Algorithm::FTRMFF_Ranked_Algorithm (
  const PROCESSOR_LIST & processors,
  unsigned int consistency_level,
  const std::string & ranking_type)
  : FTRMFF_Algorithm_Impl (consistency_level),
    schedule_ (create_schedule (processors))
{
  if (ranking_type.compare ("utilization") == 0)
    ranking_algorithm_.reset (new Utilization_Ranking ());
  else
    ranking_algorithm_.reset (new Simple_Ranking ());
}

FTRMFF_Ranked_Algorithm::~FTRMFF_Ranked_Algorithm ()
{
}

SCHEDULING_MAP
FTRMFF_Ranked_Algorithm::operator () (const TASK_LIST & tasks)
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
      // use a copy of the original schedule since we will modify it
      SCHEDULE temp_schedule = schedule_;

      RankedWCRT wcrt_algorithm (schedule_,
                                 consistency_level_);

      Ranked_Scheduler scheduler (temp_schedule,
                                  wcrt_algorithm);


      // create the right amount of backup replica tasks
      TASK_LIST task_group = create_tasks (*it, consistency_level_);

      // schedule the tasks of one application
      SCHEDULE_RESULT_LIST results;
      std::transform (task_group.begin (),
                      task_group.end (),
                      std::inserter (results,
                                     results.begin ()),
                      scheduler);
          
      // rank backups according to their wcrt
      unsigned int scheduled_replicas = 
        (*ranking_algorithm_) (results,
                               schedule_);

      if (scheduled_replicas < results.size ())
        {
          // could not schedule all backups
          ScheduleProgress pg = {*it, scheduled_replicas};
          unschedulable_.push_back (pg);
          continue;
        }

      // if we reach this code, we can add all tasks to the schedule
      add_schedule_results (results, schedule_);

      TRACE ("New Schedule:" << std::endl << schedule_ << std::endl);
    }

  return transform_schedule (schedule_);
}
const SCHEDULE & 
FTRMFF_Ranked_Algorithm::schedule () const
{
  return schedule_;
}
