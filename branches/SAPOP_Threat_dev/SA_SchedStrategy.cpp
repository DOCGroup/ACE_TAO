// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SA_SchedStrategy.cpp
 *
 * This file contains the SA_SchedStrategy concrete class implementation, which
 * implements a SchedStrategy for the high-level scheduling algorithm.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#include "SA_POP_Types.h"
#include "SA_SchedStrategy.h"
#include "Planner.h"
#include <list>
#include <set>
#include <fstream>
using namespace SA_POP;

// Constructor.
SA_SchedStrategy::SA_SchedStrategy (SA_POP::Planner *planner)
: SchedStrategy (planner),
cur_decision_pt_ (SA_POP::SA_SchedStrategy::SCHEDULE_DECISION),
cur_seq_num_ (1),
resolve_sched_cmd_ (0),
adj_min_times_cmd_ (0),
adj_max_times_cmd_ (0)
{
  // Nothing to do.
};

// Destructor.
SA_SchedStrategy::~SA_SchedStrategy (void)
{
  // Nothing to do.
};

void SA_SchedStrategy::reset()
{
  cur_decision_pt_ = SA_POP::SA_SchedStrategy::SCHEDULE_DECISION;
  cur_seq_num_ = 1;
}


// Set command prototypes to use in scheduling.
void SA_SchedStrategy::set_commands (ResolveSchedOrderCmd *resolve_sched_cmd,
  AdjustMinTimesCmd *adj_min_times_cmd,
  AdjustMaxTimesCmd *adj_max_times_cmd)
{
  this->has_cmds_ = true;

  if (resolve_sched_cmd)
    this->resolve_sched_cmd_ = resolve_sched_cmd;
  else if (this->resolve_sched_cmd_ == 0)
    this->has_cmds_ = false;

  if (adj_min_times_cmd)
    this->adj_min_times_cmd_ = adj_min_times_cmd;
  else if (this->adj_min_times_cmd_ == 0)
    this->has_cmds_ = false;

  if (adj_max_times_cmd)
    this->adj_max_times_cmd_ = adj_max_times_cmd;
  else if (this->adj_max_times_cmd_ == 0)
    this->has_cmds_ = false;
};
// Get command ID to use for next command.
CommandID SA_SchedStrategy::get_next_cmd_id (void)
{
  CommandID temp;
  temp.step = this->planner_->cur_command_id().step;
  temp.decision_pt = this->cur_decision_pt_;
  temp.seq_num = this->cur_seq_num_;

  this->cur_seq_num_++;
  return temp;
};

// Recursively satisfy all scheduling constraints (and continue
// satisfaction of open conditions by recursive call back to planning).
bool SA_SchedStrategy::satisfy_sched (TaskInstID task_inst)
{
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  //return this->planner_->recurse_plan ();
  // Adjust schedule.
	double THRESHOLD_CRIT = 0.8;
	// Get the current command id to backtrack to.
	CommandID cur_cmd_id = this->planner_->cur_command_id();
	this->cur_seq_num_=1;

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
	

  if(!this->planner_->recurse_plan ()){
	this->planner_->undo_through(cur_cmd_id);
	return false;
  }else{
	return true;
  }
};

// Satisfy fully instantiated plan.
bool SA_SchedStrategy::satisfy_full_sched ()
{
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
  return true;
};
/// Calculate the min and max levels for the consumer of a task instance
void SA_SchedStrategy::calculate_levels(TaskInstID task_inst)
{
	const TaskInstSet *unranked = this->planner_->get_prec_insts(task_inst,UNRANKED);	
	TimeWindow start_win = this->planner_->get_start_window(task_inst);
	TimeWindow end_win = this->planner_->get_end_window(task_inst);
	ResourceMap rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(task_inst));
	// Calculate the levels for all the resources this task instance uses
	for(ResourceMap::iterator rm_iter = rm.begin();rm_iter!=rm.end();rm_iter++)
	{
//		Sum of the resource usages of all the consumers that have to occur before the consumer of this task instance
		ResourceValue Q=this->planner_->get_capacity(rm_iter->first);
		// The max and min levels at the consumer
		ResourceValue cons_min=Q,cons_max=Q;
		for(TaskInstSet::const_iterator iter=unranked->begin();iter!=unranked->end();iter++)
		{
			// get the resourse usage by the task instance in the unranked set
			ResourceMap temp_rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(*iter));
			ResourceMap::iterator temp_rm_iter = temp_rm.find(rm_iter->first);
			// Check whether it uses the resource that we are calculating the level for
			if(temp_rm_iter==temp_rm.end()) continue;
			TimeWindow temp_start = this->planner_->get_start_window(*iter);
			TimeWindow temp_end = this->planner_->get_end_window(*iter);
			//The consumer of *iter can be executed before and its producer after the consumer of task_inst
			if(start_win.second==NULL_TIME || temp_start.first<start_win.second)
			{
				std::cout<<"The consumer of "<<*iter<<" can be executed before and its producer after the consumer of "<<task_inst<<std::endl;
				cons_min-=temp_rm_iter->second;
			}
			// The consumer of *iter has to executed before the consumer of task_inst
			if(temp_start.second!=NULL_TIME && temp_start.second<start_win.first)
			{
				std::cout<<"THe consumer of "<<*iter<<" has to executed before the consumer of "<<task_inst<<std::endl;
				cons_max-=temp_rm_iter->second;
			}
		}
		LevelMap temp;
		MinimumLevels::iterator iter1=this->min_cons_levels_.find(rm_iter->first);
		// Insert dummy levels if this is a new task instance
		if(iter1==this->min_cons_levels_.end())
		{
			this->min_cons_levels_.insert(std::make_pair(rm_iter->first,temp));
			this->max_cons_levels_.insert(std::make_pair(rm_iter->first,temp));
		}
		ResourceValue simul_level=0;
		const TaskInstSet *simul = this->planner_->get_prec_insts(task_inst,SIMUL);
		// Calculate the usage of all the task instances simultaneous to this one
		for(TaskInstSet::const_iterator iter2 = simul->begin();iter2!=simul->end();iter2++)
		{
			ResourceMap temp_rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(*iter2));
			ResourceMap::iterator temp_rm_iter = temp_rm.find(rm_iter->first);
			if(temp_rm_iter==temp_rm.end()) continue;
//			std::cout<<"calculating for "<<task_inst<<" adding "<<temp_rm_iter->second<<" for task inst "<<*iter2<<std::endl;
			simul_level+=temp_rm_iter->second;
		}
			this->min_cons_levels_.find(rm_iter->first)->second.insert(std::make_pair(task_inst,-1));
			this->max_cons_levels_.find(rm_iter->first)->second.insert(std::make_pair(task_inst,-1));
		cons_min-=rm_iter->second;
		// erase any previous levels and add the new one.
		this->min_cons_levels_.find(rm_iter->first)->second.erase(this->min_cons_levels_.find(rm_iter->first)->second.find(task_inst));
		this->max_cons_levels_.find(rm_iter->first)->second.erase(this->max_cons_levels_.find(rm_iter->first)->second.find(task_inst));
		this->min_cons_levels_.find(rm_iter->first)->second.insert(std::make_pair(task_inst,cons_min-simul_level));
		this->max_cons_levels_.find(rm_iter->first)->second.insert(std::make_pair(task_inst,cons_max));
	}
}
/// Get the Criticality of the consumer of a task instance
Criticality SA_SchedStrategy::crit(TaskInstID task_inst)
{
	double INF_CRIT = 0.2;
	const TaskInstSet *unranked = this->planner_->get_prec_insts(task_inst,UNRANKED);	
	TimeWindow start_win = this->planner_->get_start_window(task_inst);
	ResourceMap rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(task_inst));
	Criticality max_crit=std::make_pair(NULL_RESOURCE_ID,0);
	if(start_win.second==NULL_TIME) 
	{
		ResourceMap::iterator rm_iter = rm.begin();
		for(;rm_iter!=rm.end();rm_iter++)
		{
			ResourceValue lack = this->min_cons_levels_.find(rm_iter->first)->second.find(task_inst)->second;
				if(lack<0) break;
		}
		// If this doesn't lack, then its not critical.
		if(rm_iter==rm.end()) return max_crit;
		else return std::make_pair(rm.begin()->first,INF_CRIT);
	}
	// Calculate the criticality and return the maximum criticality.
	for(ResourceMap::iterator rm_iter = rm.begin();rm_iter!=rm.end();rm_iter++)
	{
		ResourceValue lack = this->min_cons_levels_.find(rm_iter->first)->second.find(task_inst)->second;
		if(lack>0) continue;
		ResourceValue Q = this->planner_->get_capacity(rm_iter->first);
		double crit_value = (0-lack)/(Q*(start_win.second-start_win.first+1));
		if(max_crit.second<crit_value) max_crit=std::make_pair(rm_iter->first,crit_value);
	}
	return max_crit;
}
///Get the commitment of an ordering
double SA_SchedStrategy::commit(TaskInstID first_task_inst, TaskInstID second_task_inst)
{
	double INF_COMMIT = 0.2;
	TimeValue first_min,first_max,second_min,second_max;
	TimeWindow first_end_win = this->planner_->get_end_window(first_task_inst);
	TimeWindow second_start_win = this->planner_->get_start_window(second_task_inst);
	first_min=first_end_win.first;
	first_max=first_end_win.second;
	second_min=second_start_win.first;
	second_max=second_start_win.second;
	if(first_max==NULL_TIME || second_max==NULL_TIME) return INF_COMMIT;
	// Calculate the commitment
	double A,B,C_min,C_max,delta_min,delta_max;
	if(first_min>second_min) delta_min=1;
	else delta_min=0;
	if(first_max>second_max) delta_max=1;
	else delta_max=0;
	A = (second_max-second_min+1)*(first_max-first_min+1);
	B = (first_max-second_min+1)*(first_max-second_min+1)/2;
	C_min = (first_min-second_min)*(first_min-second_min)/2;
	C_max = (first_max-second_max)*(first_max-second_max)/2;
	return (B-delta_min*C_min-delta_max*C_max)/A;
}
/// Perform the search till the ctiricality goes below a certain value
bool SA_SchedStrategy::search(double min_crit)
{
	TaskInstSet all = this->planner_->get_all_insts();
	// Task Instances at maximum criticality
	TaskInstSet maxcritical;
	Criticality max_crit;
	max_crit = std::make_pair(NULL_RESOURCE_ID,0);
	TaskInstID task_inst=NULL_TASK_INST_ID;
	for(TaskInstSet::iterator iter=all.begin();iter!=all.end();iter++)
	{
		Criticality crit_cons = this->crit(*iter);
		std::cout<<"Criticality of "<<*iter<<" consumer: "<<crit_cons.second<<std::endl;
		if(crit_cons.second>=max_crit.second)
		{
			max_crit=crit_cons;
			maxcritical.insert(*iter);
		}
	}
	if(max_crit.second<=min_crit) return true;
	for(TaskInstSet::iterator iter=maxcritical.begin(); iter!=maxcritical.end(); iter++)
	{
		task_inst=*iter;
		TimeWindow start_win = this->planner_->get_start_window(task_inst);
		const TaskInstSet *unranked = this->planner_->get_prec_insts(task_inst,UNRANKED);
		std::set<std::pair<double,TaskInstID> > probables;
		// get all the probable task instances that can be ordered before this task instance
		for(TaskInstSet::const_iterator iter=unranked->begin();iter!=unranked->end();iter++)
		{
			TimeWindow temp_end = this->planner_->get_end_window(*iter);
			if(temp_end.first!=NULL_TIME && (start_win.second==NULL_TIME || temp_end.first<start_win.second))
			{
				ResourceMap temp_rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(*iter));
				ResourceMap::iterator temp_rm_iter = temp_rm.find(max_crit.first);
				if(temp_rm_iter==temp_rm.end()) continue;
				double rel_crit=this->commit(*iter,task_inst)/(double)temp_rm_iter->second;
				if(rel_crit!=0) probables.insert(std::make_pair(rel_crit,*iter));
			}
		}
		for(std::set<std::pair<double,TaskInstID> >::iterator iter=probables.begin();iter!=probables.end();iter++)
		{
			ResolveSchedOrderCmd *sched_cmd = static_cast<ResolveSchedOrderCmd *> (this->resolve_sched_cmd_->clone ());
			sched_cmd->set_id(this->get_next_cmd_id());
			sched_cmd->set_task_insts(iter->second,task_inst);
			this->planner_->add_command(sched_cmd);
			if(this->planner_->try_next(sched_cmd->get_id()))
			{
				TaskInstSet affected_insts = sched_cmd->get_affected_insts();
				for(TaskInstSet::iterator iter2=affected_insts.begin();iter2!=affected_insts.end();iter2++)
				{
					this->calculate_levels(*iter2);
				}
				if(search(min_crit)) return true;
			}
			this->planner_->undo_command(sched_cmd->get_id());
		}
	}
	return false;
}

///Precedence Link Based Balance Constraint Propogation 5.3.3 (labourie paper)
bool SA_SchedStrategy::prec_balance_prop (TaskInstID task_inst)
{
	std::cout<<"Doing Precedence balance prop for "<<task_inst<<std::endl;
	CommandID cur_cmd_id = this->planner_->cur_command_id();
	const TaskInstSet *unranked = this->planner_->get_prec_insts(task_inst,UNRANKED);	
	TimeWindow start_win = this->planner_->get_start_window(task_inst);
	ResourceMap rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(task_inst));
	// Iterate over all resources that this task instance uses
	for(ResourceMap::iterator rm_iter = rm.begin();rm_iter!=rm.end();rm_iter++)
	{
		//Sum of the resource usages of all the consumers that have to occur before the consumer of this task instance
		ResourceValue level_before=this->planner_->get_capacity(rm_iter->first);
		TaskInstSet producers_unranked;
		for(TaskInstSet::const_iterator iter=unranked->begin();iter!=unranked->end();iter++)
		{
			TimeWindow temp_start = this->planner_->get_start_window(*iter);
			TimeWindow temp_end = this->planner_->get_end_window(*iter);
			if(temp_start.second!=NULL_TIME && temp_start.second<start_win.first)
			{
				std::cout<<"The consumer of "<<*iter<<" has to executed before"<<std::endl;
				if(temp_end.first!=NULL_TIME && (start_win.second==NULL_TIME || temp_end.first<start_win.second)) producers_unranked.insert(*iter);
				//The consumer has to be executed before this task instance
				ResourceMap temp_rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(*iter));
				ResourceMap::iterator temp_rm_iter = temp_rm.find(rm_iter->first);
				if(temp_rm_iter==temp_rm.end()) continue;
				level_before-=temp_rm_iter->second;
			}
		}
		if(level_before<0)
		{
			ResourceValue production=0;
			for(TaskInstSet::const_iterator iter=producers_unranked.begin();iter!=producers_unranked.end();iter++)
			{
				TimeWindow temp_end = this->planner_->get_end_window(*iter);
				for(TaskInstSet::const_iterator iter2=producers_unranked.begin();iter2!=producers_unranked.end();iter2++)
				{
					if(*iter2==*iter) continue;
					TimeWindow temp_end2 = this->planner_->get_end_window(*iter2);
					if(temp_end.second==NULL_TIME || temp_end2.first<temp_end.second)
						production+=this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(*iter2)).find(rm_iter->first)->second;
				}
				if(production+level_before<0)
				{
					ResolveSchedOrderCmd *sched_cmd = static_cast<ResolveSchedOrderCmd *> (this->resolve_sched_cmd_->clone ());
					sched_cmd->set_id(this->get_next_cmd_id());
					sched_cmd->set_task_insts(*iter,task_inst);
					this->planner_->add_command(sched_cmd);
					if(!this->planner_->try_next(sched_cmd->get_id()))
					{
						this->planner_->undo_through(cur_cmd_id);
						return false;
					}
				}
			}

		}
	}
	return true;
}
///Time Based Balance Constraint Propogation 5.3.3 (labourie paper)
bool SA_SchedStrategy::time_balance_prop (TaskInstID task_inst)
{
	std::cout<<"Doing time balance prop for "<<task_inst<<std::endl;
	CommandID cur_cmd_id = this->planner_->cur_command_id();
	const TaskInstSet *unranked = this->planner_->get_prec_insts(task_inst,UNRANKED);	
	TimeWindow start_win = this->planner_->get_start_window(task_inst);
	TimeWindow end_win = this->planner_->get_end_window(task_inst);
	ResourceMap rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(task_inst));
	// Iterate over all resources used by this task instance
	for(ResourceMap::iterator rm_iter = rm.begin();rm_iter!=rm.end();rm_iter++)
	{
		//Sum of the resource usages of all the consumers that have to occur before the consumer of this task instance
		ResourceValue level_before=this->planner_->get_capacity(rm_iter->first);
		//All the producers that can occur before this task instance
		std::list<TaskInstID> producers_unranked;
		for(TaskInstSet::const_iterator iter=unranked->begin();iter!=unranked->end();iter++)
		{
			TimeWindow temp_start = this->planner_->get_start_window(*iter);
			TimeWindow temp_end = this->planner_->get_end_window(*iter);
			if(temp_start.second!=NULL_TIME && temp_start.second<start_win.first)
			{
				std::cout<<"THe consumer of "<<*iter<<" has to execute before"<<std::endl;
				//The consumer has to be executed before this task instance
				ResourceMap temp_rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(*iter));
				ResourceMap::iterator temp_rm_iter = temp_rm.find(rm_iter->first);
				if(temp_rm_iter==temp_rm.end()) continue;
				level_before-=temp_rm_iter->second;
				if(temp_end.first!=NULL_TIME && (start_win.second==NULL_TIME || temp_end.first<start_win.second))
				{
					// This task instance can be pushed before the task instance
					std::cout<<"Task inst "<<*iter<<" can cause trouble"<<std::endl;
					if(producers_unranked.empty())
						producers_unranked.push_front(*iter);	
					else
					{
						std::list<TaskInstID>::iterator iter2=producers_unranked.begin();
						for(;iter2!=producers_unranked.end();iter2++)
						{	
							TimeWindow temp2_end = this->planner_->get_end_window(*iter2);
							if(temp2_end.first>temp_end.first) break;
						}
						producers_unranked.insert(iter2,*iter);
					}
				}
				else continue;
			}
		}
		std::cout<<"The producers_unranked list is: "<<std::endl;
		for(std::list<TaskInstID>::iterator iter2=producers_unranked.begin();iter2!=producers_unranked.end();iter2++)
		{
			std::cout<<*iter2<<" ";
		}

		if(level_before<0)
		{
			std::cout<<"The level_before is negative"<<std::endl;
			ResourceValue level_prod=0;
			std::list<TaskInstID>::iterator iter2=producers_unranked.begin();
			for(;iter2!=producers_unranked.end();iter2++)
			{
				TimeWindow temp2_end = this->planner_->get_end_window(*iter2);
				ResourceMap temp_rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(*iter2));
				level_prod+=temp_rm.find(rm_iter->first)->second;
				if(level_prod>=0-level_before) break;
			}
			if(iter2==producers_unranked.end()) return false;
			TimeWindow temp2_end = this->planner_->get_end_window(*iter2);
			std::cout<<"greater than "<<*iter2<<std::endl;
			if(temp2_end.first>start_win.first)
			{
				AdjustMinTimesCmd *adj_min_times_cmd = static_cast<AdjustMinTimesCmd *> (this->adj_min_times_cmd_->clone ());
				adj_min_times_cmd->set_id(this->get_next_cmd_id());
				adj_min_times_cmd->set_times(task_inst,temp2_end.first,temp2_end.first+this->planner_->get_duration(task_inst));
				this->planner_->add_command(adj_min_times_cmd);
				if(!this->planner_->try_next(adj_min_times_cmd->get_id()))
				{
					this->planner_->undo_through(cur_cmd_id);
					return false;
				}
			}
		}
	}
	// Do the same as above for the producer of this task. This is more likely to give results
	for(ResourceMap::iterator rm_iter = rm.begin();rm_iter!=rm.end();rm_iter++)
	{
		//Sum of the resource usages of all the consumers that have to occur before the producer of this task instance but there produces doesn't
		ResourceValue level_before=this->planner_->get_capacity(rm_iter->first)-rm_iter->second;
		const TaskInstSet *simul = this->planner_->get_prec_insts(task_inst,SIMUL);
		for(TaskInstSet::const_iterator iter=simul->begin();iter!=simul->end();iter++)
		{
			std::cout<<"calculating for "<<*iter<<std::endl;
			ResourceMap temp_rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(*iter));
			ResourceMap::iterator temp_rm_iter = temp_rm.find(rm_iter->first);
			//This task instance doesn't use this resource
			if(temp_rm_iter==temp_rm.end()) continue;
			level_before-=temp_rm_iter->second;
		}
		//All the producers that can occur before this task instance
		std::list<TaskInstID> producers_unranked;
		for(TaskInstSet::const_iterator iter=unranked->begin();iter!=unranked->end();iter++)
		{
		    std::cout<<"checking out "<<*iter<<std::endl;
			TimeWindow temp_start = this->planner_->get_start_window(*iter);
			TimeWindow temp_end = this->planner_->get_end_window(*iter);
			if(end_win.first==NULL_TIME ||(temp_start.second!=NULL_TIME && temp_start.second<end_win.first))
			{
				//The consumer has to be executed before this task instance
				ResourceMap temp_rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(*iter));
				ResourceMap::iterator temp_rm_iter = temp_rm.find(rm_iter->first);
				//This task instance doesn't use this resource
				if(temp_rm_iter==temp_rm.end()) continue;
				level_before-=temp_rm_iter->second;
				if(temp_end.first!=NULL_TIME && (end_win.second==NULL_TIME || temp_end.first<end_win.second))
				{
					if(producers_unranked.empty())
						producers_unranked.push_front(*iter);	
					else
					{
						std::list<TaskInstID>::iterator iter2=producers_unranked.begin();
						for(;iter2!=producers_unranked.end();iter2++)
						{
							TimeWindow temp2_end = this->planner_->get_end_window(*iter2);
							if(temp2_end.first>temp_end.first) break;
						}
						producers_unranked.insert(iter2,*iter);
					}
				}
				else continue;
			}
		}
		if(level_before<0)
		{
			ResourceValue level_prod=0;
			std::list<TaskInstID>::iterator iter2=producers_unranked.begin();
			for(;iter2!=producers_unranked.end();iter2++)
			{
				TimeWindow temp2_end = this->planner_->get_end_window(*iter2);
				ResourceMap temp_rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(*iter2));
				level_prod+=temp_rm.find(rm_iter->first)->second;
				if(level_prod>=0-level_before) break;
			}
			if(iter2==producers_unranked.end()) return false;
			TimeWindow temp2_end = this->planner_->get_end_window(*iter2);
			if(end_win.first!=NULL_TIME && temp2_end.first>end_win.first)
			{
				AdjustMinTimesCmd *adj_min_times_cmd = static_cast<AdjustMinTimesCmd *> (this->adj_min_times_cmd_->clone ());
				adj_min_times_cmd->set_id(this->get_next_cmd_id());
				adj_min_times_cmd->set_times(task_inst,temp2_end.first-this->planner_->get_duration(task_inst),temp2_end.first);
				this->planner_->add_command(adj_min_times_cmd);
				if(!this->planner_->try_next(adj_min_times_cmd->get_id()))
				{
					this->planner_->undo_through(cur_cmd_id);
					return false;
				}
			}
		}
	}
	return true;
}

//Do the energy precedence calculations for this taskinstance.
//If nothing changes go to the orderings after this and perform the calculations
//otherwise, repeat these for this task instance till the windows stop changing and stop in this branch.
bool SA_SchedStrategy::energy_prop (TaskInstID task_inst)
{
  std::cout<<"Doing energy precedence calculations for "<<task_inst<<std::endl;
  CommandID cur_cmd_id = this->planner_->cur_command_id();
  AdjustMinTimesCmd *adj_min_times_cmd = static_cast<AdjustMinTimesCmd *> (this->adj_min_times_cmd_->clone ());
  AdjustMaxTimesCmd *adj_max_times_cmd = static_cast<AdjustMaxTimesCmd *> (this->adj_max_times_cmd_->clone ());
  TimeWindow start_win = this->planner_->get_start_window(task_inst);
  TimeWindow end_win = this->planner_->get_end_window(task_inst);
  TimeValue min_start = start_win.first;
  TimeValue max_end = end_win.second;
  ResourceMap rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(task_inst));
  bool min_times_changed=false,max_times_changed=false;
  for(ResourceMap::iterator rm_iter = rm.begin();rm_iter!=rm.end();rm_iter++)
  {
	//Based on all task instances before task_inst
	ResourceValue Q = this->planner_->get_capacity(rm_iter->first);
	std::cout<<"The capacity of resource:"<<rm_iter->first<<" is "<<Q<<std::endl;
	double first_before=-1,first_after=-1,second_before=0,second_after=0;
	const TaskInstSet *before = this->planner_->get_prec_insts(task_inst,BEFORE);
	const TaskInstSet *after = this->planner_->get_prec_insts(task_inst,AFTER);
	if(!before->empty())
	{
		for(TaskInstSet::const_iterator iter = before->begin();iter!=before->end();iter++)
		{
			ResourceMap temp_rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(*iter));
			ResourceMap::iterator temp_rm_iter = temp_rm.find(rm_iter->first);
			if(temp_rm_iter==temp_rm.end()) continue;
			second_before += this->planner_->get_duration(*iter) * (temp_rm_iter->second) / Q;
			TimeWindow temp_start = this->planner_->get_start_window(*iter);
			if(first_before==-1 || first_before>temp_start.first) first_before = temp_start.first;
		}
		if(first_before!=-1 && min_start<first_before+second_before) min_start=first_before+second_before;
	}
	//Based on all task instances after task_inst
	if(!after->empty() && end_win.second!=NULL_TIME)
	{
		for(TaskInstSet::const_iterator iter = after->begin();iter!=after->end();iter++)
		{
			ResourceMap temp_rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(*iter));
			ResourceMap::iterator temp_rm_iter = temp_rm.find(rm_iter->first);
			if(temp_rm_iter==temp_rm.end()) continue;
			TimeValue temp_dur = this->planner_->get_duration(*iter);
			ResourceValue temp_rval = temp_rm_iter->second;
			second_after += (double)temp_dur * (double)temp_rval / Q;
			TimeWindow temp_end = this->planner_->get_end_window(*iter);
			if(temp_end.second==NULL_TIME)
			{
				max_end=end_win.second;
				break;
			}
			if(first_after==-1 || first_after<temp_end.second) first_after = temp_end.second;
		}
		if(first_after!=-1 && max_end>first_after-second_after) max_end=first_after-second_after;
	}
    std::cout<<"After calculations for resource "<<rm_iter->first<<" start_min="<<min_start<<" and end_max="<<max_end<<std::endl;
  }
  if(min_start!=start_win.first)
  {
    min_times_changed=true;
    adj_min_times_cmd->set_times(task_inst,min_start,min_start+this->planner_->get_duration(task_inst));
    adj_min_times_cmd->set_id(this->get_next_cmd_id());
    this->planner_->add_command (adj_min_times_cmd);
    if(!this->planner_->try_next(adj_min_times_cmd->get_id())) {
		this->planner_->undo_through(cur_cmd_id);
		return false;
    }
  }
  if(max_end!=end_win.second)
  {
    max_times_changed=true;
    adj_max_times_cmd->set_times(task_inst,max_end-this->planner_->get_duration(task_inst),max_end);
    adj_max_times_cmd->set_id(this->get_next_cmd_id());
    this->planner_->add_command (adj_max_times_cmd);
    if(!this->planner_->try_next(adj_max_times_cmd->get_id())) {
      this->planner_->undo_through(cur_cmd_id);
      return false;
    }
  }
  if(min_times_changed || max_times_changed) 
  {
    //Since the time windows have changed, some unranked task instances have gone to before or after.
    //Thus, we need to do the energy precedence calculations for this task instance again.
    //Also we have to do the energy precedence calculations for the affected task instances.
    if(!this->energy_prop(task_inst)) {
		this->planner_->undo_through(cur_cmd_id);
		return false;
    }
  }
	// Propogate the energy precedence constraint calculations to before and after tasks
	// in only one direction
  //TaskInstSet aft = this->planner_->after_orderings(task_inst);
	//TaskInstSet bef = this->planner_->before_orderings(task_inst);
  const TaskInstSet *aft = this->planner_->get_prec_insts(task_inst,AFTER);
  const TaskInstSet *bef = this->planner_->get_prec_insts(task_inst,BEFORE);

  for(TaskInstSet::const_iterator iter = aft->begin();iter!=aft->end();iter++)
    {
      if(!this->energy_prop_after(*iter))
      {
		this->planner_->undo_through(cur_cmd_id);
		return false;
      }
    }

    for(TaskInstSet::const_iterator iter = bef->begin();iter!=bef->end();iter++)
    {
      if(!this->energy_prop_before(*iter))
      {
      this->planner_->undo_through(cur_cmd_id);
      return false;
      }
    }
  return true;
}
/// Do the uni directional energy precedence propogation in the after direction
bool SA_SchedStrategy::energy_prop_after (TaskInstID task_inst)
{
  std::cout<<"Doing energy precedence after calculations for "<<task_inst<<std::endl;
  CommandID cur_cmd_id = this->planner_->cur_command_id();
  AdjustMinTimesCmd *adj_min_times_cmd = static_cast<AdjustMinTimesCmd *> (this->adj_min_times_cmd_->clone ());
  TimeWindow start_win = this->planner_->get_start_window(task_inst);
  TimeWindow end_win = this->planner_->get_end_window(task_inst);
  TimeValue min_start = start_win.first;
  ResourceMap rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(task_inst));
  bool min_times_changed=false;
  for(ResourceMap::iterator rm_iter = rm.begin();rm_iter!=rm.end();rm_iter++)
  {
	//Based on all task instances before task_inst
	ResourceValue Q = this->planner_->get_capacity(rm_iter->first);
	std::cout<<"The capacity of resource:"<<rm_iter->first<<" is "<<Q<<std::endl;
	double first_before=-1,second_before=0;
	const TaskInstSet *before = this->planner_->get_prec_insts(task_inst,BEFORE);
	if(!before->empty())
	{
		for(TaskInstSet::const_iterator iter = before->begin();iter!=before->end();iter++)
		{
			ResourceMap temp_rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(*iter));
			ResourceMap::iterator temp_rm_iter = temp_rm.find(rm_iter->first);
			if(temp_rm_iter==temp_rm.end()) continue;
			second_before += this->planner_->get_duration(*iter) * (temp_rm_iter->second) / Q;
			TimeWindow temp_start = this->planner_->get_start_window(*iter);
			if(first_before==-1 || first_before>temp_start.first) first_before = temp_start.first;
		}
		if(first_before!=-1 && min_start<first_before+second_before) min_start=first_before+second_before;
	}
    std::cout<<"After after calculations for resource "<<rm_iter->first<<" start_min="<<min_start<<std::endl;
  }
  if(min_start!=start_win.first)
  {
    min_times_changed=true;
    adj_min_times_cmd->set_times(task_inst,min_start,min_start+this->planner_->get_duration(task_inst));
    adj_min_times_cmd->set_id(this->get_next_cmd_id());
    this->planner_->add_command (adj_min_times_cmd);
    if(!this->planner_->try_next(adj_min_times_cmd->get_id())) {
		this->planner_->undo_through(cur_cmd_id);
		return false;
    }
  }
  if(min_times_changed) 
  {
    //Since the time windows have changed, some unranked task instances have gone to before or after.
    //Thus, we need to do the energy precedence calculations for this task instance again.
    //Also we have to do the energy precedence calculations for the affected task instances.
    if(!this->energy_prop_after(task_inst)) {
		this->planner_->undo_through(cur_cmd_id);
		return false;
    }
  }
	//TaskInstSet aft = this->planner_->after_orderings(task_inst);
	//TaskInstSet bef = this->planner_->before_orderings(task_inst);
  const TaskInstSet *aft = this->planner_->get_prec_insts(task_inst,AFTER);

  for(TaskInstSet::const_iterator iter = aft->begin();iter!=aft->end();iter++)
    {
      if(!this->energy_prop_after(*iter))
      {
		this->planner_->undo_through(cur_cmd_id);
		return false;
      }
    }
  return true;
}
/// Do the uni directional energy precedence propogation in the after direction
bool SA_SchedStrategy::energy_prop_before (TaskInstID task_inst)
{
  std::cout<<"Doing before energy precedence calculations for "<<task_inst<<std::endl;
  CommandID cur_cmd_id = this->planner_->cur_command_id();
  AdjustMaxTimesCmd *adj_max_times_cmd = static_cast<AdjustMaxTimesCmd *> (this->adj_max_times_cmd_->clone ());
  TimeWindow start_win = this->planner_->get_start_window(task_inst);
  TimeWindow end_win = this->planner_->get_end_window(task_inst);
  TimeValue min_start = start_win.first;
  TimeValue max_end = end_win.second;
  ResourceMap rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(task_inst));
  bool max_times_changed=false;
  for(ResourceMap::iterator rm_iter = rm.begin();rm_iter!=rm.end();rm_iter++)
  {
	//Based on all task instances before task_inst
	ResourceValue Q = this->planner_->get_capacity(rm_iter->first);
	std::cout<<"The capacity of resource:"<<rm_iter->first<<" is "<<Q<<std::endl;
	double first_after=-1,second_after=0;
	const TaskInstSet *after = this->planner_->get_prec_insts(task_inst,AFTER);
	if(!after->empty() && end_win.second!=NULL_TIME)
	{
		for(TaskInstSet::const_iterator iter = after->begin();iter!=after->end();iter++)
		{
			ResourceMap temp_rm = this->planner_->get_all_resources(this->planner_->get_task_impl_from_inst(*iter));
			ResourceMap::iterator temp_rm_iter = temp_rm.find(rm_iter->first);
			if(temp_rm_iter==temp_rm.end()) continue;
			TimeValue temp_dur = this->planner_->get_duration(*iter);
			ResourceValue temp_rval = temp_rm_iter->second;
			second_after += (double)temp_dur * (double)temp_rval / Q;
			TimeWindow temp_end = this->planner_->get_end_window(*iter);
			if(temp_end.second==NULL_TIME)
			{
				max_end=end_win.second;
				break;
			}
			if(first_after==-1 || first_after<temp_end.second) first_after = temp_end.second;
		}
		if(first_after!=-1 && max_end>first_after-second_after) max_end=first_after-second_after;
	}
    std::cout<<"After before calculations for resource "<<rm_iter->first<<" and end_max="<<max_end<<std::endl;
  }
  if(max_end!=end_win.second)
  {
    max_times_changed=true;
    adj_max_times_cmd->set_times(task_inst,max_end-this->planner_->get_duration(task_inst),max_end);
    adj_max_times_cmd->set_id(this->get_next_cmd_id());
    this->planner_->add_command (adj_max_times_cmd);
    if(!this->planner_->try_next(adj_max_times_cmd->get_id())) {
      this->planner_->undo_through(cur_cmd_id);
      return false;
    }
  }
  if(max_times_changed) 
  {
    //Since the time windows have changed, some unranked task instances have gone to before or after.
    //Thus, we need to do the energy precedence calculations for this task instance again.
    //Also we have to do the energy precedence calculations for the affected task instances.
    if(!this->energy_prop_before(task_inst)) {
		this->planner_->undo_through(cur_cmd_id);
		return false;
    }
  }
	//TaskInstSet bef = this->planner_->before_orderings(task_inst);
  const TaskInstSet *bef = this->planner_->get_prec_insts(task_inst,BEFORE);

    for(TaskInstSet::const_iterator iter = bef->begin();iter!=bef->end();iter++)
    {
      if(!this->energy_prop_before(*iter))
      {
      this->planner_->undo_through(cur_cmd_id);
      return false;
      }
    }
  return true;
}
