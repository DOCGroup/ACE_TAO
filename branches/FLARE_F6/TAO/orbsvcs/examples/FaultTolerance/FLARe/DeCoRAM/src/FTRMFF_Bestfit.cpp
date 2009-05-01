// -*- C++ -*-

//=============================================================================
/**
 *  @file    FTRMFF_Bestfit.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <sstream>
#include "FTRMFF_Bestfit.h"

#include "Forward_Ranking_Scheduler.h"
#include "Packing_Scheduler.h"

FTRMFF_Bestfit::FTRMFF_Bestfit (const std::string & algorithm)
  : algorithm_ (algorithm)
{
}

FTRMFF_Bestfit::~FTRMFF_Bestfit ()
{
}

FTRMFF_Output
FTRMFF_Bestfit::operator () (const FTRMFF_Input & input)
{
  FTRMFF_Bestfit_Algorithm algorithm (input.processors,
                                              input.backup_count,
                                              algorithm_);

  FTRMFF_Output output;
  output.schedule = algorithm (input.tasks);
  output.unscheduled_tasks = algorithm.get_unschedulable ();

  DBG_OUT (algorithm.schedule ());

  return output;
}

FTRMFF_Bestfit_Algorithm::FTRMFF_Bestfit_Algorithm (
  const PROCESSOR_LIST & processors,
  unsigned int consistency_level,
  const std::string & scheduler)
  : consistency_level_ (consistency_level),
    scheduler_ (processors,
                consistency_level_)
{
}

FTRMFF_Bestfit_Algorithm::~FTRMFF_Bestfit_Algorithm ()
{
}

SCHEDULING_MAP
FTRMFF_Bestfit_Algorithm::operator () (const TASK_LIST & tasks)
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
      // create the right amount of backup replica tasks
      TASK_LIST task_group = create_ranked_tasks (*it,
                                                  consistency_level_);

      // schedule the tasks of one application
      for (TASK_LIST::iterator task_it = task_group.begin ();
           task_it != task_group.end ();
           ++task_it)
        {
          ScheduleResult r = scheduler_ (*task_it);
          if (r.wcrt <= .0)
            {
              ScheduleProgress pg = {*task_it, 
                                     task_it->rank - consistency_level_ + 1};
              unschedulable_.push_back (pg);
              break;
            }
          else
            {
              scheduler_.update_schedule (r);
            }

          TRACE (*task_it << " -> " << r.processor);
        }
    }

  return transform_schedule (scheduler_.schedule ());
}

SCHEDULE_PROGRESS_LIST
FTRMFF_Bestfit_Algorithm::get_unschedulable ()
{
  return unschedulable_;
}

SCHEDULE
FTRMFF_Bestfit_Algorithm::schedule () const
{
  return scheduler_.schedule ();
}
