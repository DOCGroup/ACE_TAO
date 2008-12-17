// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  WorkingPlan.h
 *
 * This file contains the WorkingPlan abstract base class definition for the
 * encapsulation of a plan (partial or complete).
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_WORKING_PLAN_H_
#define SA_POP_WORKING_PLAN_H_

#include "SA_POP_Types.h"
#include "PlanCommands.h"

namespace SA_POP {

  // Forward declaration of Planner.
  class Planner;

  /**
   * @class WorkingPlan
   *
   * @brief Abstract base class for the encapsulation of a plan (partial or
   *        complete).
   */
  class WorkingPlan {
  public:
    /// Constructor.
    /**
     * @param planner  Planner object to use.
     */
    WorkingPlan (SA_POP::Planner *planner)
      : planner_ (planner) { };

    /// Destructor.
    virtual ~WorkingPlan (void) { };

	/// Reset Working Plan.
    /**	
     * 
     */
    virtual void reset_plan () = 0;

    /// Set goal.
    /**	
     * @param goal  Goal of this working plan.
     */
    virtual void set_goal (const SA_POP::Goal &goal) = 0;

    /// Get current plan.
    /**	
     * @return  Reference to current plan.
     */
    virtual const Plan& get_plan (void) = 0;

    /// Get set of task instances for a precedence relation to a task instance.
    /**
     * @param task_inst  Task instance for which to get precedence set.
     *
     * @param prec_rel  Precedence relation.
     *
     * @return  Set of task instances for given precedence relation to given
     *          task instance.
     */
    virtual const TaskInstSet *get_prec_set (TaskInstID task_inst,
      PrecedenceRelation prec_rel) = 0;

    /// Get task id of a task instance.
    /**
     * @param inst_id  The task instance id.
     *
     * @return  The task id of this task instance.
     */
    virtual TaskID get_task_from_inst (TaskInstID inst_id) = 0;

    /// Get task implementation id of a task instance.
    /**
     * @param inst_id  The task instance id.
     *
     * @return  The task implementation id of this task instance.
     */
    virtual TaskImplID get_task_impl_from_inst (TaskInstID inst_id) = 0;

    /// Get all current causal link threats
    /**
     * @return  Set of all current causal link threats.
     */
    virtual CLThreatSet get_all_threats (void) = 0;

    /// Get the Start Window for a task instance
    /**
     * @param task_inst The task instance of which the start window is returned
     *
     * @return The start window for the task instance
     */
    virtual TimeWindow get_start_window(TaskInstID task_inst) = 0;

    /// Get the End Window for a task instance
    /**
     * @param task_inst The task instance of which the end window is returned
     *
     * @return The end window for the task instance
     */
    virtual TimeWindow get_end_window(TaskInstID task_inst) = 0;

    /// Get the duration of a task instance
    /**
     * @param task_inst The task instance of which the duration is returned
     *
     * @return The duration of the task instance
     */
    virtual TimeValue get_duration(TaskInstID task_inst) = 0;

    /// Get the Causal and Scheduling orderings to this task instance
    /**
     * @param inst_id The task instance to which all orderings are required
     */
    virtual TaskInstSet before_orderings (TaskInstID inst_id) = 0;

    /// Get the Causal and Scheduling orderings from this task instance
    /**
     * @param inst_id The task instance from which all orderings are required
     */
    virtual TaskInstSet after_orderings (TaskInstID inst_id) = 0;

    /// Get all the task instances
    virtual TaskInstSet get_all_insts() = 0;
    
    /// Check if the instance id already exists and is being reused.
    /**
     * @param task_inst The task instance being checked
     *
     * @return  True If this task instance already exists.
     */
    virtual bool inst_exists (TaskInstID task_inst) = 0;

    /// Get task implementation for a task instance.
    /**
     * @param task_inst  The task instance.
     *
     * @return The task implementation id.
     */
    virtual TaskImplID get_impl_id (TaskInstID task_inst) = 0;

    // ************************************************************************
    // Methods for obtaining prototypes of commands that work on this object.
    // ************************************************************************

    /// Get a PlanCommand prototype for adding a task to the plan.
    /**
     * @return  An AddTaskCmd prototype for this WorkingPlan.
     */
    virtual AddTaskCmd *get_AddTaskCmd (void) = 0;

    /// Get a PlanCommand prototype for associating an implementation with a
    /// task instance in the plan.
    /**
     * @return  An AssocTaskImplCmd prototype for this WorkingPlan.
     */
    virtual AssocTaskImplCmd *get_AssocTaskImplCmd (void) = 0;

    /// Get a PlanCommand prototype for resolving a causal link threat in the
    /// plan (with promotion or demotion).
    /**
     * @return  An ResolveCLThreatCmd prototype for this WorkingPlan.
     */
    virtual ResolveCLThreatCmd *get_ResolveCLThreatCmd (void) = 0;

    /// Get a PlanCommand prototype for resolving a scheduling (i.e.
    /// non-causal-link ordering constraint (with promotion or demotion)
    /// between two task instances in the plan.
    /**
     * @return  An ResolveSchedOrderCmd prototype for this WorkingPlan.
     */
    virtual ResolveSchedOrderCmd *get_ResolveSchedOrderCmd (void) = 0;

    /// Get a PlanCommand prototype for adjusting the minimum times of a
    /// task instance in the plan.
    /**
     * @return  An AdjustStartWindowCmd prototype for this WorkingPlan.
     */
    virtual AdjustMinTimesCmd *get_AdjustMinTimesCmd (void) = 0;

    /// Get a PlanCommand prototype for adjusting the max times of a
    /// task instance in the plan.
    /**
     * @return  An AdjustEndWindowCmd prototype for this WorkingPlan.
     */
    virtual AdjustMaxTimesCmd *get_AdjustMaxTimesCmd (void) = 0;

  protected:
    /// Pointer to Planner object.
    SA_POP::Planner *planner_;

    /// Goal of this working plan.
    SA_POP::Goal goal_;
  };

};  /* SA_POP namespace */

#endif /* SA_POP_WORKING_PLAN_H_ */
