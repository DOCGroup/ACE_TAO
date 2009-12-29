// -*- C++ -*-

//=============================================================================
/**
 * @file  SANet.cpp
 *
 * This file contains the Network class implementation for spreading
 * activation networks.
 *
 * @author  John S. Kinnebrew <john.s.kinnebrew@vanderbilt.edu>
 */
//=============================================================================

#include <map>
#include <iostream>
#include "SANet_Types.h"
#include "SANet.h"
#include "SANode.h"
#include "SANet_Exceptions.h"

using namespace SANet;


SANet::Network::Network (void)
: step_ (0)
{
  // All map/set member variables initially empty.
  this->task_nodes_.clear ();
  this->cond_nodes_.clear ();
  this->precond_links_.clear ();
  this->effect_links_.clear ();
  this->goals_.clear ();
  this->active_tasks_.clear ();
  this->disabled_tasks_.clear ();
  this->active_conds_.clear ();
  this->disabled_conds_.clear ();
};


// Copy constructor. Performs initialization by making a deep copy
// of the provided network (including allocation of new nodes with
// state copied from nodes in provided network and creation of
// corresponding links between them).
SANet::Network::Network (const Network &s)
: step_ (s.step_)
{
  // Member variables that include node pointers are initially empty.
  this->task_nodes_.clear ();
  this->cond_nodes_.clear ();

  // Member variables that include links are initially empty (will be populated during link creation).
  this->precond_links_.clear ();
  this->effect_links_.clear ();

  // Copy active/disabled node (ID) sets (no node pointers and no links).
  this->active_tasks_ = s.active_tasks_;
  this->disabled_tasks_ = s.disabled_tasks_;
  this->active_conds_ = s.active_conds_;
  this->disabled_conds_ = s.disabled_conds_;

  // Copy goals (no node pointers and no links).
  this->goals_ = s.goals_;

  // Create new nodes corresponding to those in s, adding each to appropriate node (ID to pointer) map.
  for (TaskNodeMap::const_iterator node_iter = s.task_nodes_.begin ();
    node_iter != s.task_nodes_.end (); node_iter++)
  {
    SANet::TaskNode *new_node = new SANet::TaskNode (*(node_iter->second));
    this->task_nodes_.insert (std::make_pair (node_iter->first, new_node));
  }
  for (CondNodeMap::const_iterator node_iter = s.cond_nodes_.begin ();
    node_iter != s.cond_nodes_.end (); node_iter++)
  {
    SANet::CondNode *new_node = new SANet::CondNode (*(node_iter->second));
    this->cond_nodes_.insert (std::make_pair (node_iter->first, new_node));
  }

  // Create precondition links corresponding to all those in s
  // (using pointers to new nodes instead of the original nodes in s).
  for (PrecondLinkPortMap::const_iterator link_iter = s.precond_links_.begin ();
    link_iter != s.precond_links_.end (); link_iter++)
  {
    PrecondLink link = link_iter->first;
    PortID port = link_iter->second;

    // Get pointer to task node in original network.
    TaskNodeMap::const_iterator orig_task_iter = s.task_nodes_.find (link.second);
    if (orig_task_iter == s.task_nodes_.end ())
      throw UnknownNode ();
    TaskNode *orig_task = orig_task_iter->second;

    // Get precondition link conditional probabilities.
    ConditionalProb prob = orig_task->get_precond_prob (link.first);

    // Add precondition link.
    this->add_precond_link (link.first, link.second, prob.true_prob, prob.false_prob, port);
  }

  // Create effect links corresponding to all those in s
  // (using pointers to new nodes instead of the original nodes in s).
  for (EffectLinkPortMap::const_iterator link_iter = s.effect_links_.begin ();
    link_iter != s.effect_links_.end (); link_iter++)
  {
    EffectLink link = link_iter->first;
    PortID port = link_iter->second;

    // Get pointer to task node.
    TaskNodeMap::const_iterator orig_task_iter = s.task_nodes_.find (link.first);
    if (orig_task_iter == s.task_nodes_.end ())
      throw UnknownNode ();
    TaskNode *orig_task = orig_task_iter->second;

    // Get effect link probability.
    Probability prob = orig_task->get_effect_prob (link.second);

    // Add effect link.
    this->add_effect_link (link.first, link.second, prob, port);
  }

// Network data members
    // STRUCTURE VARIABLES.
//    TaskNodeMap task_nodes_;
//    CondNodeMap cond_nodes_;
//    PrecondLinkPortMap precond_links_;
//    EffectLinkPortMap effect_links_;
    // STATE VARIABLES.
//    TaskIDSet active_tasks_;
//    TaskIDSet disabled_tasks_;
//    CondIDSet active_conds_;
//    CondIDSet disabled_conds_;
//    GoalMap goals_;
//    int step_;
};


SANet::Network::~Network ()
{
  // Deallocate task nodes.
  for (TaskNodeMap::iterator node_iter = task_nodes_.begin ();
    node_iter != task_nodes_.end (); node_iter++)
  {
    delete node_iter->second;
  }

  // Deallocate condition nodes.
  for (CondNodeMap::iterator node_iter = cond_nodes_.begin ();
    node_iter != cond_nodes_.end (); node_iter++)
  {
    delete node_iter->second;
  }
};

void SANet::Network::add_task (TaskID ID, std::string name, MultFactor atten_factor,
                          TaskCost cost, Probability prior_prob)
{
  // If this ID has already been used, throw exception.
  if (task_nodes_.find (ID) != task_nodes_.end ()) {
    throw Duplicate_ID ();
  }
  if (cond_nodes_.find (ID) != cond_nodes_.end ()) {
    throw Duplicate_ID ();
  }

  // Task node pointer.
  TaskNode *node;

  // Set initially to active
  this->active_tasks_.insert(ID);

  // Add task node, throwing exception if insertion fails.
  node = new TaskNode (ID, name, atten_factor, cost, prior_prob);
  if (!(task_nodes_.insert (std::make_pair (ID, node))).second)
  {
    throw Insertion_Error ();
  }
};

void SANet::Network::reset_step(){
	step_ = 0;

	for(CondNodeMap::iterator it = this->cond_nodes_.begin(); it != cond_nodes_.end(); it++){
		it->second->reset_step();
	}

	for(TaskNodeMap::iterator it = this->task_nodes_.begin(); it != task_nodes_.end(); it++){
		it->second->reset_step();
	}
}


Probability SANet::Network::get_prior(TaskID ID)
{
  TaskNodeMap::iterator task_iter = task_nodes_.find (ID);
  if (task_iter == task_nodes_.end ()) {
    throw UnknownNode ();
  }
  TaskNode *task_node = task_iter->second;

  return task_node->get_prior();

}

LinkWeight SANet::Network::get_link(TaskID task_ID, CondID cond_ID)
{
  TaskNodeMap::iterator task_iter = task_nodes_.find (task_ID);
  if (task_iter == task_nodes_.end ()) {
    throw UnknownNode ();
  }
  TaskNode *task_node = task_iter->second;
  SANet::LinkMap lmap = task_node->get_post();

  LinkMap::iterator lIter =lmap.find(cond_ID);
  if (task_iter == task_nodes_.end ()) {
    throw UnknownNode ();
  }
  LinkWeight curWeight = lIter->second;

  return curWeight;

}

void SANet::Network::update_prior(TaskID tID, Probability prior)
{
  TaskNodeMap::iterator task_iter = task_nodes_.find (tID);
  if (task_iter == task_nodes_.end ()) {
    throw UnknownNode ();
  }
  TaskNode *task_node = task_iter->second;

  task_node->update_prior(prior);
  
}

void SANet::Network::add_cond (CondID ID, std::string name, MultFactor atten_factor,
                        Probability true_prob, Probability false_prob,
                        Utility goal_util, CondKind cond_kind)
{
  // If this ID has already been used, throw exception.
  if (task_nodes_.find (ID) != task_nodes_.end ()) {
    throw Duplicate_ID ();
  }
  if (cond_nodes_.find (ID) != cond_nodes_.end ()) {
    throw Duplicate_ID ();
  }

  // Condition node pointer.
  CondNode *node;

  // Set initially to active
  this->active_conds_.insert(ID);

  // Add condition node, throwing exception if insertion fails.
  node = new CondNode (ID, name, atten_factor,
    true_prob, false_prob, goal_util, cond_kind);
  if (!(cond_nodes_.insert (std::make_pair (ID, node))).second)
  {
    throw Insertion_Error ();
  }

  if (goal_util != 0)
    this->goals_.insert (std::make_pair (ID, goal_util));
};

void SANet::Network::add_precond_link (CondID cond_ID, TaskID task_ID,
                                Probability true_prob, Probability false_prob,
                                PortID port_ID)
{
  // Find task node pointer, throwing exception if not found.
  TaskNodeMap::iterator task_iter = task_nodes_.find (task_ID);
  if (task_iter == task_nodes_.end ()) {
    throw UnknownNode ();
  }
  TaskNode *task_node = task_iter->second;

  // Find condition node pointer, throwing exception if not found.
  CondNodeMap::iterator cond_iter = cond_nodes_.find (cond_ID);
  if (cond_iter == cond_nodes_.end ()) {
    throw UnknownNode ();
  }
  CondNode *cond_node = cond_iter->second;

  // Add link to task node.
  task_node->add_precond (cond_ID, cond_node, true_prob, false_prob);

  // Add link to port map.
  this->precond_links_.insert (std::make_pair (std::make_pair (cond_ID,
    task_ID), port_ID));
};

void SANet::Network::add_effect_link (TaskID task_ID, CondID cond_ID,
                               LinkWeight weight, PortID port_ID)
{
  // Find task node pointer, throwing exception if not found.
  TaskNodeMap::iterator task_iter = task_nodes_.find (task_ID);
  if (task_iter == task_nodes_.end ()) {
    throw UnknownNode ();
  }
  TaskNode *task_node = task_iter->second;

  // Find condition node pointer, throwing exception if not found.
  CondNodeMap::iterator cond_iter = cond_nodes_.find (cond_ID);
  if (cond_iter == cond_nodes_.end ()) {
    throw UnknownNode ();
  }
  CondNode *cond_node = cond_iter->second;

  // Add link.
  task_node->add_effect (cond_ID, cond_node, weight);

  // Add link to port map.
  this->effect_links_.insert (std::make_pair (std::make_pair (task_ID,
    cond_ID), port_ID));
};

void SANet::Network::update_effect_link(TaskID task_ID, CondID cond_ID,
                               LinkWeight weight, PortID port_ID)
{
   // Find task node pointer, throwing exception if not found.
  TaskNodeMap::iterator task_iter = task_nodes_.find (task_ID);
  if (task_iter == task_nodes_.end ()) {
    throw UnknownNode ();
  }
  TaskNode *task_node = task_iter->second;

  // Find condition node pointer, throwing exception if not found.
  CondNodeMap::iterator cond_iter = cond_nodes_.find (cond_ID);
  if (cond_iter == cond_nodes_.end ()) {
    throw UnknownNode ();
  }
  CondNode *cond_node = cond_iter->second;

  // update link.**************
  task_node->update_effect (cond_ID, cond_node, weight);

  // Update link to port map.
  
  EffectLinkPortMap::iterator eliter = this->effect_links_.find((std::make_pair (task_ID,
    cond_ID)));
  if(eliter != effect_links_.end())
  {
    (*eliter).second = port_ID;
  }

};

void SANet::Network::print_graphviz(std::basic_ostream<char, std::char_traits<char> >& strm, std::map<std::string, std::string>& graphmap, std::string defaultColor)
{
  //Go through the task nodes, and if it's without a color, then give it the default. Then write it to the stream.
  for (TaskNodeMap::iterator node_iter = task_nodes_.begin ();
    node_iter != task_nodes_.end (); node_iter++)
  {
    std::map<std::string, std::string>::iterator titer = graphmap.find(node_iter->second->get_name());
    if(titer == graphmap.end())
    {
      graphmap[node_iter->second->get_name()] =  defaultColor;
    }
    strm << "\t" << "\"" << node_iter->second->get_name() << " " << node_iter->first  << "\" " << "[shape=box, style=filled, color = " << graphmap[node_iter->second->get_name()] <<"];\n";
  }

  //Go through the cond nodes, and if it's without a color, then give it the default. Then write it to the stream.
  for (CondNodeMap::iterator node_iter = cond_nodes_.begin ();
    node_iter != cond_nodes_.end (); node_iter++)
  {
    std::map<std::string, std::string>::iterator citer = graphmap.find(node_iter->second->get_name());
    if(citer == graphmap.end())
    {
      graphmap[node_iter->second->get_name()] =  defaultColor;
    }
    strm << "\t" << "\"" << node_iter->second->get_name() << " " << node_iter->first  << "\" " << "[style=filled, color = " << graphmap[node_iter->second->get_name()] << "];\n";
  }

  //Go through the effect and preconds for the task nodes and link them in the stream
  for (TaskNodeMap::iterator node_iter = task_nodes_.begin ();
    node_iter != task_nodes_.end (); node_iter++)
  {
    node_iter->second->print_effects_graphviz(strm);
    node_iter->second->print_preconds_graphviz(strm);
  }

}

void SANet::Network::print_xml (std::basic_ostream<char, std::char_traits<char> >& strm)
{
  strm << "<?xml version=\"1.0\" encoding=\"utf-8\" ?>" << std::endl;
  strm << "<SANet:network" << std::endl;
  strm << "  xmlns:SANet=\"http://www.vanderbilt.edu/SANet\"" << std::endl;
  strm << "  xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" << std::endl;
  strm << "  xsi:schemaLocation=\"http://www.vanderbilt.edu/SANet SANet_Network.xsd\"" << std::endl;
  strm << "  xmlns=\"http://www.vanderbilt.edu/SANet\">" << std::endl;
  strm << std::endl;
  strm << "  <defaultAttenFactor />" << std::endl;
  strm << "  <defaultTaskCost />" << std::endl;
  strm << "  <defaultCondUtil />" << std::endl;
  strm << "  <defaultCondProbTrue />" << std::endl;
  strm << "  <linkThresh />" << std::endl;
  strm << std::endl;

  // Print all task nodes.
  for (TaskNodeMap::iterator node_iter = task_nodes_.begin ();
    node_iter != task_nodes_.end (); node_iter++)
  {
    node_iter->second->print_xml (strm);
    strm << std::endl;
  }
  strm << std::endl;
  strm << std::endl;

  // Print all condition nodes.
  for (CondNodeMap::iterator node_iter = cond_nodes_.begin ();
    node_iter != cond_nodes_.end (); node_iter++)
  {
    node_iter->second->print_xml (strm);
    strm << std::endl;
  }
  strm << std::endl;
  strm << std::endl;

  // Print all precondition links.
  for (TaskNodeMap::iterator node_iter = task_nodes_.begin ();
    node_iter != task_nodes_.end (); node_iter++)
  {
    node_iter->second->print_precond_links_xml (strm);
    strm << std::endl;
  }
  strm << std::endl;
  strm << std::endl;

  // Print all effect links.
  for (TaskNodeMap::iterator node_iter = task_nodes_.begin ();
    node_iter != task_nodes_.end (); node_iter++)
  {
    node_iter->second->print_effect_links_xml (strm);
    strm << std::endl;
  }
  strm << std::endl;
  strm << std::endl;

  strm << "  </SANet:network>" << std::endl;
};

void SANet::Network::print (std::basic_ostream<char, std::char_traits<char> >& strm,
                     bool verbose)
{
  // Print current step.
  strm << "Current Step: " << step_ << std::endl << std::endl;

  // Print all task nodes.
  for (TaskNodeMap::iterator node_iter = task_nodes_.begin ();
    node_iter != task_nodes_.end (); node_iter++)
  {
    node_iter->second->print (strm, verbose);
    strm << std::endl;
  }

  // Print all condition nodes.
  for (CondNodeMap::iterator node_iter = cond_nodes_.begin ();
    node_iter != cond_nodes_.end (); node_iter++)
  {
    node_iter->second->print (strm, verbose);
    strm << std::endl;
  }
  strm << std::endl;
};

// Print links and ports.
void SANet::Network::print_link_ports (std::basic_ostream<char,
                                 std::char_traits<char> >& strm,
                                 bool verbose)
{
  // Print all precondition links.
  strm << "Precondition Links: " << std::endl;
  for (PrecondLinkPortMap::iterator precond_iter = precond_links_.begin ();
    precond_iter != precond_links_.end (); precond_iter++)
  {
    strm << "  " << "Cond " << precond_iter->first.first;
    strm << " -> " << "Task " << precond_iter->first.second << "(";
    strm << precond_iter->second << ")" << std::endl;
  }

  // Print all effect links.
  strm << "Effect Links: " << std::endl;
  for (EffectLinkPortMap::iterator effect_iter = effect_links_.begin ();
    effect_iter != effect_links_.end (); effect_iter++)
  {
    strm << "  " << "Task " << effect_iter->first.first << "(";
    strm << effect_iter->second << ") -> " << "Cond ";
    strm << effect_iter->first.second << std::endl;
  }
  strm << std::endl;
};

void SANet::Network::update (int max_steps)
{
  // Reset network and nodes to initial step.
  this->reset_step ();

  // Flag for whether network changed on last step, initially true.
  bool net_changed = true;

  // Set step limit.
  int step_limit = step_ + max_steps;

  // Run spreading activation until step limit is reached or no changes made.
  while (net_changed && (step_ < step_limit)) {
    // Update step.
    step_++;

    // Reset net_changed flag.
    net_changed = false;

    // Update all active task nodes.
    //for (TaskNodeMap::iterator node_iter = task_nodes_.begin ();
    //  node_iter != task_nodes_.end (); node_iter++)
    for(std::set<TaskID>::iterator node_iter = this->active_tasks_.begin();
      node_iter != this->active_tasks_.end(); node_iter++)
    {
      // Update node, setting net_changed flag if node changed.
      if (task_nodes_.find(*node_iter)->second->update ()) {
        net_changed = true;
      }
    }

    // Update all active condition nodes.
    //for (CondNodeMap::iterator node_iter = cond_nodes_.begin ();
    //  node_iter != cond_nodes_.end (); node_iter++)
    for(std::set<CondID>::iterator node_iter = this->active_conds_.begin();
      node_iter != this->active_conds_.end(); node_iter++)
    {
      // Update node, setting net_changed flag if node changed.
      if (cond_nodes_.find(*node_iter)->second->update ()) {
        net_changed = true;
      }
    }
  }

};

// Update a condition's current value (probability of being true).
void SANet::Network::update_cond_val (CondID cond_id, Probability true_prob)
{
  CondNodeMap::iterator iter = this->cond_nodes_.find (cond_id);
  if (iter == this->cond_nodes_.end ())
    throw "SANet::Network::update_cond_val (): Unknown condition node.";
  iter->second->set_init_prob (true_prob);
};

// Update a condition's (goal) utility.
void SANet::Network::update_cond_util (CondID cond_id, Utility utility)
{
  CondNodeMap::iterator cond_iter = this->cond_nodes_.find (cond_id);
  if (cond_iter == this->cond_nodes_.end ())
    throw "SANet::Network::update_cond_util(): Could not find condition by ID.";
  cond_iter->second->set_goal_util (utility);
};

// Update all condition utilities based on new goal set.
void SANet::Network::update_goals (GoalMap goals)
{
  SANet::GoalMap goal;

  // Remove all old goals.
  for (GoalMap::iterator old_goal_iter = this->goals_.begin ();
    old_goal_iter != this->goals_.end ();
    old_goal_iter++)
  {
    this->update_cond_util (old_goal_iter->first, 0);
  }
  this->goals_.clear ();

  // Add all new goals.
  for (GoalMap::iterator new_goal_iter = goals.begin ();
    new_goal_iter != goals.end ();
    new_goal_iter++)
  {
    this->update_cond_util (new_goal_iter->first, new_goal_iter->second);
  }
  this->goals_ = goals;
};

// Get a condition's current value (probability of being true).
Probability SANet::Network::get_cond_val (CondID cond_id)
{
  CondNodeMap::iterator iter = this->cond_nodes_.find (cond_id);
  if (iter == this->cond_nodes_.end ())
    throw "SANet::Network::get_cond_val (): Unknown condition node.";
  return iter->second->get_init_prob ();
};

// Get a condition's future probability for a given value.
// (NOTE: Future probability is based on whatever spreading
// activation has already been executed.)

//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****
// (WARNING: Condition node must have been active for all
// spreading activation or exception will be thrown.)
//****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****

Probability SANet::Network::get_cond_future_val(CondID cond_id, bool value)
{
	CondNodeMap::iterator iter = this->cond_nodes_.find(cond_id);
	if(iter == this->cond_nodes_.end())
		throw "SANet::Network::get_cond_future_val (): Unknown condition node.";
	return (iter->second->get_prob (this->step_, value)).probability;
}


// Get all goals.
const GoalMap& SANet::Network::get_goals (void)
{
  return this->goals_;
};

// Get a task's name.
std::string SANet::Network::get_task_name (TaskID task_id)
{
  // Find task node.
  TaskNodeMap::iterator task_iter = task_nodes_.find (task_id);
  if (task_iter == task_nodes_.end ()) {
    throw UnknownNode ();
  }

  return task_iter->second->get_name ();
};

// Get a condition's name.
std::string SANet::Network::get_cond_name (CondID cond_id)
{
  // Find condition node.
  CondNodeMap::iterator cond_iter = cond_nodes_.find (cond_id);
  if (cond_iter == cond_nodes_.end ()) {
    throw UnknownNode ();
  }

  return cond_iter->second->get_name ();
};

// Get a condition's type/kind.
CondKind SANet::Network::get_cond_type (CondID cond_id)
{
  // Find condition node.
  CondNodeMap::iterator cond_iter = cond_nodes_.find (cond_id);
  if (cond_iter == cond_nodes_.end ()) {
    throw UnknownNode ();
  }

  return cond_iter->second->get_cond_kind ();
};

// Get a task's current expected utility.
Utility SANet::Network::get_task_current_eu (TaskID task_id)
{
  // ****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
  throw "Plan monitoring/updating/replanning not yet implemented: SANet::SANet::Network::get_task_current_eu ().";
};

// Get a task's future expected utility.
Utility SANet::Network::get_task_future_eu (TaskID task_id)
{
  TaskNodeMap::iterator task_iter = task_nodes_.find (task_id);
  if (task_iter == task_nodes_.end ()) {
    throw UnknownNode ();
  }
  return task_iter->second->get_utility (this->step_);
};

// Get all preconditions of a task.
CondSet SANet::Network::get_preconds (TaskID task_id)
{
  // Find task node.
  TaskNodeMap::iterator task_iter = task_nodes_.find (task_id);
  if (task_iter == task_nodes_.end ()) {
    throw UnknownNode ();
  }

  // Create set of preconditions from pre-nodes set.
  CondSet preconds;
  preconds.clear ();
  LinkMap pre_links = task_iter->second->get_pre ();
  for (LinkMap::iterator iter = pre_links.begin (); iter != pre_links.end ();
    iter++)
  {
    Condition cond;
    cond.id = iter->first;
    CondNode* cond_node = this->cond_nodes_.find(iter->first)->second;
    cond.kind = cond_node->get_cond_kind();

    if (iter->second > 0)
      cond.value = true;
    else
      cond.value = false;
    preconds.insert (cond);
  }

  return preconds;
};

// Get all effects of a task.
CondSet SANet::Network::get_effects (TaskID task_id)
{
  // Find task node.
  TaskNodeMap::iterator task_iter = task_nodes_.find (task_id);
  if (task_iter == task_nodes_.end ()) {
    throw UnknownNode ();
  }

  // Create set of effects from pre-nodes set.
  CondSet effects;
  effects.clear ();
  LinkMap post_links = task_iter->second->get_post ();
  for (LinkMap::iterator iter = post_links.begin (); iter != post_links.end ();
    iter++)
  {
    Condition cond;
    cond.id = iter->first;
    CondNodeMap::iterator cond_map_iter = cond_nodes_.find (iter->first);
    if (cond_map_iter == cond_nodes_.end ())
      throw UnknownNode ();
    cond.kind = cond_map_iter->second->get_cond_kind ();
    if (iter->second > 0)
      cond.value = true;
    else
      cond.value = false;
    effects.insert (cond);
  }

  return effects;
};



// ****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP
// Ankit's function get_duration() needs to be removed, once we ensure any
// scheduling code relying on it has been changed.

// Get the duration of a task.
TimeValue SANet::Network::get_duration (TaskID task_id)
{
	TaskNode *temp = this->task_nodes_.find(task_id)->second;
  return NULL_TIME;
}
// ****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP****TEMP



// Get all tasks that satisfy a condition.
TaskSet SANet::Network::get_satisfying_tasks (Condition cond)
{
  // Find condition node.
  CondNodeMap::iterator cond_iter = this->cond_nodes_.find (cond.id);
  if (cond_iter == this->cond_nodes_.end ()) {
    throw UnknownNode ();
  }

  // Create set of tasks from pre-nodes set.
  TaskSet tasks;
  tasks.clear ();
  LinkMap pre_links = cond_iter->second->get_pre ();
  for (LinkMap::iterator iter = pre_links.begin (); iter != pre_links.end ();
    iter++)
  {
    // Add tasks with positive link weights for true condition or
    // tasks with negative link weights for false condition.
    if (cond.value) {
      if (iter->second > 0)
        tasks.insert (iter->first);
    } else {
      if (iter->second < 0)
        tasks.insert (iter->first);
    }
  }

  return tasks;
};

// Get effect link port.
PortID SANet::Network::get_effect_port (TaskID task_id, CondID cond_id)
{
 // Find port, throwing exception if not found.
  EffectLinkPortMap::iterator iter = this->effect_links_.find (
    std::make_pair (task_id, cond_id));
  if (iter == this->effect_links_.end ()) {
    throw UnknownNode ();
  }

  return iter->second;
};

// Get precondition link port.
PortID SANet::Network::get_precond_port (CondID cond_id, TaskID task_id)
{
 // Find port, throwing exception if not found.
  PrecondLinkPortMap::iterator iter = this->precond_links_.find (
    std::make_pair (cond_id, task_id));
  if (iter == this->precond_links_.end ()) {
    throw UnknownNode ();
  }

  return iter->second;
};

// Get ports for a causal link.
LinkPorts SANet::Network::get_clink_ports (TaskID task1_id, CondID cond_id,
                                    TaskID task2_id)
{
  return std::make_pair (this->get_effect_port (task1_id, cond_id),
    this->get_precond_port (cond_id, task2_id));
};

// Set Task State.
void SANet::Network::set_task_state(TaskID task_ID, bool state)
{
  // Get pointer to node from ID.
  SANet::TaskNodeMap::iterator task_iter = this->task_nodes_.find(task_ID);
  if (task_iter == this->task_nodes_.end ())
    throw UnknownNode ();
  
  // Use node pointer to set activity state.
  task_iter->second->set_activity(state);

  if(state) {
    // Insert into active task set & remove from disabled task set.
    this->active_tasks_.insert(task_ID);
    this->disabled_tasks_.erase(task_ID);
  } else {
    // Remove from active task set & insert into disabled task set.
    this->active_tasks_.erase(task_ID);
    this->disabled_tasks_.insert(task_ID);
  }
};

// Set Cond State.
void SANet::Network::set_cond_state(CondID cond_ID, bool state)
{
    
  // Get pointer to node from ID.
  SANet::CondNodeMap::iterator cond_iter = this->cond_nodes_.find(cond_ID);
  if (cond_iter == this->cond_nodes_.end ())
    throw UnknownNode ();
  
  // Use node pointer to set activity state.
  cond_iter->second->set_activity(state);

  if(state) {
    // Insert into active condition set & remove from disabled condition set.
    this->active_conds_.insert(cond_ID);
    this->disabled_conds_.erase(cond_ID);

  } else {
    // Remove from active condition set & insert into disabled condition set.
    this->active_conds_.erase(cond_ID);
    this->disabled_conds_.insert(cond_ID);
  }

};

// Set All nodes to State.
void SANet::Network::set_nodes_state(bool state)
{
  if(state) {
    // For all tasks, activate, insert into active set, & remove from disabled set.
    for (TaskNodeMap::iterator node_iter = task_nodes_.begin ();
    node_iter != task_nodes_.end (); node_iter++)
    {
      node_iter->second->set_activity(state);
      this->active_tasks_.insert(node_iter->first);
      this->disabled_tasks_.erase(node_iter->first);
    }

    // For all conditions, activate, insert into active set, & remove from disabled set.
    for (CondNodeMap::iterator node_iter = cond_nodes_.begin ();
    node_iter != cond_nodes_.end (); node_iter++)
    {
      node_iter->second->set_activity(state);
      this->active_conds_.insert(node_iter->first);
      this->disabled_conds_.erase(node_iter->first);
    }
  } else {
    // For all tasks, deactivate, remove from active set, & insert into disabled set.
    for (TaskNodeMap::iterator node_iter = task_nodes_.begin ();
         node_iter != task_nodes_.end (); node_iter++)
    {
      node_iter->second->set_activity(state);
      this->active_tasks_.erase(node_iter->first);
      this->disabled_tasks_.insert(node_iter->first);
    }

    // For all conditions, deactivate, remove from active set, & insert into disabled set.
    for (CondNodeMap::iterator node_iter = cond_nodes_.begin ();
         node_iter != cond_nodes_.end (); node_iter++)
    {
      node_iter->second->set_activity(state);
      this->active_conds_.erase(node_iter->first);
      this->disabled_conds_.insert(node_iter->first);
    }
  }

};