// -*- C++ -*-

//=============================================================================
/**
 * @file  SANode.h
 *
 * This file contains the Node, CondNode, TaskNode and class
 * definitions for spreading activation network nodes.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#ifndef SA_NODE_H_
#define SA_NODE_H_

#include <map>
#include <string>
#include <stdexcept>
#include <iostream>
#include "SANet_Types.h"

namespace SANet {
  // Forward declaration of Node class.
  class Node;

  /// Map from node ID to pointer.
  typedef std::map<NodeID, Node *> NodeMap;

  /// Map from node ID to link weight or conditional probability.
  typedef std::map<NodeID, LinkWeight> LinkMap;

  /// Map from node ID to expected utility.
  typedef std::map<NodeID, Utility> UtilityMap;

  /// Expected utility info for a single node (at a particular step).
  typedef struct {
    /// Composite utility value.
    Utility utility;

    /// Map of goal node ID's to expected utilities (to avoid loops).
    UtilityMap common;
  } Utility_Info;

  /// Probability map from node ID to probability (positive ID's indicate
  /// probability node value is true and negative ID's indicate probability
  /// node value is false).
  typedef std::map<CondID, Probability> ProbabilityMap;

  /// Probability info for a single node (at a particular step).
  struct Probability_Info {
    /// Composite probability value for the node (maximum probability for
    /// condition nodes or conditional probability divided by prior
    /// probability for task nodes).
    Probability probability;
		
    /// Map of node ID's to component probabilities needed to avoid loops.
    ProbabilityMap common;
  };



  /**
   * @class Node
   *
   * @brief Abstract base class for nodes in the network.
   */
  class Node {
  public:
    /// Constructor.
    /**
     * @param ID  Node ID.
     *
     * @param name  Node name.
     *
     * @param atten_factor  Attenuation factor (to bias toward shorter plans).
     */
    Node (NodeID ID, std::string name, MultFactor atten_factor);

    /// Destructor.
    virtual ~Node ();

    /// Get (positive) expected utility info (from goals) for a given step.
    /**
     * @param step  Step for which to get reward map.
     *
     * @return  Positive expected utility info.
     */
    virtual Utility_Info get_reward (int step);

    /// Get (maximum) probability info for a given step and true/false value.
    /**
     * @param step  Step for which to get probability info.
     *
     * @param value  Value for which to get probability (default = true).
     *
     * @return  Probability info.
     */
    virtual Probability_Info get_prob (int step, bool value = true);

    /// Update node to next step.
    /**
     * @return  True if node changed probability or utility, false otherwise.
     */
    virtual bool update (void) = 0;

    /// Did node change probability info on last update?
    /**
     * @return  True if node changed probability info, false otherwise.
     */
    virtual bool prob_changed (void);

    /// Did node change utility info on last update?
    /**
     * @return  True if node changed utility info, false otherwise.
     */
    virtual bool util_changed (void);

    /// Get node name.
    /**
     * @return  Name of node.
     */
    virtual std::string get_name (void);

    /// Get node ID.
    /**
     * @return  ID of node.
     */
    virtual NodeID get_ID (void);

    /// Get pre-links (nodes with links to this node).
    /**
     * @return  Map of pre-node IDs to link weights.
     */
    virtual const LinkMap& get_pre (void);

    /// Get post-links (nodes with links from this node).
    /**
     * @return  Map of post-node IDs to link weights.
     */
    virtual const LinkMap& get_post (void);

    /// Print node.
    /**
     * @param strm  Output stream on which to print node representation.
     *
     * @param verbose  Whether to print verbose representation.
     */
    virtual void print (std::basic_ostream<char, std::char_traits<char> >& strm
      = std::cout, bool verbose = false) = 0;

    /// Print XML representation of node to stream.
    /**
     * @param strm  Output stream on which to print node XML representation.
     */
    virtual void print_xml (std::basic_ostream<char, std::char_traits<char> >& strm) = 0;



  protected:
    /// Unique ID of node (for identification within network).
    NodeID ID_;

    /// Name of node (descriptive only).
    std::string name_;

    /// Attenuation factor (to bias toward shorter plans).
    MultFactor atten_factor_;

    /// Current step.
    int step_;

    /// Nodes with links TO this node.
    NodeMap pre_nodes_;

    /// Nodes with links FROM this node.
    NodeMap post_nodes_;

    /// Link weights for pre-nodes.
    LinkMap pre_links_;

    /// Link weights for post-nodes.
    LinkMap post_links_;

    /// Flag for whether probability info was changed on last update.
    bool prob_changed_;

    /// Flag for whether utility info was changed on last update.
    bool util_changed_;

    /// Positive expected utility info.
    Utility_Info pos_util_;

    /// Negative expected utility info.
    Utility_Info neg_util_;

    /// Probability that value is true (equivalent to success for tasks).
    Probability_Info true_prob_;

    /// Probability that value is false (equivalent to failure for tasks).
    Probability_Info false_prob_;
  };

  // Forward declaration of CondNode class.
  class CondNode;

  /**
   * @class TaskNode
   *
   * @brief Task nodes in the network.
   */
  class TaskNode : public Node {
  public:
    /// Constructor.
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
    TaskNode (TaskID ID, std::string name, MultFactor atten_factor, TaskCost cost,
      Probability prior_prob);

    /// Destructor.
    virtual ~TaskNode (void);

    /// Print node.
    /**
     * @param strm  Output stream on which to print node representation.
     *
     * @param verbose  Whether to print verbose representation.
     */
    virtual void print (std::basic_ostream<char, std::char_traits<char> >& strm
      = std::cout, bool verbose = false);

    /// Print XML representation of node to stream.
    /**
     * @param strm  Output stream on which to print node XML representation.
     */
    virtual void print_xml (std::basic_ostream<char, std::char_traits<char> >& strm);

    /// Print XML representation of node's precondition links to stream.
    /**
     * @param strm  Output stream on which to print link XML representation.
     */
    virtual void print_precond_links_xml (std::basic_ostream<char, std::char_traits<char> >& strm);

    /// Print XML representation of node's effect links to stream.
    /**
     * @param strm  Output stream on which to print link XML representation.
     */
    virtual void print_effect_links_xml (std::basic_ostream<char, std::char_traits<char> >& strm);

    /// Print Graphviz representation of node's pre-condition links to stream.
    /**
     * @param strm  Output stream on which to print links into Graphviz representation.
     */
    virtual void print_preconds_graphviz(std::basic_ostream<char, std::char_traits<char> >& strm);

    /// Print Graphviz representation of node's effect links to stream.
    /**
     * @param strm  Output stream on which to print links into Graphviz representation.
     */
    virtual void print_effects_graphviz(std::basic_ostream<char, std::char_traits<char> >& strm);

    /// Update node to next step.
    /**
     * @return  True if node changed probability or utility, false otherwise.
     */
    virtual bool update (void);

    /// Get expected utility for a given step.
    /**
     * @param step  Step for which to get expected utility.
     *
     * @return  Expected utility.
     */
    virtual Utility get_utility (int step);

    /// Get Prior of the TaskNode.
    /**
     * 
     *
     * @return  Expected utility.
     */
    virtual Probability get_prior (void);

    /// Update Prior of the TaskNode.
    /**
     * 
     * @param prior the new prior for the task
     *
     * 
     */
    virtual void update_prior (Probability prior);

    /// Add precondition link.
    /**
     * @param ID  Node ID.
     *
     * @param node  Node pointer.
     *
     * @param true_prob  Conditional probability of success given
     * node = true.
     *
     * @param false_prob  Conditional probability of success given
     * node = false.
     */
    virtual void add_precond (CondID ID, CondNode *node, Probability true_prob,
      Probability false_prob);

    /// Add effect link.
    /**
     * @param ID  Node ID.
     *
     * @param node  Node pointer.
     *
     * @param weight  Link weight (probability task sets condition to
     * true, or negative of the probability task sets condition to false).
     */
    virtual void add_effect (CondID ID, CondNode *node, LinkWeight weight);

    /// Update effect link.
    /**
     * @param ID  Node ID.
     *
     * @param node  Node pointer.
     *
     * @param weight  Link weight (probability task sets condition to
     * true, or negative of the probability task sets condition to false).
     */
    virtual void update_effect (CondID ID, CondNode *node, LinkWeight weight);

	void set_pos_util(double util);

	Utility_Info get_pos_util();


  protected:
    /// Unconditional prior probability of success.
    Probability prior_prob_;

    /// Cost of performing task.
    TaskCost cost_;

    /// Conditional probabilities of success for pre-nodes having true values.
    LinkMap pre_true_probs_;

    /// Conditional probabilities of success for pre-nodes having false values.
    LinkMap pre_false_probs_;
  };

  /**
   * @class CondNode
   *
   * @brief Condition nodes in the network.
   */
  class CondNode : public Node {
  public:
    /// Constructor.
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
     * @param goal_util  Initial utility (positive for goals, zero otherwise).
     *
     * @param cond_kind  The type of condition.
     *
     */
    CondNode (CondID ID, std::string name, MultFactor atten_factor,
       Probability true_prob, Probability false_prob, Utility goal_util, CondKind cond_kind);

    /// Destructor.
    virtual ~CondNode (void);

    /// Update goal utility.
    /**
     * @param util  Goal utility.
     */
    virtual void set_goal_util (Utility util);

    /// Get initial/current probability.
    /**
     * @param value  Value for which to get probability (default = true).
     *
     * @return  Initial probability of given value.
     */
    virtual Probability get_init_prob (bool value = true);

    /// Update initial/current probability.
    /**
     * @param init_true_prob  Initial/current probability of truth.
     */
    virtual void set_init_prob (Probability init_true_prob);

    /// Print node.
    /**
     * @param strm  Output stream on which to print node representation.
     *
     * @param verbose  Whether to print verbose representation.
     */
    virtual void print (std::basic_ostream<char, std::char_traits<char> >& strm
      = std::cout, bool verbose = false);

    /// Print XML representation of node to stream.
    /**
     * @param strm  Output stream on which to print node XML representation.
     */
    virtual void print_xml (std::basic_ostream<char, std::char_traits<char> >& strm);

    /// Update node to next step.
    /**
     * @return  True if node changed probability or utility, false otherwise.
     */
    virtual bool update (void);

    /// Add pre-link.
    /**
     * @param ID  Node ID.
     *
     * @param node  Node pointer.
     *
     * @param weight  Link weight.
     */
    virtual void add_pre_link (TaskID ID, TaskNode *node, LinkWeight weight);

    /// update pre-link.
    /**
     * @param ID  Node ID.
     *
     * @param node  Node pointer.
     *
     * @param weight  Link weight.
     */
    virtual void update_pre_link (TaskID ID, TaskNode *node, LinkWeight weight);

    /// Add post-link.
    /**
     * @param ID  Node ID.
     *
     * @param node  Node pointer.
     *
     * @param weight  Link weight.
     */
    virtual void add_post_link (TaskID ID, TaskNode *node, LinkWeight weight);

    /// Get the kind/type of condition.
    /**
     * @return The kind of the condition.
     */
    virtual CondKind get_cond_kind();

  protected:
    /// Goal utility for this node.
    Utility goal_util_;

    /// Initial probability (of truth/success) for this node.
    Probability init_true_prob_;

    /// Which node (ID) the current true probability value came from.
    TaskID true_prob_from_;

    /// Which node (ID) the current false probability value came from.
    TaskID false_prob_from_;

    /// Kind/type of the condition.
    CondKind cond_kind_;
  };


};


#endif /* SA_NODE_H_ */
