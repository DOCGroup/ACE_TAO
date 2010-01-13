// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  Exp_EU_Planner.cpp
 *
 * This file contains the Exp_EU_Planner class implementation for a planner,
 * which implements a Planner that finds all possible plans during planning
 * and outputs statistics comparing expected utility of each to the SA-POP
 * preferred plan.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#include <string>
#include <set>
#include <map>
#include <stdlib.h>
#include <fstream>

#include "SA_POP_Types.h"
#include "Exp_EU_Planner.h"

//#include "Planner.h"
//#include "PlanStrategy.h"
//#include "SchedStrategy.h"
//#include "WorkingPlan.h"
//#include "TaskMap.h"
//#include "OutAdapter.h"
//#include "PlanCommands.h"

using namespace SA_POP;

// Constructor.
Exp_EU_Planner::Exp_EU_Planner (void)
{
  // All initialization handled by Planner base class constructor.
};


/// Destructor.
Exp_EU_Planner::~Exp_EU_Planner (void)
{
  // All deletion handled by Planner base class destructor.
}




// Run planning.
bool Exp_EU_Planner::plan (size_t sa_max_steps, SA_POP::Goal goal)
{
  // Add goal to working plan and task network.
  this->working_plan_->set_goal (goal);
  this->sanet_->update_goals (goal.goal_conds);

  // Run spreading activation.
  this->sanet_->update (sa_max_steps);

  // Set planning strategy goals and satisfy open conditions.
  this->plan_strat_->set_goals (goal.goal_conds);

  if (this->plan_strat_->satisfy_open_conds ()) {
    this->plan_ = this->working_plan_->get_plan ();

    this->notify_plan_changed ();
    return true;
  }

  return false;
};

// Replan with new goal.
bool Exp_EU_Planner::replan (size_t sa_max_steps, SA_POP::Goal goal)
{
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****
  // Full replanning not implemented, so just restart planning.
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****

  // Clear plan.
  this->plan_.causal_links.clear ();
  this->plan_.connections.clear ();
  this->plan_.sched_links.clear ();
  this->plan_.task_insts.clear ();
  this->plan_.threat_links.clear ();

  // Add goal to working plan and task network.
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****
  // Need to reset working plan.
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****
  this->working_plan_->reset_plan();
  this->working_plan_->set_goal (goal);
  this->sanet_->update_goals (goal.goal_conds);

  // Run spreading activation.
  this->sanet_->update (sa_max_steps);


  // Set planning strategy goals and satisfy open conditions.
  this->plan_strat_->set_goals (goal.goal_conds);
  if (this->plan_strat_->satisfy_open_conds ()) {
    this->plan_ = this->working_plan_->get_plan ();
    this->notify_plan_changed ();
    return true;
  }

  return false;
};

// Replan with existing goal.
bool Exp_EU_Planner::replan (size_t sa_max_steps)
{
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****
  // Full replanning not implemented, so just restart planning.
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****
  // Clear plan.
  //TODO: Refactor this plan clearing into own function.
  this->plan_.causal_links.clear ();
  this->plan_.connections.clear ();
  this->plan_.sched_links.clear ();
  this->plan_.task_insts.clear ();
  this->plan_.threat_links.clear ();
  // Reset the working plan

  // Run spreading activation.
  this->sanet_->update (sa_max_steps);

  // Set planning strategy goals and satisfy open conditions.
  this->plan_strat_->set_goals (this->sanet_->get_goals ());
  if (this->plan_strat_->satisfy_open_conds ()) {
    this->plan_ = this->working_plan_->get_plan ();
    this->notify_plan_changed ();
    return true;
  }

  return false;
};


// Recursively plan (satisfy all open conditions & schedule constraints).
bool Exp_EU_Planner::recurse_plan (void)
{
  return this->plan_strat_->satisfy_open_conds ();
};

// Satisfy scheduling constraints in fully instantiated plan (no
// recursive call backs).
bool Exp_EU_Planner::full_sched ()
{
  if (this->sched_strat_->satisfy_full_sched ()) {
    this->plan_ = this->working_plan_->get_plan ();
    this->notify_plan_changed ();
    Utility plan_eu = this->calc_plan_eu (this->plan_);
    std::cout << "Expected utility of generated plan:  " << plan_eu << std::endl << std::endl;
  }

  return false;
};

