// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SA_Builder.h
 *
 * This file contains the definition of the SA_Builder concrete class,
 * which implements a Builder creating SA_Planner and associated
 * objects for planning with spreading activation networks and scheduling
 * with precedence graphs.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_SA_BUILDER_H_
#define SA_POP_SA_BUILDER_H_

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "SA_POP_Types.h"
#include "Builder.h"
#include "Planner.h"
#include "SANet/SANet.h"
#include "SA_PlanHeuristics.h"
#include "SA_PlanStrategy.h"
#include "SA_SchedStrategy.h"
#include "SA_WorkingPlan.h"
#include "TaskMap.h"
#include "SA_Builder_Export.h"

namespace SA_POP {

  /**
   * @class SA_Builder
   *
   * @brief Builder concrete class for creating SA_Planner and
   *        associated objects for planning with spreading activation networks
   *        and scheduling with precedence graphs.
   */
  class SA_BUILDER_Export SA_Builder : public Builder {
//  class SA_Builder : public Builder {
  public:
    /// Constructor.
    SA_Builder (void);

    /// Destructor.
    virtual ~SA_Builder (void);

    /// Reset for building a new set of SA-POP objects.
    /**
     * WARNING: Assumes that planner has been handed off and will be
     * deleted by someone else.
     */
    virtual void reset (void);

    /// Get Planner object.
    /**
     * @return  Pointer to the configured planner object.
     */
    virtual Planner *get_planner (void);


    // ************************************************************************
    // Problem construction methods (tasks, conditions, & links between them).
    // ************************************************************************

    /// Add a task.
    /**
     * @param id  The unique task id.
     *
     * @param prior_prob  The prior probability of this task succeeding.
     *
     * @param name  The task name.
     *
     * @exception Duplicate_Task  The given task id already exists.
     */
    virtual void add_task (TaskID id, double prior_prob,
      std::string name = "");

    /// Add a condition.
    /**
     * @param id  The unique condition id.
     *
     * @param name  The condition name.
     *
     * @param utility  The (goal) utility of the condition.
     *
     * @param init_prob_true  The initial probability that the condition
     *                        is true.
     *
     * @param cond_kind  The type of condition.
     *
     * @exception Duplicate_Cond  The given condition id already exists.
     */
    virtual void add_cond (CondID id, Utility utility = 0,
      double init_prob_true = 0, std::string name = "", CondKind cond_kind = SA_POP::DATA);

    /// Add a link between a precondition and task.
    /**
     * @param cond_id  The precondition id.
     *
     * @param task_id  The task id.
     *
     * @param port  The receiving port id (on the task for this precondition).
     *
     * @param true_prob  Conditional probability of task success given
     *                   condition = true.
     *
     * @param false_prob  Conditional probability of task success given
     *                    condition = false.
     *
     * @exception Unknown_Cond  Unknown condition id.
     */
    virtual void set_precond (CondID cond_id, TaskID task_id,
      PortID port, double true_prob, double false_prob);

    /// Add a link between a task and an effect.
    /**
     * @param cond_id  The effect condition id.
     *
     * @param task_id  The task id.
     *
     * @param port  The sending port id (on the task for this effect).
     *
     * @param weight  Positive probability that task sets condition to true, or
     *                negative of probability that task sets condition to false.
     */
    virtual void set_effect (TaskID task_id, CondID cond_id,
      PortID port, double weight);



    // ************************************************************************
    // TaskMap construction methods (resources, task->implementations,
    // and implementation->resources).
    // ************************************************************************

    /// Add a resource.
    /**
     * @param resource  The resource info.
     */
    virtual void add_resource (Resource resource);

    /// Add an implementation.
    /**
     * @param task_impl  The task implementation info.
     */
    virtual void add_task_impl (TaskImpl *task_impl);

    /// Associate a task with a task implementation.
    /**
     * @param task_id  ID of the task.
     *
     * @param task_impl_id  ID of the task implementation.
     *
     * @param duration  The duration of the task implementation for this task.
     */
    virtual void assoc_task_with_impl (TaskID task_id, TaskImplID task_impl_id,
      TimeValue duration);

    /// Associate a task implementation with its utilization of a resource.
    /**
     * @param impl_id  ID of the task implementation.
     *
     * @param resource_id  ID of the associated resource.
     *
     * @param resource_usage  The quantity of resource used.
     */
    virtual void assoc_impl_with_resource (TaskImplID impl_id,
      ResourceID resource_id, ResourceValue resource_usage);

  protected:
    /// Is the builder initialized?
    bool is_init_;

    /// Planner object to centralize/mediate planning.
    Planner *planner_;

    /// Spreading activation network.
    SANet::Network *sanet_;

    /// Condition choice strategy used by PlanStrategy.
    SA_CondStrategy *cond_choice_;

    /// Task choice strategy used by PlanStrategy.
    SA_TaskStrategy *task_choice_;

    /// Task implementation choice strategy used by PlanStrategy.
    SA_ImplStrategy *impl_choice_;

    /// PlanStrategy object for planning.
    SA_PlanStrategy *plan_strat_;

    /// SchedStrategy object for scheduling.
    SA_SchedStrategy *sched_strat_;

    /// WorkingPlan object for holding plan in progress.
    SA_WorkingPlan *working_plan_;

    /// TaskMap object for associating tasks with implementations and resources.
    TaskMap *task_map_;


    /// Create SA-POP objects.
    virtual void init (void);
  };

};  /* SA_POP namespace */

#endif /* SA_POP_SA_BUILDER_H_ */
