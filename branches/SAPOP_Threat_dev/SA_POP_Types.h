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
#include <iostream>

#include "SA_POP_Utils.h"

#if defined (SA_POP_HAS_ACE)
#include "ace/Log_Msg.h"
#include "ace/Log_Priority.h"
#endif  /* SA_POP_HAS_ACE */

#define ANKET 1
#define SA_POP_DEBUG_NORMAL 5
#define SPARTAN 9
#define SA_POP_DEBUG_HIGH 10

#if defined (SA_POP_HAS_ACE)
#define SA_POP_DEBUG(x,y) \
if (x > 8) \
ACE_DEBUG ((LM_DEBUG, ACE_LIB_TEXT("SA-POP: %s\n"), y));
#define SA_POP_DEBUG_STR(x,y) \
if (x > 8) \
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
	bool operator== (const Condition &s) const { return this->id == s.id && this->value == s.value; };
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
  typedef long TaskInstID;

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
		  if (last_instance < instance){
			this->last_instance = instance;
		  }
	  }

	 bool operator<(const SortTaskByTime & s) {
		return this->last_instance > s.last_instance;
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
    bool operator== (const CommandID &s) const { return (this->step == s.step && this->decision_pt == s.decision_pt && this->seq_num == s.seq_num); };
    bool operator!= (const CommandID &s) const { return !(*this == s); };
    bool operator< (const CommandID &s) const
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
          return this->cond < s.cond;
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

  struct TaskInstEndTimeSet{
	TaskInstID inst;
	double end_time;

	bool operator< (const TaskInstEndTimeSet & compare) const{
		return end_time < compare.end_time;
	};
  };

  
  /// Type of particular Task Implementation mapped to a Task Implementation Set.
  /// This is a particular Precedence set. Like Before, After etc.
  typedef std::map <TaskInstID, TaskInstSet> PrecedenceSet;

  /// Type of a Precedence Relation mapped to a Precedence Set.
  /// This gives the whole Precedence Graph
  typedef std::map <PrecedenceRelation, PrecedenceSet> PrecedenceGraph;
  

  /// Type of a set of open conditions, each associated with task instances
  /// for which it is a precondition.
//  typedef std::multimap<Condition, TaskInstID> OpenCondMap;

  /// Type of a set of open conditions, each associated with task instances
  /// for which it is a precondition. With new list functionality
  typedef SA_POP::ListMultiMap<Condition, TaskInstID> OpenCondMap;



  struct StoredCondition{
	
	Condition satisfied_cond;
	TaskID satisfying_task;
	bool satisfied_to;

	StoredCondition(){};

	StoredCondition(Condition cond, TaskID task){
		this->satisfied_cond = cond;
		this->satisfied_to = cond.value;
		this->satisfying_task = task;
	}

	bool operator==(const StoredCondition & s)const{
		return (satisfied_cond == s.satisfied_cond &&
			satisfying_task == s.satisfying_task &&
			satisfied_to == s.satisfied_to);
	};

	bool operator<(const StoredCondition & s) const{
		if(this->satisfied_cond == s.satisfied_cond)
			return this->satisfying_task < s.satisfying_task;
		return this->satisfied_cond < s.satisfied_cond;
	};
  };

  struct StoredConditionKey{

	CommandID satisfying_cmd;
	CommandID free_pass_cmd;

	StoredCondition satisfy_set;
	bool free_pass_used;

//	StoredConditionKey(){};

	StoredConditionKey(Condition cond, TaskID task, CommandID cmd){
		satisfying_cmd = cmd;
		satisfy_set.satisfied_cond = cond;
		satisfy_set.satisfying_task = task;

		free_pass_cmd.decision_pt = 0;
		free_pass_cmd.step = 0;
		free_pass_cmd.seq_num = 0;

		free_pass_used = false;
	}

	bool operator==(const StoredConditionKey & s)const{
		return (this->satisfy_set == s.satisfy_set);
	};

	bool operator<(const StoredConditionKey & s) const {
	//	if(this->satisfying_cmd == s.satisfying_cmd)
			return this->satisfy_set < s.satisfy_set;
	//	return this->satisfying_cmd < s.satisfying_cmd;
	};

  };



  typedef std::set<StoredCondition> StoredConditionSet;
  typedef std::multimap<StoredConditionKey, StoredConditionSet> StoredConditionMap;

  struct StoredConditionEvaluator{
	  StoredConditionMap condition_map;
	
	  std::pair<bool, CommandID> should_continue(CommandID cur_cmd, Condition satisfied_cond, 
		  TaskID satisfying_task, OpenCondMap & open_conds, InstToTaskMap & task_insts){

			  StoredConditionKey stored_cond (satisfied_cond, satisfying_task, cur_cmd);

			  std::pair<StoredConditionMap::iterator,
				   StoredConditionMap::iterator>
				  range 
				  = condition_map.equal_range(stored_cond);

			  bool should_continue = true;
			  CommandID return_to;
			  return_to.decision_pt = 0;
			  return_to.seq_num = 0;
			  return_to.step = 0;


			  bool using_free_pass = false;
	
			  if(range.first != range.second)
			  {
				for(StoredConditionMap::iterator it = range.first; it != range.second; it++)
				{
					bool all_pairs_in_open_conds = true;

					StoredConditionSet old_open_conds = it->second;
		
					for(StoredConditionSet::iterator it2 = old_open_conds.begin();
						it2 != old_open_conds.end(); it2++)
					{


						std::pair<OpenCondMap::iterator, OpenCondMap::iterator>
							cond_map_range = open_conds.equal_range((*it2).satisfied_cond);

						bool found_no_equiv_in_open = true;

						for(OpenCondMap::iterator it3 = cond_map_range.first; it3 != cond_map_range.second; it3++){
							if((*it2).satisfying_task == -1){
								if((*it3).second == -1){
									found_no_equiv_in_open = false;
								}
							}
							else{
								if((*it3).second == -1)
								{}
								else if((*it2).satisfying_task == task_insts.find((*it3).second)->second &&
									(*it2).satisfied_to == ((*it3).first).value){
									found_no_equiv_in_open = false;
								}
							}
						}

						if(found_no_equiv_in_open){
							all_pairs_in_open_conds = false;
							break;
						}
					}

					if(all_pairs_in_open_conds){

			//			if(!(it->first.free_pass_used)){
							
			//				StoredConditionKey key2 = it->first;
			//				StoredConditionSet set2 = it->second;

			//				condition_map.erase(it);
							
			//				key2.free_pass_used = true;
			//				key2.free_pass_cmd = cur_cmd;

			//				condition_map.insert(std::pair<StoredConditionKey, StoredConditionSet>(key2, set2));
			
			//				using_free_pass = true;

			//				break;
			//			}else{
							should_continue = false;
				//			return_to = it->first.free_pass_cmd;
							return_to = it->first.satisfying_cmd;
							break;
			//			}
					}
				}
			}else{
				should_continue = true;
			}

			if(should_continue && !using_free_pass)
			{

				StoredConditionSet current_open;
				for(OpenCondMap::iterator it = open_conds.begin(); it != open_conds.end(); it++)
				{
					if(it->second == -1)
					{
						current_open.insert(StoredCondition(it->first, -1));
					}else{
						current_open.insert(StoredCondition(it->first, task_insts.find(it->second)->second));
					}
				}

				StoredConditionKey this_choice(satisfied_cond, satisfying_task, cur_cmd);

				this->condition_map.insert(
					std::pair<StoredConditionKey, StoredConditionSet>(this_choice, current_open));
			}

			return std::pair<bool, CommandID>(should_continue, return_to);
		}

	  void undo_binding(CommandID cur_cmd, Condition satisfied_cond, TaskID satisfying_task){
		StoredConditionKey key(satisfied_cond, satisfying_task, cur_cmd);

		std::pair<StoredConditionMap::iterator, StoredConditionMap::iterator> range
			= condition_map.equal_range(key);

		StoredConditionMap::iterator it;

		for(it = range.first; it != range.second; it++){
			if(it->first.satisfying_cmd == cur_cmd){

		//		if(it->first.free_pass_used == true){
		//			StoredConditionKey key2 = it->first;
		//			StoredConditionSet set2 = it->second;

		//			condition_map.erase(it);

		//			key2.free_pass_used = false;

		//			condition_map.insert(std::pair<StoredConditionKey, StoredConditionSet>(key2, set2));
		//			break;
		//		}else{				
					condition_map.erase(it);
					break;
		//		}
			}
		}



	  }
  };

  //Map from a causal link to the suspended conditions that are waiting for something to come
  //between them and the orig. condition so they can be unsuspended
 // typedef std::map<CLink, std::list<std::pair<Condition, TaskInstID>>> SuspendedConditionListenerMap;
  typedef std::multimap<CausalLink, std::pair<Condition, TaskInstID>> SuspendedConditionListenerMap;

  //List of all condition/task insts that are suspended
  typedef std::set<std::pair<Condition, TaskInstID>> SuspendedConditionSet;

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
