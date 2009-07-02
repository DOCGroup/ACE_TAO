// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  PlanCommand.h
 *
 * This file contains the PlanCommand abstract base class definition for
 * encapsulation of a command to modify a WorkingPlan.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_PLAN_COMMAND_H_
#define SA_POP_PLAN_COMMAND_H_

#include <string>
#include "SA_POP_Types.h"

namespace SA_POP {

  /**
   * @class PlanCommand
   *
   * @brief Abstract base class for the encapsulation of a command to modify a
   *        WorkingPlan.
   */
  class PlanCommand {
  public:
    /// Constructor.
    PlanCommand (void)
      : prev_ (0)
    { id_.step = -1; id_.decision_pt = -1; id_.seq_num = -1; };

    /// Destructor.
    virtual ~PlanCommand (void) { };

    /// Create a deep copy of this command.
    /**
     * @return A deep copy of this command.
     */
    virtual PlanCommand *clone (void) = 0;

    /// Set id (i.e. info on when this command was created during planning).
    /**
     * @param id  The command id.
     */
    virtual void set_id (CommandID id) { this->id_ = id; };

    /// Get id (i.e. info on when this command was created during planning).
    /**
     * @return  The command id.
     */
    virtual CommandID get_id (void) { return this->id_; };

    /// Set pointer to previous command.
    /**
     * @param prev  Pointer to the previous command.
     */
    virtual void set_prev (PlanCommand *prev) { this->prev_ = prev;};

    /// Get pointer to previous command.
    /**
     * @return  Pointer to the previous command.
     */
    virtual PlanCommand *get_prev (void) { return this->prev_; };

    /// Execute next option for this command.
    /**
     * @return  True if there was an option executed for this command, or
     *          false if no options remain.
     */
    virtual bool execute_next (void) = 0;

    /// Undo this command.
    /**
     * @return  True if undo succeeded, false otherwise.
     */
    virtual void undo (void) = 0;

    /// Get log text for most recent execution of command.
    /**
     * @return  Log text for most recent execution of command.
     */
    virtual std::string get_log_text (void) = 0;
    int choices;


  protected:
    /// ID of this command.
    CommandID id_;

    /// Pointer to previous command.
    PlanCommand *prev_;

  };

  /**
   * @class AdjustMinTimesCmd
   *
   * @brief Abstract base class for the encapsulation of a command to adjust
   *        the start-min and end-min times of a task instance in the plan.
   */
  class AdjustMinTimesCmd : public PlanCommand {
  public:
    /// Constructor.
    AdjustMinTimesCmd (void) { };

    /// Destructor.
    virtual ~AdjustMinTimesCmd (void) { };

    /// Set the task instance and window adjustment.
    /**
     * @param task_inst  The task instance for which to adjust to the window.
     *
     * @param start_min  The new start-min time.
     *
     * @param end_min  The new end-min time.
     */
    virtual void set_times (TaskInstID task_inst, TimeValue start_min,
      TimeValue end_min) = 0;
  };

  /**
   * @class AdjustMaxTimesCmd
   *
   * @brief Abstract base class for the encapsulation of a command to adjust
   *        the start-max and end-max times of a task instance in the plan.
   */
  class AdjustMaxTimesCmd : public PlanCommand {
  public:
    /// Constructor.
    AdjustMaxTimesCmd (void) { };

    /// Destructor.
    virtual ~AdjustMaxTimesCmd (void) { };

    /// Set the task instance and window adjustment.
    /**
     * @param task_inst  The task instance for which to adjust to the window.
     *
     * @param start_max  The new start-max time.
     *
     * @param end_max  The new end-max time.
     */
    virtual void set_times (TaskInstID task_inst, TimeValue start_max,
      TimeValue end_max) = 0;
  };

  /**
   * @class AddTaskCmd
   *
   * @brief Abstract base class for the encapsulation of a command to add a
   *        task to the current plan.
   */
  class AddTaskCmd : public PlanCommand {
  public:
    /// Constructor.
    AddTaskCmd (void) { };

    /// Destructor.
    virtual ~AddTaskCmd (void) { };

    /// Set (ordered) list of tasks to add (one per execution) to the plan.
    /**
     * @param tasks  Ordered list of tasks.
     */
    virtual void set_tasks (const TaskList &tasks) = 0;

    /// Set causal link info to add to the plan with task.
    /**
     * @param cond  Condition this task satisifies.
     *
     * @param task_insts  Set of task instances that this task satisifes
     *                    the given precondition.
     */
    virtual void set_causal_info (Condition cond, TaskInstSet task_insts) = 0;

    /// Get ID of last task added to the plan by this command.
    /**
     * @return  ID of last task added to the plan by this command.
     */
    virtual TaskID get_task (void) = 0;

    /// Get ID of instance of last task added to the plan by this command.
    /**
     * @return  ID of instance of last task added to the plan by this command.
     */
    virtual TaskInstID get_task_inst (void) = 0;

    /// Check if the instance id used by the task of this command already exists.
    /**
     * @return  True if this command is using an already existing instance id.
     */
    virtual bool inst_exists (void) = 0;

	virtual TaskInstSet get_satisfied_tasks(void) = 0;
  };

  /**
   * @class AssocTaskImplCmd
   *
   * @brief Abstract base class for the encapsulation of a command to
   *        associate an implementation with a task instance in the plan.
   */
  class AssocTaskImplCmd : public PlanCommand {
  public:
    /// Constructor.
    AssocTaskImplCmd (void) { };

    /// Destructor.
    virtual ~AssocTaskImplCmd (void) { };

    /// Set the task->implementation associate.
    /**
     * @param task_inst  The task instance.
     *
     * @param impl_list  The task implementations to try for this instance.
     */
    virtual void set_assoc (TaskInstID task_inst, TaskImplList impl_list) = 0;
    /// Get ID of instance of this command.
    /**
     * @return  ID of instance of this command.
     */
    virtual TaskInstID get_task_inst (void) = 0;
  };

  /**
   * @class AddOpenCondsCmd
   *
   * @brief Abstract base class for the encapsulation of a command to add
   *        open conditions to the current planning strategy.
   */
  class AddOpenCondsCmd : public PlanCommand {
  public:
    /// Constructor.
    AddOpenCondsCmd (void) { };

    /// Destructor.
    virtual ~AddOpenCondsCmd (void) { };

    /// Set the open conditions to add.
    /**
     * @param conds  Set of open conditions to add.
     */
    virtual void set_conds (const CondSet &conds) = 0;

    /// Set the task instance for which these are preconditions.
    /**
     * @param task_inst  Task instance for which these are preconditions.
     */
    virtual void set_task_inst (TaskInstID) = 0;
  };

  /**
   * @class RemoveOpenCondsCmd
   *
   * @brief Abstract base class for the encapsulation of a command to remove
   *        open conditions from the current planning strategy.
   */
  class RemoveOpenCondsCmd : public PlanCommand {
  public:
    /// Constructor.
    RemoveOpenCondsCmd (void) { };

    /// Destructor.
    virtual ~RemoveOpenCondsCmd (void) { };

    /// Set the open conditions to remove.
    /**
     * @param conds  Set of open conditions to remove.
     */
    virtual void set_conds (const CondSet &conds, const TaskInstSet &tasks) = 0;
  };

  /**
   * @class AddOpenThreatsCmd
   *
   * @brief Abstract base class for the encapsulation of a command to add
   *        open causal link threats to the current planning strategy.
   */
  class AddOpenThreatsCmd : public PlanCommand {
  public:
    /// Constructor.
    AddOpenThreatsCmd (void) { };

    /// Destructor.
    virtual ~AddOpenThreatsCmd (void) { };

    /// Set the open threats to add.
    /**
     * @param conds  Set of open threats to add.
     */
    virtual void set_threats (const CLThreatSet &threats) = 0;
  };

  /**
   * @class RemoveOpenThreatsCmd
   *
   * @brief Abstract base class for the encapsulation of a command to remove
   *        open causal link threats from the current planning strategy.
   */
  class RemoveOpenThreatsCmd : public PlanCommand {
  public:
    /// Constructor.
    RemoveOpenThreatsCmd (void) { };

    /// Destructor.
    virtual ~RemoveOpenThreatsCmd (void) { };

    /// Set the open threats to remove.
    /**
     * @param conds  Set of open threats to remove.
     */
    virtual void set_threats (const CLThreatSet &threats) = 0;
  };

  /**
   * @class ResolveCLThreatCmd
   *
   * @brief Abstract base class for the encapsulation of a command to resolve
   *        a causal link threat in the plan.
   */
  class ResolveCLThreatCmd : public PlanCommand {
  public:
    /// Constructor.
    ResolveCLThreatCmd (void) { };

    /// Destructor.
    virtual ~ResolveCLThreatCmd (void) { };

    /// Set the causal link threat to resolve.
    /**
     * @param threat  The causal link threat to resolve.
     */
    virtual void set_threat (CLThreat &threat) = 0;
    

   virtual TaskInstID get_first_task(void) = 0;
   virtual TaskInstID get_second_task(void)=0;
  };

  /**
   * @class ResolveSchedOrderCmd
   *
   * @brief Abstract base class for the encapsulation of a command to add
   *        a non-causal-link ordering constraint (promotion or demotion)
   *        between two task instances in the plan.
   */
  class ResolveSchedOrderCmd : public PlanCommand {
  public:
    /// Constructor.
    ResolveSchedOrderCmd (void) { };

    /// Destructor.
    virtual ~ResolveSchedOrderCmd (void) { };

    /// Set the task instances to order.
    /**
     * @param task_inst_a  One task instance.
     *
     * @param task_inst_b  The other task instance.
     */
    virtual void set_task_insts (TaskInstID task_inst_a,
      TaskInstID task_inst_b) = 0;
    /// Get the affected task instances.
    /**
     * @return A set of the affected task instances.
     */
    virtual TaskInstSet get_affected_insts () = 0;
  };



};  /* SA_POP namespace */

#endif /* SA_POP_PLAN_COMMAND_H_ */
