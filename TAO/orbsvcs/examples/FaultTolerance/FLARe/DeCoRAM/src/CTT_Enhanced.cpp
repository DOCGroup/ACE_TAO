// -*- C++ -*-

//=============================================================================
/**
 *  @file    CTT_Optimized.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include <numeric>
#include <cmath>
#include "CTT_Enhanced.h"

CTT_Enhanced::~CTT_Enhanced ()
{
}

double
CTT_Enhanced::operator () (const TASK_LIST & tasks)
{
  //TRACE ("(" << tasks << ")");

  // copy tasks for local modifications
  TASK_LIST scheduled_tasks = tasks;

  // take last element and use it for schedulability analysis.
  // It is assumed that all other tasks are schedulable
  Task t = scheduled_tasks [scheduled_tasks.size () - 1];
  scheduled_tasks.pop_back ();

  // return the wcrt for this task
  return this->worst_case_response_time_check (t, scheduled_tasks);
}

double
CTT_Enhanced::worst_case_response_time_check (
  const Task & task, 
  const TASK_LIST & higher_prio_tasks)
{
  // determine execution time based on type of application
  double exec_time = (task.role == PRIMARY ? 
                      task.execution_time : 
                      task.sync_time);

  // R0 = C1 + C2 + ... + Cn
  double R = std::accumulate (higher_prio_tasks.begin (),
			      higher_prio_tasks.end (),
			      exec_time,
			      AddExecutionTimes ());

  // R_k = C1 + sum_j (ceil (R_k-1 / T_j) * Cj)
  double R_copy = 0;
  double_equal equals;

  while (R <= task.period)
    {
      R_copy = R;
      R = std::accumulate (higher_prio_tasks.begin (),
                           higher_prio_tasks.end (),
                           exec_time,
                           WCET_Heuristic_Step (R));

      if (equals (R_copy, R))
        break;
    }

  if (R <= task.period)
    return R;
  else
    return .0;
}

double
AddExecutionTimes::operator () (double time, const Task & task)
{
  // determine execution time based on type of application
  double exec_time = (task.role == PRIMARY ? 
                      task.execution_time : 
                      task.sync_time);

  return time + exec_time;
}

WCET_Heuristic_Step::WCET_Heuristic_Step (double R)
  : R_ (R)
{
}

double
WCET_Heuristic_Step::operator () (double time, const Task & task)
{
  // determine execution time based on type of application
  double exec_time = (task.role == PRIMARY ? 
                      task.execution_time : 
                      task.sync_time);

  return time + 
    ceil (R_ / task.period) * 
    exec_time;
}
