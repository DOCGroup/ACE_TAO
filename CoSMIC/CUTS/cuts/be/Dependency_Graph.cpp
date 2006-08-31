// $Id$

#include "Dependency_Graph.h"

//=============================================================================
/*
 * CUTS_Dependency_Node
 */
//=============================================================================

//
// CUTS_Dependency_Node
//
CUTS_Dependency_Node::CUTS_Dependency_Node (const std::string & name)
: flags_ (DNF_NONE),
  name_ (name)
{
  this->basename_ = this->name_.find_last_of ("//") + 1;
}

//
// ~CUTS_Dependency_Node
//
CUTS_Dependency_Node::~CUTS_Dependency_Node (void)
{

}

//
// basename
//
const char * CUTS_Dependency_Node::basename (void) const
{
  return this->name_.c_str () + this->basename_;
}

//
// name
//
const std::string & CUTS_Dependency_Node::name (void) const
{
  return this->name_;
}

//
// reset
//
void CUTS_Dependency_Node::reset (void)
{
  this->basename_ = 0;
  this->flags_ = 0;
  this->includes_.clear ();
  this->libpaths_.clear ();
  this->libs_.clear ();
  this->references_.clear ();
}

//=============================================================================
/*
 * CUTS_Dependency_Graph
 */
//=============================================================================

//
// CUTS_Dependency_Graph
//
CUTS_Dependency_Graph::CUTS_Dependency_Graph (void)
{

}

//
// ~CUTS_Dependency_Graph
//
CUTS_Dependency_Graph::~CUTS_Dependency_Graph (void)
{
  for (Dependency_Graph::iterator iter = this->graph_.begin ();
       iter != this->graph_.end ();
       iter ++)
  {
    delete iter->second;
  }
}

//
// find_node
//
bool CUTS_Dependency_Graph::find_node (
  const std::string & name,
  CUTS_Dependency_Node * &node) const
{
  Dependency_Graph::const_iterator iter = this->graph_.find (name);

  if (iter != this->graph_.end ())
  {
    node = iter->second;
    return true;
  }
  else
  {
    return false;
  }
}

//
// remove_node
//
void CUTS_Dependency_Graph::remove_node (const std::string & name)
{
  this->graph_.erase (name);
}

//
// create_node
//
CUTS_Dependency_Node * CUTS_Dependency_Graph::create_node (
  const std::string & name)
{
  CUTS_Dependency_Node * node = 0;

  if (!this->find_node (name, node))
  {
    node = new CUTS_Dependency_Node (name);
    this->graph_.insert (Dependency_Graph::value_type (name, node));
  }

  return node;
}

//
// graph
//
const CUTS_Dependency_Graph::Dependency_Graph &
CUTS_Dependency_Graph::graph (void) const
{
  return this->graph_;
}

//
// reset_visit_flag
//
void CUTS_Dependency_Graph::reset_visit_flag (void)
{
  for (Dependency_Graph::iterator iter = this->graph_.begin ();
       iter != this->graph_.end ();
       iter ++)
  {
    iter->second->flags_ &= ~CUTS_Dependency_Node::DNF_VISITED;
  }
}
