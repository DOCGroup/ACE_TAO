// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SA_PlanStrategy.h
 *
 * This file contains the definition of the SA_PlanStrategy concrete class,
 * which implements a PlanStrategy for use with spreading activation networks
 * and precedence graphs.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_SA_PLAN_STRATEGY_H_
#define SA_POP_SA_PLAN_STRATEGY_H_

#include <set>
#include "SA_POP_Types.h"
#include "PlanStrategy.h"
#include "PlanHeuristics.h"
#include "PlanCommands.h"
#include "SA_PlanCommands.h"

namespace SA_POP {

  /**
   * @class SA_PlanStrategy
   *
   * @brief PlanStrategy concrete class for the high-level
   *        planning/scheduling algorithm for use with spreading activation
   *        networks and precedence graphs.
   */
  class SA_PlanStrategy : public PlanStrategy {
  public:
    /// Constructor.
    /**
     * @param planner  Planner object to use.
     *
     * @param cond_choice  Strategy for choosing open condition to satisfy.
     *
     * @param task_choice  Strategy for choosing/ordering tasks to satisfy
     *                     an open condition.
     *
     * @param impl_choice  Strategy for choosing an implementation of a task.
     */
    SA_PlanStrategy (SA_POP::Planner *planner, CondStrategy *cond_choice,
      TaskStrategy *task_choice, ImplStrategy *impl_choice);

    /// Destructor.
    virtual ~SA_PlanStrategy (void);

    /// Set command prototypes to use in planning.
    /**
     * @param assoc_impl_cmd  Prototype of command for associating an
     *                        implementation with a task instance in the plan.
     *
     * @param add_task_cmd  Prototype of command for adding a task to the plan.
     *
     * @param resolve_threat_cmd  Prototype of command for resolving causal
     *                            link threats.
     */
    virtual void set_commands (AddTaskCmd *add_task_cmd,
      AssocTaskImplCmd *assoc_impl_cmd,
      ResolveCLThreatCmd *resolve_threat_cmd);


    ///Reset Object.
    /**
    
    **/
    virtual void reset(void);


    // ************************************************************************
    // Planning methods.
    // ************************************************************************

    /// Set goals.
    /**
     * @param goals  Set of goals and associated utilities.
     */
    virtual void set_goals (GoalMap goals);

    /// Get command ID to use for next command.
    /**
     * @return  Command ID to use for next command.
     */
    virtual CommandID get_next_cmd_id (void);

    /// Recursively satisfy all open conditions (including recursive
    /// scheduling constraint satisfaction through call back).
    /**
     * @return  True if all open conditions were satisfied, false otherwise.
     */
    virtual bool satisfy_open_conds (void);


    // ************************************************************************
    // Methods for obtaining prototypes of commands that work on this strategy.
    // ************************************************************************

    /// Get a PlanCommand prototype for adding open conditions,
    /// which works on this strategy.
    /**
     * @return  An AddOpenCondsCmd prototype for this strategy.
     */
    virtual AddOpenCondsCmd *get_AddOpenCondsCmd (void);

    /// Get a PlanCommand prototype for removing open conditions,
    /// which works on this strategy.
    /**
     * @return  A RemoveOpenCondsCmd prototype for this strategy.
     */
    virtual RemoveOpenCondsCmd *get_RemoveOpenCondsCmd (void);

    /// Get an command prototype for adding causal link threats, which works on
    /// this strategy.
    /**
     * @return  An AddOpenThreatsCmd prototype for this strategy.
     */
    virtual AddOpenThreatsCmd *get_AddOpenThreatsCmd (void);

    /// Get a PlanCommand prototype for removing causal link threats,
    /// which works on this strategy.
    /**
     * @return  A RemoveOpenThreatsCmd prototype for this strategy.
     */
    virtual RemoveOpenThreatsCmd *get_RemoveOpenThreatsCmd (void);



    // ************************************************************************
    // Methods for executing and undoing commands.
    // ************************************************************************

    /// Execute a command to add open conditions to planning.
    /**
     * @param cmd  Command object.
     */
    virtual void execute (SA_AddOpenCondsCmd *cmd);

    /// Undo a command to add open conditions to planning.
    /**
     * @param cmd  Command object.
     */
    virtual void undo (SA_AddOpenCondsCmd *cmd);

    /// Execute a command to remove open conditions from planning.
    /**
     * @param cmd  Command object.
     */
    virtual void execute (SA_RemoveOpenCondsCmd *cmd);

    /// Undo a command to remove open conditions from planning.
    /**
     * @param cmd  Command object.
     */
    virtual void undo (SA_RemoveOpenCondsCmd *cmd);

    /// Execute a command to add causal link threats to planning.
    /**
     * @param cmd  Command object.
     */
    virtual void execute (SA_AddOpenThreatsCmd *cmd);

    /// Undo a command to add causal link threats to planning.
    /**
     * @param cmd  Command object.
     */
    virtual void undo (SA_AddOpenThreatsCmd *cmd);

    /// Execute a command to remove causal link threats from planning.
    /**
     * @param cmd  Command object.
     */
    virtual void execute (SA_RemoveOpenThreatsCmd *cmd);

    /// Undo a command to remove causal link threats from planning.
    /**
     * @param cmd  Command object.
     */
    virtual void undo (SA_RemoveOpenThreatsCmd *cmd);

    virtual bool satisfy_everything();
    virtual bool satisfy_schedule(void);
    virtual bool get_next_threat_resolution();

  protected:
    // ************************************************************************
    // State information.
    // ************************************************************************

    /// Flag for whether command prototypes have been set.
    bool has_cmds_;

    /// Set of open conditions with associated task instances that require
    /// them as preconditions.
    OpenCondMap open_conds_;

    /// Set of open causal link threats.
    CLThreatSet open_threats_;

    /// Set of open causal link threats.
    CLThreatSet closed_threats_;

    /// ID of current task being tried (to satisfy an open condition).
    TaskID cur_task_;

    /// ID of instance of current task being tried.
    TaskInstID cur_task_inst_;

    /// Current step number.
    int cur_step_;

    /// Current decision point.
    int cur_decision_pt_;

    /// Current sequence number for commands in this decision point.
    int cur_seq_num_;

	///Holds info about which tasks an add task cmd has satisfied
	TaskInstSet satisfied_insts;

	CLSet added_links;

    // ************************************************************************
    // Decision point numbers.
    // ************************************************************************

    /// Task choice is first decision point.
    static const int  TASK_DECISION = 1;

    /// Causal link threat handling is second decision point.
    static const int THREAT_DECISION = 3;

    /// Task implementation choice is third decision point.
    static const int IMPL_DECISION = 2;

    /// Scheduling is fourth decision point.
    static const int SCHEDULE_DECISION = 4;

	StoredConditionEvaluator store_map;
	
    // ************************************************************************
    // Decision point heuristic strategies.
    // ************************************************************************

    /// Strategy for choosing open condition to satisfy.
    CondStrategy *cond_choice_;

    /// Strategy for choosing tasks to satisfy an open condition.
    TaskStrategy *task_choice_;

    /// Strategy for choosing task implementation.
    ImplStrategy *impl_choice_;



    // ************************************************************************
    // Prototypes of commands that work on this object.
    // ************************************************************************

    /// PlanCommand prototype to add open conditions.
    SA_AddOpenCondsCmd *add_conds_cmd_;

    /// PlanCommand prototype to remove open conditions.
    SA_RemoveOpenCondsCmd *rmv_conds_cmd_;

    /// PlanCommand prototype to add open causal link threats.
    SA_AddOpenThreatsCmd *add_threats_cmd_;

    /// PlanCommand prototype to remove open causal link threats.
    SA_RemoveOpenThreatsCmd *rmv_threats_cmd_;



    // ************************************************************************
    // Prototypes of commands that work on other objects.
    // ************************************************************************

    /// PlanCommand prototype for adding a task to the plan.
    AddTaskCmd *add_task_cmd_;

    /// PlanCommand prototype for associating an implementation with a
    /// task instance in the plan.
    AssocTaskImplCmd *assoc_impl_cmd_;

    /// PlanCommand prototype for resolving a causal link threat in the
    /// plan (with promotion or demotion).
    ResolveCLThreatCmd *resolve_threat_cmd_;

    // ************************************************************************
    // Internal helper methods.
    // ************************************************************************

    /// Recursively satisfy all open causal link threats and continue planning.
    /**
     * @return  True if planning succeeded, false otherwise.
     */
//    virtual bool satisfy_open_threats (void);

    /// Satisfy an open condition with an appropriate task.
    /**
     * @param open_cond  Open condition to be satisfied.
     *
     * @return  Pointer to command passed to planner.
     */
    virtual AddTaskCmd *satisfy_cond (Condition open_cond);

    /// Add open conditions.
    /**
     * @param open_conds  Set of open conditions to add.
     *
     * @param task_inst  The task instance for which these are preconditions.
     *
     * @return  ID of command passed to planner to add open conditions.
     */
    virtual CommandID add_open_conds (const CondSet &open_conds,
      TaskInstID task_inst);

    /// Remove open condition.
    /**
     * @param open_cond  Open condition to remove.
     *
     * @return  ID of command passed to planner to remove open condition.
     */
    virtual CommandID rmv_open_cond (Condition open_cond, TaskInstSet tasks);

    /// Add open causal link threats.
    /**
     * @param threats  Set of open causal link threats to add.
     *
     * @return  ID of command passed to planner to add open threats.
     */
    virtual CommandID add_open_threats (const CLThreatSet &threats);

    /// Remove open causal link threat.
    /**
     * @param threat  Open causal link threat to remove.
     *
     * @return  ID of command passed to planner to remove open threat.
     */
    virtual CommandID rmv_open_threat (CLThreat threat);

  };

};  /* SA_POP namespace */

#endif /* SA_POP_SA_PLAN_STRATEGY_H_ */
