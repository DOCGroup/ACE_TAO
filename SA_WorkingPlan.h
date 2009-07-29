// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SA_WorkingPlan.h
 *
 * This file contains the definition of the SA_WorkingPlan concrete class,
 * which implements a WorkingPlan that uses precedence graphs for scheduling.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_SA_WORKING_PLAN_H_
#define SA_POP_SA_WORKING_PLAN_H_

#include <map>
#include "SA_POP_Types.h"
#include "WorkingPlan.h"
#include "PlanCommands.h"
#include "SA_PlanCommands.h"
#include <stack>
#include <map>

namespace SA_POP {

  /**
   * @class SA_WorkingPlan
   *
   * @brief WorkingPlan concrete class for encapsulation of a plan that uses
   *        precedence graphs for scheduling.
   */
  class SA_WorkingPlan : public WorkingPlan {
  public:
    /// Constructor.
    /**
     * @param planner  Planner object to use.
     */
    SA_WorkingPlan (SA_POP::Planner *planner);

    /// Destructor.
    virtual ~SA_WorkingPlan (void);

    /// Set command prototypes to use.
    /**
     * @param add_threats_cmd  Prototype of command for adding open causal
     *                         link threats to be handled.
     */
    virtual void set_commands (AddOpenThreatsCmd *add_threats_cmd);

    /// Set goal.
    /**
     * @param goal  Goal of this working plan.
     */
    virtual void set_goal (const SA_POP::Goal &goal);

    /// Get current plan.
    /**
     * @return  Reference to current plan.
     */
    virtual const Plan& get_plan (void);

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
      PrecedenceRelation prec_rel);

    /// Get task id of a task instance.
    /**
     * @param inst_id  The task instance id.
     *
     * @return  The task id of this task instance.
     */
    virtual TaskID get_task_from_inst (TaskInstID inst_id);

    /// Get task implementation id of a task instance.
    /**
     * @param inst_id  The task instance id.
     *
     * @return  The task implementation id of this task instance.
     */
	virtual TaskImplID get_task_impl_from_inst (TaskInstID inst_id);

	/// Get all current causal link threats.
    /**
     * @return  Set of all current causal link threats.
     */
    virtual CLThreatSet get_all_threats (void);

   //Get the Start Window for a task instance
    /**
     * @param task_inst The task instance of which the start window is returned
     *
     * @return The start window for the task instance
     */
    virtual TimeWindow get_start_window(TaskInstID task_inst);

    //Get the End Window for a task instance
    /**
     * @param task_inst The task instance of which the end window is returned
     *
     * @return The end window for the task instance
     */
    virtual TimeWindow get_end_window(TaskInstID task_inst);

    //Get the duration of a task instance
    /**
     * @param task_inst The task instance of which the duration is returned
     *
     * @return The duration of the task instance
     */
    virtual TimeValue get_duration(TaskInstID task_inst);

    //Get all the causal and data links TO the task instance
    /**
     * @param task_inst All causal and data links to this task instance have to be returned
     *
     * @return All causal and data links to this task instance
     */
    virtual CLSet get_before(TaskInstID task_inst);
    //Get all the causal and data links FROM the task instance
    /**
     * @param task_inst All causal and data links from this task instance have to be returned
     *
     * @return All causal and data links from this task instance
     */
    virtual CLSet get_after(TaskInstID task_inst);

    /// Get the Causal and Scheduling orderings to this task instance
    /**
     * @param inst_id The task instance to which all orderings are required
     */
  virtual TaskInstSet before_orderings (TaskInstID inst_id);

    /// Get the Causal and Scheduling orderings from this task instance
    /**
     * @param inst_id The task instance from which all orderings are required
     */
  virtual TaskInstSet after_orderings (TaskInstID inst_id);

  /// Adds the sched order to the sched_links_ map by putting the first task instance before the second
  /**
   * @param first_task_inst The task instance before the second one
   *
   * @param second_task_inst The task instance after the first one
   */
	virtual void add_sched_link(TaskInstID first_task_inst, TaskInstID second_task_inst);
	/// Removes the sched order from the sched_links_ map
  /**
   * @param first_task_inst The task instance before the second one
   *
   * @param second_task_inst The task instance after the first one
   */
	virtual void remove_sched_link(TaskInstID first_task_inst, TaskInstID second_task_inst);

	/// Get all the task instances
	virtual TaskInstSet get_all_insts();

     /// Check if the instance id already exists and is being reused.
    /**
     * @param task_inst The task instance being checked
     *
     * @return  True If this task instance already exists.
     */
    virtual bool inst_exists (TaskInstID task_inst);

    /// Get task implementation for a task instance.
    /**
     * @param task_inst  The task instance.
     *
     * @return The task implementation id.
     */
    virtual TaskImplID get_impl_id (TaskInstID task_inst);

    // ************************************************************************
    // Methods for obtaining prototypes of commands that work on this object.
    // ************************************************************************

    /// Get a PlanCommand prototype for adding a task to the plan.
    /**
     * @return  An AddTaskCmd prototype for this WorkingPlan.
     */
    virtual AddTaskCmd *get_AddTaskCmd (void);

    /// Get a PlanCommand prototype for associating an implementation with a
    /// task instance in the plan.
    /**
     * @return  An AssocTaskImplCmd prototype for this WorkingPlan.
     */
    virtual AssocTaskImplCmd *get_AssocTaskImplCmd (void);

    /// Get a PlanCommand prototype for resolving a causal link threat in the
    /// plan (with promotion or demotion).
    /**
     * @return  An ResolveCLThreatCmd prototype for this WorkingPlan.
     */
    virtual ResolveCLThreatCmd *get_ResolveCLThreatCmd (void);

    /// Get a PlanCommand prototype for resolving a scheduling conflict (i.e.
    /// non-causal-link ordering constraint (with promotion or demotion)
    /// between two task instances in the plan.
    /**
     * @return  An ResolveSchedOrderCmd prototype for this WorkingPlan.
     */
    virtual ResolveSchedOrderCmd *get_ResolveSchedOrderCmd (void);

    /// Get a PlanCommand prototype for adjusting the minimum times of a
    /// task instance in the plan.
    /**
     * @return  An AdjustStartWindowCmd prototype for this WorkingPlan.
     */
    virtual AdjustMinTimesCmd *get_AdjustMinTimesCmd (void);

    /// Get a PlanCommand prototype for adjusting the max times of a
    /// task instance in the plan.
    /**
     * @return  An AdjustEndWindowCmd prototype for this WorkingPlan.
     */
    virtual AdjustMaxTimesCmd *get_AdjustMaxTimesCmd (void);



    // ************************************************************************
    // Methods for executing and undoing commands.
    // ************************************************************************

    /// Execute a command to add a task to the plan.
    /**
     * @param cmd  Command object.
     */
    virtual void execute (SA_AddTaskCmd *cmd);

    /// Undo a command to add a task to the plan.
    /**
     * @param cmd  Command object.
     */
    virtual void undo (SA_AddTaskCmd *cmd);

    /// Execute a command to associate an implementation with a
    /// task instance in the plan.
    /**
     * @param cmd  Command object.
     */
    virtual bool execute (SA_AssocTaskImplCmd *cmd);

    /// Undo a command to associate an implementation with a
    /// task instance in the plan.
    /**
     * @param cmd  Command object.
     */
    virtual void undo (SA_AssocTaskImplCmd *cmd);

    /// Execute a command to resolve a causal link threat in the
    /// plan (with promotion or demotion).
    /**
     * @param cmd  Command object.
     */
    virtual bool execute (SA_ResolveCLThreatCmd *cmd);

    /// Undo a command to resolve a causal link threat in the
    /// plan (with promotion or demotion).
    /**
     * @param cmd  Command object.
     */
    virtual void undo (SA_ResolveCLThreatCmd *cmd);

    /// Execute a command to resolve a scheduling conflict (i.e.
    /// non-causal-link ordering constraint with promotion or demotion)
    /// between two task instances in the plan.
    /**
     * @param cmd  Command object.
     *
     * @return Returns true if the new scheduling order is without any violations
     */
    virtual bool execute (SA_ResolveSchedOrderCmd *cmd);

    /// Undo a command to resolve a scheduling conflict (i.e.
    /// non-causal-link ordering constraint with promotion or demotion)
    /// between two task instances in the plan.
    /**
     * @param cmd  Command object.
     */
    virtual void undo (SA_ResolveSchedOrderCmd *cmd);

    /// Execute a command to adjust the minimum times of a
    /// task instance in the plan.
    /**
     * @param cmd  Command object.
     *
     * @return Returns true if the adjustment succeeded without any violations
     */
    virtual bool execute (SA_AdjustMinTimesCmd *cmd);

    /// Undo a command to adjust the minimum times of a
    /// task instance in the plan.
    /**
     * @param cmd  Command object.
     */
    virtual void undo (SA_AdjustMinTimesCmd *cmd);

    /// Execute a command to adjust the maximum times of a
    /// task instance in the plan.
    /**
     * @param cmd  Command object.
     *
     * @return Returns true if the adjustment succeeded without any violations
     */
    virtual bool execute (SA_AdjustMaxTimesCmd *cmd);

    /// Undo a command to adjust the maximum times of a
    /// task instance in the plan.
    /**
     * @param cmd  Command object.
     */
    virtual void undo (SA_AdjustMaxTimesCmd *cmd);

	/// Reset a working plan
    /**
     * 
     */
	virtual void reset_plan ();


    virtual void generate_all_threats(void);


	virtual InstToTaskMap get_task_insts(void){return task_insts_;};

	virtual bool inst_has_impl(TaskInstID inst_id);

	void print_precedence_graph(std::string source);

	bool condition_in_suspended(Condition condition, TaskInstID required_by_);

	CausalLink clink_on_path(Condition condition, TaskInstID required_by);

	CausalLink clink_on_path_aux(Condition condition, TaskInstID required_by, 
										std::set<TaskInstID> & expanded);

	void unsuspend_listeners(CausalLink link, TaskInstID exception);
	void unsuspend_listeners_aux(CausalLink link, TaskInstID exception);

	void suspend_condition(Condition cond, TaskInstID required_by, CausalLink suspended_by);

	void resume_condition(Condition cond, TaskInstID required_by, CausalLink suspended_by);

	bool is_null_condition(Condition cond);

	bool is_null_link(CausalLink link);

	Condition get_no_condition();

	PrecedenceGraph get_precedence_graph();

  protected:
    // ************************************************************************
    // State information.
    // ************************************************************************

    //Ben's.  Kill if it breaks this
    CLThreatSet threat_set;

    /// Flag for whether command prototypes have been set.
    bool has_cmds_;

    /// Task instance ID to use for next instance created.
    TaskInstID next_inst_id_;


    /// Task instances in plan (mapping to task id).
    InstToTaskMap task_insts_;

    /// Implementations of task instances in plan (mapping from task instance).
    InstToImplMap task_impls_;

    // Type of a map from conditions to causal links.
    typedef std::multimap <Condition, CausalLink> CondToCLinksMap;

	typedef std::multimap <TaskInstID, CausalLink> TaskToCLinksMap;

    /// Causal links in plan (mapping from conditions to all causal links
    /// containing that condition).
    CondToCLinksMap causal_links_;
	TaskToCLinksMap causal_links_by_first;
	TaskToCLinksMap causal_links_by_second;

	SuspendedConditionListenerMap suspended_listener_map;
	SuspendedConditionSet suspended_conditions;

    /// Current plan (generated upon request).
    Plan plan_;

    /// Helper function to create next instance id.
    virtual TaskInstID get_next_inst_id (void);


	Condition no_condition;
	CausalLink no_link;

	/// PrecedenceGraph
	PrecedenceGraph precedence_graph_;

	/// Type of an ordering imposed during Scheduling.
	/// First task instance is ordered before the second.
	typedef std::multimap <TaskInstID, TaskInstID> SchedulingLinks;

	/// Map of the imposed orderings
	SchedulingLinks sched_links_;

	/// Map from task instances to the time windows
	typedef std::map <TaskInstID, TimeWindow> InstToWinMap;

	/// InstToWinMap and InstToWinMap
	InstToWinMap start_window_map_;
	InstToWinMap end_window_map_;

  /// Type of a map of task instances and the durations
  typedef std::map <TaskInstID, TimeValue> DurationMap;

  DurationMap durations;

  // The initial start and end windows. There has to be something
  // in this for every task instance be it {NULL_TIME,NULL_TIME}
  InstToWinMap init_start;
	InstToWinMap init_end;

	// The set of reused task instances
	std::multiset<TaskInstID> reused_insts_;

  //I can't use sched_links_ because something craps itself when it gets a link
  //and the tasks don't have windows.  I hate it too.
  SchedulingLinks ordering_links;
  //useful for doing the loop detection algorithm
  SchedulingLinks reverse_ordering_links;
 // SA_AssocTaskImplCmd* associate_cmd;
  
  /*
  bool is_cycle_in_ordering(void);
  void dfs_aux(TaskInstID current, std::stack<TaskInstID>& s, std::map<TaskInstID, bool>& visited, std::map<TaskInstID, bool>& unvisited);
  bool dfs_aux2(TaskInstID current, std::map<TaskInstID, bool>& visited);
 */

  /// Insert initially task by task in the precedence graph
  /**
   * @param task_inst The task instance to insert into the precedence graph
   *
   * @param cmd The task Command executing this
   *
   * @return True if this can be successfully added into the precedence graph
   */
	virtual bool init_prec_insert(TaskInstID task_inst, SA_AssocTaskImplCmd *cmd);

/// prints the precedence graph
  	virtual void print_prec(void);

	/// Erases a task instance from the precedence graph
	/**
	 * @param task_instance The task instance to be erased
   *
   * @param cmd The task Command executing this
   *
	 */
	virtual void prec_erase(TaskInstID task_inst, SA_AssocTaskImplCmd *cmd);


    // ************************************************************************
    // Prototypes of commands that work on this object.
    // ************************************************************************

    /// PlanCommand prototype for adding a task to the plan.
    SA_AddTaskCmd *add_task_cmd_;

    /// PlanCommand prototype for associating an implementation with a
    /// task instance in the plan.
    SA_AssocTaskImplCmd *assoc_impl_cmd_;

    /// PlanCommand prototype for resolving a causal link threat in the
    /// plan (with promotion or demotion).
    SA_ResolveCLThreatCmd *resolve_threat_cmd_;

    /// PlanCommand prototype for resolving a scheduling conflict (i.e.
    /// non-causal-link ordering constraint (with promotion or demotion)
    /// between two task instances in the plan.
    SA_ResolveSchedOrderCmd *resolve_sched_cmd_;

    /// PlanCommand prototype for adjusting the minimum times of a
    /// task instance in the plan.
    SA_AdjustMinTimesCmd *adj_min_times_cmd_;

    /// PlanCommand prototype for adjusting the maximum times of a
    /// task instance in the plan.
    SA_AdjustMaxTimesCmd *adj_max_times_cmd_;



    // ************************************************************************
    // Prototypes of commands that work on other objects.
    // ************************************************************************

    /// PlanCommand prototype for adding open causal link threats to be
    /// handled.
    AddOpenThreatsCmd *add_threats_cmd_;
  };

};  /* SA_POP namespace */

#endif /* SA_POP_SA_WORKING_PLAN_H_ */
