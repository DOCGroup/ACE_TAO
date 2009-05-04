// -*- C++ -*-

//=============================================================================
/**
 *  @file    FTRMFF_Worstfit.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include "FTRMFF_Worstfit.h"
#include "RankedWCRT.h"
#include "Utilization_Ranking.h"
#include <algorithm>

FTRMFF_Worstfit::~FTRMFF_Worstfit ()
{
}

FTRMFF_Output
FTRMFF_Worstfit::operator () (const FTRMFF_Input & input)
{
  FTRMFF_Worstfit_Algorithm algorithm (input.processors,
                                      input.backup_count);

  FTRMFF_Output output;
  output.schedule = algorithm (input.tasks);
  output.unscheduled_tasks = algorithm.get_unschedulable ();

  DBG_OUT (remove_empty_processors (algorithm.schedule ()));

  return output;
}

FTRMFF_Worstfit_Algorithm::FTRMFF_Worstfit_Algorithm (
  const PROCESSOR_LIST & processors,
  unsigned int consistency_level)
  : FTRMFF_Algorithm_Impl (consistency_level),
    schedule_ (create_schedule (processors))
{
}

FTRMFF_Worstfit_Algorithm::~FTRMFF_Worstfit_Algorithm ()
{
}

SCHEDULING_MAP
FTRMFF_Worstfit_Algorithm::operator () (const TASK_LIST & tasks)
{
  // sort tasks according to their worst case response times
  TASK_LIST sorted_tasks = tasks;

  std::sort (sorted_tasks.begin (), 
	     sorted_tasks.end (), 
	     PeriodComparison <Task> ());

  // for each task
  for (TASK_LIST::const_iterator task_it = sorted_tasks.begin ();
       task_it != sorted_tasks.end ();
       ++task_it)
    {
      // create a set of replicas
      TASK_LIST replica_group = create_tasks (*task_it, consistency_level_);

      // copy global schedule for per-task scheduling
      SCHEDULE local_schedule = schedule_;

      // initialize worst-case response time algorithm
      RankedWCRT wcrt_algorithm (local_schedule,
                                 consistency_level_);

      // this data-structure will store result schedules
      SCHEDULE_RESULT_LIST schedule_results;

      // for each replica
      for (TASK_LIST::iterator replica_it = replica_group.begin ();
           replica_it != replica_group.end ();
           ++replica_it)
        {
          ScheduleResult best_result;
          best_result.wcrt = -1.0;

          // for each processor
          for (SCHEDULE::iterator proc_it = local_schedule.begin ();
               proc_it != local_schedule.end ();
               ++proc_it)
            {
              // calculate worst-case response time of replica on processor
              TASK_LIST local_tasks = local_schedule[proc_it->first];

              local_tasks.push_back (*task_it);

              double wcrt = wcrt_algorithm (local_tasks);

              // remember result with minimum worst-case response time
              if ((best_result.wcrt < .0) || (wcrt < best_result.wcrt))
                {
                  best_result.task = *task_it;
                  best_result.processor = proc_it->first;
                  best_result.wcrt = wcrt;
                }
            }

          // use best schedule entry
          schedule_results.push_back (best_result);

          local_schedule.erase (best_result.processor);
        }

      // rank replicas based on the non-failure case response time
      Utilization_Ranking ranking_algorithm;

      unsigned int scheduled_replicas = 
        ranking_algorithm (schedule_results,
                           schedule_);

      // if all tasks are schedulable
      if (scheduled_replicas == schedule_results.size ())
        {
          // schedule all schedule entries for replicas
          add_schedule_results (schedule_results, schedule_);
        }
      else
        {
          // if not add to unschedulable list
          ScheduleProgress pg = {*task_it, scheduled_replicas};
          unschedulable_.push_back (pg);
        }
    }

  return transform_schedule (schedule_);
}

const SCHEDULE & 
FTRMFF_Worstfit_Algorithm::schedule () const
{
  return schedule_;
}
