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
#include <list>
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

  // Default values for use in EU Experiment.
  namespace Default {
    /// Threshold (>=) for probability of a goal to be considered satisfied.
    const ::SA_POP::Probability GOAL_PROB_THRESH = 0.6;

    /// Percentage/multiplier threshold for two plan EUs to be considered different.
    /// (WARNING:  This should be a very small value.)
    const double PERCENT_DIFF_EU = 0.000001;
  };

  /// Container for experimental trial parameters.
  struct Exp_EU_Trial_Params {
    /// Number of goal conditions to generate.
    size_t num_goal_conds;
    
    /// Percentage of conditions to set initial value to true.
    double percent_init_true;

    /// Minimum goal utility (uniform random choice in range [util_min, util_max]).
    int util_min;

    /// Maximum goal utility (uniform random choice in range [util_min, util_max]).
    int util_max;

    bool operator== (const Exp_EU_Trial_Params &s) const { return (this->num_goal_conds == s.num_goal_conds && this->percent_init_true == s.percent_init_true && this->util_min == s.util_min && this->util_max == s.util_max); };
    bool operator!= (const Exp_EU_Trial_Params &s) const { return !(*this == s); };
    bool operator< (const Exp_EU_Trial_Params &s) const
    {
      if (this->num_goal_conds == s.num_goal_conds) {
        if (this->percent_init_true == s.percent_init_true) {
          if (this->util_min == s.util_min) {
            return this->util_max < s.util_max;
          }
          return this->util_min < s.util_min;
        }
        return this->percent_init_true < s.percent_init_true;
      }
      return this->num_goal_conds < s.num_goal_conds;
    };
  };

  /// Container for experimental results of a trial.
  struct Exp_EU_Trial_Results {
    /// Expected utility of SA-POP preferred plan.
    SA_POP::Utility pref_plan_eu;

    /// Highest expected utility of any (schedulable/valid) plan (including preferred plan).
    SA_POP::Utility max_plan_eu;

    /// Number of plans generated.
    size_t num_plans;

    /// Trial goal.
    SA_POP::Goal goal;

    bool operator== (const Exp_EU_Trial_Results &s) const { return (this->pref_plan_eu == s.pref_plan_eu && this->max_plan_eu == s.max_plan_eu && this->num_plans == s.num_plans && this->goal == s.goal); };
    bool operator!= (const Exp_EU_Trial_Results &s) const { return !(*this == s); };
    bool operator< (const Exp_EU_Trial_Results &s) const
    {
      if (this->pref_plan_eu == s.pref_plan_eu) {
        if (this->max_plan_eu == s.max_plan_eu) {
          if (this->num_plans == s.num_plans) {
            return this->goal < s.goal;
          }
          return this->num_plans < s.num_plans;
        }
        return this->max_plan_eu < s.max_plan_eu;
      }
      return this->pref_plan_eu < s.pref_plan_eu;
    };
  };

  /// Container for a series of experimental results.
  struct Exp_EU_Run_Results {
    /// Run counter for number of trial attempts.
    size_t num_trial_attempts;

    /// Run counter for number of trials with initial plan generated.
    size_t num_init_plans;

    /// Run counter for number of trials with preferred plan generated.
    size_t num_pref_plans;

    /// Run counter for number of trials with at least one alternate plan generated.
    size_t num_alt_plans;

    /// Name of network.
    std::string net_name;

    /// List of trial results.
    std::list<Exp_EU_Trial_Results> trials;

    bool operator== (const Exp_EU_Run_Results &s) const { return (this->num_trial_attempts == s.num_trial_attempts && this->num_init_plans == s.num_init_plans && this->num_pref_plans == s.num_pref_plans && this->num_alt_plans == s.num_alt_plans && this->net_name == s.net_name && this->trials == s.trials); };
    bool operator!= (const Exp_EU_Run_Results &s) const { return !(*this == s); };
    bool operator< (const Exp_EU_Run_Results &s) const
    {
      if (this->num_trial_attempts == s.num_trial_attempts) {
        if (this->num_init_plans == s.num_init_plans) {
          if (this->num_pref_plans == s.num_pref_plans) {
            if (this->num_alt_plans == s.num_alt_plans) {
              if (this->net_name == s.net_name) {
                return this->trials < s.trials;
              }
              return this->net_name < s.net_name;
            }
            return this->num_alt_plans < s.num_alt_plans;
          }
          return this->num_pref_plans < s.num_pref_plans;
        }
        return this->num_init_plans < s.num_init_plans;
      }
      return this->num_trial_attempts < s.num_trial_attempts;
    };
  };



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
     * @param log_trials_filename  Name of file to output trial statistics to.
     *
     * @param log_runs_filename  Name of file to output cumulative statistics from run to.
     *
     * @param log_runs_filename  Name of current spreading activation network for logging.
     *
     * @param trial_params  Parameters for random initialization of each trial.
     *
     * @param max_trial_attempts  Maximum number of trial attempts to execute in order to reach assigned number of valid trials.
     *
     * @param num_trials  Number of valid trials to attempt to achieve (each with randomly chosen initial conditions and goal).
     *
     * @param do_log_headers  Flag to output header lines to log files (if true).
     */
    virtual SA_POP::Exp_EU_Run_Results exp_run (std::string log_trials_filename, std::string log_runs_filename,
      std::string net_name, SA_POP::Exp_EU_Trial_Params trial_params, size_t max_trial_attempts,
      size_t num_trials = 1, bool do_log_headers = false);
    

    // ************************************************************************
    // Recursive planning/scheduling methods.
    // ************************************************************************

    /// Run planning.
    /**
     * @param sa_max_steps  Maximum steps to run spreading activation.
     *
     * @param goal  Goal for which to plan.
     *
     * @return  True if planning succeeded, false otherwise.
     */
    virtual bool plan (size_t sa_max_steps, SA_POP::Goal goal);

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

    /// Trial EU value for initial (thrown out) plan.
    SA_POP::Utility trial_init_plan_eu;

    /// Results of current experimental trial.
    SA_POP::Exp_EU_Trial_Results trial_results_;

    /// Results of current experimental run (multiple trials).
    SA_POP::Exp_EU_Run_Results run_results_;

    /// Trial counter for number of plans generated.
//    size_t trial_num_plans_;

    /// Run counter for number of trial attempts.
//    size_t run_num_trial_attempts_;

    /// Run counter for number of trials with initial plan generated.
//    size_t run_num_init_plans_;

    /// Run counter for number of trials with preferred plan generated.
//    size_t run_num_pref_plans_;

    /// Run counter for number of trials with at least one alternate plan generated.
//    size_t run_num_alt_plans_;

    /// Expected utility of SA-POP preferred plan.
//    SA_POP::Utility pref_plan_eu_;

    /// Highest expected utility of any (schedulable/valid) plan (including preferred plan).
//    SA_POP::Utility max_plan_eu_;


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
    /**
     * @param trial_params  Parameters used for random initialization of trials in this experimental run.
     */
    virtual void log_run_stats (SA_POP::Exp_EU_Trial_Params trial_params);

    /// Output run header to log.
    virtual void log_run_header (void);

    /// Initialize experiment (set values for initial conditions and
    /// create goal).
    /**
     * @param params  Parameters for random initialization of trial.
     *
     * @return  Goal to use in current experimental run.
     */
    virtual SA_POP::Goal exp_init (SA_POP::Exp_EU_Trial_Params params);
  };

};  /* SA_POP namespace */

#endif /* SA_POP_EXP_EU_PLANNER_H_ */
