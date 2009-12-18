// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  Exp_EU_SchedStrategy.cpp
 *
 * This file contains the Exp_EU_SchedStrategy concrete class implementation, which
 * implements a SchedStrategy for "roadblock" scheduling that only updates the
 * precedence graph without any other scheduling and which can randomly choose a
 * "roadblock" of a set of tasks in the working plan.  It considers any working
 * plan with the "roadblock" set of tasks to be unschedulable and any other
 * working plan to be schedulable.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#include "SA_POP_Types.h"

//#include "SA_SchedStrategy.h"
#include "Exp_EU_SchedStrategy.h"

#include "SA_WorkingPlan.h"
#include "Planner.h"
#include <list>
#include <set>
#include <fstream>
#include <algorithm>
#include <vector>
using namespace SA_POP;

// Constructor.
Exp_EU_SchedStrategy::Exp_EU_SchedStrategy (SA_POP::Planner *planner)
: SA_SchedStrategy (planner)
{
  // Nothing to do.
};

// Destructor.
Exp_EU_SchedStrategy::~Exp_EU_SchedStrategy (void)
{
  // Nothing to do.
};


// Recursively satisfy all scheduling constraints (and continue
// satisfaction of open conditions by recursive call back to planning).
bool Exp_EU_SchedStrategy::satisfy_sched (TaskInstID task_inst)
{



  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  //return this->planner_->recurse_plan ();
  // Adjust schedule.
	double THRESHOLD_CRIT = 0.8;
	// Get the current command id to backtrack to.
	CommandID cur_cmd_id = this->planner_->cur_command_id();
	this->cur_seq_num_=1;

  SA_POP_DEBUG (SA_POP_DEBUG_MINIMAL, "NO SCHEDULING with ROADBLOCK SCHEDULER");
  // Don't do any propagation for roadblock scheduler.
/*
  // Do the energy propogation for this task instance
	// This function automatically does this for the task instances before and after it.
  if(!this->energy_prop(task_inst))
  {


	  this->planner_->undo_through(cur_cmd_id);
	  return false;
  }

  // Do the balance propogation related to time windows for this task instance 
  // and those unranked with respect to it
  if(!this->time_balance_prop(task_inst))
  {
	  this->planner_->undo_through(cur_cmd_id);
	  return false;
  }
	const TaskInstSet *unranked = this->planner_->get_prec_insts(task_inst,UNRANKED);
	for(TaskInstSet::const_iterator iter = unranked->begin();iter!=unranked->end();iter++)
	{
		if(!this->time_balance_prop(*iter))
		{
			this->planner_->undo_through(cur_cmd_id);
			return false;
		}
	}
  // Do the balance propogation related to precedence graph links for this task instance 
  // and those unranked with respect to it
 // if(!this->prec_balance_prop(task_inst))
 // {
	//  this->planner_->undo_through(cur_cmd_id);
	//  return false;
 // }
 // unranked = this->planner_->get_prec_insts(task_inst,UNRANKED);
	//for(TaskInstSet::const_iterator iter = unranked->begin();iter!=unranked->end();iter++)
	//{
	//	if(!this->prec_balance_prop(*iter))
	//	{
	//		this->planner_->undo_through(cur_cmd_id);
	//		return false;
	//	}
	//}

	// Recalculate all the levels
	TaskInstSet all = this->planner_->get_all_insts();
	for(TaskInstSet::iterator iter=all.begin();iter!=all.end();iter++)
		this->calculate_levels(*iter);

	// start the search
  if(!search(THRESHOLD_CRIT))
  {
	  this->planner_->undo_through(cur_cmd_id);
	  return false;
  }
*/
  

  if(!this->planner_->recurse_plan ()){
	  this->planner_->undo_through(cur_cmd_id);
	  return false;
  }else{
	  return true;
  }
};

// Satisfy fully instantiated plan.
bool Exp_EU_SchedStrategy::satisfy_full_sched ()
{
  // Just use normal satisfy schedule for "roadblock" scheduler.
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  return true;
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

  // Don't do normal scheduling for "roadblock" scheduler.
/*

  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  //return true;
  // Adjust schedule.
	CommandID cur_cmd_id = this->planner_->cur_command_id();
	this->cur_seq_num_=1;
  if(!search(0))
  {
	  this->planner_->undo_through(cur_cmd_id);
	  return false;
  }
	
  SA_WorkingPlan* working_plan_tmp = (SA_WorkingPlan*)this->planner_->get_working_plan();

 // working_plan_tmp->get_precedence_graph()
  PrecedenceSet befores = (working_plan_tmp->get_precedence_graph().find(BEFORE)->second);
  PrecedenceSet afters = (working_plan_tmp->get_precedence_graph().find(AFTER)->second);
  PrecedenceSet simuls = (working_plan_tmp->get_precedence_graph().find(SIMUL)->second);
  PrecedenceSet unrankeds = (working_plan_tmp->get_precedence_graph().find(UNRANKED)->second);

  //double current_execution_time = 0
  //vector currently executing actions


  std::vector<TaskInstEndTimeSet> executing_tasks;

  double current_execution_time = 0;

  int total_tasks = befores.size();
  int completed_tasks = 0;

  while(completed_tasks < total_tasks){

	  for(PrecedenceSet::iterator it = befores.begin(); it != befores.end();){
		
		  PrecedenceSet::iterator prev_it = it++;
		  if((befores.find(prev_it->first))->second.empty()){
			
			  TaskInstEndTimeSet new_execute;
			  new_execute.inst = prev_it->first;

			
			  if(prev_it->first == INIT_TASK_INST_ID){
				  new_execute.end_time = current_execution_time + 0;
			  }else{
				  new_execute.end_time = current_execution_time + this->planner_->get_impl(working_plan_tmp->get_impl_id(prev_it->first))
					  ->get_duration();
			  }

			  executing_tasks.push_back(new_execute);
			  befores.erase(prev_it);
		  }
	  }

	  std::sort(executing_tasks.begin(), executing_tasks.end());

	  TaskInstEndTimeSet next_done = *executing_tasks.begin();
	  current_execution_time = next_done.end_time;

	  std::list<TaskInstEndTimeSet> to_remove;

	  for(std::vector<TaskInstEndTimeSet>::iterator it3 = executing_tasks.begin(); 
		  it3 != executing_tasks.end();){

			if(it3->end_time != current_execution_time){	 
			  break;
			 }
		  std::vector<TaskInstEndTimeSet>::iterator prev_it = it3++;

		  std::cout<<"Task "<<prev_it->inst<<" finishes at time "<<prev_it->end_time<<std::endl;
		  completed_tasks++;

		  for(PrecedenceSet::iterator it2 = befores.begin(); it2 != befores.end(); it2++){
			  it2->second.erase(prev_it->inst);
		  }
		  befores.erase(prev_it->inst);
		  to_remove.push_front(*prev_it);
	//	  executing_tasks.erase(prev_it);
	  }

	  for( std::list<TaskInstEndTimeSet>::iterator it3 = to_remove.begin(); it3 != to_remove.end(); it3++){
		  std::vector<TaskInstEndTimeSet>::iterator it4;
		  for(it4 = executing_tasks.begin(); it4 != executing_tasks.end(); it4++){
			  if(it4->inst == it3->inst){
				break;
			  }
		  }
		  executing_tasks.erase(it4);
	  }
  }

  //while #actions finished != total actions
  //find all actions with no more befores
  //add to currently executing actions sorted by current_time + their duration
  //take action(s if there are multiple that end at the same time) off currently executing actions
  //
  //remove them from the befores of all other actions
  //remove them from befores
  //current execution time = when they finish
*/





  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  // DON'T KNOW WHO'S CODE THIS IS OR WHY IT WAS COMMENTED OUT...
  // PLEASE REMOVE CODE OR ADD COMMENTS TO EXPLAIN.
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  /*
  std::list<TaskInstID> execute_this_time;

  int time = 0;
  while(!befores.empty()){

	  execute_this_time.clear();

	  for(PrecedenceSet::iterator it = befores.begin(); it != befores.end();){
		
		  PrecedenceSet::iterator prev_it = it++;

		  if((befores.find(prev_it->first))->second.empty()){
			
			  execute_this_time.push_front(prev_it->first);

			  befores.erase(prev_it);
		  }
	  }

	
	  for(std::list<TaskInstID>::iterator it = execute_this_time.begin(); it != execute_this_time.end(); it++){
	
		  std::cout<<"Task "<<(*it)<<" can execute at time "<<time<<std::endl;

		  for(PrecedenceSet::iterator it2 = befores.begin(); it2 != befores.end(); it2++){
				it2->second.erase(*it);
		  }
	  }
	  time++;
  }
  */

  return true;
};

