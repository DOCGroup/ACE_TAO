// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  Exp_EU_SchedStrategy.h
 *
 * This file contains the Exp_EU_SchedStrategy concrete class definition, which
 * implements a SchedStrategy for "roadblock" scheduling that only updates the
 * precedence graph without any other scheduling and which can randomly choose a
 * "roadblock" of a set of tasks in the working plan.  It considers any working
 * plan with the "roadblock" set of tasks to be unschedulable and any other
 * working plan to be schedulable.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_EXP_EU_SCHED_STRATEGY_H_
#define SA_POP_EXP_EU_SCHED_STRATEGY_H_

#include<map>
#include "SA_POP_Types.h"

//#include "SchedStrategy.h"
#include "SA_SchedStrategy.h"

#include "PlanCommands.h"

namespace SA_POP {

  /**
   * @class Exp_EU_SchedStrategy
   *
   * @brief  Concrete class of SchedStrategy for "roadblock" scheduling
   *         that only updates precedence graph without any other
   *         scheduling and which can randomly choose a "roadblock"
   *         of a set of tasks in the working plan.  It considers
   *         any working plan with the "roadblock" set of tasks to be
   *         unschedulable and any other working plan to be schedulable.
   */
  class Exp_EU_SchedStrategy : public SA_SchedStrategy {
  public:
    /// Constructor.
    /**
     * @param planner  Planner object to use.
     */
    Exp_EU_SchedStrategy (SA_POP::Planner *planner);

    /// Destructor.
    virtual ~Exp_EU_SchedStrategy (void);

    /// Recursively satisfy all scheduling constraints (and continue
    /// satisfaction of open conditions by recursive call back to planning).
    /**
     * @param task_inst  Current task instance being tried in the plan.
     *
     * @return  True if fully satisfied plan found, false otherwise.
     */
    virtual bool satisfy_sched (TaskInstID task_inst);

    /// Satisfy fully instantiated plan.
    /**
     * @param task_inst  Current task instance being tried in the plan.
     *
     * @return  True if fully satisfied plan found, false otherwise.
     */
    virtual bool satisfy_full_sched ();

  protected:
    
  };

};  /* SA_POP namespace */

#endif /* SA_POP_EXP_EU_SCHED_STRATEGY_H_ */
