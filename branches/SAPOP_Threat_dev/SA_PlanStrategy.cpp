// -*- C++ -*-
// $Id$

//=============================================================================
/**
* @file  SA_PlanStrategy.cpp
*
* This file contains the implementation of the SA_PlanStrategy concrete class,
* which implements a PlanStrategy for use with spreading activation networks
* and precedence graphs.
*
* @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
*/
//=============================================================================

#include <set>
#include "SA_POP_Types.h"
#include "SA_PlanStrategy.h"
#include "Planner.h"
#include "PlanHeuristics.h"
#include "PlanCommands.h"
#include "SA_WorkingPlan.h"
#include <fstream>




using namespace SA_POP;

// Constructor.
SA_PlanStrategy::SA_PlanStrategy (SA_POP::Planner *planner,
								  CondStrategy *cond_choice, TaskStrategy *task_choice,
								  ImplStrategy *impl_choice)
								  : PlanStrategy (planner),
								  has_cmds_ (false),
								  cur_task_ (SA_POP::NULL_TASK_ID),
								  cur_task_inst_ (SA_POP::NULL_TASK_ID),
								  cur_step_ (0),
								  cur_decision_pt_ (SA_POP::SA_PlanStrategy::TASK_DECISION),
								  cur_seq_num_ (1),
								  cond_choice_ (cond_choice),
								  task_choice_ (task_choice),
								  impl_choice_ (impl_choice),
								  add_conds_cmd_ (0),
								  rmv_conds_cmd_ (0),
								  add_threats_cmd_ (0),
								  rmv_threats_cmd_ (0),
								  add_task_cmd_ (0),
								  assoc_impl_cmd_ (0),
								  resolve_threat_cmd_ (0),
								  open_conds_(SA_POP::FrontBack::BACK, SA_POP::FrontBack::FRONT)
{
	this->add_conds_cmd_ = new SA_AddOpenCondsCmd (this);
	this->rmv_conds_cmd_ = new SA_RemoveOpenCondsCmd (this);
	this->add_threats_cmd_ = new SA_AddOpenThreatsCmd (this);
	this->rmv_threats_cmd_ = new SA_RemoveOpenThreatsCmd (this);
	this->open_conds_.clear ();
	this->open_threats_.clear ();
};

// Destructor.
SA_PlanStrategy::~SA_PlanStrategy (void)
{
	// Delete heuristic strategies.
	delete this->cond_choice_;
	delete this->task_choice_;
	delete this->impl_choice_;
};

void SA_PlanStrategy::reset()
{
	has_cmds_ = false;
	cur_task_ = SA_POP::NULL_TASK_ID;
	cur_task_inst_ = SA_POP::NULL_TASK_ID;
	cur_step_ = 0;
	cur_decision_pt_ = SA_POP::SA_PlanStrategy::TASK_DECISION;
	cur_seq_num_ = 1;
	this->add_conds_cmd_ = new SA_AddOpenCondsCmd (this);
	this->rmv_conds_cmd_ = new SA_RemoveOpenCondsCmd (this);
	this->add_threats_cmd_ = new SA_AddOpenThreatsCmd (this);
	this->rmv_threats_cmd_ = new SA_RemoveOpenThreatsCmd (this);
	this->open_conds_.clear ();
	this->open_threats_.clear ();
};

// Set command prototypes to use in planning.
void SA_PlanStrategy::set_commands (AddTaskCmd *add_task_cmd,
									AssocTaskImplCmd *assoc_impl_cmd,
									ResolveCLThreatCmd *resolve_threat_cmd)
{
	this->has_cmds_ = true;

	if (add_task_cmd)
		this->add_task_cmd_ = add_task_cmd;
	else if (this->add_task_cmd_ == 0)
		this->has_cmds_ = false;

	if (assoc_impl_cmd)
		this->assoc_impl_cmd_ = assoc_impl_cmd;
	else if (this->assoc_impl_cmd_ == 0)
		this->has_cmds_ = false;

	if (resolve_threat_cmd)
		this->resolve_threat_cmd_ = resolve_threat_cmd;
	else if (this->resolve_threat_cmd_ == 0)
		this->has_cmds_ = false;
};

// Set goals.
void SA_PlanStrategy::set_goals (GoalMap goals)
{
	for (SA_POP::GoalMap::iterator iter = goals.begin ();
		iter != goals.end (); iter++)
	{
		Condition cond;
		cond.id = iter->first;
		if (iter->second > 0)
			cond.value = true;
		else
			cond.value = false;
		//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
		// Get type of condition.
		cond.kind = SA_POP::SYSTEM;

		this->open_conds_.insert (std::make_pair (cond, SA_POP::GOAL_TASK_INST_ID));
	}
};

// Get command ID to use for next command.
CommandID SA_PlanStrategy::get_next_cmd_id (void)
{
	CommandID temp;
	temp.step = this->cur_step_;
	temp.decision_pt = this->cur_decision_pt_;
	temp.seq_num = this->cur_seq_num_;

	this->cur_seq_num_++;
	return temp;
};

// Recursively satisfy all open conditions (including recursive
// scheduling constraint satisfaction through call back).
bool SA_PlanStrategy::satisfy_open_conds (void)
{
	// If all open conditions have been satisfied, then return true for success.
	if (this->open_conds_.empty ())
		return this->planner_->full_sched();

	//Note: change this number to limit the size of the final plan.  Set true for unlimited
	//!(this->planner_->get_working_plan()->get_all_insts().size() > 12)
	if(!(this->planner_->get_working_plan()->get_all_insts().size() > 8)){
		// Increment step counter.
		this->cur_step_++;

		// Set decision point and reset sequence number for commands.
		this->cur_decision_pt_ = SA_PlanStrategy::TASK_DECISION;
		this->cur_seq_num_ = 1;

		// Variable for preconditions of current task.
		CondSet preconds;

		// Choose an open condition to satisfy.
		Condition open_cond = this->cond_choice_->choose_cond_suspension (this->open_conds_);
		SA_WorkingPlan* working_plan_tmp = (SA_WorkingPlan*)this->planner_->get_working_plan();
		if(working_plan_tmp->is_null_condition(open_cond)){
			return false;
		}

		// Choose task to satisfy open condition (actually an ordered list of
		// tasks to try), passing command to planner to be executed next.
		AddTaskCmd *add_task_cmd = this->satisfy_cond (open_cond);

		TaskInstSet old_satisfied_insts = this->satisfied_insts;

		this->satisfied_insts = add_task_cmd->get_satisfied_tasks();

		TaskInstID prev_cur_inst = this->cur_task_inst_;

		// Try tasks until one yields a complete plan or all have been tried.
		while (this->planner_->try_next (add_task_cmd->get_id ())) {

			this->added_links = add_task_cmd->get_causal_insertions();

			// Get current task and task instance.
			this->cur_task_ = add_task_cmd->get_task ();

			this->cur_task_inst_ = add_task_cmd->get_task_inst ();
			// Remove open condition.
			CommandID rmv_cond_cmd_id = this->rmv_open_cond (open_cond, add_task_cmd->get_satisfied_tasks());

			TaskID stored_task = this->cur_task_;

			bool to_fail = false;

			// Add preconditions of this task of we didn't reuse the task instance.
			CommandID add_preconds_cmd_id;
			if(!add_task_cmd->inst_exists())
			{
				preconds = this->planner_->get_unsat_preconds (this->cur_task_);
				add_preconds_cmd_id = 
					this->add_open_conds (preconds, this->cur_task_inst_);
			}

			//Do not execute if the condition has been noted before and not helped
			std::pair<bool, CommandID> return_data = this->store_map.should_continue(add_task_cmd->get_id(), add_task_cmd->get_condition(), 
				stored_task, this->open_conds_, this->planner_->get_working_plan()->get_task_insts());

			if(return_data.first)
			{
				// Try to satisfy threats and continue recursive planning.
				if (this->satisfy_everything())
					return true;

			}

			this->store_map.undo_binding(add_task_cmd->get_id(), add_task_cmd->get_condition(), stored_task);

			std::ostringstream debug_text;
			debug_text<<" the task instance being deleted is "<<add_task_cmd->get_task_inst()<<std::endl;
			SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, debug_text.str ());
			debug_text.str("");

			// Undo addition of preconditions from this task if we didn't reuse the task instance.
			if(!add_task_cmd->inst_exists())
				this->planner_->undo_command (add_preconds_cmd_id);

			// Undo removal of open condition.
			this->planner_->undo_command (rmv_cond_cmd_id);

		}

		this->cur_task_inst_ = prev_cur_inst;

		SA_POP_DEBUG (SA_POP_DEBUG_NORMAL, "Backtracking to previous step...");
		// Undo addition of task.

		this->satisfied_insts = old_satisfied_insts;
		this->planner_->undo_command (add_task_cmd->get_id ());



		// Decrement step.
		this->cur_step_--;

	}

	// No task could satisfy open condition, so return failure.
	return false;
};

bool SA_PlanStrategy::satisfy_everything(){

	//for each implementation
	this->cur_decision_pt_ = SA_PlanStrategy::IMPL_DECISION;
	this->cur_seq_num_ = 1;

	AssocTaskImplCmd *assoc_impl_cmd;
	TaskImplList impl_list;

	// Choose a task implementation.
	assoc_impl_cmd =
		static_cast<AssocTaskImplCmd *> (this->assoc_impl_cmd_->clone ());
	if(!this->planner_->inst_exists(this->cur_task_inst_)) impl_list = this->impl_choice_->choose_impl (this->cur_task_inst_);
	else impl_list.push_back(this->planner_->get_impl_id(this->cur_task_inst_));
	assoc_impl_cmd->set_id (this->get_next_cmd_id ());
	assoc_impl_cmd->set_assoc (this->cur_task_inst_, impl_list);
	this->planner_->add_command (assoc_impl_cmd);

	assoc_impl_cmd->set_satisfied_insts(this->satisfied_insts);

	assoc_impl_cmd->set_added_links(this->added_links);


	this->cur_task_inst_ = assoc_impl_cmd->get_task_inst ();

	while (this->planner_->try_next (assoc_impl_cmd->get_id ())) 
	{

		//Move this code to the threat resolution sequence
		// Set decision point and reset sequence number for commands.
		this->cur_decision_pt_ = SA_PlanStrategy::THREAT_DECISION;
		this->cur_seq_num_ = 1;


		//Deal with threats

		//Actually build the list of threats
		this->planner_->generate_all_threats();



		// Add causal link threats to open threats.
		bool are_threats = !(this->planner_->get_all_threats().empty ());
		CommandID add_threats_cmd_id;
		if (are_threats)
			add_threats_cmd_id = 
			this->add_open_threats (this->planner_->get_all_threats ());

		if(this->get_next_threat_resolution()){
			return true;
		}
		else{

			this->cur_decision_pt_ = SA_PlanStrategy::IMPL_DECISION;
		}

		SA_POP_DEBUG(SA_POP_DEBUG_NORMAL, "Backtracking from task assoc...");
		// Undo addition of causal link threats from this task.
		if (are_threats)
			this->planner_->undo_command (add_threats_cmd_id);


	}

	//Undo the AssocImplCmd

	planner_->undo_command(assoc_impl_cmd->get_id());

	this->cur_decision_pt_ = SA_PlanStrategy::IMPL_DECISION;

	return false;
}
bool SA_PlanStrategy::satisfy_schedule(void){


	// Set decision point and reset sequence number for commands.
	this->cur_decision_pt_ = SA_PlanStrategy::SCHEDULE_DECISION;
	this->cur_seq_num_ = 1;

	// Try to schedule and recursively continue planning.
	if (this->planner_->recurse_sched (this->cur_task_inst_))
		return true;

	return false;
}

bool SA_PlanStrategy::get_next_threat_resolution(){

	// Choose an open threat to satisfy and remove from open threats.

	if(open_threats_.empty()){

		return satisfy_schedule();
	}
	this->cur_decision_pt_ = SA_PlanStrategy::THREAT_DECISION;
	CLThreat threat = *this->open_threats_.begin ();
	CommandID rmv_threat_cmd_id = this->rmv_open_threat (threat);

	this->cur_decision_pt_ = SA_PlanStrategy::THREAT_DECISION;

	// Create threat resolution command and add to planner.
	ResolveCLThreatCmd *resolve_threat_cmd =
		static_cast<ResolveCLThreatCmd *> (this->resolve_threat_cmd_->clone ());
	resolve_threat_cmd->set_id (this->get_next_cmd_id ());
	resolve_threat_cmd->set_threat (threat);
	this->planner_->add_command (resolve_threat_cmd);

	while(this->planner_->try_next(resolve_threat_cmd->get_id())){

		if (this->get_next_threat_resolution ())
			return true;

	}

	this->cur_decision_pt_ = SA_PlanStrategy::THREAT_DECISION;
	this->planner_->undo_command (resolve_threat_cmd->get_id ());

	this->open_threats_.insert(threat);

	// Undo removal of open threat.
	this->planner_->undo_command (rmv_threat_cmd_id);

	return false;

}


// Get a PlanCommand prototype for adding open conditions,
// which works on this strategy.
AddOpenCondsCmd *SA_PlanStrategy::get_AddOpenCondsCmd (void)
{
	return static_cast<AddOpenCondsCmd *> (this->add_conds_cmd_->clone ());
};

// Get a PlanCommand prototype for removing open conditions,
// which works on this strategy.
RemoveOpenCondsCmd *SA_PlanStrategy::get_RemoveOpenCondsCmd (void)
{
	return static_cast<RemoveOpenCondsCmd *> (this->rmv_conds_cmd_->clone ());
};

// Get an command prototype for adding causal link threats, which works on
// this strategy.
AddOpenThreatsCmd *SA_PlanStrategy::get_AddOpenThreatsCmd (void)
{
	return static_cast<AddOpenThreatsCmd *> (this->add_threats_cmd_->clone ());
};

// Get a PlanCommand prototype for removing causal link threats,
// which works on this strategy.
RemoveOpenThreatsCmd *SA_PlanStrategy::get_RemoveOpenThreatsCmd (void)
{
	return static_cast<RemoveOpenThreatsCmd *> (this->rmv_threats_cmd_->clone ());
};

// Execute a command to add open conditions to planning.
void SA_PlanStrategy::execute (SA_AddOpenCondsCmd *cmd)
{
	SA_WorkingPlan* working_plan_tmp = (SA_WorkingPlan*)this->planner_->get_working_plan();

	for (CondSet::iterator iter = cmd->conds_.begin ();
		iter != cmd->conds_.end (); iter++)
	{
		this->open_conds_.insert (std::make_pair (*iter, cmd->task_inst_));

		CausalLink closest_on_path = working_plan_tmp->clink_on_path(*iter, cmd->task_inst_);
		if(!working_plan_tmp->is_null_link(closest_on_path)){
			working_plan_tmp->suspend_condition(*iter, cmd->task_inst_, closest_on_path);

		}
		cmd->link_suspended_on.insert(std::pair<Condition, CausalLink>(*iter, closest_on_path));
	}
};

// Undo a command to add open conditions to planning.
void SA_PlanStrategy::undo (SA_AddOpenCondsCmd *cmd)
{
	std::ostringstream debug_text;

	SA_WorkingPlan* work_plan_tmp = (SA_WorkingPlan*)this->planner_->get_working_plan();

	// Remove open conditions mapped to the specified task instance.
	debug_text<<"removing open conds mapped to "<<cmd->task_inst_<<std::endl;
	for (CondSet::iterator cond_iter = cmd->conds_.begin ();
		cond_iter != cmd->conds_.end (); cond_iter++)
	{
		CausalLink lnk = (cmd->link_suspended_on.find(*cond_iter))->second;
		if(!work_plan_tmp->is_null_link(lnk)){
			work_plan_tmp->resume_condition(*cond_iter, cmd->task_inst_, lnk);
		}

		debug_text<<"checking for "<<cond_iter->id<<std::endl;
		for (OpenCondMap::iterator open_iter =
			this->open_conds_.lower_bound (*cond_iter);
			open_iter != this->open_conds_.upper_bound (*cond_iter);)
		{
			OpenCondMap::iterator prev_iter = open_iter;
			open_iter++;
			if (prev_iter->second == cmd->task_inst_)
			{
				debug_text<<"in planstrat erasing from open_conds_ "<<prev_iter->first.id<<" to "<<prev_iter->second<<std::endl;
				this->open_conds_.erase (prev_iter);
			}
		}
	}

	SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, debug_text.str ());
	debug_text.str("");

};

// Execute a command to remove open conditions from planning.
void SA_PlanStrategy::execute (SA_RemoveOpenCondsCmd *cmd)
{
	// Remove open conditions, keeping track of removed cond->inst mapping.

	SA_WorkingPlan* working_plan_tmp = (SA_WorkingPlan*)this->planner_->get_working_plan();

	for(CondSet::iterator cond_it = cmd->conds_.begin(); cond_it != cmd->conds_.end();
		cond_it++){

			for(TaskInstSet::iterator task_it = cmd->tasks_.begin(); 
				task_it != cmd->tasks_.end(); task_it++){

					for(OpenCondMap::iterator o_it = open_conds_.lower_bound(*cond_it);
						o_it != open_conds_.upper_bound(*cond_it); ){

							if(o_it->second == *task_it){

								OpenCondMap::iterator prev_iter = o_it;
								o_it++;

								cmd->removed_.insert (std::make_pair
									(*cond_it, *task_it));

								open_conds_.erase(prev_iter);
							}else{
								o_it++;
							}
			  }
			}
	}
};

// Undo a command to remove open conditions from planning.
void SA_PlanStrategy::undo (SA_RemoveOpenCondsCmd *cmd)
{

	std::ostringstream debug_text;
	// Insert removed open condition to task instance mapping.
	for (OpenCondMap::iterator open_iter = cmd->removed_.begin ();
		open_iter != cmd->removed_.end (); open_iter++)
	{
		debug_text<<"in planstrat undo adding "<<open_iter->first.id<<" to "<<open_iter->second<<std::endl;
		this->open_conds_.push_front (std::make_pair
			(open_iter->first, open_iter->second));
	}
	SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, debug_text.str ());
	debug_text.str("");
};

// Execute a command to add causal link threats to planning.
void SA_PlanStrategy::execute (SA_AddOpenThreatsCmd *cmd)
{
	for (CLThreatSet::iterator iter = cmd->threats_.begin ();
		iter != cmd->threats_.end (); iter++)
	{
		CLThreat threat = *iter;
		this->open_threats_.insert (threat);
	}
};

// Undo a command to add causal link threats to planning.
void SA_PlanStrategy::undo (SA_AddOpenThreatsCmd * cmd)
{
	std::ostringstream debug_text;
	// Remove open conditions mapped to the specified task instance.
	debug_text<<"undoing open threats" <<std::endl;
	for (CLThreatSet::iterator cond_iter = cmd->threats_.begin ();
		cond_iter != cmd->threats_.end (); cond_iter++)
	{
		CLThreat threat = *cond_iter;
		this->open_threats_.erase (threat);
	}

	SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, debug_text.str ());
	debug_text.str("");
};

// Execute a command to remove causal link threats from planning.
void SA_PlanStrategy::execute (SA_RemoveOpenThreatsCmd * cmd)
{
	std::ostringstream debug_text;
	// Remove open conditions mapped to the specified task instance.
	debug_text<<"removing open threats" <<std::endl;

	SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, debug_text.str ());
	debug_text.str("");

	for (CLThreatSet::iterator cond_iter = cmd->threats_.begin ();
		cond_iter != cmd->threats_.end (); cond_iter++)
	{
		CLThreat threat = *cond_iter;
		this->open_threats_.erase (threat);
	}
};

// Undo a command to remove causal link threats from planning.
void SA_PlanStrategy::undo (SA_RemoveOpenThreatsCmd * cmd)
{
	for (CLThreatSet::iterator iter = cmd->threats_.begin ();
		iter != cmd->threats_.end (); iter++)
	{
		CLThreat threat = *iter;
		//this->closed_threats_.erase (threat);
		this->open_threats_.insert (threat);
	}
};




// Satisfy an open condition with an appropriate task.
AddTaskCmd *SA_PlanStrategy::satisfy_cond (Condition open_cond)
{
	// Get add task command.
	AddTaskCmd *add_task_cmd =
		static_cast<AddTaskCmd *> (this->add_task_cmd_->clone ());
	TaskChoiceList task_list = this->task_choice_->choose_task_fair (open_cond);
	add_task_cmd->set_id (this->get_next_cmd_id ());
	add_task_cmd->set_tasks (task_list);

	// Get task instances requiring this open condition, and set in command.
	TaskInstSet inst_set;
	inst_set.clear ();
	for (OpenCondMap::iterator iter = this->open_conds_.lower_bound (open_cond);
		iter != this->open_conds_.upper_bound (open_cond); iter++)
	{
		if(iter->first.value == open_cond.value){
			inst_set.insert (iter->second);
			break;
		}
	}
	add_task_cmd->set_causal_info (open_cond, inst_set);

	// Add command to planner to be executed next.
	this->planner_->add_command (add_task_cmd);

	// Return pointer to command.
	return add_task_cmd;
};

// Add open conditions.
CommandID SA_PlanStrategy::add_open_conds (const CondSet &open_conds,
										   TaskInstID task_inst)
{
	// Get command to add open conditions for task instance.
	AddOpenCondsCmd *add_conds_cmd =
		static_cast<AddOpenCondsCmd *> (this->add_conds_cmd_->clone ());
	add_conds_cmd->set_conds (open_conds);
	add_conds_cmd->set_task_inst (task_inst);
	CommandID cmd_id = this->get_next_cmd_id ();
	add_conds_cmd->set_id (cmd_id);

	// Execute command immediately and return command id.
	this->planner_->execute_command (add_conds_cmd);
	return cmd_id;
};

// Remove open condition.
CommandID SA_PlanStrategy::rmv_open_cond (Condition open_cond, TaskInstSet tasks)
{
	// Get commmand to remove open conditions.
	RemoveOpenCondsCmd *rmv_conds_cmd =
		static_cast<RemoveOpenCondsCmd *> (this->rmv_conds_cmd_->clone ());
	CondSet conds;
	conds.insert (open_cond);
	rmv_conds_cmd->set_conds (conds, tasks);
	CommandID cmd_id = this->get_next_cmd_id ();
	rmv_conds_cmd->set_id (cmd_id);

	// Execute command immediately and return command id.
	this->planner_->execute_command (rmv_conds_cmd);
	return cmd_id;
};

// Add open causal link threats.
CommandID SA_PlanStrategy::add_open_threats (const CLThreatSet &threats)
{
	// Get commmand to add open threats.
	AddOpenThreatsCmd *add_threats_cmd =
		static_cast<AddOpenThreatsCmd *> (this->add_threats_cmd_->clone ());
	add_threats_cmd->set_threats (threats);
	CommandID cmd_id = this->get_next_cmd_id ();
	add_threats_cmd->set_id (cmd_id);

	// Execute command immediately and return command id.
	this->planner_->execute_command (add_threats_cmd);
	return cmd_id;
};

// Remove open causal link threat.
CommandID SA_PlanStrategy::rmv_open_threat (CLThreat threat)
{
	// Get commmand to remove open threats.
	RemoveOpenThreatsCmd *rmv_threats_cmd =
		static_cast<RemoveOpenThreatsCmd *> (this->rmv_threats_cmd_->clone ());
	CLThreatSet threats;
	threats.insert (threat);
	rmv_threats_cmd->set_threats (threats);
	CommandID cmd_id = this->get_next_cmd_id ();
	rmv_threats_cmd->set_id (cmd_id);

	// Execute command immediately and return command id.
	this->planner_->execute_command (rmv_threats_cmd);
	return cmd_id;
};
