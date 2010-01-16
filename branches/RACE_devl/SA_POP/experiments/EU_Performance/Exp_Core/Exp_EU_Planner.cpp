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
#include <algorithm>

#include <boost/random.hpp>
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
 trial_init_plan_eu (0.0)
{
  this->reset_trial_stats ();
  this->reset_run_stats ();

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
  this->trial_init_plan_eu = 0.0;
  this->trial_results_.num_plans = 0;
  this->trial_results_.pref_plan_eu = 0.0;
  this->trial_results_.max_plan_eu = 0.0;

  // Create empty goal.
  SA_POP::Goal goal;
  goal.goal_id = "UserSpecifiedGoal ID";
  goal.name = "User specified goal";
  goal.abs_times.clear ();
  goal.rel_times.clear ();
  goal.goal_conds.clear ();
  goal.start_window = std::make_pair (0, 0);

  this->trial_results_.goal = goal;
};

// Reset all planning statistics for an individual experimental run.
void Exp_EU_Planner::reset_run_stats (void)
{
  this->run_results_.num_trial_attempts = 0;
  this->run_results_.num_init_plans = 0;
  this->run_results_.num_pref_plans = 0;
  this->run_results_.num_alt_plans = 0;
  this->run_results_.net_name = "UnknownNetwork";
  this->run_results_.trials.clear ();
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
SA_POP::Exp_EU_Run_Results Exp_EU_Planner::exp_run (std::string log_trials_filename, std::string log_runs_filename, std::string net_name, SA_POP::Exp_EU_Trial_Params trial_params, size_t max_trial_attempts, size_t num_trials, bool do_log_headers)
{
  // Maximum spreading activation steps to update network is twice
  // the number of tasks (for worst case of linear network).
  size_t sa_max_steps = 2 * this->sanet_->get_num_tasks ();

  this->run_results_.net_name = net_name;

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
    if (this->run_results_.num_trial_attempts >= max_trial_attempts)
      break;

    // Initialize trial statistics.
    this->reset_trial_stats ();

    // Initialize conditions and goal.
    SA_POP::Goal goal = this->exp_init (trial_params);
    this->trial_results_.goal = goal;

    // Perform experimental run.
    this->plan (sa_max_steps, goal);

    // Increment trial attempts counter.
    this->run_results_.num_trial_attempts++;

    // Trial only valid if at least one alternate plan was generated.
    if (this->trial_results_.num_plans >= 3) {
      // Log trial statistics to output file.
      this->log_trial_stats ();
      
      // Add to run results.
      this->run_results_.trials.push_back (this->trial_results_);
    } else {
      // For invalid trial, decrement trial counter.
      trial_num--;
    }

  }

  // Log run statistics to output file.
  this->log_run_stats (trial_params);

  // Close output files.
  this->log_trials_out_.close ();
  this->log_runs_out_.close ();

  return this->run_results_;
};


// Run planning.
bool Exp_EU_Planner::plan (size_t sa_max_steps, SA_POP::Goal goal)
{
  // Add goal to working plan and task network.
  this->working_plan_->set_goal (goal);
  this->sanet_->update_goals (goal.goal_conds);

  // Reset network and run spreading activation.
  this->sanet_->reset_sa ();
  this->sanet_->update (sa_max_steps);

//*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****
  // Output goal probabilities
  //std::ostringstream goal_str;
  //goal_str << "Goals (probability true):  ";
  //for (SA_POP::GoalMap::iterator goal_iter = goal.goal_conds.begin (); goal_iter != goal.goal_conds.end (); goal_iter++) {
  //  goal_str << this->sanet_->get_cond_name (goal_iter->first) << "(";
  //  goal_str << this->sanet_->get_cond_future_val(goal_iter->first, true) << ")  ";
  //}
  //SA_POP_DEBUG_STR(SA_POP_DEBUG_TEMP, goal_str.str ());
//*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****

  // Check that goal conditions meet probability criteria.
  // Skip planning and return false if any goal condition does not.
  for (SA_POP::GoalMap::iterator goal_iter = goal.goal_conds.begin (); goal_iter != goal.goal_conds.end (); goal_iter++) {
    if (goal_iter->second >= 0) {
      if (this->sanet_->get_cond_future_val(goal_iter->first, true) > 1.0 || this->sanet_->get_cond_future_val(goal_iter->first, true) < SA_POP::Default::GOAL_PROB_THRESH) {
        std::ostringstream invalid_goal_str;
        invalid_goal_str << "Goal condition (" << this->sanet_->get_cond_name (goal_iter->first) << ") ";
        invalid_goal_str << "with probability, " << this->sanet_->get_cond_future_val(goal_iter->first, true) << ", ";
        invalid_goal_str << "does not meet goal probability criteria.  Skipping planning.";
        SA_POP_DEBUG_STR(SA_POP_DEBUG_TEMP, invalid_goal_str.str ());

        return false;
      }
    } else {
      if (this->sanet_->get_cond_future_val(goal_iter->first, false) > 1.0 || this->sanet_->get_cond_future_val(goal_iter->first, false) < SA_POP::Default::GOAL_PROB_THRESH) {
        std::ostringstream invalid_goal_str;
        invalid_goal_str << "Goal condition (" << this->sanet_->get_cond_name (goal_iter->first) << ") ";
        invalid_goal_str << "with probability, " << this->sanet_->get_cond_future_val(goal_iter->first, false) << ", ";
        invalid_goal_str << "does not meet goal probability criteria.  Skipping planning.";
        SA_POP_DEBUG_STR(SA_POP_DEBUG_TEMP, invalid_goal_str.str ());

        return false;
      }
    }
  }

  // Set planning strategy goals and satisfy open conditions.
  this->plan_strat_->set_goals (goal.goal_conds);

  if (this->plan_strat_->satisfy_open_conds ()) {
    this->plan_ = this->working_plan_->get_plan ();

    this->notify_plan_changed ();
    return true;
  }

  return false;
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
  this->trial_results_.num_plans++;

  // Get plan EU.
  SA_POP::Utility plan_eu = this->calc_plan_eu (plan);

//*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****
  // Output plan EUs
  //std::ostringstream plan_eu_str;
  //plan_eu_str << "Plan (EU = " << plan_eu << ") ";
  //SA_POP_DEBUG_STR(SA_POP_DEBUG_TEMP, plan_eu_str.str ());
//*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****

  // Ignore first plan.
  if (this->trial_results_.num_plans <= 1) {
    // First plan.
    this->trial_results_.num_plans = 1;

    // Save EU for comparison with following plans.
    this->trial_init_plan_eu = plan_eu;

    // Increment counter of trials with initial plan.
    this->run_results_.num_init_plans++;

    // Ignore plan.
    return;
  }


//*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****
  // If this plan has the same EU as first plan, ignore.
  // WARNING:  This assumes that all plans will have EUs
  //           that differ by at least a small percentage
  //           unless they are the same plan with unnecessary
  //           support for initially true conditions.
  if (plan_eu >= ((1.0 - SA_POP::Default::PERCENT_DIFF_EU) * this->trial_init_plan_eu) &&
    plan_eu <= ((1.0 + SA_POP::Default::PERCENT_DIFF_EU) * this->trial_init_plan_eu))
  {
    // Decrement plan counter, so this plan is not counted.
    this->trial_results_.num_plans--;

//*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****
    // Output plan EUs
    //std::ostringstream ignore_plan_str;
    //ignore_plan_str << "Ignoring plan (EU = " << plan_eu << ") ";
    //ignore_plan_str << "within ignore percentage/multiplier (" << SA_POP::Default::PERCENT_DIFF_EU << ") ";
    //ignore_plan_str << "of initial plan (EU = " << this->trial_init_plan_eu << ").";
    //SA_POP_DEBUG_STR(SA_POP_DEBUG_TEMP, ignore_plan_str.str ());
//*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****

    // Ignore plan.
    return;
  }
//*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****TEMP*****


  // Second plan is SA-POP preferred plan.
  if (this->trial_results_.num_plans == 2) {
    // Current maximum plan EU is this (preferred) plan's EU.
    this->trial_results_.pref_plan_eu = this->trial_results_.max_plan_eu = plan_eu;

    // Increment counter of trials with preferred plan.
    this->run_results_.num_pref_plans++;

    // Nothing else to do for first plan.
    return;
  }

  // Third or more is an alternate plan.

  // If this is the first alternate plan (third plan generated),
  // increment counter of trials with at least one alternate plan.
  if (this->trial_results_.num_plans == 3) {
    this->run_results_.num_alt_plans++;
  }

  // If this plan has a higher EU than current max, update max.
  if (plan_eu > this->trial_results_.max_plan_eu)
    this->trial_results_.max_plan_eu = plan_eu;
};

// Initialize experiment (set values for initial conditions and create goal).
SA_POP::Goal Exp_EU_Planner::exp_init (SA_POP::Exp_EU_Trial_Params params)
{
  // Check goal utility min & max for consistency.
  if (params.util_min > params.util_max) {
    std::cerr << "SA-POP ERROR in Exp_EU_Planner::exp_init (): goal utility min is >= goal utility max";
    throw "SA-POP ERROR in Exp_EU_Planner::exp_init (): goal utility min is >= goal utility max";
  }


  SA_POP::Goal goal;
  goal.goal_id = "ExperimentGeneratedGoalID";
  goal.name = "Experiment generated goal";
  goal.abs_times.clear ();
  goal.rel_times.clear ();
  goal.goal_conds.clear ();
  goal.start_window = std::make_pair (0, 0);

  // Get all conditions.
  SA_POP::CondSet conds = this->sanet_->get_all_conds ();

  // Check number of goal conditions for consistency.
  if (params.num_goal_conds > conds.size ()) {
    std::cerr << "SA-POP ERROR in Exp_EU_Planner::exp_init (): more goal conditions than number of goals";
    throw "SA-POP ERROR in Exp_EU_Planner::exp_init (): more goal conditions than number of goals";
  }

  // Initialize random number generators.
  boost::uniform_int<> goal_choice_dist(0,  conds.size () - 1);
  boost::variate_generator<boost::mt19937&, boost::uniform_int<> > gen_goal_choice(this->rand_gen_, goal_choice_dist);
  boost::uniform_int<> goal_util_dist(params.util_min, params.util_max);
  boost::variate_generator<boost::mt19937&, boost::uniform_int<> > gen_goal_util(this->rand_gen_, goal_util_dist);
  boost::uniform_real<> cond_init_dist(0.0, 1.0);
  boost::variate_generator<boost::mt19937&, boost::uniform_real<> > gen_cond_init(this->rand_gen_, cond_init_dist);

  // Set goals.
  std::set<SA_POP::CondID> goal_ids;
  goal_ids.clear ();
  for (size_t goal_ctr = 0; goal_ctr < params.num_goal_conds; goal_ctr++) {
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
    if (init_check <= params.percent_init_true) {
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
  this->log_trials_out_ << this->trial_results_.pref_plan_eu << "\t";
  this->log_trials_out_ << this->trial_results_.max_plan_eu << "\t";
  this->log_trials_out_ << (this->trial_results_.num_plans - 2);
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
  this->log_runs_out_ << "SANet name" << "\t";
  this->log_runs_out_ << "Number of goal conditions" << "\t";
  this->log_runs_out_ << "Percent initial conditions true" << "\t";
  this->log_runs_out_ << "Goal utility min" << "\t";
  this->log_runs_out_ << "Goal utility max" << "\t";
  this->log_runs_out_ << "Plan EU ratio (preferred : best) Mean" << "\t";
  this->log_runs_out_ << "Plan EU ratio (preferred : best) Std Dev" << "\t";
  this->log_runs_out_ << "Trial plans generated Mean" << "\t";
  this->log_runs_out_ << "Trial plans generated Std Dev" << "\t";
  this->log_runs_out_ << "Trial attempts" << "\t";
  this->log_runs_out_ << "Trials w/ initial plan" << "\t";
  this->log_runs_out_ << "Trials w/ preferred plan" << "\t";
  this->log_runs_out_ << "Trials w/ alternate plan(s)";
  this->log_runs_out_ << std::endl;
};

// Output run statistics to log.
void Exp_EU_Planner::log_run_stats (SA_POP::Exp_EU_Trial_Params trial_params)
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

  // Calculate cumulative statistics for trials.
  boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::mean, boost::accumulators::tag::variance> > acc_pref_best_ratio;
  boost::accumulators::accumulator_set<double, boost::accumulators::stats<boost::accumulators::tag::mean, boost::accumulators::tag::variance> > acc_num_plans;
  for (std::list<Exp_EU_Trial_Results>::iterator trial_iter = this->run_results_.trials.begin (); trial_iter != this->run_results_.trials.end (); trial_iter++) {
    double pref_best_ratio = (*trial_iter).pref_plan_eu / (*trial_iter).max_plan_eu;

    acc_pref_best_ratio (pref_best_ratio);
    acc_num_plans ((*trial_iter).num_plans);
  }

  // Output run statistics to file.
  this->log_runs_out_ << this->run_results_.net_name << "\t";
  this->log_runs_out_ << trial_params.num_goal_conds << "\t";
  this->log_runs_out_ << trial_params.percent_init_true << "\t";
  this->log_runs_out_ << trial_params.util_min << "\t";
  this->log_runs_out_ << trial_params.util_max << "\t";
  this->log_runs_out_ << boost::accumulators::mean(acc_pref_best_ratio) << "\t";
  this->log_runs_out_ << std::sqrt(boost::accumulators::variance(acc_pref_best_ratio)) << "\t";
  this->log_runs_out_ << boost::accumulators::mean(acc_num_plans) << "\t";
  this->log_runs_out_ << std::sqrt(boost::accumulators::variance(acc_num_plans)) << "\t";
  this->log_runs_out_ << this->run_results_.num_trial_attempts << "\t";
  this->log_runs_out_ << this->run_results_.num_init_plans << "\t";
  this->log_runs_out_ << this->run_results_.num_pref_plans << "\t";
  this->log_runs_out_ << this->run_results_.num_alt_plans;
  this->log_runs_out_ << std::endl;
};




