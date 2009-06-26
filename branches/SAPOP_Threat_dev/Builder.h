// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  Builder.h
 *
 * This file contains the Builder abstract base class definition for
 * SA-POP builders, which handle creation and configuration of Planner objects
 * and the associated objects they use/mediate-between.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_BUILDER_H_
#define SA_POP_BUILDER_H_

#include "SA_POP_Types.h"
#include "Planner.h"


namespace SA_POP {

  /**
   * @class Builder
   *
   * @brief Abstract base class of SA-POP builders, which handle creation and
   *        configuration of Planner and associated objects.
   */
  class Builder {
  public:
    /// Constructor.
    Builder (void) {};

    /// Destructor.
    virtual ~Builder (void) {};

    /// Reset for building a new set of SA-POP objects.
    virtual void reset (void) = 0;

    /// Get Planner object.
    /**
     * @return  Pointer to the configured planner object.
     */
    virtual Planner *get_planner (void) = 0;


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
      std::string name = "") = 0;

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
     * @param cond_kind  The type of condition
     *
     * @exception Duplicate_Cond  The given condition id already exists.
     */
    virtual void add_cond (CondID id, Utility utility = 0,
      double init_prob_true = 0, std::string name = "", CondKind cond_kind = SA_POP::DATA) = 0;

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
      PortID port, double true_prob, double false_prob) = 0;

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
      PortID port, double weight) = 0;



    // ************************************************************************
    // TaskMap construction methods (resources, task->implementations,
    // and implementation->resources).
    // ************************************************************************

    /// Add a resource.
    /**
     * @param resource  The resource info.
     */
    virtual void add_resource (Resource resource) = 0;

    /// Add an implementation.
    /**
     * @param task_impl  The task implementation info.
     */
    virtual void add_task_impl (TaskImpl *task_impl) = 0;

    /// Associate a task with a task implementation.
    /**
     * @param task_id  ID of the task.
     *
     * @param task_impl_id  ID of the task implementation.
     *
     * @param duration  The duration of the task implementation for this task.
     */
    virtual void assoc_task_with_impl (TaskID task_id, TaskImplID task_impl_id,
      TimeValue duration) = 0;

    /// Associate a task implementation with its utilization of a resource.
    /**
     * @param impl_id  ID of the task implementation.
     *
     * @param resource_id  ID of the associated resource.
     *
     * @param resource_usage  The quantity of resource used.
     */
    virtual void assoc_impl_with_resource (TaskImplID impl_id,
      ResourceID resource_id, ResourceValue resource_usage) = 0;
  };

};  /* SA_POP namespace */

#endif /* SA_POP_BUILDER_H_ */
