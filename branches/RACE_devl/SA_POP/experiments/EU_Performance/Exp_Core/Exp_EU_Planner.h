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

    /// Run experiment planning.
    /**
     * @param sa_max_steps  Maximum steps to run spreading activation.
     *
     * @param goal  Goal for which to plan.
     *
     * @param pause  True to pause after next plan is generated or
     *               false to find all remaining plans.
     *
     * @return  True if plan generated, false otherwise.
     */
//    virtual bool plan_exp (size_t sa_max_steps, SA_POP::Goal goal, bool pause = false);
    

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
    /// Flag for whether to pause for user input after each plan is generated.
    bool do_pause_;

    /// User in/out interface to use for continuation question after each plan is generated.
    UserInterface::InputCL *input_;

    /// Question to ask user whether to continue after each plan is generated.
    UserInterface::QuestionBool *ques_;
  };

};  /* SA_POP namespace */

#endif /* SA_POP_EXP_EU_PLANNER_H_ */
