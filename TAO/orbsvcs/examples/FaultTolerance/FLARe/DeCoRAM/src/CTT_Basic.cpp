// -*- C++ -*-

//=============================================================================
/**
 *  @file    CTT_Basic.cpp
 *
 *  $Id$
 *
 *  @author  Friedhelm Wolf (fwolf@dre.vanderbilt.edu)
 */
//=============================================================================

#include "CTT_Basic.h"
#include <iostream>
#include <numeric>
#include <cmath>

CTT_Basic::~CTT_Basic ()
{
}

double
CTT_Basic::operator () (const TASK_LIST & tasks)
{
  TRACE ("begin");
  TASK_LIST scheduled_tasks;
  Task t;
  double wcrt = 0;

  for (TASK_LIST::const_iterator next_task = tasks.begin ();
       next_task != tasks.end (); 
       ++next_task)
    {
      t = *next_task;

      wcrt = this->worst_case_response_time_check (t, scheduled_tasks);

      if (wcrt > .0)
        scheduled_tasks.push_back (t);
      else break;
    }

  return wcrt;
}

struct AddExecutionTimes : public std::binary_function <double,
							Task,
							double>
{
  double operator () (double time, const Task & task)
  {
    return time + task.execution_time;
  }
};

struct WCET_Heuristic_Step : public std::binary_function <double,
							  Task,
							  double>
{
  WCET_Heuristic_Step (double R)
    : R_ (R)
  {
  }

  double operator () (double time, const Task & task)
  {
    return time + 
      ceil (R_ / task.period) * 
      task.execution_time;
  }
private:
  double R_;
};


double
CTT_Basic::worst_case_response_time_check (const Task & task, 
                                           const TASK_LIST & higher_prio_tasks)
{
  // std::cout << "WCRT for " << task
  //          << " and " << higher_prio_tasks
  //          << std::endl;

  // R0 = C1 + C2 + ... + Cn
  double R = std::accumulate (higher_prio_tasks.begin (),
			      higher_prio_tasks.end (),
			      task.execution_time,
			      AddExecutionTimes ());

  // R_k = C1 + sum_j (ceil (R_k-1 / T_j) * Cj)
  double R_copy = 0;
  double_equal equals;

  while (R <= task.period)
    {
      R_copy = R;
      R = std::accumulate (higher_prio_tasks.begin (),
                           higher_prio_tasks.end (),
                           task.execution_time,
                           WCET_Heuristic_Step (R));

      // std::cout << "R = " << R << std::endl;
      
      if (equals (R_copy, R))
        break;
    }

  if (R <= task.period)
    return R;
  else
    return .0;
}
