/* -*- c++ -*- */
// $Id$

#include "removing_visitor.h"
#include "removing_closure_visitor.h"
#include "be_extern.h"
#include "XercesString.h"

removing_visitor::removing_visitor (void)
  : id_holder_ (0)
{}

bool
removing_visitor::visit_gme_folder (DOMElement *node)
{
  const XMLCh *kind = node->getAttribute (X ("kind"));
  
  // We traverse only this folder. For each item we remove,
  // we spawn a removing_closure_visitor to traverse the others.
  return
    (X ("InterfaceDefinitions") == kind ? this->visit_children (node) : true);
}

bool
removing_visitor::visit_gme_model (DOMElement *node)
{
  // Visit the children first so we don't miss something in this
  // scope that has to go to the closure visitor.
  if (!this->visit_children (node))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        "removing_visitor::visit_gme_model - "
                        "visit_children failed\n"),
                        false);
    }
    
  return (this->found_in_idl (node)
            ? true
            : this->do_closure ());
}

bool
removing_visitor::visit_gme_atom (DOMElement *node)
{
  (void) this->found_in_idl (node);
  return true;
}

bool
removing_visitor::visit_gme_reference (DOMElement *node)
{
  return (this->found_in_idl (node)
            ? true
            : this->do_closure ());
}

bool
removing_visitor::visit_gme_connection (DOMElement *node)
{
  (void) this->found_in_idl (node);
  return true;
}

bool
removing_visitor::found_in_idl (DOMElement *node)
{
  this->id_holder_ = node->getAttribute (X ("id"));
  int status = be_global->gme_id_set ().find (this->id_holder_);
  
  if (0 != status)
    {
      // Remove the node we were passed.
      be_global->release_node (node);    
    }
    
  return (0 == status);
}

bool
removing_visitor::do_closure (void)
{
  // Remove references and connections to the node corresponding
  // to this id in the other folders.
  removing_closure_visitor rc_visitor (this->id_holder_);
  return rc_visitor.visit_root (be_global->root_folder ());
}



