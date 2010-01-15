// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  Exp_EU_Planner.h
 *
 * This file contains the Exp_EU_Planner class definition for a planner,
 * which implements a Planner that finds all possible plans during planning
 * and outputs statistics comparing expected utility of each to the SA-POP
 * preferred plan.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_EXP_EU_PLANNER_H_
#define SA_POP_EXP_EU_PLANNER_H_


#include <string>
#include <set>
#include <map>
#include <fstream>

#include <boost/random.hpp>

#include "SA_POP_Types.h"
#include "Planner.h"
#include "InputCL.h"

//#include "PlanStrategy.h"
//#include "SchedStrategy.h"
//#include "WorkingPlan.h"
//#include "TaskMap.h"
//#include "OutAdapter.h"
//#include "PlanCommands.h"
//#include "SANet/SANet.h"

namespace SA_POP {

  /**
   * @class Exp_EU_Planner
   *
   * @brief  A derived Planner class that finds all possible plans during
   *         planning and outputs statistics comparing expected utility
   *         of each to the SA-POP preferred plan.
   */
  class Exp_EU_Planner : public Planner {
  public:

    /// Constructor.
    Exp_EU_Planner (void);

    /// Destructor.
    virtual ~Exp_EU_Planner (void);


    /// Set to pause and ask user whether to continue after each plan is generated.
    /**
     * @param input_cl  User in/out interface to use.
     *
     * @param ques  Question to ask user whether to continue.
     */
    virtual void set_pause (UserInterface::InputCL *input_cl, UserInterface::QuestionBool *ques);

    /// Set to generate all plans without user input (instead of asking whether to continue after each plan is generated).
    virtual void unset_pause (void);

    // ************************************************************************
    // Planning/re-planning methods.
    // ************************************************************************

    /// Perform experimental trial(s).
    /**
     * @param sa_max_steps  Maximum steps to run spreading activation.
     *
     * @param log_trials_filename  Name of file to output trial statistics to.
     *
     * @param log_runs_filename  Name of file to output cumulative statistics from run to.
     *
     * @param num_goal_conds  Number of goal conditions to generate.
     *
     * @param percent_init_true  Percentage of conditions to set initial value to true.
     *
     * @param util_min  Minimum goal utility (uniform random choice in range [util_min, util_max]).
     *
     * @param util_max  Maximum goal utility (uniform random choice in range [util_min, util_max]).
     *
     * @param max_trial_attempts  Maximum number of trial attempts to execute in order to reach assigned number of valid trials.
     *
     * @param num_trials  Number of valid trials to attempt to achieve (each with randomly chosen initial conditions and goal).
     *
     * @param do_log_headers  Flag to output header lines to log files (if true).
     */
    virtual void exp_run (size_t sa_max_steps, std::string log_trials_filename,
      std::string log_runs_filename, size_t num_goal_conds, double percent_init_true,
      size_t util_min, size_t util_max, size_t max_trial_attempts,
      size_t num_trials = 1, bool do_log_headers = false);
    

    // ************************************************************************
    // Recursive planning/scheduling methods.
    // ************************************************************************

    /// Satisfy scheduling constraints in fully instantiated plan (no
    /// recursive call backs).
    /**
     * @return  True if fully satisfied plan found, false otherwise.
     */
    virtual bool full_sched ();

  protected:
    // Random number generator.
    boost::mt19937 rand_gen_;

    /// Flag for whether to pause for user input after each plan is generated.
    bool do_pause_;

    /// User in/out interface to use for continuation question after each plan is generated.
    UserInterface::InputCL *input_;

    /// Question to ask user whether to continue after each plan is generated.
    UserInterface::QuestionBool *ques_;

    /// Flag for whether to output statistics to files.
    bool do_stats_out_;

    /// Output file stream for logging trial statistics.
    std::ofstream log_trials_out_;

    /// Output file stream for logging trial statistics.
    std::ofstream log_runs_out_;

    /// Trial counter for number of plans generated.
    size_t trial_num_plans_;

    /// Run counter for number of trial attempts.
    size_t run_num_trial_attempts_;

    /// Run counter for number of trials with initial plan generated.
    size_t run_num_init_plans_;

    /// Run counter for number of trials with preferred plan generated.
    size_t run_num_pref_plans_;

    /// Run counter for number of trials with at least one alternate plan generated.
    size_t run_num_alt_plans_;

    /// Expected utility of SA-POP preferred plan.
    SA_POP::Utility pref_plan_eu_;

    /// Highest expected utility of any (schedulable/valid) plan (including preferred plan).
    SA_POP::Utility max_plan_eu_;


    // ************************************************************************
    // Internal helper methods.
    // ************************************************************************

    /// Reset all planning statistics for an individual trial.
    virtual void reset_trial_stats (void);

    /// Reset all planning statistics for an individual experimental run.
    virtual void reset_run_stats (void);

    /// Add plan to tracking statistics.
    virtual void track_stats (SA_POP::Plan plan);

    /// Output trial header to log.
    virtual void log_trial_header (void);

    /// Output trial statistics to log.
    virtual void log_trial_stats (void);

    /// Output run statistics to log.
    virtual void log_run_stats (void);

    /// Output run header to log.
    virtual void log_run_header (void);

    /// Initialize experiment (set values for initial conditions and
    /// create goal).
    /**
     * @param num_goal_conds  Number of goal conditions to generate.
     *
     * @param percent_init_true  Percentage of conditions to set initial value to true.
     *
     * @param util_min  Minimum goal utility (uniform random choice in range [util_min, util_max]).
     *
     * @param util_max  Maximum goal utility (uniform random choice in range [util_min, util_max]).
     *
     * @return  Goal to use in current experimental run.
     */
    virtual SA_POP::Goal exp_init (size_t num_goal_conds, double percent_init_true, size_t util_min, size_t util_max);
  };

};  /* SA_POP namespace */

#endif /* SA_POP_EXP_EU_PLANNER_H_ */
