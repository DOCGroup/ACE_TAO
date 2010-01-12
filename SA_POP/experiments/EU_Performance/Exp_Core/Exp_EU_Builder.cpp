// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  Exp_EU_Builder.cpp
 *
 * This file contains the implementation of the Exp_EU_Builder concrete class,
 * which implements a Builder creating Exp_EU_Planner and associated
 * objects for planning with spreading activation networks and scheduling
 * with the "roadblock" scheduler (Exp_EU_SchedStrategy).
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================


#include "SA_POP_Types.h"

//#include "SA_Builder.h"
#include "Exp_EU_Builder.h"

//#include "Planner.h"
#include "Exp_EU_Planner.h"

#include "SANet/SANet.h"
#include "SA_PlanHeuristics.h"
#include "SA_PlanStrategy.h"

//#include "SA_SchedStrategy.h"
#include "Exp_EU_SchedStrategy.h"

#include "SA_WorkingPlan.h"
#include "TaskMap.h"

using namespace SA_POP;

// Constructor.
Exp_EU_Builder::Exp_EU_Builder (void)
: exp_eu_planner_ (0)
{
  // All other initialization handled by SA_Builder base class constructor.
};

// Destructor.
Exp_EU_Builder::~Exp_EU_Builder (void)
{
  // Nothing to do because client deletes Planner and Planner
  // handles deletion of the rest of the objects.
};

// Create SA-POP objects.
void Exp_EU_Builder::init (void)
{
  // Set init flag.
  this->is_init_ = true;

  // Create objects.
  this->exp_eu_planner_ = new Exp_EU_Planner ();
  this->planner_ = this->exp_eu_planner_;

  this->sanet_ = new SANet::Network ();
  this->cond_choice_ = new SA_CondStrategy (this->planner_);
  this->task_choice_ = new SA_TaskStrategy (this->planner_);
  this->impl_choice_ = new SA_ImplStrategy (this->planner_);
  this->plan_strat_ = new SA_PlanStrategy (this->planner_,
    this->cond_choice_, this->task_choice_, this->impl_choice_);
  this->sched_strat_ = new Exp_EU_SchedStrategy (this->planner_);
  this->working_plan_ = new SA_WorkingPlan (this->planner_);
  this->task_map_ = new TaskMap ();

  // Provide PlanCommand prototypes to objects.
  this->plan_strat_->set_commands (
    this->working_plan_->get_AddTaskCmd (),
    this->working_plan_->get_AssocTaskImplCmd (),
    this->working_plan_->get_ResolveCLThreatCmd());
  this->sched_strat_->set_commands (
    this->working_plan_->get_ResolveSchedOrderCmd (),
	this->working_plan_->get_AdjustMinTimesCmd (),
	this->working_plan_->get_AdjustMaxTimesCmd ());
  this->working_plan_->set_commands (
    this->plan_strat_->get_AddOpenThreatsCmd ());

  // Provide planning objects to Planner.
  this->planner_->set_objects (
    this->sanet_,
    this->plan_strat_,
    this->sched_strat_,
    this->working_plan_,
    this->task_map_);
};

// Get Exp_EU_Planner object.
Exp_EU_Planner *Exp_EU_Builder::get_exp_eu_planner (void)
{
  if (!this->is_init_)
    this->init ();
  return this->exp_eu_planner_;
};

