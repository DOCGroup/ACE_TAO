// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SA_PlanHeuristics.h
 *
 * This file contains the definitions of concrete classes,
 * which implement plan heuristic strategies.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_SA_PLAN_HEURISTICS_H_
#define SA_POP_SA_PLAN_HEURISTICS_H_


#include "SA_POP_Types.h"
#include "PlanHeuristics.h"

namespace SA_POP {

  /**
   * @class SA_CondStrategy
   *
   * @brief CondStrategy concrete class for a PlanHeuristic that
   *        chooses an open condition in the plan to satisfy next.
   */
  class SA_CondStrategy : public CondStrategy {
  public:
    /// Constructor.
    /**
     * @param planner  Planner object to use.
     */
    SA_CondStrategy (SA_POP::Planner *planner);

    /// Destructor.
    virtual ~SA_CondStrategy (void);

    /// Choose the next open condition to satisfy.
    /**
     * @param open_conds  Open conditions in the plan.
     *
     * @return  Next open condition to satisfy.
     */
    virtual Condition choose_cond (const OpenCondMap &open_conds);

	virtual Condition choose_cond_suspension (const OpenCondMap &open_conds);
  
	virtual Condition choose_cond_suspension_most_constrained (const OpenCondMap &open_conds);
  };

  /**
   * @class SA_TaskStrategy
   *
   * @brief TaskStrategy concrete class for a PlanHeuristic that
   *        chooses an ordering of tasks for the choice to satisfy an
   *        open condition.
   */
  class SA_TaskStrategy : public TaskStrategy {
  public:
    /// Constructor.
    /**
     * @param planner  Planner object to use.
     */
    SA_TaskStrategy (SA_POP::Planner *planner);

    /// Destructor.
    virtual ~SA_TaskStrategy (void);

    /// Choose the (ordering of) task(s) to satisfy an open condition.
    /**
     * @param open_cond  Open condition to satisfy.
     *
     * @return  Sorted list of tasks that satisfy given condition.
     */
    virtual TaskChoiceList choose_task (Condition open_cond);
  
	/// Choose the (ordering of) task(s) to satisfy an open condition.  The cool way
	 /**
     * @param open_cond  Open condition to satisfy.
     *
     * @return  Sorted list of tasks that satisfy given condition.
     */
	virtual TaskChoiceList choose_task_fair (Condition open_cond);
  };

  /**
   * @class SA_ImplStrategy
   *
   * @brief ImplStrategy concrete class for a PlanHeuristic that
   *        chooses an ordering of implementations to try for a given
   *        task instance.
   */
  class SA_ImplStrategy : public ImplStrategy {
  public:
    /// Constructor.
    /**
     * @param planner  Planner object to use.
     */
    SA_ImplStrategy (SA_POP::Planner *planner);

    /// Destructor.
    virtual ~SA_ImplStrategy (void);

    /// Choose the (ordering of) task implementation(s) for a task instance.
    /**
     * @param task_inst  Task instance for which to choose implementations.
     *
     * @return  Ordered list of implementations for the given task instance.
     */
    virtual TaskImplList choose_impl (TaskInstID task_inst);
  };




};  /* SA_POP namespace */


#endif /* SA_POP_SA_PLAN_HEURISTICS_H_ */
