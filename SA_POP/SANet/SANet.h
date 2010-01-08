// -*- C++ -*-

//=============================================================================
/**
 * @file  SANet.h
 *
 * This file contains the Network class definition for spreading activation
 * networks.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_NETWORK_H_
#define SA_NETWORK_H_

#include <iostream>
#include <map>
#include <set>
#include "SANet_Types.h"
#include "SANode.h"

namespace SANet {
  /// Set of task node IDs.
  typedef std::set<TaskID> TaskIDSet;

  /// Set of condition node IDs.
  typedef std::set<CondID> CondIDSet;

  /// Map from task node ID to pointer.
  typedef std::map<TaskID, TaskNode *> TaskNodeMap;

  /// Map from condition node ID to pointer.
  typedef std::map<CondID, CondNode *> CondNodeMap;

  /// Map of precondition links to ports.
  typedef std::map<PrecondLink, PortID> PrecondLinkPortMap;

  /// Map of effect links to ports.
  typedef std::map<EffectLink, PortID> EffectLinkPortMap;

  /**
   * @class Network
   *
   * @brief Spreading activation network class.
   */
  class Network {
  public:
    /// Constructor.
    Network (void);

    /// Copy constructor. Performs initialization by making a deep copy
    /// of the provided network (including allocation of new nodes with
    /// state copied from nodes in provided network and creation of
    /// corresponding links between them).
    /**
     * @param s  Network to copy.
     */
    Network (const Network &s);

    /// Destructor.
    virtual ~Network ();



    // ************************************************************************
    // Network creation methods.
    // ************************************************************************

    /// Add a new task node to the network.
    /**
     * @param ID  Node ID.
     *
     * @param name  Node name.
     *
     * @param atten_factor  Attenuation factor (to bias toward shorter plans).
     *
     * @param cost  Cost of performing task.
     *
     * @param prior_prob  Prior probability of success.
     */
    virtual void add_task (TaskID ID, std::string name,
      MultFactor atten_factor, TaskCost cost, Probability prior_prob);

    /// Add a new condition node to the network.
    /**
     * @param ID  Node ID.
     *
     * @param name  Node name.
     *
     * @param atten_factor  Attenuation factor (to bias toward shorter plans).
     *
     * @param true_prob  Initial probability that value is true.
     *
     * @param false_prob  Initial probability that value is false.
     *
     * @param cond_kind  The type of condition.
     *
     * @param goal_util  Initial utility (positive for TRUE goals, negative for FALSE goals, and zero for non-goals).
     */
    virtual void add_cond (CondID ID, std::string name,
      MultFactor atten_factor, Probability true_prob, Probability false_prob,
      Utility goal_util, CondKind cond_kind);

    /// Add condition to task link.
    /**
     * @param cond_ID  Condition node ID.
     *
     * @param task_ID  Task node ID.
     *
     * @param true_prob  Conditional probability of task success given
     * condition node = true.
     *
     * @param false_prob  Conditional probability of task success given
     * condition node = false.
     *
     * @param port_ID  ID of port (on task) associated with this condition
     *                 (used for data nodes).
     */
    virtual void add_precond_link (CondID cond_ID, TaskID task_ID,
      Probability true_prob, Probability false_prob, PortID port_ID = "");

    /// Add task to condition link.
    /**
     * @param cond_ID  Condition node ID.
     *
     * @param task_ID  Task node ID.
     *
     * @param weight  Link weight (probability task sets condition to
     * true, or negative of the probability task sets condition to false).
     *     * @param port_ID  ID of port (on task) associated with this condition
     *                 (used for data nodes).
     */
    virtual void add_effect_link (TaskID task_ID, CondID cond_ID,
      LinkWeight weight, PortID port_ID = "");




    // ************************************************************************
    // Print methods.
    // ************************************************************************

    /// Print network.
    /**
     * @param strm  Output stream on which to print network representation.
     *
     * @param verbose  Whether to print verbose representation.
     */
    virtual void print (std::basic_ostream<char, std::char_traits<char> >& strm
      = std::cout, bool verbose = false);
    
    /// Print Graphviz network representation to stream.
    /**
     * @param strm  Output stream on which to print network representation.
     *
     * @param graphmap  The color mapping being used.
     *
     * @param defaultColor  The default color if it's not in the mapping.     
     */
    virtual void print_graphviz (std::basic_ostream<char,
      std::char_traits<char> >& strm, std::map<std::string, std::string>& graphmap,
      std::string defaultColor = "grey");

    /// Print XML network representation to stream.
    /**
     * @param strm  Output stream on which to print network representation.
     */
    virtual void print_xml (std::basic_ostream<char, std::char_traits<char> >& strm);

    /// Print links and ports.
    /**
     * @param strm  Output stream on which to print network representation.
     *
     * @param verbose  Whether to print verbose representation.
     */
    virtual void print_link_ports (std::basic_ostream<char,
      std::char_traits<char> >& strm = std::cout, bool verbose = false);



    // ************************************************************************
    // Network update methods (spreading activation, environment/system state
    // changes, and goal changes).
    // ************************************************************************

    /// Run spreading activation.
    /**
     * @param max_steps  Maximum steps for which to run spreading activation.
     */
    virtual void update (int max_steps);

    /// Update a condition's current value (probability of being true).
    /**
     * @param cond_id  The condition id.
     *
     * @param true_prob  New probability that condition is true.
     */
    virtual void update_cond_val (CondID cond_id, Probability true_prob);

    /// Update a condition's (goal) utility.
    /**
     * @param cond_id  The condition id.
     *
     * @param utility  New goal utility of condition.
     */
    virtual void update_cond_util (CondID cond_id, Utility utility);

    /// Update all goal condition utilities based on new goal set.
    /**
     * @param goals  Set of goal condition ids and associated utilities.
     */
    virtual void update_goals (GoalMap goals);

    /// Update the prior probability of a task.
    /**
     * @param task_ID  Task node ID.
     *
     * @param prior  New prior probability;
     */
    virtual void update_prior (TaskID task_ID, Probability prior);

    /// Update a task to condition link.
    /**
     * @param cond_ID  Condition node ID.
     *
     * @param task_ID  Task node ID.
     *
     * @param weight  Link weight (probability task sets condition to
     * true, or negative of the probability task sets condition to false).
     *
     * @param port_ID  ID of port (on task) associated with this condition
     *                 (used for data nodes).
     */
    virtual void update_effect_link (TaskID task_ID, CondID cond_ID,
      LinkWeight weight, PortID port_ID= "");

    /// Set the state of a task node.
    /**
     * @param task_ID  Task node ID.
     *
     * @param state  New state (false for inactive, and true for active);
     */
    virtual void set_task_state(TaskID task_ID, bool state);

    /// Set the state of a condition node.
    /**
     * @param cond_ID  Condition node ID.
     *
     * @param state  New state (false for inactive, and true for active);
     */
    virtual void set_cond_state(CondID cond_ID, bool state);

    /// Set state of all nodes.
    /**
     * @param state  New state (false for inactive, and true for active);
     */
    virtual void set_nodes_state(bool state);





    // ************************************************************************
    // General task/condition accessor methods.
    // ************************************************************************

    /// Get a task's name.
    /**
     * @param task_id  The task id.
     *
     * @return  Task name.
     */
    virtual std::string get_task_name (TaskID task_id);

    /// Get a condition's name.
    /**
     * @param cond_id  The condition id.
     *
     * @return  Condition name.
     */
    virtual std::string get_cond_name (CondID cond_id);

    /// Get a condition's type/kind.
    /**
     * @param cond_id  The condition id.
     *
     * @return  Condition type.
     */
    virtual CondKind get_cond_type (CondID cond_id);

    /// Get a condition's current value (probability of being true).
    /**
     * @param cond_id  The condition id.
     *
     * @return  Probability that condition is true.
     */
    virtual Probability get_cond_val (CondID cond_id);

    /// Get a condition's future probability for a given value.
    /// (NOTE: Future probability is based on whatever spreading
    /// activiation has already been executed.)
    /// (WARNING: Condition node must have been active for all
    /// spreading activation or exception will be thrown.)
    /**
     * @param cond_id  Condition ID.
     *
     * @param value  Value for which to get condition future probability.
     *
     * @return  Future task expected utility.
     */
    Probability get_cond_future_val(CondID cond_id, bool value);

    /// Get all goals.
    /**
     * @return  Set of condition ids and associated utilities.
     */
    virtual const GoalMap& get_goals (void);

    /// Get a task's future expected utility (EU) from spreading activation.
    /// (NOTE: Future EU is based on whatever spreading
    /// activation has already been executed).
    /**
     * @param task_id  The task id.
     *
     * @return  Future task expected utility.
     */
    virtual Utility get_task_sa_eu (TaskID task_id);

    /// Get all preconditions of a task.
    /**
     * @param task_id  The task id.
     *
     * @return  Set of all preconditions with associated values.
     */
    virtual CondSet get_preconds (TaskID task_id);

    /// Get all effects of a task.
    /**
     * @param task_id  The task id.
     *
     * @return  Set of all effects with associated values.
     */
    virtual CondSet get_effects (TaskID task_id);

    /// Get all tasks that satisfy a condition.
    /**
     * @param cond_id  The condition id.
     *
     * @return  Set of all tasks that satisfy the given condition.
     */
    virtual TaskSet get_satisfying_tasks (Condition cond);

    /// Get the name of the port associated with a precondition of a task.
    /**
     * @param cond_id  The precondition id.
     *
     * @param task_id  The task id.
     *
     * @return  Port id.
     */
    virtual PortID get_precond_port (CondID cond_id, TaskID task_id);

    /// Get the name of the port associated with an effect of a task.
    /**
     * @param task_id  The task id.
     *
     * @param cond_id  The effect condition id.
     *
     * @return  Port id.
     */
    virtual PortID get_effect_port (TaskID task_id, CondID cond_id);

    /// Get ports for a causal link.
    /**
     * @param task1_id  ID of start task node in causal link.
     *
     * @param cond_id  ID of condition node in both precondition and effect
     *                 links.
     *
     * @param task2_id  ID of end task node in causal link.
     */
    virtual LinkPorts get_clink_ports (TaskID task1_id, CondID cond_id,
      TaskID task2_id);

    /// Get the prior probability for a task node in the network.
    /**
     * @param ID  ID of task node.
     *
     * @returns  Prior probability of the task.
     */
    virtual Probability get_prior (TaskID ID);

    /// Get the probability of a task's effect in the network.
    /**
     * @param task_ID  ID of task node.
     *
     * @param cond_ID  ID of effect condition.
     *
     * @returns  Probability of effect given successful task execution.
     */
    virtual Probability get_effect_prob(TaskID task_ID, CondID cond_ID);


    // ****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
    // Ankit's function get_duration() needs to be removed, once we ensure any
    // scheduling code relying on it has been changed.
    // ****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

    /// Get the duration of a task
    /**
     * @param task_id  Task ID.
     *
     * @return  Duration of the task.
     */
    virtual TimeValue get_duration (TaskID task_id);



  protected:
    // ************************************************************************
    // Structure variables.
    // ************************************************************************

    /// Map from ID to node pointer for all task nodes in network.
    TaskNodeMap task_nodes_;

    /// Map from ID to node pointer for all condition nodes in network.
    CondNodeMap cond_nodes_;

    /// Map from precondition links to associated ports.
    PrecondLinkPortMap precond_links_;

    /// Map from effect links to associated ports.
    EffectLinkPortMap effect_links_;



    // ************************************************************************
    // State variables.
    // ************************************************************************

    /// Set of active task nodes (by ID).
    TaskIDSet active_tasks_;

    /// Set of disabled task nodes (by ID).
    TaskIDSet disabled_tasks_;

    /// Set of active condition nodes (by ID).
    CondIDSet active_conds_;

    /// Set of disabled condition nodes (by ID).
    CondIDSet disabled_conds_;

    /// Goals.
    GoalMap goals_;

    /// Current step.
    int step_;


    // ************************************************************************
    // State helper methods.
    // ************************************************************************

    void reset_step();
  };
};


#endif /* SA_NETWORK_H_ */
