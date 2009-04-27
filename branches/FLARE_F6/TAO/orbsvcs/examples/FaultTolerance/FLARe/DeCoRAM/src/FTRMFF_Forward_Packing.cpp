// -*- C++ -*-

//=============================================================================
/**
 *  @file    FTRMFF_Forward_Packing.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <sstream>
#include "FTRMFF_Forward_Packing.h"
#include "Packing_Scheduler.h"

FTRMFF_Forward_Packing::~FTRMFF_Forward_Packing ()
{
}

FTRMFF_Output
FTRMFF_Forward_Packing::operator () (const FTRMFF_Input & input)
{
  FTRMFF_Forward_Packing_Algorithm algorithm (input.processors,
                                              input.backup_count);

  FTRMFF_Output output;
  output.schedule = algorithm (input.tasks);
  output.unscheduled_tasks = algorithm.get_unschedulable ();

  DBG_OUT (algorithm.schedule ());

  return output;
}

FTRMFF_Forward_Packing_Algorithm::FTRMFF_Forward_Packing_Algorithm (
  const PROCESSOR_LIST & processors,
  unsigned int consistency_level)
  : schedule_ (create_schedule (processors)),
    consistency_level_ (consistency_level),
    scheduler_ (schedule_, consistency_level)
{
}

FTRMFF_Forward_Packing_Algorithm::~FTRMFF_Forward_Packing_Algorithm ()
{
}

SCHEDULING_MAP
FTRMFF_Forward_Packing_Algorithm::operator () (const TASK_LIST & tasks)
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
      TASK_LIST task_group = this->create_tasks (*it);

      // schedule the tasks of one application
      for (TASK_LIST::iterator task_it = task_group.begin ();
           task_it != task_group.end ();
           ++task_it)
        {
          if (scheduler_ (*task_it).wcrt <= .0)
            {
              ScheduleProgress pg = {*task_it, 
                                     task_it->rank - consistency_level_ + 1};
              unschedulable_.push_back (pg);
              break;
            }
        }
    }

  return transform_schedule (schedule_);
}

SCHEDULE_PROGRESS_LIST
FTRMFF_Forward_Packing_Algorithm::get_unschedulable ()
{
  return unschedulable_;
}

const SCHEDULE & 
FTRMFF_Forward_Packing_Algorithm::schedule () const
{
  return schedule_;
}

TASK_LIST 
FTRMFF_Forward_Packing_Algorithm::create_tasks (const Task & task)
{
  TASK_LIST tasks;

  for (unsigned int i = 0; i <= consistency_level_; ++i)
    {
      Task t = task;
      t.rank = i;
      if (t.rank > 0)
        {
          t.role = BACKUP;

          std::stringstream ss;
          ss << t.name << "_" << i;
          t.name = ss.str ();
        }
      else
        {
          t.role = PRIMARY;
        }

      tasks.push_back (t);
    }

  return tasks;
}
