// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SA_SchedStrategy.h
 *
 * This file contains the SA_SchedStrategy concrete class definition, which
 * implements a SchedStrategy for the high-level scheduling algorithm.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_SA_SCHED_STRATEGY_H_
#define SA_POP_SA_SCHED_STRATEGY_H_

#include<map>
#include "SA_POP_Types.h"
#include "SchedStrategy.h"
#include "PlanCommands.h"

namespace SA_POP {

  /**
   * @class SA_SchedStrategy
   *
   * @brief  Concrete class of SchedStrategy for the high-level scheduling
   *         algorithm.
   */
  class SA_SchedStrategy : public SchedStrategy {
  public:
    /// Constructor.
    /**
     * @param planner  Planner object to use.
     */
    SA_SchedStrategy (SA_POP::Planner *planner);

    /// Destructor.
    virtual ~SA_SchedStrategy (void);


    /// reset the object
    virtual void reset();

    /// Set command prototypes to use in scheduling.
    /**
     * @param resolve_sched_cmd  Prototype of command for resolving a
     *                           scheduling conflict between two task
     *                           instances in the plan.
     *
     * @param adj_min_times_cmd  Prototype of command for adjusting the
     *                           min times of a task instance in the plan.
     *
     * @param adj_max_times_cmd  Prototype of command for adjusting the
     *                         max times of a task instance in the plan.
     */
    virtual void set_commands (ResolveSchedOrderCmd *resolve_sched_cmd,
      AdjustMinTimesCmd *adj_min_times_cmd,
      AdjustMaxTimesCmd *adj_max_times_cmd);


    /// Get command ID to use for next command.
    /**
     * @return  Command ID to use for next command.
     */
    virtual CommandID get_next_cmd_id (void);

    /// Recursively satisfy all scheduling constraints (and continue
    /// satisfaction of open conditions by recursive call back to planning).
    /**
     * @param task_inst  Current task instance being tried in the plan.
     *
     * @return  True if fully satisfied plan found, false otherwise.
     */
    virtual bool satisfy_sched (TaskInstID task_inst);

    /// Satisfy fully instantiated plan.
    /**
     * @param task_inst  Current task instance being tried in the plan.
     *
     * @return  True if fully satisfied plan found, false otherwise.
     */
    virtual bool satisfy_full_sched ();

	///Energy Constraint Propogation
	/**
	 * @param task_inst Current task instance being tried in the plan
	 *
	 * @return True if the energy constraint propogation is successfull
	 */
	virtual bool energy_prop (TaskInstID task_inst);
	///Energy Constraint Propogation Helper functions
	/**
	 * @param task_inst Current task instance being tried in the plan
	 *
	 * @return True if the energy constraint propogation is successfull
	 */
	virtual bool energy_prop_after (TaskInstID task_inst);
	///Energy Constraint Propogation Helper functions
	/**
	 * @param task_inst Current task instance being tried in the plan
	 *
	 * @return True if the energy constraint propogation is successfull
	 */
	virtual bool energy_prop_before (TaskInstID task_inst);
	///Time Based Balance Constraint Propogation 5.3.3 (labourie paper)
	/**
	 * @param task_inst Current task instance being tried in the plan
	 *
	 * @return True if the balance constraint propogation is successfull
	 */
	virtual bool time_balance_prop (TaskInstID task_inst);
	///Precedence Link Based Balance Constraint Propogation 5.3.4 (labourie paper)
	/**
	 * @param task_inst Current task instance being tried in the plan
	 *
	 * @return True if the balance constraint propogation is successfull
	 */
	virtual bool prec_balance_prop (TaskInstID task_inst);
	///Commitment of an ordering
	/**
	 * @param first_task_inst The task_instance scheduled before the second one.
	 *
	 * @param second_task_inst The task instance schedules after the first one.
	 *
	 * @return The value of the commitment of this order.
	 */
	virtual double commit(TaskInstID first_task_inst, TaskInstID second_task_inst);
	///Criticality of the consumer of a task instance
	/**
	 * @param task_inst The task instance.
	 *
	 * @return The value of the criticality of the task instance.
	 */
	virtual Criticality crit(TaskInstID task_inst);
	/// Calculate the min and max levels
	virtual void calculate_levels(TaskInstID task_inst);
	/// Perform the search till the ctiricality goes below a certain value
	/**
	 * @param min_crit The threshold criticality value
	 *
	 * @return True if it succeds
	 */
	virtual bool search(double min_crit);
  protected:
    
    /// Flag for whether command prototypes have been set.
    bool has_cmds_;

    /// Current decision point.
    int cur_decision_pt_;

    /// Current sequence number for commands in this decision point.
    int cur_seq_num_;

	/// Type of a map from task instance to its level
	typedef std::map<TaskInstID,ResourceValue> LevelMap;
	/// Type of a map from the Resource ID to the minimum levels
	typedef std::map<ResourceID,LevelMap> MinimumLevels;
	/// Type of a map from the Resource ID to the maximum levels
	typedef std::map<ResourceID,LevelMap> MaximumLevels;
	
	MinimumLevels min_cons_levels_;
	MaximumLevels max_cons_levels_;
    // ************************************************************************
    // Decision point numbers.
    // ************************************************************************

    /// Scheduling is fourth decision point.
    static const int SCHEDULE_DECISION = 4;

    // ************************************************************************
    // Prototypes of commands that work on other objects.
    // ************************************************************************

    /// PlanCommand prototype for resolving a scheduling conflict (i.e.
    /// non-causal-link ordering constraint (with promotion or demotion)
    /// between two task instances in the plan.
    ResolveSchedOrderCmd *resolve_sched_cmd_;

    /// PlanCommand prototype for adjusting the min times of a
    /// task instance in the plan.
    AdjustMinTimesCmd *adj_min_times_cmd_;

    /// PlanCommand prototype for adjusting the max times of a
    /// task instance in the plan.
    AdjustMaxTimesCmd *adj_max_times_cmd_;
  };

};  /* SA_POP namespace */

#endif /* SA_POP_SA_SCHED_STRATEGY_H_ */
