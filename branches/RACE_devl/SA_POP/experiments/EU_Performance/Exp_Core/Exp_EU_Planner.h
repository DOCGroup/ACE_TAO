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


    // ************************************************************************
    // Planning/re-planning methods.
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
    
    /// Replan with new goal.
    /**
     * @param sa_max_steps  Maximum steps to run spreading activation.
     *
     * @param goal  Goal for which to plan.
     *
     * @return  True if planning succeeded, false otherwise.
     */
    virtual bool replan (size_t sa_max_steps, SA_POP::Goal goal);

    /// Replan with current goal.
    /**
     * @param sa_max_steps  Maximum steps to run spreading activation.
     *
     * @return  True if planning succeeded, false otherwise.
     */
    virtual bool replan (size_t sa_max_steps);



    // ************************************************************************
    // Recursive planning/scheduling methods.
    // ************************************************************************

    /// Recursively plan (satisfy all open conditions & schedule constraints).
    /**
     * @return  True if fully satisfied plan found, false otherwise.
     */
    virtual bool recurse_plan (void);

    /// Satisfy scheduling constraints in fully instantiated plan (no
    /// recursive call backs).
    /**
     * @return  True if fully satisfied plan found, false otherwise.
     */
    virtual bool full_sched ();

  };

};  /* SA_POP namespace */

#endif /* SA_POP_EXP_EU_PLANNER_H_ */
