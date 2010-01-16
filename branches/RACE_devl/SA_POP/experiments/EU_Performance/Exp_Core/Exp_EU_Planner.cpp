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
#include <sstream>

#include "boost/random.hpp"
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/variance.hpp>


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
:do_pause_ (false),
 input_ (0),
 ques_ (0),
 do_stats_out_ (false),
 trial_num_plans_ (0),
 run_num_trial_attempts_ (0),
 run_num_init_plans_ (0),
 run_num_pref_plans_ (0),
 run_num_alt_plans_ (0),
 pref_plan_eu_ (0.0),
 max_plan_eu_ (0.0)
{
//    std::ofstream log_trials_out_;
//    std::ofstream log_runs_out_;

  // All other initialization handled by Planner base class constructor.
};


/// Destructor.
Exp_EU_Planner::~Exp_EU_Planner (void)
{
  // Close output files, if necessary.
  if (this->log_trials_out_ != 0){
    if (this->log_trials_out_.is_open ())
      this->log_trials_out_.close ();
  }
  if (this->log_runs_out_ != 0){
    if (this->log_runs_out_.is_open ())
      this->log_runs_out_.close ();
  }

  // All deletion handled by Planner base class destructor.
};


// Reset all planning statistics for an individual trial.
void Exp_EU_Planner::reset_trial_stats (void)
{
  this->trial_num_plans_ = 0;
  this->pref_plan_eu_ = 0.0;
  this->max_plan_eu_ = 0.0;
};

// Reset all planning statistics for an individual experimental run.
void Exp_EU_Planner::reset_run_stats (void)
{
  this->run_num_trial_attempts_ = 0;
  this->run_num_init_plans_ = 0;
  this->run_num_pref_plans_ = 0;
  this->run_num_alt_plans_ = 0;
};

// Set to pause and ask user whether to continue after each plan is generated.
void Exp_EU_Planner::set_pause (UserInterface::InputCL *input_cl, UserInterface::QuestionBool *ques)
{
  this->input_ = input_cl;
  this->ques_ = ques;
  this->do_pause_ = true;
};

// Set to generate all plans without user input (instead of asking whether to continue after each plan is generated).
void Exp_EU_Planner::unset_pause (void)
{
  this->do_pause_ = false;
};

// Run experiment planning.
void Exp_EU_Planner::exp_run (size_t sa_max_steps, std::string log_trials_filename, std::string log_runs_filename, size_t num_goal_conds, double percent_init_true, size_t util_min, size_t util_max, size_t max_trial_attempts, size_t num_trials, bool do_log_headers)
{
  // Set flag to output statistics to file.
  this->do_stats_out_ = true;

  // Open/create trials output file if not already open.
  if (this->log_trials_out_ != 0){
    if (!this->log_trials_out_.is_open ())
      this->log_trials_out_.open (log_trials_filename.c_str (), std::ios_base::out | std::ios_base::app);
  }
  if (this->log_trials_out_ == 0){
    std::string msg = "SA-POP ERROR: in Exp_EU_Planner::plan_exp():  Unable to open experiment trials log file (";
    msg += log_trials_filename;
    msg += ") for writing (append).";
    std::cerr << msg;
    throw msg;
  }

  // Open/create runs output file if not already open.
  if (this->log_runs_out_ != 0){
    if (!this->log_runs_out_.is_open ())
      this->log_runs_out_.open (log_runs_filename.c_str (), std::ios_base::out | std::ios_base::app);
  }
  if (this->log_runs_out_ == 0){
    std::string msg = "SA-POP ERROR: in Exp_EU_Planner::plan_exp():  Unable to open experiment runs log file (";
    msg += log_runs_filename;
    msg += ") for writing (append).";
    std::cerr << msg;
    throw msg;
  }

  // Log headers if flag set.
  if (do_log_headers) {
    this->log_run_header ();
    this->log_trial_header ();
  }

  // Do experimental runs.
  for (size_t trial_num = 0; trial_num < num_trials; trial_num++) {
    // If maximum number of trial attempts have been reached,
    // stop running trials.
    if (this->run_num_trial_attempts_ >= max_trial_attempts)
      break;

    // Initialize trial statistics.
    this->reset_trial_stats ();

    // Initialize conditions and goal.
    SA_POP::Goal goal = this->exp_init (num_goal_conds, percent_init_true, util_min, util_max);

    // Perform experimental run.
    this->plan (sa_max_steps, goal);

    // Increment trial attempts counter.
    this->run_num_trial_attempts_++;

    // Trial only valid if at least one alternate plan was generated.
    if (this->trial_num_plans_ >= 3) {
      // Log trial statistics to output file.
      this->log_trial_stats ();
    } else {
      // For invalid trial, decrement trial counter.
      trial_num--;
    }

  }

  // Log run statistics to output file.
  this->log_run_stats ();

  // Close output files.
  this->log_trials_out_.close ();
};


// Satisfy scheduling constraints in fully instantiated plan (no
// recursive call backs).
bool Exp_EU_Planner::full_sched ()
{
  if (this->sched_strat_->satisfy_full_sched ()) {
    // Track statistics for the generated plan.
    this->plan_ = this->working_plan_->get_plan ();
    this->track_stats (this->plan_);

    // Notify output adapters.
    this->notify_plan_changed ();

    // If pausing to ask user whether to continue, ask continuation
    // question.
    if (this->do_pause_) {
      this->input_->ask ((*this->ques_));

      // Return true (so the current plan will be returned instead
      // of continuing planning) only if user replies "No" to
      // continuation question.
      if (!this->ques_->get_answer_bool ())
        return true;
    }
  }

  return false;
};

// Add plan to tracking statistics.
void Exp_EU_Planner::track_stats (SA_POP::Plan plan)
{
  // Update number of plans generated.
  this->trial_num_plans_++;

  // Ignore first plan.
  if (this->trial_num_plans_ <= 1) {
    // First plan.
    this->trial_num_plans_ = 1;

    // Increment counter of trials with initial plan.
    this->run_num_init_plans_++;

    // Nothing else to do for first plan.
    return;
  }

  // Get plan EU.
  SA_POP::Utility plan_eu = this->calc_plan_eu (plan);

  // Second plan is SA-POP preferred plan.
  if (this->trial_num_plans_ == 2) {
    // Current maximum plan EU is this (preferred) plan's EU.
    this->pref_plan_eu_ = this->max_plan_eu_ = plan_eu;

    // Increment counter of trials with preferred plan.
    this->run_num_pref_plans_++;

    // Nothing else to do for first plan.
    return;
  }

  // Third or more is an alternate plan.

  // If this is the first alternate plan (third plan generated),
  // increment counter of trials with at least one alternate plan.
  if (this->trial_num_plans_ == 3) {
    this->run_num_alt_plans_++;
  }

  // If this plan has a higher EU than current max, update max.
  if (plan_eu > this->max_plan_eu_)
    this->max_plan_eu_ = plan_eu;
};

// Initialize experiment (set values for initial conditions and create goal).
SA_POP::Goal Exp_EU_Planner::exp_init (size_t num_goal_conds, double percent_init_true, size_t util_min, size_t util_max)
{
  // Check goal utility min & max for consistency.
  if (util_min > util_max) {
    std::cerr << "SA-POP ERROR in Exp_EU_Planner::exp_init (): goal utility min is >= goal utility max";
    throw "SA-POP ERROR in Exp_EU_Planner::exp_init (): goal utility min is >= goal utility max";
  }


  SA_POP::Goal goal;
  goal.goal_id = "ExperimentGoalID";
  goal.name = "Experiment generated goal";
  goal.abs_times.clear ();
  goal.rel_times.clear ();
  goal.goal_conds.clear ();
  goal.start_window = std::make_pair (0, 0);

  // Get all conditions.
  SA_POP::CondSet conds = this->sanet_->get_all_conds ();

  // Check number of goal conditions for consistency.
  if (num_goal_conds > conds.size ()) {
    std::cerr << "SA-POP ERROR in Exp_EU_Planner::exp_init (): more goal conditions than number of goals";
    throw "SA-POP ERROR in Exp_EU_Planner::exp_init (): more goal conditions than number of goals";
  }

  // Initialize random number generators.
  boost::uniform_int<> goal_choice_dist(0,  conds.size () - 1);
  boost::variate_generator<boost::mt19937&, boost::uniform_int<> > gen_goal_choice(this->rand_gen_, goal_choice_dist);
  boost::uniform_int<> goal_util_dist(util_min, util_max);
  boost::variate_generator<boost::mt19937&, boost::uniform_int<> > gen_goal_util(this->rand_gen_, goal_util_dist);
  boost::uniform_real<> cond_init_dist(0.0, 1.0);
  boost::variate_generator<boost::mt19937&, boost::uniform_real<> > gen_cond_init(this->rand_gen_, cond_init_dist);

  // Set goals.
  std::set<SA_POP::CondID> goal_ids;
  goal_ids.clear ();
  for (size_t goal_ctr = 0; goal_ctr < num_goal_conds; goal_ctr++) {
    // Choose goal condition number.
    size_t goal_choice = gen_goal_choice ();

    // Get goal condition.
    SA_POP::Condition goal_cond;
    goal_cond.id = SA_POP::NULL_COND_ID;
    size_t cond_count = 0;
    for (SA_POP::CondSet::iterator cond_iter = conds.begin (); cond_iter != conds.end (); cond_iter++) {
      if (cond_count == goal_choice) {
        goal_cond = (*cond_iter);
        break;
      }
      cond_count++;
    }

    SA_POP::CondID goal_id = goal_cond.id;

    // Try again if this condition is already a goal condition.
    if (goal_ids.find (goal_id) != goal_ids.end ()) {
      goal_ctr--;
      continue;
    }

    // Choose goal utility.
    SA_POP::Utility goal_util = gen_goal_util ();

    // Add goal condition.
    goal.goal_conds.insert (std::make_pair (goal_id, goal_util));
    goal_ids.insert (goal_id);
  }

  // Reset all conditions to false.
  for (CondSet::iterator cond_iter = conds.begin (); cond_iter != conds.end (); cond_iter++) {
    cond_iter->value = 0.0;
  }
  this->sanet_->update_conds_val (conds);

  // Set initial conditions.
  for (SA_POP::CondSet::iterator cond_iter = conds.begin (); cond_iter != conds.end (); cond_iter++) {
    // Skip goal conditions.
    if (goal_ids.find ((*cond_iter).id) != goal_ids.end ())
      continue;

    // Randomly determine whether to set condition to true.
    double init_check = gen_cond_init ();
    if (init_check <= percent_init_true) {
      this->sanet_->update_cond_val ((*cond_iter).id, true);
    }
  }

 
  return goal;
};

// Output trial header to log.
void Exp_EU_Planner::log_trial_header (void)
{
  // Only log header if flag is set.
  if (!this->do_stats_out_)
    return;

  // Throw error if log filestream is null or log file is not open.
  if (this->log_trials_out_ == 0 || !this->log_trials_out_.is_open ()) {
    std::string msg = "SA-POP ERROR: in Exp_EU_Planner::log_trial_header():  Experimental trials log file is not open.";
    std::cerr << msg;
    throw msg;
  }

  // Output trial header to file.
  this->log_trials_out_ << "Preferred plan EU" << "\t";
  this->log_trials_out_ << "Maximum plan EU" << "\t";
  this->log_trials_out_ << "Number of alternate plans";
  this->log_trials_out_ << std::endl;

};

// Output trial statistics to log.
void Exp_EU_Planner::log_trial_stats (void)
{
  // Only log statistics if flag is set.
  if (!this->do_stats_out_)
    return;

  // Throw error if log filestream is null or log file is not open.
  if (this->log_trials_out_ == 0 || !this->log_trials_out_.is_open ()) {
    std::string msg = "SA-POP ERROR: in Exp_EU_Planner::log_trial_stats():  Experimental trials log file is not open.";
    std::cerr << msg;
    throw msg;
  }

  // Output trial statistics to file.
  this->log_trials_out_ << this->pref_plan_eu_ << "\t";
  this->log_trials_out_ << this->max_plan_eu_ << "\t";
  this->log_trials_out_ << (this->trial_num_plans_ - 2);
  this->log_trials_out_ << std::endl;

};

// Output run header to log.
void Exp_EU_Planner::log_run_header (void)
{
  // Only log statistics if flag is set.
  if (!this->do_stats_out_)
    return;

  // Throw error if log filestream is null or log file is not open.
  if (this->log_runs_out_ == 0 || !this->log_runs_out_.is_open ()) {
    std::string msg = "SA-POP ERROR: in Exp_EU_Planner::log_run_header():  Experimental runs log file is not open.";
    std::cerr << msg;
    throw msg;
  }
  
  // Output run header to file.
  this->log_runs_out_ << "Trial attempts" << "\t";
  this->log_runs_out_ << "Trials w/ initial plan" << "\t";
  this->log_runs_out_ << "Trials w/ preferred plan" << "\t";
  this->log_runs_out_ << "Trials w/ alternate plan(s)";
  this->log_runs_out_ << std::endl;
};

// Output run statistics to log.
void Exp_EU_Planner::log_run_stats (void)
{
  // Only log statistics if flag is set.
  if (!this->do_stats_out_)
    return;

  // Throw error if log filestream is null or log file is not open.
  if (this->log_runs_out_ == 0 || !this->log_runs_out_.is_open ()) {
    std::string msg = "SA-POP ERROR: in Exp_EU_Planner::log_run_stats():  Experimental runs log file is not open.";
    std::cerr << msg;
    throw msg;
  }

  // Output run statistics to file.
  this->log_runs_out_ << this->run_num_trial_attempts_ << "\t";
  this->log_runs_out_ << this->run_num_init_plans_ << "\t";
  this->log_runs_out_ << this->run_num_pref_plans_ << "\t";
  this->log_runs_out_ << this->run_num_alt_plans_;
  this->log_runs_out_ << std::endl;
};




