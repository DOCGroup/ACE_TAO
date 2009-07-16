// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SA_PlanCommands.cpp
 *
 * This file contains the implementation of the concrete classes,
 * which implement PlanCommand.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#include "SA_POP_Types.h"
#include "SA_PlanCommands.h"
#include "SA_WorkingPlan.h"
#include "SA_PlanStrategy.h"
#include <string>
#include <stdlib.h>
#include <fstream>
#include <iostream>

using namespace SA_POP;

// Constructor.
SA_AdjustMinTimesCmd::SA_AdjustMinTimesCmd (SA_WorkingPlan *working_plan)
: working_plan_ (working_plan)
{
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
};

// Destructor.
SA_AdjustMinTimesCmd::~SA_AdjustMinTimesCmd (void)
{
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
};

// Create a deep copy of this command.
PlanCommand *SA_AdjustMinTimesCmd::clone (void)
{
  return new SA_AdjustMinTimesCmd (*this);
};

// Execute next option for this command.
bool SA_AdjustMinTimesCmd::execute_next (void)
{
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
//  SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, this->get_log_text ());
//  this->working_plan_->execute (this);
  SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, this->get_log_text ());
  return this->working_plan_->execute (this);
};

// Undo this command.
void SA_AdjustMinTimesCmd::undo (void)
{
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  this->working_plan_->undo (this);
};

// Get log text for most recent execution of command.
std::string SA_AdjustMinTimesCmd::get_log_text (void)
{
  std::string log_str = "";
  log_str += "Adjusting Schedule Minimum Times (CommandID ";
  log_str += to_string (this->get_id ());
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  log_str += "): [NOT HANDLING FULL SCHEDULING YET]";

  return log_str;
};

// Set the task instance and window adjustment.
void SA_AdjustMinTimesCmd::set_times (TaskInstID task_inst, TimeValue start_min,
TimeValue end_min)
{
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  TimeWindow start_old = this->working_plan_->get_start_window(task_inst);
  TimeWindow end_old = this->working_plan_->get_end_window(task_inst);
  this->task_inst_=task_inst;
  this->old_start_min = start_old.second;
  this->old_end_min = end_old.second;
  this->new_start_min = start_min;
  this->new_end_min = end_min;
};



// Constructor.
SA_AdjustMaxTimesCmd::SA_AdjustMaxTimesCmd (SA_WorkingPlan *working_plan)
: working_plan_ (working_plan)
{
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
};

// Destructor.
SA_AdjustMaxTimesCmd::~SA_AdjustMaxTimesCmd (void)
{
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
};

// Create a deep copy of this command.
PlanCommand *SA_AdjustMaxTimesCmd::clone (void)
{
  return new SA_AdjustMaxTimesCmd (*this);
};

// Execute next option for this command.
bool SA_AdjustMaxTimesCmd::execute_next (void)
{
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
//  SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, this->get_log_text ());
//  this->working_plan_->execute (this);
  SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, this->get_log_text ());
  return this->working_plan_->execute (this);
};

// Undo this command.
void SA_AdjustMaxTimesCmd::undo (void)
{
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  this->working_plan_->undo (this);
};

// Get log text for most recent execution of command.
std::string SA_AdjustMaxTimesCmd::get_log_text (void)
{
  std::string log_str = "";
  log_str += "Adjusting Schedule Maximum Times (CommandID ";
  log_str += to_string (this->get_id ());
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  log_str += "): [NOT HANDLING FULL SCHEDULING YET]";

  return log_str;
};

// Set the task instance and window adjustment.
void SA_AdjustMaxTimesCmd::set_times (TaskInstID task_inst, TimeValue start_max,
TimeValue end_max)
{
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  TimeWindow start_old = this->working_plan_->get_start_window(task_inst);
  TimeWindow end_old = this->working_plan_->get_end_window(task_inst);
  this->task_inst_=task_inst;
  this->old_start_max = start_old.second;
  this->old_end_max = end_old.second;
  this->new_start_max = start_max;
  this->new_end_max = end_max;
  // std::cout<<"setting max times "<<start_max<<" "<<end_max<<std::endl;
};



// Constructor.
SA_AddTaskCmd::SA_AddTaskCmd (SA_WorkingPlan *working_plan)
: working_plan_ (working_plan),
last_task_ (SA_POP::NULL_TASK_ID),
last_task_inst_ (SA_POP::NULL_TASK_INST_ID),
num_tries_ (0)
{
	this->tasks_.clear ();
  this->cond_.id = SA_POP::NULL_COND_ID;
  this->task_insts_.clear ();
  this->used_task_insts_.clear();
};

// Destructor.
SA_AddTaskCmd::~SA_AddTaskCmd (void)
{
  // Nothing to do.
};

// Create a deep copy of this command.
PlanCommand *SA_AddTaskCmd::clone (void)
{
  return new SA_AddTaskCmd (*this);
};

// Execute next option for this command.
bool SA_AddTaskCmd::execute_next (void)
{

  SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, this->get_log_text ());


  this->undo();

  bool isInitial = false;

  if (this->tasks_.empty())
    return false;

  this->working_plan_->execute (this);
  this->num_tries_++;

  return true;
};

// Undo this command.
void SA_AddTaskCmd::undo (void)
{
  if (this->last_task_ == SA_POP::NULL_TASK_ID)
    return;

  this->working_plan_->undo (this);
  
  if (this->tasks_.empty ()) {
    this->task_insts_.clear ();
    this->cond_.id = SA_POP::NULL_COND_ID;
    this->last_task_ = SA_POP::NULL_TASK_ID;
  }
};

// Get log text for most recent execution of command.
std::string SA_AddTaskCmd::get_log_text (void)
{
  std::string log_str = "";
  log_str += "Adding Task (CommandID ";
  log_str += to_string (this->get_id ());
  log_str += "): ";
  TaskChoiceList::iterator task_iter = this->tasks_.begin ();
  if (task_iter == this->tasks_.end ())
    log_str += "[NO TASKS TO ADD]";
  else
    log_str += to_string (task_iter->task_id);

  return log_str;
};

// Set (ordered) list of tasks to add (one per execution) to the plan.
void SA_AddTaskCmd::set_tasks (const TaskChoiceList &tasks)
{
  if (!this->tasks_.empty ())
    throw "SA_POP::SA_AddTaskCmd::set_tasks (): called while current task list is not empty.";

  if (this->last_task_ != SA_POP::NULL_TASK_ID)
    throw "SA_POP::SA_AddTaskCmd::set_tasks (): called before last task was undone.";

  this->tasks_ = tasks;
};

// Set causal link info to add to the plan with task.
void SA_AddTaskCmd::set_causal_info (Condition cond, TaskInstSet task_insts)
{
  if (!this->task_insts_.empty ())
    throw "SA_POP::SA_AddTaskCmd::set_causal_info (): called while current task instance set is not empty.";

  if (this->cond_.id != SA_POP::NULL_COND_ID)
    throw "SA_POP::SA_AddTaskCmd::set_causal_info (): called while current condition is already set.";

  this->cond_ = cond;
  this->task_insts_ = task_insts;
};

// Get ID of last task added to the plan by this command.
TaskID SA_AddTaskCmd::get_task (void)
{
  return this->last_task_;
};

// Get ID of instance of last task added to the plan by this command.
TaskInstID SA_AddTaskCmd::get_task_inst (void)
{
  return this->last_task_inst_;
};

TaskInstSet SA_AddTaskCmd::get_satisfied_tasks(void){
  return this->task_insts_;
}

/// Check if the instance id used by the task of this command already exists.


bool SA_AddTaskCmd::inst_exists (void)
{
	return (this->last_task_choice_.choice == REUSE_INST);

}

// Constructor.
SA_AssocTaskImplCmd::SA_AssocTaskImplCmd (SA_WorkingPlan *working_plan)
: working_plan_ (working_plan),
task_inst_ (SA_POP::NULL_TASK_INST_ID),
last_impl_ (SA_POP::NULL_TASK_IMPL_ID),
num_tries_ (0),
min_adj_cmd(working_plan),
max_adj_cmd(working_plan)
{
  this->causal_insertions.clear();
  this->simul_insertions.clear();
  this->impls_.clear ();
};

// Destructor.
SA_AssocTaskImplCmd::~SA_AssocTaskImplCmd (void)
{
  // Nothing to do.
};

// Create a deep copy of this command.
PlanCommand *SA_AssocTaskImplCmd::clone (void)
{
  return new SA_AssocTaskImplCmd (*this);
};

// Execute next option for this command.
bool SA_AssocTaskImplCmd::execute_next (void)
{
  SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, this->get_log_text ());

  	if(get_id().step == 12 && get_id().decision_pt == 2 && get_id().seq_num == 1){
		bool k = true;
	}

  this->undo();

  got_to_scheduling = false;


  if (this->impls_.empty ()){
    return false;
  }


  this->num_tries_++;
  return this->working_plan_->execute (this);
  //return true;
};

// Undo this command.
void SA_AssocTaskImplCmd::undo (void)
{
  if (this->last_impl_ == SA_POP::NULL_TASK_IMPL_ID)
    return;

  this->working_plan_->undo (this);
  this->last_impl_ = SA_POP::NULL_TASK_IMPL_ID;

  if (this->impls_.empty ())
    this->task_inst_ = SA_POP::NULL_TASK_INST_ID;
};

// Get log text for most recent execution of command.
std::string SA_AssocTaskImplCmd::get_log_text (void)
{
  std::string log_str = "";
  log_str += "Associating Task(Instance)->Implementation (CommandID ";
  log_str += to_string (this->get_id ());
  log_str += "): ";
  log_str += to_string (this->working_plan_->get_task_from_inst (this->task_inst_));
  log_str += "(";
  log_str += to_string (this->task_inst_);
  log_str += ")->";
  TaskImplList::iterator impl_iter = this->impls_.begin ();
  if (impl_iter == this->impls_.end ())
    log_str += "[NO IMPLEMENTATIONS TO ASSOCIATE]";
  else
    log_str += *impl_iter;

  return log_str;
};

// Set the task->implementation associations.
void SA_AssocTaskImplCmd::set_assoc (TaskInstID task_inst,
                                     TaskImplList impl_list)
{
  if (!this->impls_.empty ())
    throw "SA_POP::SA_AssocTaskImplCmd::set_assoc (): called while current task implementation list is not empty.";

  if (this->last_impl_ != SA_POP::NULL_TASK_IMPL_ID)
    throw "SA_POP::SA_AssocTaskImplCmd::set_assoc (): called before last execution was undone.";

  this->impls_ = impl_list;
  this->task_inst_ = task_inst;
};

// Get ID of instance of this command.
TaskInstID SA_AssocTaskImplCmd::get_task_inst (void)
{
  return this->task_inst_;
};

void SA_AssocTaskImplCmd::set_satisfied_insts(TaskInstSet set){
	this->satisfied_insts = set;
}


// Constructor.
SA_AddOpenCondsCmd::SA_AddOpenCondsCmd (SA_PlanStrategy *plan_strat)
: plan_strat_ (plan_strat),
task_inst_ (SA_POP::NULL_TASK_INST_ID),
has_executed_ (false)
{
  this->conds_.clear ();
};

// Destructor.
SA_AddOpenCondsCmd::~SA_AddOpenCondsCmd (void)
{
  // Nothing to do.
};

// Create a deep copy of this command.
PlanCommand *SA_AddOpenCondsCmd::clone (void)
{
  return new SA_AddOpenCondsCmd (*this);
};

// Execute next option for this command.
bool SA_AddOpenCondsCmd::execute_next (void)
{
  SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, this->get_log_text ());

  if (this->conds_.empty ())
    return false;

  if (this->task_inst_ == SA_POP::NULL_TASK_INST_ID)
    throw "SA_POP::SA_AddOpenCondsCmd::execute_next (): called without task instance set.";

  this->undo();
  this->plan_strat_->execute (this);
  this->has_executed_ = true;
  return true;
};

// Undo this command.
void SA_AddOpenCondsCmd::undo (void)
{
  if (!this->has_executed_ || this->conds_.empty ())
    return;

  this->plan_strat_->undo (this);
  this->task_inst_ = SA_POP::NULL_TASK_INST_ID;
  this->conds_.clear ();
};

// Get log text for most recent execution of command.
std::string SA_AddOpenCondsCmd::get_log_text (void)
{
  std::string log_str = "";
  log_str += "Adding Open Conditions (CommandID ";
  log_str += to_string (this->get_id ());
  log_str += "):";
  for (CondSet::iterator cond_iter = this->conds_.begin ();
    cond_iter != this->conds_.end (); cond_iter++)
  {
    log_str += " ";
    log_str += to_string ((*cond_iter).id);
    log_str += " = ";
    if ((*cond_iter).value)
      log_str += "TRUE;";
    else
      log_str += "FALSE;";
  }

  return log_str;
};

// Set the open conditions to add.
void SA_AddOpenCondsCmd::set_conds (const CondSet &conds)
{
  if (!this->conds_.empty ())
    throw "SA_POP::SA_AddOpenCondsCmd::set_conds (): called while current condition set is not empty.";

  this->conds_ = conds;
};

// Set the task instance for which these are preconditions.
void SA_AddOpenCondsCmd::set_task_inst (TaskInstID task_inst)
{
  if (this->task_inst_ != SA_POP::NULL_TASK_INST_ID)
    throw "SA_POP::SA_AddOpenCondsCmd::set_conds (): called before last command was undone.";

  this->task_inst_ = task_inst;
};



// Constructor.
SA_RemoveOpenCondsCmd::SA_RemoveOpenCondsCmd (SA_PlanStrategy *plan_strat)
: plan_strat_ (plan_strat)
{
  this->conds_.clear ();
  this->removed_.clear ();
};

// Destructor.
SA_RemoveOpenCondsCmd::~SA_RemoveOpenCondsCmd (void)
{
  // Nothing to do.
};

// Create a deep copy of this command.
PlanCommand *SA_RemoveOpenCondsCmd::clone (void)
{
  return new SA_RemoveOpenCondsCmd (*this);
};

// Execute next option for this command.
bool SA_RemoveOpenCondsCmd::execute_next (void)
{
  SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, this->get_log_text ());

  if (this->conds_.empty ())
    return false;

  this->undo();
  this->plan_strat_->execute (this);
  return true;
};

// Undo this command.
void SA_RemoveOpenCondsCmd::undo (void)
{
  if (this->removed_.empty ())
    return;

  this->plan_strat_->undo (this);
  this->removed_.clear ();
  this->conds_.clear ();
};

// Get log text for most recent execution of command.
std::string SA_RemoveOpenCondsCmd::get_log_text (void)
{
  std::string log_str = "";
  log_str += "Removing Open Conditions (CommandID ";
  log_str += to_string (this->get_id ());
  log_str += "):";
  for (CondSet::iterator cond_iter = this->conds_.begin ();
    cond_iter != this->conds_.end (); cond_iter++)
  {
    log_str += " ";
    log_str += to_string ((*cond_iter).id);
    log_str += " = ";
    if ((*cond_iter).value)
      log_str += "TRUE;";
    else
      log_str += "FALSE;";
  }

  return log_str;
};

// Set the open conditions to remove.
void SA_RemoveOpenCondsCmd::set_conds (const CondSet &conds, const TaskInstSet &tasks)
{
  if (!this->conds_.empty ())
    throw "SA_POP::SA_RemoveOpenCondsCmd::set_conds (): called while current condition set is not empty.";

  if (!this->removed_.empty ())
    throw "SA_POP::SA_RemoveOpenCondsCmd::set_conds (): called before last execution undone.";

  this->conds_ = conds;
  this->tasks_ = tasks;
};



// Constructor.
SA_AddOpenThreatsCmd::SA_AddOpenThreatsCmd (SA_PlanStrategy *plan_strat)
: plan_strat_ (plan_strat),
has_executed_ (false)
{
  this->threats_.clear ();
};

// Destructor.
SA_AddOpenThreatsCmd::~SA_AddOpenThreatsCmd (void)
{
  // Nothing specific to destruct
};

// Create a deep copy of this command.
PlanCommand *SA_AddOpenThreatsCmd::clone (void)
{
  return new SA_AddOpenThreatsCmd (*this);
};

// Execute next option for this command.
bool SA_AddOpenThreatsCmd::execute_next (void)
{
  SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, this->get_log_text ());

  if (this->threats_.empty ())
    return false;

  this->undo();
  this->plan_strat_->execute (this);
  this->has_executed_ = true;
  return true;
};

// Undo this command.
void SA_AddOpenThreatsCmd::undo (void)
{
  if (!this->has_executed_ || this->threats_.empty ())
    return;

  this->plan_strat_->undo (this);
  this->threats_.clear ();
};

// Get log text for most recent execution of command.
std::string SA_AddOpenThreatsCmd::get_log_text (void)
{
  std::string log_str = "";
  log_str += "Adding Open Threats (CommandID ";
  log_str += to_string (this->get_id ());
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  log_str += "): [NOT HANDLING THREATS YET]";

  return log_str;
};

// Set the open threats to add.
void SA_AddOpenThreatsCmd::set_threats (const CLThreatSet & threats)
{
    if (!this->threats_.empty ())
    throw "SA_POP::SA_AddOpenThreatsCmd::set_conds (): called while current Threat set is not empty.";

  this->threats_ = threats;
};



// Constructor.
SA_RemoveOpenThreatsCmd::SA_RemoveOpenThreatsCmd (SA_PlanStrategy *plan_strat)
: plan_strat_ (plan_strat),
executed_ (false)
{
  this->threats_.clear ();
};

// Destructor.
SA_RemoveOpenThreatsCmd::~SA_RemoveOpenThreatsCmd (void)
{
  //Nothing to destruct
};

// Create a deep copy of this command.
PlanCommand *SA_RemoveOpenThreatsCmd::clone (void)
{
  return new SA_RemoveOpenThreatsCmd (*this);
};

// Execute next option for this command.
bool SA_RemoveOpenThreatsCmd::execute_next (void)
{
  SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, this->get_log_text ());

  if (this->threats_.empty ())
    return false;

  this->undo();
  this->plan_strat_->execute (this);
  this->executed_ = true;
  return true;
};

// Undo this command.
void SA_RemoveOpenThreatsCmd::undo (void)
{  if (!(this->executed_) || (this->threats_.empty ()))
    return;

  this->plan_strat_->undo (this);
  this->threats_.clear ();
};

// Get log text for most recent execution of command.
std::string SA_RemoveOpenThreatsCmd::get_log_text (void)
{
  std::string log_str = "";
  log_str += "Removing Open Threats (CommandID ";
  log_str += to_string (this->get_id ());
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  log_str += "): [NOT HANDLING THREATS YET]";

  return log_str;
};

// Set the open threats to remove.
void SA_RemoveOpenThreatsCmd::set_threats (const CLThreatSet & threats)
{
  if (!this->threats_.empty ())
    throw "SA_POP::SA_RemoveOpenThreatsCmd::set_threats (): called while current threat set is not empty.";


  this->threats_ = threats;
};



// Constructor.
SA_ResolveCLThreatCmd::SA_ResolveCLThreatCmd (SA_WorkingPlan *working_plan)
: working_plan_ (working_plan)
{
  choices = 0;
};

// Destructor.
SA_ResolveCLThreatCmd::~SA_ResolveCLThreatCmd (void)
{
  //No Temps Necessary, this does nothing
};

// Create a deep copy of this command.
PlanCommand *SA_ResolveCLThreatCmd::clone (void)
{
  return new SA_ResolveCLThreatCmd (*this);
};

// Execute next option for this command.
bool SA_ResolveCLThreatCmd::execute_next (void)
{
  bool goodOption = true;

  SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, this->get_log_text ());
  if(choices == 0)
  {
	got_to_change_precedences = false;

    //Add this next line to execute function
    this->working_plan_->add_sched_link(this->threat.threat,this->threat.clink.first);
    choices++;
    this->first = this->threat.threat;
    this->second = this->threat.clink.first;
    this->condition = this->threat.clink.cond;

      
    goodOption =  this->working_plan_->execute (this);

    if(goodOption)
    {
      return goodOption;
    }

  }

  if(choices == 1)
  {
     this->undo();

	 got_to_change_precedences = false;

    //Add this next line to execute function
     this->working_plan_->add_sched_link(this->threat.clink.second, this->threat.threat);
     choices++;
     this->second = this->threat.threat;
     this->first = this->threat.clink.second;
    this->condition = this->threat.clink.cond;

	  goodOption = this->working_plan_->execute (this);

    if(goodOption)
    {
      return goodOption;
    }
    else
    {
        return false;
    }


  }
  else
  {
    return false;
  }
	
  
};

// Undo this command.
void SA_ResolveCLThreatCmd::undo (void)
{
    SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, this->get_log_text ());
  if(choices == 1)
  {
    //Add this next line to execute function
    this->working_plan_->remove_sched_link(this->threat.threat,this->threat.clink.first);
	  return this->working_plan_->undo (this);
  }
  else if(choices == 2)
  {
    //Add this next line to execute function
     this->working_plan_->remove_sched_link(this->threat.clink.second, this->threat.threat);
	   return this->working_plan_->undo (this);
  }
};

// Get log text for most recent execution of command.
std::string SA_ResolveCLThreatCmd::get_log_text (void)
{
  std::string log_str = "";
  log_str += "Resolving Open Threat (CommandID ";
  log_str += to_string (this->get_id ());
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  log_str += "): [NOT HANDLING THREATS YET]";

  return log_str;
};

// Set the causal link threat to resolve.
void SA_ResolveCLThreatCmd::set_threat (CLThreat & tht)
{
  threat = tht;
};

// Set the task instances to order.
void SA_ResolveCLThreatCmd::set_task_insts (TaskInstID task_inst_a, TaskInstID task_inst_b)
{
	this->first = task_inst_a;
	this->second = task_inst_b;
};


// Constructor.
SA_ResolveSchedOrderCmd::SA_ResolveSchedOrderCmd (SA_WorkingPlan *working_plan)
: working_plan_ (working_plan)
{
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
};

// Destructor.
SA_ResolveSchedOrderCmd::~SA_ResolveSchedOrderCmd (void)
{
	this->adj_max_times_cmd_=NULL;
	this->adj_min_times_cmd_=NULL;
};

// Create a deep copy of this command.
PlanCommand *SA_ResolveSchedOrderCmd::clone (void)
{
  return new SA_ResolveSchedOrderCmd (*this);
};

// Execute next option for this command.
bool SA_ResolveSchedOrderCmd::execute_next (void)
{
	SA_POP_DEBUG_STR (SA_POP_DEBUG_NORMAL, this->get_log_text ());
	this->working_plan_->add_sched_link(this->first,this->second);
	return this->working_plan_->execute (this);
};

// Undo this command.
void SA_ResolveSchedOrderCmd::undo (void)
{
	this->working_plan_->remove_sched_link(this->first,this->second);
	this->working_plan_->undo (this);
};

// Get log text for most recent execution of command.
std::string SA_ResolveSchedOrderCmd::get_log_text (void)
{
  std::string log_str = "";
  log_str += "Resolving Schedule Ordering (CommandID ";
  log_str += to_string (this->get_id ());
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  log_str += "): [NOT HANDLING FULL SCHEDULING YET]";

  return log_str;
};

// Set the task instances to order.
void SA_ResolveSchedOrderCmd::set_task_insts (TaskInstID task_inst_a, TaskInstID task_inst_b)
{
	this->first = task_inst_a;
	this->second = task_inst_b;
};

/// Get the affected task instances.
TaskInstSet SA_ResolveSchedOrderCmd::get_affected_insts ()
{
	TaskInstSet affected_insts;
	affected_insts.insert(this->first);
	affected_insts.insert(this->second);
	for(ResolveSchedOrderCmdList::iterator iter=this->cmds_.begin();iter!=this->cmds_.end();iter++)
	{
    TaskInstSet temp=(*iter)->get_affected_insts();
    for(TaskInstSet::iterator iter2=temp.begin();iter2!=temp.end();iter2++)
    {
      if(*iter2!=this->first && *iter2!=this->second) affected_insts.insert(*iter2);
    }
  }
	return affected_insts;
};


