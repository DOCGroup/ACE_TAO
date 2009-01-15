// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SchedStrategy.h
 *
 * This file contains the SchedStrategy abstract base class definition for
 * the high-level scheduling algorithm.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_SCHED_STRATEGY_H_
#define SA_POP_SCHED_STRATEGY_H_

#include "SA_POP_Types.h"

namespace SA_POP {

  // Forward declaration of Planner.
  class Planner;

  /**
   * @class SchedStrategy
   *
   * @brief  Abstract base class for the high-level scheduling algorithm.
   */
  class SchedStrategy {
  public:
    /// Constructor.
    /**
     * @param planner  Planner object to use.
     */
    SchedStrategy (SA_POP::Planner *planner)
      : planner_ (planner) { };

    /// Destructor.
    virtual ~SchedStrategy (void) { };

    /// Recursively satisfy all scheduling constraints (and continue
    /// satisfaction of open conditions by recursive call back to planning).
    /**
     * @param task_inst  Current task instance being tried in the plan.
     *
     * @return  True if fully satisfied plan found, false otherwise.
     */
    virtual bool satisfy_sched (TaskInstID task_inst) = 0;

    ///reset SchedStrategy

    virtual void reset(void) = 0;

    /// Satisfy fully instantiated plan.
    /**
     * @param task_inst  Current task instance being tried in the plan.
     *
     * @return  True if fully satisfied plan found, false otherwise.
     */
    virtual bool satisfy_full_sched () = 0;

  protected:
    /// Pointer to Planner object.
    SA_POP::Planner *planner_;
  };

};  /* SA_POP namespace */

#endif /* SA_POP_SCHED_STRATEGY_H_ */
