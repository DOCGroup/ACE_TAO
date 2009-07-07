// -*- C++ -*-
// $Id$

//=============================================================================
/**
 * @file  SA_POP_Types.h
 *
 * This file contains the definitions of types used throughout SA-POP.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_POP_TYPES_H_
#define SA_POP_TYPES_H_

#include <string>
#include <set>
#include <list>
#include <map>
#include <sstream>

#include "SA_POP_Utils.h"

#if defined (SA_POP_HAS_ACE)
#include "ace/Log_Msg.h"
#include "ace/Log_Priority.h"
#endif  /* SA_POP_HAS_ACE */

#define SA_POP_DEBUG_NORMAL 5
#define SA_POP_DEBUG_HIGH 10

#if defined (SA_POP_HAS_ACE)
#define SA_POP_DEBUG(x,y) \
if (x > 0) \
ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT("SA-POP: %s\n"), y));
#define SA_POP_DEBUG_STR(x,y) \
if (x > 0) \
ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT("SA-POP: %s\n"), y.c_str ()));
#else  /* SA_POP_HAS_ACE not defined */
#include <iostream>
#define SA_POP_DEBUG(x,y) \
if (x > 0) \
  std::cout << "SA-POP: " << y << std::endl;
#define SA_POP_DEBUG_STR(x,y) \
if (x > 0) \
  std::cout << "SA-POP: " << y << std::endl;
#endif  /* SA_POP_HAS_ACE */


namespace SA_POP {

  /// Type of a node id (tasks and conditions).
  /// (must be > 0 and unique across all tasks *and* conditions).
  typedef int NodeID;

  /// Type of a condition id.
  /// (must be > 0 and unique across all tasks *and* conditions).
  typedef NodeID CondID;

  /// Type of a condition value.
  typedef bool CondValue;

  /// Type of a condition kind/type.
  enum CondKind {ENVIRON, SYSTEM, DATA};

  /// Type of a condition.
  struct Condition {
    CondID id;
    CondValue value;
    CondKind kind;
    bool operator== (const Condition &s) const { return this->id == s.id; };
    bool operator!= (const Condition &s) const { return !(*this == s); };
    bool operator< (const Condition &s) const { return this->id < s.id; };
  };

  /// Type of a task ID.
  /// (must be > 0 and unique across all tasks *and* conditions).
  typedef NodeID TaskID;

  /// Type of a task type ID.
  typedef std::string TaskTypeID;

  /// NULL task ID (for unknown/uninitialized tasks).
  const TaskID NULL_TASK_ID = 0;

  /// NULL condition ID (for unknown/uninitialized conditions).
  const CondID NULL_COND_ID = 0;

  /// Type of a port id.
  typedef std::string PortID;

  /// Type of an expected utility calculation (basetype for others).
  /// (N.B. this must be a double or float to handle [0,1] range probabilities
  /// and multiplication factors).
  typedef double EUCalc;
          
  /// Type of a condition utility.
  typedef EUCalc Utility;

  /// Type of a probability.
  typedef EUCalc Probability;

  /// Type of a (precondition or effect) link weight.
  typedef EUCalc LinkWeight;

  /// Type of a task cost.
  typedef EUCalc TaskCost;

  /// Type of a multiplicative factor (e.g. attenuation factor).
  typedef EUCalc MultFactor;

  /// Type of a precondition link.
  typedef std::pair<CondID, TaskID> PrecondLink;

  /// Type of an effect link.
  typedef std::pair<TaskID, CondID> EffectLink;

  /// Type of a pair of ports for a link.
  typedef std::pair<PortID, PortID> LinkPorts;

  /// Type of a list of tasks.
  typedef std::list<TaskID> TaskList;

  /// Type of a set of goal conditions with associated utilities.
  typedef std::map<CondID, Utility> GoalMap;

  /// Type of a set of tasks with associated expected utilities.
  typedef std::map<TaskID, Utility> TaskEUMap;

  /// Type of a set of conditions (condition & value).
  typedef std::set<Condition> CondSet;

  /// Type of a set of task ids.
  typedef std::set<TaskID> TaskSet;


  /// Type of a task instance id.
  /// (must be unique across all task instances).
  typedef int TaskInstID;

  /// Goal task instance ID (for task instance placeholders in open goals).
  const TaskInstID GOAL_TASK_INST_ID = -1;

  /// Null task instance ID (for unknown/uninitialized task instances).
  const TaskInstID NULL_TASK_INST_ID = -2;

  /// Type of a task implementation id.
  /// (must be unique across all task implementations).
  typedef std::string TaskImplID;

  /// Null task implementation ID (for unknown/uninitialized task implementations).
  const TaskImplID NULL_TASK_IMPL_ID = "NULL_TASK_IMPL_ID";

  /// Type of a task implementation parameter id/name.
  typedef std::string ImplParamID;

  /// Type of a task implementation parameter kind/type.
  typedef std::string ImplParamKind;

  /// Type of a task implementation parameter value.
  typedef std::string ImplParamValue;
  enum TaskChoiceType{REUSE_INST, NEW_INST};
  struct TaskChoice {
	
		TaskChoiceType choice;
		TaskInstID task_inst_id;
		TaskID task_id;
	};

  ///List of TaskChoices
  typedef std::list<TaskChoice> TaskChoiceList;

  typedef std::map <TaskInstID, TaskID> InstToTaskMap;

  struct SortTaskByTime{
	
	  TaskID task_id;
	  TaskInstID last_instance;
	
	  void note_instance(TaskInstID instance){
		  if (last_instance > instance){
			this->last_instance = instance;
		  }
	  }

	 bool operator<(const SortTaskByTime & s) {
		return this->last_instance < s.last_instance;
	}
	 bool operator!=(const SortTaskByTime & s) {
		return this->last_instance != s.last_instance;
	}
	 bool operator==(const SortTaskByTime & s) {
		return this->last_instance == s.last_instance;
	}

  };


  /// Type of an implementation parameter.
  struct ImplParam {
    ImplParamID id;
    ImplParamKind kind;
    ImplParamValue value;
    bool operator== (const ImplParam &s) const { return this->id == s.id; };
    bool operator!= (const ImplParam &s) const { return !(*this == s); };
    bool operator< (const ImplParam &s) const { return this->id < s.id; };
  };

  /// Type of a resource id.
  /// (must be unique across all resources).
  typedef std::string ResourceID;

  const ResourceID NULL_RESOURCE_ID = "NULL_RESOURCE_ID";

  /// Type of a resource capacity or utilization value.
  /// (must be >= 0.)
  typedef int ResourceValue;

  /// Type of a resource kind/type.
  enum ResourceKind {UNARY, DISCRETE, RESERVOIR};

  /// Type of a resource.
  struct Resource {
    ResourceID id;
    ResourceValue capacity;
    ResourceKind kind;
    bool operator== (const Resource &s) const { return this->id == s.id; };
    bool operator!= (const Resource &s) const { return !(*this == s); };
    bool operator< (const Resource &s) const { return this->id < s.id; };
  };

  /// Type of a time (Seconds since Jan. 1, 1970).
  /// (must be >= 0, or = NULL_TIME.)
  typedef long TimeValue;

  /// NULL time (for unknown or unconstrained times).
  #define NULL_TIME -1

  /// Time Window (e.g. start or end window for a task instance or plan).
  typedef std::pair<SA_POP::TimeValue, SA_POP::TimeValue> TimeWindow;

  /// Type of a command id, indicating when command was issued.
  /// (indicates planning step, decision point within step, and
  /// number of command within decision point and step.)
  struct CommandID {
    int step;
    int decision_pt;
    int seq_num;
    bool operator== (CommandID &s) const { return (this->step == s.step && this->decision_pt == s.decision_pt && this->seq_num == s.seq_num); };
    bool operator!= (CommandID &s) const { return !(*this == s); };
    bool operator< (CommandID &s) const
    {
      if (this->step == s.step) {
        if (this->decision_pt == s.decision_pt)
          return this->seq_num < s.seq_num;
        return this->decision_pt < s.decision_pt;
      }
      return this->step < s.step;
    };
  };

  /// Type of a causal link.
  /// ("first" task instance achieves "cond" for "second" task instance.)
  struct CausalLink {
    TaskInstID first;
    TaskInstID second;
    Condition cond;
    bool operator== (const CausalLink &s) const { return (this->first == s.first && this->second == s.second && this->cond == s.cond); };
    bool operator!= (const CausalLink &s) const { return !(*this == s); };
    bool operator< (const CausalLink &s) const
    {
      if (this->first == s.first) {
        if (this->second == s.second)
          return this->cond < cond;
        return this->second < s.second;
      }
      return this->first < s.first;
    };
  };

  // Type of a set of causal links.
  typedef std::set<CausalLink> CLSet;

  /// Type of a causal link threat.
  /// ("threat" task instance threatens "clink" causal link)
  struct CLThreat {
    CausalLink clink;
    TaskInstID threat;
    bool operator== (const CLThreat &s) const { return (this->clink == s.clink && this->threat == s.threat); };
    bool operator!= (const CLThreat &s) const { return !(*this == s); };
    bool operator< (const CLThreat &s) const
    {
      if (this->clink == s.clink)
        return this->threat < s.threat;
      return this->clink < s.clink;
    };
  };

  /// Type of a precedence relation.
  enum PrecedenceRelation {UNRANKED, BEFORE, AFTER, SIMUL,
    BEFORE_SIMUL, AFTER_SIMUL};

  /// Type of a set of task implementation ids.
  typedef std::set<TaskImplID> TaskImplSet;

  /// Type of a list of task implementation ids.
  typedef std::list<TaskImplID> TaskImplList;

  /// Type of a set of task instance ids.
  typedef std::set<TaskInstID> TaskInstSet;

  /// Type of a set of task implementation parameters and associated values.
  typedef std::set<ImplParam> ImplParamSet;

  /// Type of a set of task instance ids.
  typedef std::set<TaskInstID> TaskInstSet;

  /// Type of a set of task instances mapped to task implementations.
  typedef std::map <TaskInstID, TaskImplID> InstToImplMap;

  /// Type of a set of resources and associated utilization.
  typedef std::map<ResourceID, ResourceValue> ResourceMap;

  /// Type of a set of resources (map from id to resource).
  typedef std::map<ResourceID, Resource> Resources;

	/// Type of Criticality as a pair of the Resource it is in and the value.
	typedef std::pair<ResourceID,double> Criticality;

	/// Type of a set of conditions (condition & value).
  typedef std::set<Condition> CondSet;

  

  /// Type of a set of causal link threats.
  typedef std::set<CLThreat> CLThreatSet;

  /// Type of a data connection in a plan.
  struct PlanConnection {
    TaskInstID first_task_inst;
    TaskInstID second_task_inst;
    PortID first_port;
    PortID second_port;
    CondID cond;
    bool operator== (const PlanConnection &s) const { return (this->first_task_inst == s.first_task_inst && this->second_task_inst == s.second_task_inst && this->first_port == s.first_port && this->second_port == s.second_port && this->cond == s.cond); };
    bool operator!= (const PlanConnection &s) const { return !(*this == s); };
    bool operator< (const PlanConnection &s) const
    {
      if (this->first_task_inst == s.first_task_inst) {
        if (this->second_task_inst == s.second_task_inst) {
          if (this->first_port == s.first_port) {
            if (this->second_port == s.second_port)
              return this->cond < s.cond;
            return this->second_port < s.second_port;
          }
          return this->first_port < s.first_port;
        }
        return this->second_task_inst < s.second_task_inst;
      }
      return this->first_task_inst < s.first_task_inst;
    };
  };

  /// Type of a set of plan connections.
  typedef std::set<PlanConnection> PlanConnSet;

  /// Type of an instance in a plan.
  struct PlanTaskInst {
    // Unique ID of this task instance.
    TaskInstID inst_id;

    // Human-readable name (for debugging, not identification).
    std::string name;

    // Task ID of this instance.
    TaskID task_id;

    // Task type ID of this instance.
    TaskTypeID type_id;

    // ID of suggested implementation.
    TaskImplID suggested_impl;

    // Derived start window.
    TimeWindow start_window;

    // Derived end window.
    TimeWindow end_window;

    bool operator== (const PlanTaskInst &s) const { return (this->inst_id == s.inst_id); };
    bool operator!= (const PlanTaskInst &s) const { return !(*this == s); };
    bool operator< (const PlanTaskInst &s) const
    {
      return this->inst_id < s.inst_id;
    };
  };

  /// Type of a set of plan instances.
  typedef std::set<PlanTaskInst> PlanInstSet;

  /// Type of a generic ordering link.
  typedef std::pair<TaskInstID, TaskInstID> OrderLink;

  /// Type of a scheduling link.
  typedef OrderLink SchedLink;

  /// Type of a set of scheduling links.
  typedef std::set<SchedLink> SchedLinkSet;

  // Type of a threat link.
  typedef OrderLink ThreatLink;

  /// Type of a set of threat links.
  typedef std::set<ThreatLink> ThreatLinkSet;

  /// Type of a goal ID.
  typedef std::string GoalID;

  /// Type of a plan ID.
  typedef GoalID PlanID;

  /// Type of a plan generated by SA-POP (output of a WorkingPlan object).
  struct Plan {
    // Unique ID of plan (from goal).
    PlanID plan_id;

    // Human-readable name of plan (used for debugging, not identification).
    std::string name;

    // Task instances in plan.
    PlanInstSet task_insts;

    // Data connections in plan.
    PlanConnSet connections;

    // Causal links (non-data) in plan.
    CLSet causal_links;

    // Scheduling links (added to prevent resource conflicts) in plan.
    SchedLinkSet sched_links;

    // Threat links (added to prevent causal link threats) in plan.
    ThreatLinkSet threat_links;

    // Expected start window.
    TimeWindow start_window;

    // Expected end window.
    TimeWindow end_window;

    bool operator== (const Plan &s) const { return (this->task_insts == s.task_insts && this->connections == s.connections); };
    bool operator!= (const Plan &s) const { return !(*this == s); };
    bool operator< (const Plan &s) const
    {
      if (this->task_insts == s.task_insts)
        return this->connections < s.connections;
      return this->task_insts < s.task_insts;
    };
  };

  /// Type of a set of conditions with associated time constraints.
  typedef std::map<CondID, TimeValue> TimeConstraintMap;

  /// Type of a goal for SA-POP.
  struct Goal
  {
    // Unique ID of goal.
    GoalID goal_id;

    // Human-readable name of goal (used for debugging, not identification).
    std::string name;

    // Goal conditions with utility values.
    GoalMap goal_conds;

    // Start-by and end-by constraints on conditions, in relative time
    // (relative to start of opstring to achieve this goal).
    TimeConstraintMap rel_times;

    // Start-by and end-by constraints on conditions, in clock time.
    TimeConstraintMap abs_times;

    // Expected time range for start of opstring (for achieving this goal)
    // execution.
    TimeWindow start_window;

    bool operator== (const Goal &s) const { return (this->goal_id == s.goal_id); };
    bool operator!= (const Goal &s) const { return !(*this == s); };
    bool operator< (const Goal &s) const { return this->goal_id < s.goal_id; };
  };

  

  

  /// Type of a set of open conditions, each associated with task instances
  /// for which it is a precondition.
//  typedef std::multimap<Condition, TaskInstID> OpenCondMap;

  /// Type of a set of open conditions, each associated with task instances
  /// for which it is a precondition. With new list functionality
  typedef SA_POP::ListMultiMap<Condition, TaskInstID> OpenCondMap;

  inline std::string to_string(int x)
  {
    std::ostringstream o;
    if (!(o << x))
      throw "to_string(int) error writing to ostringstream";
    return o.str();
  };

  inline std::string to_string(long x)
  {
    std::ostringstream o;
    if (!(o << x))
      throw "to_string(long) error writing to ostringstream";
    return o.str();
  };

  inline std::string to_string(unsigned long x)
  {
    std::ostringstream o;
    if (!(o << x))
      throw "to_string(unsigned long) error writing to ostringstream";
    return o.str();
  };

  inline std::string to_string(double x)
  {
    std::ostringstream o;
    if (!(o << x))
      throw "to_string(double) error writing to ostringstream";
    return o.str();
  };

  inline std::string to_string(float x)
  {
    std::ostringstream o;
    if (!(o << x))
      throw "to_string(float) error writing to ostringstream";
    return o.str();
  };

  inline std::string to_string(CommandID x)
  {
    std::ostringstream o;
    if (!(o << x.step << "." << x.decision_pt << "." << x.seq_num))
      throw "to_string(CommandID) error writing to ostringstream";
    return o.str();
  };

};  /* SA_POP namespace */

#endif /* SA_POP_TYPES_H_ */
