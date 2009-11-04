// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  PlanHeuristic.h
 *
 * This file contains the class definitions for the encapsulations of
 * algorithms corresponding to planning/scheduling substeps.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_PLAN_STEP_H_
#define SA_POP_PLAN_STEP_H_

#include "SA_POP_Types.h"
#include "PlanStrategy.h"

namespace SA_POP {

  // Forward declaration of Planner.
  class Planner;

  /**
   * @class CondStrategy
   *
   * @brief CondStrategy abstract base class for a PlanHeuristic that
   *        chooses an open condition in the plan to satisfy next.
   */
  class CondStrategy {
  public:
    /// Constructor.
    /**
     * @param planner  Planner object to use.
     */
    CondStrategy (SA_POP::Planner *planner)
      : planner_ (planner) { };

    /// Destructor.
    virtual ~CondStrategy (void) { };

    /// Choose the next open condition to satisfy.
    /**
     * @param open_conds  Open conditions in the plan.
     *
     * @return  Next open condition to satisfy.
     */
    virtual Condition choose_cond (const OpenCondMap &open_conds) = 0;
	virtual Condition choose_cond_suspension (const OpenCondMap &open_conds) = 0;
	virtual Condition choose_cond_suspension_most_constrained (const OpenCondMap &open_conds)=0;

  protected:
    /// Pointer to Planner object.
    SA_POP::Planner *planner_;
  };

  /**
   * @class TaskStrategy
   *
   * @brief TaskStrategy abstract base class for a PlanHeuristic that
   *        chooses an ordering of tasks for the choice to satisfy an
   *        open condition.
   */
  class TaskStrategy {
  public:
    /// Constructor.
    /**
     * @param planner  Planner object to use.
     */
    TaskStrategy (SA_POP::Planner *planner)
      : planner_ (planner) { };

    /// Destructor.
    virtual ~TaskStrategy (void) { };

    /// Choose the (ordering of) task(s) to satisfy an open condition.
    /**
     * @param open_cond  Open condition to satisfy.
     *
     * @return  Sorted list of tasks that satisfy given condition.
     */
    virtual TaskChoiceList choose_task (Condition open_cond) = 0;

    /// Choose the (ordering of) task(s) to satisfy an open condition.  Better
    /**
     * @param open_cond  Open condition to satisfy.
     *
     * @return  Sorted list of tasks that satisfy given condition.
     */
	virtual TaskChoiceList choose_task_fair (Condition open_cond) = 0;

  protected:
    /// Pointer to Planner object.
    SA_POP::Planner *planner_;
  };

  /**
   * @class ImplStrategy
   *
   * @brief ImplStrategy abstract base class for a PlanHeuristic that
   *        chooses an ordering of implementations to try for a given
   *        task instance.
   */
  class ImplStrategy {
  public:
    /// Constructor.
    /**
     * @param planner  Planner object to use.
     */
    ImplStrategy (SA_POP::Planner *planner)
      : planner_ (planner) { };

    /// Destructor.
    virtual ~ImplStrategy (void) { };

    /// Choose the (ordering of) task implementation(s) for a task instance.
    /**
     * @param task_inst  Task instance for which to choose implementations.
     *
     * @return  Ordered list of implementations for the given task instance.
     */
    virtual TaskImplList choose_impl (TaskInstID task_inst) = 0;

  protected:
    /// Pointer to Planner object.
    SA_POP::Planner *planner_;
  };

};  /* SA_POP namespace */

#endif /* SA_POP_PLAN_STEP_H_ */
