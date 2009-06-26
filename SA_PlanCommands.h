// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SA_PlanCommands.h
 *
 * This file contains the definition of the concrete classes,
 * which implement PlanCommand.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_SA_PLAN_COMMANDS_H_
#define SA_POP_SA_PLAN_COMMANDS_H_

#include <list>

#include "SA_POP_Types.h"
#include "PlanCommands.h"

namespace SA_POP {

  // Forward declarations.
  class SA_WorkingPlan;
  class SA_PlanStrategy;
  /**
   * @class SA_AdjustMinTimesCmd
   *
   * @brief AdjustMinTimesCmd concrete class for the encapsulation of a
   *        command to adjust the start-min and end-min times of a task 
   *        instance in the plan.
   */
  class SA_AdjustMinTimesCmd : public AdjustMinTimesCmd {
  public:
    /// SA_WorkingPlan needs access to private data to execute this command.
    friend class SA_WorkingPlan;

    /// Constructor.
    /**
     * @param working_plan  WorkingPlan object associated with this command.
     */
    SA_AdjustMinTimesCmd (SA_WorkingPlan *working_plan);

    /// Destructor.
    virtual ~SA_AdjustMinTimesCmd (void);

    /// Create a deep copy of this command.
    /**
     * @return A deep copy of this command.
     */
    virtual PlanCommand *clone (void);

    /// Execute next option for this command.
    /**
     * @return  True if there was an option executed for this command, or
     *          false if no options remain.
     */
    virtual bool execute_next (void);

    /// Undo this command.
    virtual void undo (void);

    /// Get log text for most recent execution of command.
    /**
     * @return  Log text for most recent execution of command.
     */
    virtual std::string get_log_text (void);

    /// Set the task instance and window adjustment.
    /**
     * @param task_inst  The task instance for which to adjust to the window.
     *
     * @param start_min  The new start-min time.
     *
     * @param end_min  The new end-min time.
     */
    virtual void set_times (TaskInstID task_inst, TimeValue start_min,
      TimeValue end_min);

  protected:
    /// WorkingPlan object that this command works on.
    SA_WorkingPlan *working_plan_;

    //Task Instance Id for which the min times are being adjusted
    TaskInstID task_inst_;

    //Time values associated with this command 
    TimeValue new_start_min,new_end_min,old_start_min,old_end_min;

	// Type of a list of AdjustMinTimesCmd
    typedef std::list<SA_AdjustMinTimesCmd*> MinTimesAdjustList;

	// A list of all the minimum times changes due to this command
    MinTimesAdjustList min_adjust_cmds;
	
	// A set of all the task instances taht are removed from the unranked set
	TaskInstSet ranked;
  };

  /**
   * @class SA_AdjustMaxTimesCmd
   *
   * @brief AdjustMaxTimesCmd concrete class for the encapsulation of a
   *        command to adjust the start-max and end-max times of a task 
   *        instance in the plan.
   */
  class SA_AdjustMaxTimesCmd : public AdjustMaxTimesCmd {
  public:
    /// SA_WorkingPlan needs access to private data to execute this command.
    friend class SA_WorkingPlan;

    /// Constructor.
    /**
     * @param working_plan  WorkingPlan object associated with this command.
     */
    SA_AdjustMaxTimesCmd (SA_WorkingPlan *working_plan);

    /// Destructor.
    virtual ~SA_AdjustMaxTimesCmd (void);

    /// Create a deep copy of this command.
    /**
     * @return A deep copy of this command.
     */
    virtual PlanCommand *clone (void);

    /// Execute next option for this command.
    /**
     * @return  True if there was an option executed for this command, or
     *          false if no options remain.
     */
    virtual bool execute_next (void);

    /// Undo this command.
    virtual void undo (void);

    /// Get log text for most recent execution of command.
    /**
     * @return  Log text for most recent execution of command.
     */
    virtual std::string get_log_text (void);

    /// Set the task instance and window adjustment.
    /**
     * @param task_inst  The task instance for which to adjust to the window.
     *
     * @param start_max  The new start-max time.
     *
     * @param end_max  The new end-max time.
     */
    virtual void set_times (TaskInstID task_inst, TimeValue start_max,
      TimeValue end_max);

  protected:
    /// WorkingPlan object that this command works on.
    SA_WorkingPlan *working_plan_;

    //Task Instance Id for which the max times are being adjusted
    TaskInstID task_inst_;

    //Time values associated with this command 
    TimeValue new_start_max,new_end_max,old_start_max,old_end_max;

	// Type of a list of AdjustMaxTimesCmd
	typedef std::list<SA_AdjustMaxTimesCmd*> MaxTimesAdjustList;

	// A list of all the maximum times changes due to this command
	MaxTimesAdjustList max_adjust_cmds;

	// A set of all the task instances taht are removed from the unranked set
	TaskInstSet ranked;
  };

  /**
   * @class SA_AddTaskCmd
   *
   * @brief AddTaskCmd concrete class for the encapsulation of a command to add
   *        a task to the current plan.
   */
  class SA_AddTaskCmd : public AddTaskCmd {
  public:
    /// SA_WorkingPlan needs access to private data to execute this command.
    friend class SA_WorkingPlan;
    
    /// Constructor.
    /**
     * @param working_plan  WorkingPlan object associated with this command.
     */
    SA_AddTaskCmd (SA_WorkingPlan *working_plan);

    /// Destructor.
    virtual ~SA_AddTaskCmd (void);

    /// Create a deep copy of this command.
    /**
     * @return A deep copy of this command.
     */
    virtual PlanCommand *clone (void);

    /// Execute next option for this command.
    /**
     * @return  True if there was an option executed for this command, or
     *          false if no options remain.
     */
    virtual bool execute_next (void);

    /// Undo this command.
    virtual void undo (void);

    /// Get log text for most recent execution of command.
    /**
     * @return  Log text for most recent execution of command.
     */
    virtual std::string get_log_text (void);

    /// Set (ordered) list of tasks to add (one per execution) to the plan.
    /**
     * @param tasks  Ordered list of tasks.
     */
    virtual void set_tasks (const TaskList &tasks);

    /// Set causal link info to add to the plan with task.
    /**
     * @param cond  Condition this task satisifies.
     *
     * @param task_insts  Set of task instances that this task satisifes
     *                    the given precondition.
     */
    virtual void set_causal_info (Condition cond, TaskInstSet task_insts);

    /// Get ID of last task added to the plan by this command.
    /**
     * @return  ID of last task added to the plan by this command.
     */
    virtual TaskID get_task (void);

    /// Get ID of instance of last task added to the plan by this command.
    /**
     * @return  ID of instance of last task added to the plan by this command.
     */
    virtual TaskInstID get_task_inst (void);
 
    /// Check if the instance id used by the task of this command already exists.
    /**
     * @return  True if this command is using an already existing instance id.
     */
    virtual bool inst_exists (void);

  protected:
    /// WorkingPlan object that this command works on.
    SA_WorkingPlan *working_plan_;

    /// Ordered list of tasks to try adding to the plan.
    TaskList tasks_;

    /// Open condition satisfied by these tasks.
    Condition cond_;

    /// Task instances requiring the open condition satisfied by these tasks.
    TaskInstSet task_insts_;

    /// Task instances of the task at the front of the tasks_ list.
    TaskInstSet used_task_insts_;

    /// Last task tried.
    TaskID last_task_;

    /// Instance of last task tried.
    TaskInstID last_task_inst_;

    /// Number of tasks tried.
    size_t num_tries_;
    
    /// Causal Links added by this command
    CLSet added_links_;

  };

  /**
   * @class SA_AssocTaskImplCmd
   *
   * @brief AssocTaskImplCmd concrete class for the encapsulation of a command
   *        to associate an implementation with a task instance in the plan.
   */
  class SA_AssocTaskImplCmd : public AssocTaskImplCmd {
  public:
    /// SA_WorkingPlan needs access to private data to execute this command.
    friend class SA_WorkingPlan;

    /// Constructor.
    /**
     * @param working_plan  WorkingPlan object associated with this command.
     */
    SA_AssocTaskImplCmd (SA_WorkingPlan *working_plan);

    /// Destructor.
    virtual ~SA_AssocTaskImplCmd (void);

    /// Create a deep copy of this command.
    /**
     * @return A deep copy of this command.
     */
    virtual PlanCommand *clone (void);

    /// Execute next option for this command.
    /**
     * @return  True if there was an option executed for this command, or
     *          false if no options remain.
     */
    virtual bool execute_next (void);

    /// Undo this command.
    virtual void undo (void);

    /// Get log text for most recent execution of command.
    /**
     * @return  Log text for most recent execution of command.
     */
    virtual std::string get_log_text (void);

    /// Set the task->implementation associate.
    /**
     * @param task_inst  The task instance.
     *
     * @param impl_list  The task implementations to try for this instance.
     */
    virtual void set_assoc (TaskInstID task_inst, TaskImplList impl_list);

    /// Get ID of instance of this command.
    /**
     * @return  ID of instance of this command.
     */
    virtual TaskInstID get_task_inst (void);

  protected:
    /// WorkingPlan object that this command works on.
    SA_WorkingPlan *working_plan_;

    /// Ordered list of task implementations to try.
    TaskImplList impls_;

    /// Task instance with which to associate an implementation.
    TaskInstID task_inst_;

    /// Last task implementation tried.
    TaskImplID last_impl_;

    /// Number of task implementations tried.
    size_t num_tries_;

	// The min and max time adjusttments made for the selected 
	// task implementation and the task instance associated with it
    SA_AdjustMinTimesCmd min_adj_cmd;
	SA_AdjustMaxTimesCmd max_adj_cmd;
	
    bool got_to_scheduling;

	// The changes made in the precedence set due to the selection of this task instance
	// are recorded in these sets.
	std::set< std::pair<TaskInstID,TaskInstID> > causal_insertions;
	std::set< std::pair<TaskInstID,TaskInstID> > simul_insertions;
  };

  /**
   * @class SA_AddOpenCondsCmd
   *
   * @brief AddOpenCondsCmd concrete class for the encapsulation of a command
   *        to add open conditions to the current planning strategy.
   */
  class SA_AddOpenCondsCmd : public AddOpenCondsCmd {
  public:
    /// SA_PlanStrategy needs access to private data to execute this command.
    friend class SA_PlanStrategy;

    /// Constructor.
    /**
     * @param plan_strat  PlanStrategy object associated with this command.
     */
    SA_AddOpenCondsCmd (SA_PlanStrategy *plan_strat);

    /// Destructor.
    virtual ~SA_AddOpenCondsCmd (void);

    /// Create a deep copy of this command.
    /**
     * @return A deep copy of this command.
     */
    virtual PlanCommand *clone (void);

    /// Execute next option for this command.
    /**
     * @return  True if there was an option executed for this command, or
     *          false if no options remain.
     */
    virtual bool execute_next (void);

    /// Undo this command.
    virtual void undo (void);

    /// Get log text for most recent execution of command.
    /**
     * @return  Log text for most recent execution of command.
     */
    virtual std::string get_log_text (void);

    /// Set the open conditions to add.
    /**
     * @param conds  Set of open conditions to add.
     */
    virtual void set_conds (const CondSet &conds);

    /// Set the task instance for which these are preconditions.
    /**
     * @param task_inst  Task instance for which these are preconditions.
     */
    virtual void set_task_inst (TaskInstID task_inst);

  protected:
    /// PlanStrategy object that this command works on.
    SA_PlanStrategy *plan_strat_;

    /// Set of conditions to add to open conditions.
    CondSet conds_;

    /// Task instance for which these are preconditions.
    TaskInstID task_inst_;

    /// Flag for whether this has been executed.
    bool has_executed_;
  };

  /**
   * @class SA_RemoveOpenCondsCmd
   *
   * @brief RemoveOpenCondsCmd concrete class for the encapsulation of a
   *        command to remove open conditions from the current planning
   *        strategy.
   */
  class SA_RemoveOpenCondsCmd : public RemoveOpenCondsCmd {
  public:
    /// SA_PlanStrategy needs access to private data to execute this command.
    friend class SA_PlanStrategy;

    /// Constructor.
    /**
     * @param plan_strat  PlanStrategy object associated with this command.
     */
    SA_RemoveOpenCondsCmd (SA_PlanStrategy *plan_strat);

    /// Destructor.
    virtual ~SA_RemoveOpenCondsCmd (void);

    /// Create a deep copy of this command.
    /**
     * @return A deep copy of this command.
     */
    virtual PlanCommand *clone (void);

    /// Execute next option for this command.
    /**
     * @return  True if there was an option executed for this command, or
     *          false if no options remain.
     */
    virtual bool execute_next (void);

    /// Undo this command.
    virtual void undo (void);

    /// Get log text for most recent execution of command.
    /**
     * @return  Log text for most recent execution of command.
     */
    virtual std::string get_log_text (void);

    /// Set the open conditions to remove.
    /**
     * @param conds  Set of open conditions to remove.
     */
    virtual void set_conds (const CondSet &conds);

  protected:
    /// PlanStrategy object that this command works on.
    SA_PlanStrategy *plan_strat_;

    /// Set of conditions to remove from open conditions.
    CondSet conds_;

    /// Map from removed conditions to (removed) task instances.
    OpenCondMap removed_;
  };

  /**
   * @class SA_AddOpenThreatsCmd
   *
   * @brief AddOpenThreatsCmd concrete class for the encapsulation of a command
   *        to add open causal link threats to the current planning strategy.
   */
  class SA_AddOpenThreatsCmd : public AddOpenThreatsCmd {
  public:
    /// SA_PlanStrategy needs access to private data to execute this command.
    friend class SA_PlanStrategy;

    /// Constructor.
    /**
     * @param plan_strat  PlanStrategy object associated with this command.
     */
    SA_AddOpenThreatsCmd (SA_PlanStrategy *plan_strat);

    /// Destructor.
    virtual ~SA_AddOpenThreatsCmd (void);

    /// Create a deep copy of this command.
    /**
     * @return A deep copy of this command.
     */
    virtual PlanCommand *clone (void);

    /// Execute next option for this command.
    /**
     * @return  True if there was an option executed for this command, or
     *          false if no options remain.
     */
    virtual bool execute_next (void);

    /// Undo this command.
    virtual void undo (void);

    /// Get log text for most recent execution of command.
    /**
     * @return  Log text for most recent execution of command.
     */
    virtual std::string get_log_text (void);

    /// Set the open threats to add.
    /**
     * @param conds  Set of open threats to add.
     */
    virtual void set_threats (const CLThreatSet &threats);

  protected:
    /// PlanStrategy object that this command works on.
    SA_PlanStrategy *plan_strat_;

    /// Set of Threats to add to open threats.
    CLThreatSet threats_;

    /// Flag for whether this has been executed.
    bool has_executed_;

  };

  /**
   * @class SA_RemoveOpenThreatsCmd
   *
   * @brief RemoveOpenThreatsCmd concrete class for the encapsulation of a
   *        command to remove open causal link threats from the current
   *        planning strategy.
   */
  class SA_RemoveOpenThreatsCmd : public RemoveOpenThreatsCmd {
  public:
    /// SA_PlanStrategy needs access to private data to execute this command.
    friend class SA_PlanStrategy;

    /// Constructor.
    /**
     * @param plan_strat  PlanStrategy object associated with this command.
     */
    SA_RemoveOpenThreatsCmd (SA_PlanStrategy *plan_strat);

    /// Destructor.
    virtual ~SA_RemoveOpenThreatsCmd (void);

    /// Create a deep copy of this command.
    /**
     * @return A deep copy of this command.
     */
    virtual PlanCommand *clone (void);

    /// Execute next option for this command.
    /**
     * @return  True if there was an option executed for this command, or
     *          false if no options remain.
     */
    virtual bool execute_next (void);

    /// Undo this command.
    virtual void undo (void);

    /// Get log text for most recent execution of command.
    /**
     * @return  Log text for most recent execution of command.
     */
    virtual std::string get_log_text (void);

    /// Set the open threats to remove.
    /**
     * @param conds  Set of open threats to remove.
     */
    virtual void set_threats (const CLThreatSet &threats);

  protected:
    /// PlanStrategy object that this command works on.
    SA_PlanStrategy *plan_strat_;

    /// Set of threats to remove from open threats.
    CLThreatSet threats_;

    /// Flag for whether this has been executed.
    bool executed_;



  };

  /**
   * @class SA_ResolveCLThreatCmd
   *
   * @brief ResolveCLThreatCmd concrete class for the encapsulation of a
   *        command to resolve a causal link threat in the plan.
   */
  class SA_ResolveCLThreatCmd : public ResolveCLThreatCmd {
  public:
    /// SA_WorkingPlan needs access to private data to execute this command.
    friend class SA_WorkingPlan;

    /// Constructor.
    /**
     * @param working_plan  WorkingPlan object associated with this command.
     */
    SA_ResolveCLThreatCmd (SA_WorkingPlan *working_plan);

    /// Destructor.
    virtual ~SA_ResolveCLThreatCmd (void);

    /// Create a deep copy of this command.
    /**
     * @return A deep copy of this command.
     */
    virtual PlanCommand *clone (void);

    /// Execute next option for this command.
    /**
     * @return  True if there was an option executed for this command, or
     *          false if no options remain.
     */
    virtual bool execute_next (void);

    /// Undo this command.
    virtual void undo (void);

    /// Get log text for most recent execution of command.
    /**
     * @return  Log text for most recent execution of command.
     */
    virtual std::string get_log_text (void);

    /// Set the causal link threat to resolve.
    /**
     * @param threat  The causal link threat to resolve.
     */
    virtual void set_threat (CLThreat &threat);


    /// Set the task instances to order.
    /**
     * @param task_inst_a  One task instance.
     *
     * @param task_inst_b  The other task instance.
     */
    virtual void set_task_insts (TaskInstID task_inst_a,
      TaskInstID task_inst_b);

  //  int choices;
    // A Type of a list of ResolveSchedOrderCmd pointers
	typedef std::list<SA_ResolveCLThreatCmd* > ResolveCLThreatCmdList;

  virtual TaskInstID get_first_task(void){return first;}
  virtual TaskInstID get_second_task(void){return second;}

  protected:
    /// WorkingPlan object that this command works on.
    SA_WorkingPlan *working_plan_;

    CLThreat threat;
    // The first task instance is scheduled before the second one
	  TaskInstID first,second;
    Condition condition;

    std::map <TaskInstID, TaskInstSet> befores;
    std::map <TaskInstID, TaskInstSet> afters;
    std::map <TaskInstID, TaskInstSet> simuls;
    std::map <TaskInstID, TaskInstSet> unrankeds;
    
    bool got_to_change_precedences;


    	// The min and the max times changed due to this command
	  SA_AdjustMinTimesCmd *adj_min_times_cmd_;
	  SA_AdjustMaxTimesCmd *adj_max_times_cmd_;

    // A list of all the scheduling orderings introduced by this command
    // This may need to change for a list of ResolveThreatCmd List
	  ResolveCLThreatCmdList cmds_;
  };

  /**
   * @class SA_ResolveSchedOrderCmd
   *
   * @brief ResolveSchedOrderCmd concrete class for the encapsulation of a
   *        command to add a non-causal-link ordering constraint (promotion
   *        or demotion) between two task instances in the plan.
   */
  class SA_ResolveSchedOrderCmd : public ResolveSchedOrderCmd {
  public:
    /// SA_WorkingPlan needs access to private data to execute this command.
    friend class SA_WorkingPlan;

    /// Constructor.
    /**
     * @param working_plan  WorkingPlan object associated with this command.
     */
    SA_ResolveSchedOrderCmd (SA_WorkingPlan *working_plan);

    /// Destructor.
    virtual ~SA_ResolveSchedOrderCmd (void);

    /// Create a deep copy of this command.
    /**
     * @return A deep copy of this command.
     */
    virtual PlanCommand *clone (void);

    /// Execute next option for this command.
    /**
     * @return  True if there was an option executed for this command, or
     *          false if no options remain.
     */
    virtual bool execute_next (void);

    /// Undo this command.
    virtual void undo (void);

    /// Get log text for most recent execution of command.
    /**
     * @return  Log text for most recent execution of command.
     */
    virtual std::string get_log_text (void);

    /// Set the task instances to order.
    /**
     * @param task_inst_a  One task instance.
     *
     * @param task_inst_b  The other task instance.
     */
    virtual void set_task_insts (TaskInstID task_inst_a,
      TaskInstID task_inst_b);
    /// Get the affected task instances.
    /**
     * @return A set of the affected task instances.
     */
    virtual TaskInstSet get_affected_insts ();
	
	// A Type of a list of ResolveSchedOrderCmd pointers
	typedef std::list<SA_ResolveSchedOrderCmd* > ResolveSchedOrderCmdList;
  protected:
    /// WorkingPlan object that this command works on.
    SA_WorkingPlan *working_plan_;

	// The first task instance is scheduled before the second one
	TaskInstID first,second;


	// The min and the max times changed due to this command
	SA_AdjustMinTimesCmd *adj_min_times_cmd_;
	SA_AdjustMaxTimesCmd *adj_max_times_cmd_;

	// A list of all the scheduling orderings introduced by this command
	ResolveSchedOrderCmdList cmds_;
  };


};  /* SA_POP namespace */

#endif /* SA_POP_SA_PLAN_COMMANDS_H_ */
