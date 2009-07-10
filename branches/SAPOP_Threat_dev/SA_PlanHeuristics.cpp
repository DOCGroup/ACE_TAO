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
#include "SA_WorkingPlan.h"
#include <algorithm>
#include <vector>

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



TaskChoiceList SA_TaskStrategy::choose_task_fair (Condition open_cond)
{

  TaskSet tasks;

 // if(this->planner_->get_working_plan()->get_all_insts().size() > 8){
		
//	  TaskChoiceList s;
	//  s.clear();
//	  return s;
 // }

  tasks = this->planner_->get_satisfying_tasks (open_cond);

    
  // Add tasks to map with EU (to sort).
  std::multimap<EUCalc, TaskID> task_map;
  task_map.clear ();
  for (TaskSet::iterator iter = tasks.begin (); iter != tasks.end (); iter++)
  {
    task_map.insert (std::make_pair (
      this->planner_->get_task_future_eu (*iter), *iter));
  }

  std::multimap<TaskID, TaskInstID> tasks_to_insts;

  SA_WorkingPlan* working_plan = (SA_WorkingPlan*)this->planner_->get_working_plan();

  InstToTaskMap inst_task_map = working_plan->get_task_insts();

  for(InstToTaskMap::iterator it = inst_task_map.begin(); 
	  it != inst_task_map.end(); it++){

		  tasks_to_insts.insert(std::pair<TaskID, TaskInstID>(it->second, it->first));
  }

  // Add tasks to list in reverse order of map (highest EU first).
  TaskChoiceList task_list;
  task_list.clear ();

  std::vector<SortTaskByTime> tasks_with_existing_instances;

  for (std::multimap<EUCalc, TaskID>::reverse_iterator iter = task_map.rbegin ();
    iter != task_map.rend (); iter++)
  {

	  if(tasks_to_insts.lower_bound(iter->second) == tasks_to_insts.upper_bound(iter->second)){
	  
		TaskChoice task_choice;
		task_choice.choice = NEW_INST;
		task_choice.task_id = iter->second;
		task_choice.task_inst_id = -2;

		task_list.push_back(task_choice);




	  }else{

		  SortTaskByTime to_sort;
		  to_sort.task_id = iter->second;
		  to_sort.last_instance = 0;

		  for(std::multimap<TaskID, TaskInstID>::iterator it = tasks_to_insts.lower_bound(iter->second); it != tasks_to_insts.upper_bound(iter->second);
			  it++){

			TaskChoice task_choice;
			task_choice.choice = REUSE_INST;
			task_choice.task_id = it->first;
			task_choice.task_inst_id = it->second;
			task_list.push_back(task_choice);

			to_sort.note_instance(it->second);
		  }
	
		  if(!(iter->second == 20 && this->planner_->init_added)){
			tasks_with_existing_instances.push_back(to_sort);
		 }
	  
	  }
  }

  std::sort(tasks_with_existing_instances.begin(), tasks_with_existing_instances.end());

  for(std::vector<SortTaskByTime>::iterator it = tasks_with_existing_instances.begin(); it != tasks_with_existing_instances.end(); it++){
		
		

		TaskChoice task_choice;
		task_choice.choice = NEW_INST;
		task_choice.task_id = it->task_id;
		task_choice.task_inst_id = -2;
		task_list.push_back(task_choice);
  }


  return task_list;


};




// Choose the (ordering of) task(s) to satisfy an open condition.
TaskChoiceList SA_TaskStrategy::choose_task (Condition open_cond)
{
  TaskSet tasks = this->planner_->get_satisfying_tasks (open_cond);

  if(this->planner_->init_added){
    tasks.erase(20);
  }
    
  // Add tasks to map with EU (to sort).
  std::multimap<EUCalc, TaskID> task_map;
  task_map.clear ();
  for (TaskSet::iterator iter = tasks.begin (); iter != tasks.end (); iter++)
  {
    task_map.insert (std::make_pair (
      this->planner_->get_task_future_eu (*iter), *iter));
  }

  std::multimap<TaskID, TaskInstID> tasks_to_insts;

  SA_WorkingPlan* working_plan = (SA_WorkingPlan*)this->planner_->get_working_plan();

  InstToTaskMap inst_task_map = working_plan->get_task_insts();

  for(InstToTaskMap::iterator it = inst_task_map.begin(); 
	  it != inst_task_map.end(); it++){

		  tasks_to_insts.insert(std::pair<TaskID, TaskInstID>(it->second, it->first));
  }

  // Add tasks to list in reverse order of map (highest EU first).
  TaskChoiceList task_list;
  task_list.clear ();

  for (std::multimap<EUCalc, TaskID>::reverse_iterator iter = task_map.rbegin ();
    iter != task_map.rend (); iter++)
  {

	  for(std::multimap<TaskID, TaskInstID>::iterator it = tasks_to_insts.lower_bound(iter->second); it != tasks_to_insts.upper_bound(iter->second);
		  it++){

	    TaskChoice task_choice;
		task_choice.choice = REUSE_INST;
		task_choice.task_id = it->first;
		task_choice.task_inst_id = it->second;

		task_list.push_back(task_choice);
	  }
	  
	TaskChoice task_choice;
	task_choice.choice = NEW_INST;
	task_choice.task_id = iter->second;
	task_choice.task_inst_id = -2;

	task_list.push_back(task_choice);
	//  task_list.push_back(
//	  task_list.choice_list.push_back (std::pair<int, int>(TASK_INST_ID, iter->second));
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
