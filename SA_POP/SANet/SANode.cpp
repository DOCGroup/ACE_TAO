// -*- C++ -*-

//=============================================================================
/**
 * @file  SANode.cpp
 *
 * This file contains the Node, CondNode, & TaskNode class
 * implementations for spreading activation network nodes.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#include "SANode.h"
#include "SANet_Exceptions.h"
#include <utility>
#include <iostream>

using namespace SANet;

Node::Node (NodeID ID, std::string name, MultFactor atten_factor)
: ID_ (ID),
  name_ (name),
  atten_factor_ (atten_factor),
  step_ (0),
  prob_changed_ (false),
  util_changed_ (false)
{
  // Initialize probability and utility info.
  pos_util_.utility = 0;
  pos_util_.common.clear ();
  neg_util_.utility = 0;
  neg_util_.common.clear ();
  true_prob_.probability = 0;
  true_prob_.common.clear ();
  false_prob_.probability = 0;
  false_prob_.common.clear ();

  // Initialize (empty) pre and post node maps and links.
  pre_nodes_.clear ();
  pre_links_.clear ();
  post_nodes_.clear ();
  post_links_.clear ();
};

Node::~Node ()
{
  // Nothing to do.
};

Utility_Info Node::get_reward (int step)
{
  // Check to ensure step is the current step, or else throw exception
  if (step != step_) {
    throw Invalid_Step ();
  }

  // Return positive utility info.
  return pos_util_;
};

Probability_Info Node::get_prob (int step, bool value)
{
  // Check to ensure step is the current step, or else throw exception
  if (step != step_) {
    throw Invalid_Step ();
  }

  // Return probability for true if value is true.
  if (value) {
    return true_prob_;
  }
  // Otherwise return probability for false.
  return false_prob_;
};

bool Node::prob_changed (void)
{
  return prob_changed_;
};

bool Node::util_changed (void)
{
  return util_changed_;
};

std::string Node::get_name (void)
{
  return name_;
};

NodeID Node::get_ID (void)
{
  return ID_;
};

// Get pre-links (nodes with links to this node).
const LinkMap& Node::get_pre (void)
{
  return this->pre_links_;
};

// Get post-links (nodes with links from this node).
const LinkMap& Node::get_post (void)
{
  return this->post_links_;  
};

TaskNode::TaskNode (NodeID ID, std::string name, MultFactor atten_factor,
                          TaskCost cost, Probability prior_prob)
: Node (ID, name, atten_factor),
  cost_ (cost),
  prior_prob_ (prior_prob)
{
  // Nothing to do.
};

TaskNode::~TaskNode (void)
{
  // Nothing to do.
};

bool TaskNode::update (void)
{
  // Reset change flags.
  prob_changed_ = false;
  util_changed_ = false;

  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  // Probability flag is not being set correctly, so run to max steps...
  prob_changed_ = true;
  util_changed_ = true;
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

  // Flag for detection of loops.
  bool is_loop = false;

  // Variables for current node and link in updates.
  CondNode *cur_node;
  CondID cur_ID;
  LinkMap::iterator link_iter;
  LinkWeight cur_weight;
  MultFactor cur_mult;

  // Variables for current probability value.
  Probability_Info cur_prob;
  ProbabilityMap::iterator true_prob_iter;
  ProbabilityMap::iterator false_prob_iter;
  ProbabilityMap::iterator prev_prob_iter;

  // Reset probability info.
  true_prob_.probability = prior_prob_;
  //****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  // clearing common probabilities set removes self (if "loop" found where
  // this node is receiving utility from same goal on multiple paths).  so
  // will never realize that nothing has changed.
  // and (possible) common probabilities are never removed from the set once
  // detected so this does not need to be cleared as long as any
  // changed probabilities are appropriately changed in common set... right??? 
  true_prob_.common.clear ();

  // Update probability.
  for (NodeMap::iterator node_iter = pre_nodes_.begin ();
    node_iter != pre_nodes_.end (); node_iter++)
  {
    // Get current node info.
    cur_ID = node_iter->first;
    cur_node = (CondNode *) node_iter->second;

    // Get current link info.
    link_iter = pre_links_.find(cur_ID);
    if (link_iter == pre_links_.end ()) {
      throw Update_Error ();
    }
    cur_weight = link_iter->second;

    // Get current probability info.
    try {
      if (cur_weight >= 0) {
        cur_prob = cur_node->get_prob (step_, true);
      } else {
        cur_prob = cur_node->get_prob (step_, false);
      }
    } catch (Invalid_Step e) {
      std::cerr << "Error in task node update:  Invalid step value.";
      std::cerr << std::endl;
      return false;
    } catch (...) {
      std::cerr << "Unexpected exception thrown in task node update.";
      std::cerr << std::endl;
      return false;
    }

    // Get conditional probability of success for true precondition.
    true_prob_iter = pre_true_probs_.find (cur_ID);
    if (true_prob_iter == pre_true_probs_.end ()) {
      throw Update_Error ();
    }

    // Get conditional probability of success for false precondition.
    false_prob_iter = pre_false_probs_.find (cur_ID);
    if (false_prob_iter == pre_false_probs_.end ()) {
      throw Update_Error ();
    }

    // Update true probability depending on sign of link weight.
    if (cur_weight >= 0) {
      // Update conditional probability of success given this node.
      cur_prob.probability = ((cur_prob.probability * true_prob_iter->second) +
        ((1 - cur_prob.probability) * false_prob_iter->second));

      // Update probability.
      true_prob_.probability = true_prob_.probability * cur_prob.probability /
        prior_prob_;
    } else {
      // Update conditional probability of success given this node.
      cur_prob.probability = ((cur_prob.probability * false_prob_iter->second)
        + ((1 - cur_prob.probability) * true_prob_iter->second));

      // Update probability.
      true_prob_.probability = true_prob_.probability * cur_prob.probability /
        prior_prob_;
    }

    // Update prob_changed_ flag if necessary.
    if (cur_node->prob_changed ()) {
      prob_changed_ = true;
    }

    // Update common probabilities and handle duplicates.
    for (ProbabilityMap::iterator prob_iter = cur_prob.common.begin ();
      prob_iter != cur_prob.common.end (); prob_iter++)
    {
      prev_prob_iter = true_prob_.common.find (prob_iter->first);

      // If a common probability is already in the probability map,
      // divide from probability, otherwise add it to the probability map.
      if (prev_prob_iter != true_prob_.common.end ())
      {
        true_prob_.probability = true_prob_.probability / 
          prev_prob_iter->second;
      } else {
        true_prob_.common.insert (std::make_pair (prob_iter->first,
          prob_iter->second));
      }
    }
  }

  // Variables for current utility updates.
  Utility_Info cur_util;
  UtilityMap::iterator prev_util_iter;

  // Reset utility info.
  pos_util_.utility = 0;
  pos_util_.common.clear ();
  neg_util_.utility = 0;
  neg_util_.common.clear ();

  // Update utility.
  for (NodeMap::iterator node_iter = post_nodes_.begin ();
    node_iter != post_nodes_.end (); node_iter++)
  {
    // Get current node info.
    cur_ID = node_iter->first;
    cur_node = (CondNode *) node_iter->second;

    // Update util_changed_ flag if necessary.
    if (cur_node->util_changed ()) {
      util_changed_ = true;
    }

    // Get current link info.
    link_iter = post_links_.find(cur_ID);
    if (link_iter == post_links_.end ()) {
      throw Update_Error ();
    }
    cur_weight = link_iter->second;

    // Set multiplier as attenuation factor times link weight, probability
    // that task will succeed, and probability that effect node has the
    // opposite value from what task would change it to.
    if (cur_weight >= 0) {
      cur_mult = atten_factor_ * cur_weight * true_prob_.probability *
        cur_node->get_prob (step_, false).probability;
    } else {
      cur_mult = atten_factor_ * cur_weight * true_prob_.probability *
        cur_node->get_prob (step_, true).probability;
    }

    // Get current utility info.
    try {
      cur_util = cur_node->get_reward (step_);
    } catch (Invalid_Step e) {
      std::cerr << "Error in task node update:  Invalid step value.";
      std::cerr << std::endl;
      return false;
    } catch (...) {
      std::cerr << "Unexpected exception thrown in task node update.";
      std::cerr << std::endl;
      return false;
    }

    // Updated utility based on current multiplier.
    cur_util.utility = cur_mult * cur_util.utility;

    // Update positive or negative utilities depending on sign of link weight.
    if (cur_weight >= 0) {
      // Add utility to expected utility value.
      pos_util_.utility += cur_util.utility;

      // Update all component utilities based on link weight, and handle
      // utilities from common goals.
      for (UtilityMap::iterator util_iter = cur_util.common.begin ();
        util_iter != cur_util.common.end (); util_iter++)
      {
        util_iter->second = cur_mult * util_iter->second;
        prev_util_iter = pos_util_.common.find (util_iter->first);

        // If a utility from this goal is already in the utility map,
        // delete smaller utility value, otherwise add the utility from this
        // goal to the utility map.
        if (prev_util_iter != pos_util_.common.end ())
        {
          if (prev_util_iter->second < util_iter->second) {
            pos_util_.utility -= prev_util_iter->second;
          } else {
            pos_util_.utility -= util_iter->second;
          }
          // Set loop flag.
          is_loop = true;
        } else {
          pos_util_.common.insert (std::make_pair (util_iter->first,
            util_iter->second));
        }
      }
    } else {
      // Add utility to expected utility value.
      neg_util_.utility += cur_util.utility;

      // Update all component utilities based on link weight, and handle
      // utilities from common goals.
      for (UtilityMap::iterator util_iter = cur_util.common.begin ();
        util_iter != cur_util.common.end (); util_iter++)
      {
        util_iter->second = cur_mult * util_iter->second;
        prev_util_iter = neg_util_.common.find (util_iter->first);

        // If a utility from this goal is already in the utility map,
        // delete larger (less negative) utility value, otherwise add the
        // utility from this goal to the utility map.
        if (prev_util_iter != neg_util_.common.end ())
        {
          if (prev_util_iter->second > util_iter->second) {
            neg_util_.utility -= prev_util_iter->second;
          } else {
            neg_util_.utility -= util_iter->second;
          }
          // Set loop flag.
          is_loop = true;
        } else {
          neg_util_.common.insert (std::make_pair (util_iter->first,
            util_iter->second));
        }
      }
    }
  }

//*****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG**** 
  // Include cost in current expected utility.
//  pos_util_.utility -= cost_;
//*****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG****DEBUG**** 

  // If a loop was detected, add current probability to common probabilities.
  if (is_loop) {
    // Update common probabilities for true probability.
    ProbabilityMap::iterator prob_iter = true_prob_.common.find (ID_);
    if (prob_iter != true_prob_.common.end ()) {
      if (prob_iter->second != true_prob_.probability) {
        prob_iter->second = true_prob_.probability;
        prob_changed_ = true;
      }
    } else {
      true_prob_.common.insert (std::make_pair (ID_,
        true_prob_.probability));
      // Update prob_changed_ flag.
      prob_changed_ = true;
    }
  }

  // Set false probability (i.e. probability of task failure) to 1 minus
  // probability of success.
  false_prob_.probability = 1 - true_prob_.probability;

  // Update step (at end of step for task node).
  step_++;

  // Return boolean changed value based on change flags.
  return (prob_changed_ || util_changed_);

};

Utility TaskNode::get_utility (int step)
{
  // Check to ensure step is the current step, or else throw exception
  if (step != step_) {
    throw Invalid_Step ();
  }

  // Variable for summing expected utility.
  Utility utility = 0;

  // Add positive expected utility, negative expected utility values, and
  // subtract cost.
  utility += pos_util_.utility + neg_util_.utility - cost_;

  return utility;
};

void TaskNode::print_xml (std::basic_ostream<char, std::char_traits<char> >&
                          strm)
{
  strm << "  <taskNode>" << std::endl;

  strm << "    <nodeID>" << SANet::to_string (this->ID_);
  strm << "</nodeID>" << std::endl;

  strm << "    <name>" << this->name_;
  strm << "</name>" << std::endl;

  strm << "    <priorProb>" << SANet::to_string (this->prior_prob_);
  strm << "</priorProb>" << std::endl;

  strm << "    <attenFactor>" << SANet::to_string (this->atten_factor_);
  strm << "</attenFactor>" << std::endl;

  strm << "    <cost>" << SANet::to_string (this->cost_);
  strm << "</cost>" << std::endl;

  strm << "  </taskNode>" << std::endl;
};

// Print XML representation of node's precondition links to stream.
void TaskNode::print_precond_links_xml (std::basic_ostream<char, std::char_traits<char> >&
                                       strm)
{
  // Print all precondition links.
  for (NodeMap::iterator node_iter = pre_nodes_.begin ();
    node_iter != pre_nodes_.end (); node_iter++)
  {
    strm << "  <precondLink>" << std::endl;

    // Print precondition ID.
    strm << "    <condID>";
    strm << SANet::to_string (node_iter->first);
    strm << "</condID>" << std::endl;

    // Print task ID.
    strm << "    <taskID>";
    strm << SANet::to_string (this->ID_);
    strm << "</taskID>" << std::endl;

//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
    // Print port ID.
    strm << "    <portID>";
    strm << "portID";
    strm << "</portID>" << std::endl;
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

    // Print conditional probability for true condition value.
    strm << "    <trueProb>";
    strm << SANet::to_string (pre_true_probs_.find (node_iter->first)->second);
    strm << "</trueProb>" << std::endl;

    // Print conditional probability for false condition value.
    strm << "    <falseProb>";
    strm << SANet::to_string (pre_false_probs_.find (node_iter->first)->second);
    strm << "</falseProb>" << std::endl;

    strm << "  </precondLink>" << std::endl;
  }
};

// Print XML representation of node's effect links to stream.
void TaskNode::print_effect_links_xml (std::basic_ostream<char, std::char_traits<char> >&
                                      strm)
{
  // Print all effect links.
  for (NodeMap::iterator node_iter = post_nodes_.begin ();
    node_iter != post_nodes_.end (); node_iter++)
  {
    strm << "  <effectLink>" << std::endl;

    // Print task ID.
    strm << "    <taskID>";
    strm << SANet::to_string (this->ID_);
    strm << "</taskID>" << std::endl;

    // Print effect condition ID.
    strm << "    <condID>";
    strm << SANet::to_string (node_iter->first);
    strm << "</condID>" << std::endl;

//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
    // Print port ID.
    strm << "    <portID>";
    strm << "portID";
    strm << "</portID>" << std::endl;
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP

    // Print link weight.
    LinkMap::iterator cur_link_iter = post_links_.find (node_iter->first);
    strm << "    <weight>";
    if (cur_link_iter == post_links_.end ()) {
      strm << "ERROR: weight not found";
    } else {
      strm << SANet::to_string (cur_link_iter->second);
    }
    strm << "</weight>" << std::endl;

    strm << "  </effectLink>" << std::endl;
  }
};

void TaskNode::print (std::basic_ostream<char, std::char_traits<char> >&
                         strm, bool verbose)
{
  // Print node name and ID.
  strm << "Task Node \"" << name_ << "\":" << std::endl;
  strm << "  ID: " << ID_ << std::endl;

  // Print cost and expected utility.
  strm << "  Cost: " << cost_ << std::endl;
  strm << "  Expected Utility: " << get_utility (step_) << std::endl;

  // If verbose, print positive and negative component utilities from goals.
  if (verbose) {
    // Print positive utility and its component utilities.
    strm << "  Positive utility received: " << pos_util_.utility << std::endl;
    for (UtilityMap::iterator iter = pos_util_.common.begin ();
      iter != pos_util_.common.end (); iter++)
    {
      strm << "    Component expected utility from goal " << iter->first;
      strm << " = " << iter->second;
      strm << std::endl;
    }

    // Print negative utility and its component utilities.
    strm << "  Negative utility received: " << neg_util_.utility << std::endl;
    for (UtilityMap::iterator iter = neg_util_.common.begin ();
      iter != neg_util_.common.end (); iter++)
    {
      strm << "    Component expected utility from goal " << iter->first;
      strm << " = " << iter->second;
      strm << std::endl;
    }
  }

  // Print probability of task success.
  strm << "  Probability (success): " << true_prob_.probability << std::endl;

  // If verbose, print common utility
  if (verbose) {
    for (ProbabilityMap::iterator iter = true_prob_.common.begin ();
      iter != true_prob_.common.end (); iter++)
    {
      strm << "    Component probability (possible loops only) from ";
      strm << iter->first << " = " << iter->second;
      strm << std::endl;
    }
  }

  // If verbose, print probability of task failure.
  if (verbose) {
    strm << "  Probability (failure): " << false_prob_.probability;
    strm << std::endl;
  }

  // Current link map iterator input and output link printing loops.
  LinkMap::iterator cur_link_iter;

  // Print node input links.
  strm << "  In Links (preconditions):" << std::endl;
  for (NodeMap::iterator node_iter = pre_nodes_.begin ();
    node_iter != pre_nodes_.end (); node_iter++)
  {
    // Print input node name and ID.
    strm << "    " << node_iter->first << " ";
    strm << "(" << node_iter->second->get_name () << "); ";

    // Find link for this node.
    cur_link_iter = pre_links_.find (node_iter->first);

    // If this node couldn't be found in pre_links_ print error,
    // otherwise print link weight.
    if (cur_link_iter == pre_links_.end ()) {
      strm << "ERROR: weight not found";
    } else {
      strm << "weight = " << cur_link_iter->second;
    }
    strm << std::endl;

    // If verbose, print conditional probabilities of success for precondition.
    if (verbose) {
      strm << "      Conditional probability if true: ";
      strm << pre_true_probs_.find (node_iter->first)->second << std::endl;
      strm << "      Conditional probability if false: ";
      strm << pre_false_probs_.find (node_iter->first)->second << std::endl;
    }
  }

  // Print node output links.
  strm << "  Out Links (effects):" << std::endl;
  for (NodeMap::iterator node_iter = post_nodes_.begin ();
    node_iter != post_nodes_.end (); node_iter++)
  {
    // Print output node name and ID.
    strm << "    " << node_iter->first << " ";
    strm << "(" << node_iter->second->get_name () << "); ";

    // Find link for this node.
    cur_link_iter = post_links_.find (node_iter->first);

    // If this node couldn't be found in post_links_ print error,
    // otherwise print link weight.
    if (cur_link_iter == post_links_.end ()) {
      strm << "ERROR: weight not found";
    } else {
      strm << "weight = " << cur_link_iter->second;
    }
    strm << std::endl;
  }
};

void TaskNode::add_precond (CondID ID, CondNode *node, Probability true_prob,
                               Probability false_prob)
{
  // Add node to pre-nodes.
  pre_nodes_.insert (std::make_pair (ID, node));

  // Add probabilities.
  pre_true_probs_.insert (std::make_pair (ID, true_prob));
  pre_false_probs_.insert (std::make_pair (ID, false_prob));

  // Add link weight.
  LinkWeight weight = (true_prob - false_prob)/(true_prob + false_prob);
  pre_links_.insert (std::make_pair (ID, weight));

  // Add link for precondition node.
  node->add_post_link (ID_, this, weight);
};

void TaskNode::add_effect (CondID ID, CondNode *node, LinkWeight weight)
{
  // Add node to post-nodes.
  post_nodes_.insert (std::make_pair (ID, node));

  // Add link weight.
  post_links_.insert (std::make_pair (ID, weight));

  // Add link for precondition node.
  node->add_pre_link (ID_, this, weight);
};

CondNode::CondNode (CondID ID, std::string name, MultFactor atten_factor,
                      Probability true_prob, Probability false_prob, Utility goal_util, CondKind condkind)
: Node (ID, name, atten_factor),
  goal_util_ (goal_util),
  init_true_prob_ (true_prob),
  true_prob_from_ (ID),
  false_prob_from_ (ID),
  cond_kind_ (condkind)
{
  // Set initial probabilities.
  true_prob_.probability = true_prob;
  false_prob_.probability = false_prob;

  // Set prob_changed_ flag.
  prob_changed_ = true;

  // If this node is a goal, set util_changed_ flag and add goal utility.
  if (goal_util > 0) {
    this->util_changed_ = true;
    this->pos_util_.utility = goal_util;
    this->pos_util_.common.insert (std::make_pair (ID, goal_util));
  } else if (goal_util < 0) {
    this->util_changed_ = true;
    this->neg_util_.utility = goal_util;
    this->neg_util_.common.insert (std::make_pair (ID, goal_util));
  }
};

CondNode::~CondNode (void)
{
  // Nothing to do.
};

// Update goal utility.
void CondNode::set_goal_util (Utility util)
{
  // Remove old goal utility.
  if (this->goal_util_ > 0) {
    this->pos_util_.utility -= this->goal_util_;
    this->pos_util_.common.erase (this->ID_);
  } else if (this->goal_util_ < 0) {
    this->neg_util_.utility -= this->goal_util_;
    this->neg_util_.common.erase (this->ID_);
  }

  // Update total utility and utility maps for new utility.
  if (util > 0) {
    this->pos_util_.utility += util;
    this->pos_util_.common.insert (std::make_pair (this->ID_, util));
  } else if (util < 0) {
    this->neg_util_.utility += util;
    this->neg_util_.common.insert (std::make_pair (this->ID_, util));
  }

  // Update utility changed flag and goal utility.
  this->util_changed_ = true;
  this->goal_util_ = util;
};


// Get initial/current probability.
Probability CondNode::get_init_prob (bool value)
{
  if (value)
    return this->init_true_prob_;

  return (1.0 - this->init_true_prob_);
};

// Update initial/current probability.
void CondNode::set_init_prob (Probability init_true_prob)
{
  this->init_true_prob_ = init_true_prob;
  this->prob_changed_ = true;
};

void CondNode::print_xml (std::basic_ostream<char, std::char_traits<char> >&
                         strm)
{
  strm << "  <condNode>" << std::endl;

  strm << "    <nodeID>" << SANet::to_string (this->ID_);
  strm << "</nodeID>" << std::endl;

  strm << "    <name>" << this->name_;
  strm << "</name>" << std::endl;

  strm << "    <probTrue>" << SANet::to_string (this->init_true_prob_);
  strm << "</probTrue>" << std::endl;

  strm << "    <utility>" << SANet::to_string (this->goal_util_);
  strm << "</utility>" << std::endl;

  strm << "    <kind>" << SANet::to_string (this->cond_kind_);
  strm << "</kind>" << std::endl;

  strm << "    <attenFactor>" << SANet::to_string (this->atten_factor_);
  strm << "</attenFactor>" << std::endl;

  strm << "  </condNode>" << std::endl;
};

void CondNode::print (std::basic_ostream<char, std::char_traits<char> >&
                         strm, bool verbose)
{
  // Print node name and ID.
  strm << "Condition Node \"" << name_ << "\":" << std::endl;
  strm << "  ID: " << ID_ << std::endl;

  // Print goal utility.
  strm << "  Goal utility: " << goal_util_ << std::endl;

  // Print positive utility.
  strm << "  Expected utility for true value: " << pos_util_.utility;
  strm << std::endl;

  // If verbose, print component utilities from all goals.
  if (verbose) {
    for (UtilityMap::iterator iter = pos_util_.common.begin ();
      iter != pos_util_.common.end (); iter++)
    {
      strm << "    Component expected utility from goal " << iter->first;
      strm << " = " << iter->second;
      strm << std::endl;
    }
  }

  // Print negative utility.
  strm << "  Expected utility for false value: " << neg_util_.utility;
  strm << std::endl;

  // If verbose, print component utilities from all goals.
  if (verbose) {
    for (UtilityMap::iterator iter = neg_util_.common.begin ();
      iter != neg_util_.common.end (); iter++)
    {
      strm << "    Component expected utility from goal " << iter->first;
      strm << " = " << iter->second;
      strm << std::endl;
    }
  }

  // Print probability (maximum) that condition is true.
  strm << "  Probability (true): " << true_prob_.probability << std::endl;

  // If verbose, print component probabilities for loops.
  if (verbose) {
    for (ProbabilityMap::iterator iter = true_prob_.common.begin ();
      iter != true_prob_.common.end (); iter++)
    {
      strm << "    Component probability (possible loops only) from ";
      strm << iter->first << " = " << iter->second;
      strm << std::endl;
    }
  }

  // Print probability (maximum) that condition is false.
  strm << "  Probability (false): " << false_prob_.probability << std::endl;

  // If verbose, print component probabilities for loops.
  if (verbose) {
    for (ProbabilityMap::iterator iter = false_prob_.common.begin ();
      iter != false_prob_.common.end (); iter++)
    {
      strm << "    Component probability (possible loops only) from ";
      strm << iter->first << " = " << iter->second;
      strm << std::endl;
    }
  }

  // Current link map iterator input and output link printing loops.
  LinkMap::iterator cur_link_iter;

  // Print node input links.
  strm << "  In Links (tasks that change this condition):" << std::endl;
  for (NodeMap::iterator node_iter = pre_nodes_.begin ();
    node_iter != pre_nodes_.end (); node_iter++)
  {
    // Print input node name and ID.
    strm << "    " << node_iter->first << " ";
    strm << "(" << node_iter->second->get_name () << "); ";

    // Find link for this node.
    cur_link_iter = pre_links_.find (node_iter->first);

    // If this node couldn't be found in pre_links_ print error,
    // otherwise print link weight.
    if (cur_link_iter == pre_links_.end ()) {
      strm << "ERROR: weight not found";
    } else {
      strm << "weight = " << cur_link_iter->second;
    }
    strm << std::endl;
  }

  // Print node output links.
  strm << "  Out Links (tasks that have this precondition):" << std::endl;
  for (NodeMap::iterator node_iter = post_nodes_.begin ();
    node_iter != post_nodes_.end (); node_iter++)
  {
    // Print output node name and ID.
    strm << "    " << node_iter->first << " ";
    strm << "(" << node_iter->second->get_name () << "); ";

    // Find link for this node.
    cur_link_iter = post_links_.find (node_iter->first);

    // If this node couldn't be found in post_links_ print error,
    // otherwise print link weight.
    if (cur_link_iter == post_links_.end ()) {
      strm << "ERROR: weight not found";
    } else {
      strm << "weight = " << cur_link_iter->second;
    }
    strm << std::endl;
  }
};

bool CondNode::update (void)
{
  // Reset change flags.
  prob_changed_ = false;
  util_changed_ = false;

  // Update step (at beginning of step for condition node).
  step_++;

  // Flag for detection of loops.
  bool is_loop = false;

  // Variables for current node and link in updates.
  TaskNode *cur_node;
  TaskID cur_ID;
  LinkMap::iterator link_iter;
  LinkWeight cur_weight;
  MultFactor cur_mult;

  // Reset utility info.
  pos_util_.utility = 0;
  pos_util_.common.clear ();
  neg_util_.utility = 0;
  neg_util_.common.clear ();
  // If this node is a goal, set add goal utility to positive utilities.
  if (goal_util_ > 0) {
    pos_util_.utility = goal_util_;
    pos_util_.common.insert (std::make_pair (ID_, goal_util_));
  };

  // Variables for current utility updates.
  Utility_Info cur_util;
  UtilityMap::iterator prev_util_iter;

  // Update utility.
  for (NodeMap::iterator node_iter = post_nodes_.begin ();
    node_iter != post_nodes_.end (); node_iter++)
  {
    // Get current node info.
    cur_ID = node_iter->first;
    cur_node = (TaskNode *) node_iter->second;

    // Update util_changed_ flag if necessary.
    if (cur_node->util_changed ()) {
      util_changed_ = true;
    }

    // Get current link info.
    link_iter = post_links_.find(cur_ID);
    if (link_iter == post_links_.end ()) {
      throw Update_Error ();
    }
    cur_weight = link_iter->second;

    // Set multiplier as attenuation factor times link weight.
    cur_mult = atten_factor_ * cur_weight;

    // Get current utility info.
    try {
      cur_util = cur_node->get_reward (step_);
    } catch (Invalid_Step e) {
      std::cerr << "Error in condition node update:  Invalid step value.";
      std::cerr << std::endl;
      return false;
    } catch (...) {
      std::cerr << "Unexpected exception thrown in condition node update.";
      std::cerr << std::endl;
      return false;
    }

    // Updated utility based on current multiplier.
    cur_util.utility = cur_mult * cur_util.utility;

    // Update positive or negative utilities depending on sign of link weight.
    if (cur_weight >= 0) {
      // Add utility to expected utility value.
      pos_util_.utility += cur_util.utility;

      // Update all component utilities based on link weight, and handle
      // utilities from common goals.
      for (UtilityMap::iterator util_iter = cur_util.common.begin ();
        util_iter != cur_util.common.end (); util_iter++)
      {
        util_iter->second = cur_mult * util_iter->second;
        prev_util_iter = pos_util_.common.find (util_iter->first);

        // If a utility from this goal is already in the utility map,
        // delete smaller utility value, otherwise add the utility from this
        // goal to the utility map.
        if (prev_util_iter != pos_util_.common.end ())
        {
          if (prev_util_iter->second < util_iter->second) {
            pos_util_.utility -= prev_util_iter->second;
          } else {
            pos_util_.utility -= util_iter->second;
          }
          // Set loop flag.
          is_loop = true;
        } else {
          pos_util_.common.insert (std::make_pair (util_iter->first,
            util_iter->second));
        }
      }
    } else {
      // Add utility to expected utility value.
      neg_util_.utility += cur_util.utility;

      // Update all component utilities based on link weight, and handle
      // utilities from common goals.
      for (UtilityMap::iterator util_iter = cur_util.common.begin ();
        util_iter != cur_util.common.end (); util_iter++)
      {
        util_iter->second = cur_mult * util_iter->second;
        prev_util_iter = neg_util_.common.find (util_iter->first);

        // If a utility from this goal is already in the utility map,
        // delete larger (less negative) utility value, otherwise add the
        // utility from this goal to the utility map.
        if (prev_util_iter != neg_util_.common.end ())
        {
          if (prev_util_iter->second > util_iter->second) {
            neg_util_.utility -= prev_util_iter->second;
          } else {
            neg_util_.utility -= util_iter->second;
          }
          // Set loop flag.
          is_loop = true;
        } else {
          neg_util_.common.insert (std::make_pair (util_iter->first,
            util_iter->second));
        }
      }
    }
  }

        


  // Variables for current probability value.
  Probability_Info cur_prob;
  ProbabilityMap::iterator prob_iter;
  NodeMap::iterator temp_node_iter;
  


  // Check for change in probability of node where current
  // highest true probability comes from.  Update true
  // probability if it has changed, so that newly inserted
  // condition probabilities will be noticed even if they
  // are lower than previous condition probability values.

  if (this->true_prob_from_ != this->ID_) {
    temp_node_iter = this->pre_nodes_.find(this->true_prob_from_);
    if (temp_node_iter == this->pre_nodes_.end ()) {
      throw Update_Error ();
    }
    cur_node = (TaskNode *) temp_node_iter->second;

    // Get current link info.
    link_iter = pre_links_.find(this->true_prob_from_);
    if (link_iter == pre_links_.end ()) {
      throw Update_Error ();
    }
    cur_weight = link_iter->second;

    // Set multiplier as absolute value of link weight.
    if (cur_weight >= 0) {
      cur_mult = cur_weight;
    } else {
      cur_mult = -1 * cur_weight;
    }

    // Get current probability info.
    try {
      cur_prob = cur_node->get_prob (step_);
    } catch (Invalid_Step e) {
      std::cerr << "Error in condition node update:  Invalid step value.";
      std::cerr << std::endl;
      throw Update_Error ();
    } catch (...) {
      std::cerr << "Unexpected exception thrown in condition node update.";
      std::cerr << std::endl;
      throw Update_Error ();
    }

    // Update probability based on current multiplier.
    cur_prob.probability = cur_mult * cur_prob.probability;

    // Check true probability from node against current value (link weight should not be negative for true probability).
    if (cur_weight >= 0) {
      // Change true probability if it has changed.
      if (true_prob_.probability != cur_prob.probability) {
        this->true_prob_ = cur_prob;

        // Update prob_changed_ flag.
        prob_changed_ = true;
      }
    } else {
      std::cerr << "Error in condition node update:  Link weight changed.";
      std::cerr << std::endl;
      throw Update_Error ();
    }
  }


  // Check for change in probability of node where current
  // highest false probability comes from.  Update true
  // probability if it has changed, so that newly inserted
  // condition probabilities will be noticed even if they
  // are lower than previous condition probability values.

  if (this->false_prob_from_ != this->ID_) {
    temp_node_iter = this->pre_nodes_.find(this->false_prob_from_);
    if (temp_node_iter == this->pre_nodes_.end ()) {
      throw Update_Error ();
    }
    cur_node = (TaskNode *) temp_node_iter->second;

    // Get current link info.
    link_iter = pre_links_.find(this->false_prob_from_);
    if (link_iter == pre_links_.end ()) {
      throw Update_Error ();
    }
    cur_weight = link_iter->second;

    // Set multiplier as absolute value of link weight.
    if (cur_weight >= 0) {
      cur_mult = cur_weight;
    } else {
      cur_mult = -1 * cur_weight;
    }

    // Get current probability info.
    try {
      cur_prob = cur_node->get_prob (step_);
    } catch (Invalid_Step e) {
      std::cerr << "Error in condition node update:  Invalid step value.";
      std::cerr << std::endl;
      throw Update_Error ();
    } catch (...) {
      std::cerr << "Unexpected exception thrown in condition node update.";
      std::cerr << std::endl;
      throw Update_Error ();
    }

    // Update probability based on current multiplier.
    cur_prob.probability = cur_mult * cur_prob.probability;

    // Check false probability from node against current value (link weight should not be positive for false probability).
    if (cur_weight <= 0) {
      // Change false probability if it has changed.
      if (false_prob_.probability != cur_prob.probability) {
        this->false_prob_ = cur_prob;

        // Update prob_changed_ flag.
        prob_changed_ = true;
      }
    } else {
      std::cerr << "Error in condition node update:  Link weight changed.";
      std::cerr << std::endl;
      throw Update_Error ();
    }
  }





  // Update probability.
  for (NodeMap::iterator node_iter = pre_nodes_.begin ();
    node_iter != pre_nodes_.end (); node_iter++)
  {
    // Get current node info.
    cur_ID = node_iter->first;
    cur_node = (TaskNode *) node_iter->second;

    // Get current link info.
    link_iter = pre_links_.find(cur_ID);
    if (link_iter == pre_links_.end ()) {
      throw Update_Error ();
    }
    cur_weight = link_iter->second;

    // Set multiplier as absolute value of link weight.
    if (cur_weight >= 0) {
      cur_mult = cur_weight;
    } else {
      cur_mult = -1 * cur_weight;
    }

    // Get current probability info.
    try {
      cur_prob = cur_node->get_prob (step_);
    } catch (Invalid_Step e) {
      std::cerr << "Error in condition node update:  Invalid step value.";
      std::cerr << std::endl;
      return false;
    } catch (...) {
      std::cerr << "Unexpected exception thrown in condition node update.";
      std::cerr << std::endl;
      return false;
    }

    // Update probability based on current multiplier.
    cur_prob.probability = cur_mult * cur_prob.probability;

    // Update true or false probability depending on sign of link weight.
    if (cur_weight >= 0) {
      // Change true probability if this probability is higher than previous.
      if (cur_prob.probability > true_prob_.probability) {
        true_prob_ = cur_prob;
        true_prob_from_ = cur_ID;

        // Update prob_changed_ flag.
        prob_changed_ = true;
      }
      // Otherwise check for newly detected common probabilities if current
      // probability is from this node.
      else if (true_prob_from_ == cur_ID) {
        for (prob_iter = cur_prob.common.begin ();
          prob_iter != cur_prob.common.end (); prob_iter++)
        {
          // If this common probability is not in current common probability
          // map, add it.
          if (true_prob_.common.find (prob_iter->first) ==
            true_prob_.common.end ())
          {
            // Add common probability.
            true_prob_.common.insert (std::make_pair (prob_iter->first,
              prob_iter->second));

            // Update prob_changed_ flag.
            prob_changed_ = true;
          }
        }
      }
    } else {
      // Change false probability if this probability is higher than previous.
      if (cur_prob.probability > false_prob_.probability) {
        false_prob_ = cur_prob;
        false_prob_from_ = cur_ID;

        // Update prob_changed_ flag.
        prob_changed_ = true;
      }
      // Otherwise check for newly detected common probabilities if current
      // probability is from this node.
      else if (false_prob_from_ == cur_ID) {
        for (prob_iter = cur_prob.common.begin ();
          prob_iter != cur_prob.common.end (); prob_iter++)
        {
          // If this common probability is not in current common probability
          // map, add it.
          if (false_prob_.common.find (prob_iter->first) ==
            false_prob_.common.end ())
          {
            // Add common probability.
            false_prob_.common.insert (std::make_pair (prob_iter->first,
              prob_iter->second));

            // Update prob_changed_ flag.
            prob_changed_ = true;
          }
        }
      }
    }
  }

//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****
  // SHOULDN'T WE DO THIS SEPARATELY FOR TRUE AND/OR FALSE PROBABILITIES????????????????????????????
  // If a loop was detected, add current probability to common probabilities.
  if (is_loop) {
    // Update common probabilities for true probability.
    prob_iter = true_prob_.common.find (ID_);
    if (prob_iter != true_prob_.common.end ()) {
      prob_iter->second = true_prob_.probability;
    } else {
      true_prob_.common.insert (std::make_pair (ID_,
        true_prob_.probability));
      // Update prob_changed_ flag.
      prob_changed_ = true;
    }
  }
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****

  // Make sure conditions/links in the network have not changed to bring
  // max true/false probabilities below actual current values for condition.
  if (this->true_prob_.probability < this->init_true_prob_) {
    this->true_prob_.probability = this->init_true_prob_;
    this->true_prob_.common.clear ();
    this->true_prob_from_ = this->ID_;
    // Update prob_changed_ flag.
    prob_changed_ = true;
  }
  if (this->false_prob_.probability < 1 - this->init_true_prob_) {
    this->false_prob_.probability = 1 - this->init_true_prob_;
    this->false_prob_.common.clear ();
    this->false_prob_from_ = this->ID_;
    // Update prob_changed_ flag.
    prob_changed_ = true;
  }

  // Return boolean changed value based on change flags.
  return (prob_changed_ || util_changed_);
};

void CondNode::add_pre_link (TaskID ID, TaskNode *node, LinkWeight weight)
{
  // Add node to pre-nodes.
  pre_nodes_.insert (std::make_pair (ID, node));

  // Add link weight.
  pre_links_.insert (std::make_pair (ID, weight));
};

void CondNode::add_post_link (TaskID ID, TaskNode *node, LinkWeight weight)
{
  // Add node to post-nodes.
  post_nodes_.insert (std::make_pair (ID, node));

  // Add link weight.
  post_links_.insert (std::make_pair (ID, weight));
};

/// Get the kind
CondKind CondNode::get_cond_kind()
{
  return this->cond_kind_;
};

