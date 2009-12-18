// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SA_Builder.cpp
 *
 * This file contains the implementation of the SA_Builder concrete class,
 * which implements a Builder creating SA_Planner and associated
 * objects for planning with spreading activation networks and scheduling
 * with precedence graphs.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================


#include "SA_POP_Types.h"
#include "SA_Builder.h"
#include "Planner.h"
#include "SANet/SANet.h"
#include "SA_PlanHeuristics.h"
#include "SA_PlanStrategy.h"
#include "SA_SchedStrategy.h"
#include "SA_WorkingPlan.h"
#include "TaskMap.h"

using namespace SA_POP;

// Constructor.
SA_Builder::SA_Builder (void)
: planner_ (0),
sanet_ (0),
cond_choice_ (0),
task_choice_ (0),
impl_choice_ (0),
plan_strat_ (0),
sched_strat_ (0),
working_plan_ (0),
task_map_ (0),
is_init_ (false)
{
};

// Destructor.
SA_Builder::~SA_Builder (void)
{
  // Nothing to do because client deletes Planner and Planner
  // handles deletion of the rest of the objects.
};

// Reset for building a new set of SA-POP objects.
// WARNING: Assumes that planner has been handed off and will be
// deleted by someone else.
void SA_Builder::reset (void)
{
  // Reset planning object pointers to null.
  this->is_init_ = false;
  this->planner_ = 0;
  this->sanet_ = 0;
  this->cond_choice_ = 0;
  this->task_choice_ = 0;
  this->impl_choice_ = 0;
  this->plan_strat_ = 0;
  this->sched_strat_ = 0;
  this->working_plan_ = 0;
  this->task_map_ = 0;

  // Create and initialize planning objects.
  this->init ();
};

// Get Planner object.
Planner *SA_Builder::get_planner (void)
{
  if (!this->is_init_)
    this->init ();
  return this->planner_;
};

// Add a task.
void SA_Builder::add_task (TaskID id, double prior_prob,
std::string name)
{
  if (!this->is_init_)
    this->init ();
  this->sanet_->add_task (id, name, 1, 0, prior_prob);
};

// Add a condition.
void SA_Builder::add_cond (CondID id, Utility utility,
double init_prob_true, std::string name, CondKind cond_kind)
{
  if (!this->is_init_)
    this->init ();
  this->sanet_->add_cond (id, name, 1,
    init_prob_true, 1.0 - init_prob_true, utility, cond_kind);
};

// Add a link between a precondition and task.
void SA_Builder::set_precond (CondID cond_id, TaskID task_id,
PortID port, double true_prob, double false_prob)
{
  if (!this->is_init_)
    this->init ();
  this->sanet_->add_precond_link (cond_id, task_id,
    true_prob, false_prob, port);
};

// Add a link between a task and an effect.
void SA_Builder::set_effect (TaskID task_id, CondID cond_id,
PortID port, double weight)
{
  if (!this->is_init_)
    this->init ();
  this->sanet_->add_effect_link (task_id, cond_id, weight, port);
};

// Add a resource.
void SA_Builder::add_resource (Resource resource)
{
  if (!this->is_init_)
    this->init ();
  this->task_map_->add_resource (resource);
};

// Add an implementation.
void SA_Builder::add_task_impl (TaskImpl *task_impl)
{
  if (!this->is_init_)
    this->init ();
  this->task_map_->add_task_impl (task_impl);
};

// Associate a task with a task implementation.
void SA_Builder::assoc_task_with_impl (TaskID task_id, TaskImplID task_impl_id,
                                       TimeValue duration)
{
  if (!this->is_init_)
    this->init ();
  this->task_map_->assoc_task_with_impl (task_id, task_impl_id, duration);
};

// Associate a task implementation with its utilization of a resource.
void SA_Builder::assoc_impl_with_resource (TaskImplID impl_id,
ResourceID resource_id, ResourceValue resource_usage)
{
  if (!this->is_init_)
    this->init ();
  this->task_map_->assoc_impl_with_resource (impl_id, resource_id,
    resource_usage);
};

// Create SA-POP objects.
void SA_Builder::init (void)
{
  // Set init flag.
  this->is_init_ = true;

  // Create objects.
  this->planner_ = new Planner ();
  this->sanet_ = new SANet::Network ();
  this->cond_choice_ = new SA_CondStrategy (this->planner_);
  this->task_choice_ = new SA_TaskStrategy (this->planner_);
  this->impl_choice_ = new SA_ImplStrategy (this->planner_);
  this->plan_strat_ = new SA_PlanStrategy (this->planner_,
    this->cond_choice_, this->task_choice_, this->impl_choice_);
  this->sched_strat_ = new SA_SchedStrategy (this->planner_);
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
