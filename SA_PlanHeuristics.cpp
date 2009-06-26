// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SA_PlanHeuristics.cpp
 *
 * This file contains the implementations of concrete classes,
 * which implement plan heuristic strategies.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#include "SA_POP_Types.h"
#include "SA_PlanHeuristics.h"
#include "Planner.h"

using namespace SA_POP;

// Constructor.
SA_CondStrategy::SA_CondStrategy (SA_POP::Planner *planner)
: CondStrategy (planner)
{
  // Nothing to do.
};

// Destructor.
SA_CondStrategy::~SA_CondStrategy (void)
{
  // Nothing to do.
};

// Choose the next open condition to satisfy.
Condition SA_CondStrategy::choose_cond (const OpenCondMap &open_conds)
{
  if (open_conds.empty ())
    throw "SA_POP::SA_CondStrategy::choose_cond (): Empty condition list.";

  // Return first data condition.
  for (OpenCondMap::const_iterator iter = open_conds.begin ();
    iter != open_conds.end (); iter++)
  {
    if (iter->first.kind == SA_POP::DATA)
      return iter->first;
  }

  // If no data conditions, just return first condition.
  return open_conds.front().first;
};



// Constructor.
SA_TaskStrategy::SA_TaskStrategy (SA_POP::Planner *planner)
: TaskStrategy (planner)
{
  // Nothing to do.
};

// Destructor.
SA_TaskStrategy::~SA_TaskStrategy (void)
{
  // Nothing to do.
};

// Choose the (ordering of) task(s) to satisfy an open condition.
TaskList SA_TaskStrategy::choose_task (Condition open_cond)
{
  TaskSet tasks = this->planner_->get_satisfying_tasks (open_cond);

  if(this->planner_->init_added){
    tasks.erase(20);
  }
    
  // Add tasks to map with EU (to sort).
  std::map<EUCalc, TaskID> task_map;
  task_map.clear ();
  for (TaskSet::iterator iter = tasks.begin (); iter != tasks.end (); iter++)
  {
    task_map.insert (std::make_pair (
      this->planner_->get_task_future_eu (*iter), *iter));
  }

  // Add tasks to list in reverse order of map (highest EU first).
  TaskList task_list;
  task_list.clear ();
  for (std::map<EUCalc, TaskID>::reverse_iterator iter = task_map.rbegin ();
    iter != task_map.rend (); iter++)
  {
    task_list.push_back (iter->second);
  }
  return task_list;
};



// Constructor.
SA_ImplStrategy::SA_ImplStrategy (SA_POP::Planner *planner)
: ImplStrategy (planner)
{
  // Nothing to do.
};

// Destructor.
SA_ImplStrategy::~SA_ImplStrategy (void)
{
  // Nothing to do.
};

// Choose the (ordering of) task implementation(s) for a task instance.
TaskImplList SA_ImplStrategy::choose_impl (TaskInstID task_inst)
{
  TaskID task = this->planner_->get_task_from_inst (task_inst);
  TaskImplSet impls = this->planner_->get_all_impls (task);

  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  // Order by resource impact score.

  TaskImplList impl_list;
  impl_list.clear ();
  for (TaskImplSet::iterator iter = impls.begin ();
    iter != impls.end (); iter++)
  {
    impl_list.push_back (*iter);
  }

  return impl_list;
};
