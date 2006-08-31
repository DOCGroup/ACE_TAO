/* -*- c++ -*- */
// $Id$

#include "removing_closure_visitor.h"
#include "be_extern.h"
#include "XercesString.h"

removing_closure_visitor::removing_closure_visitor (const XMLCh *id)
  : id_ (id)
{}

bool
removing_closure_visitor::visit_gme_folder (DOMElement *node)
{
  const XMLCh *kind = node->getAttribute (X ("kind"));
  
  // The InterfaceDefinitions folder is handled by the removing_visitor
  // instance that spawned us, and the PredefinedTypes folder should
  // never be touched.
  bool skip =
    (X ("InterfaceDefinitions") == kind || X ("PredefinedTypes") == kind);
        
  return (skip ? true : this->visit_children (node));
}

bool
removing_closure_visitor::visit_gme_model (DOMElement *node)
{
  // Component instances in PICML have a "derivedfrom" attribute
  // containing the id of the element in InterfaceDefinitions.
  return (this->derived_from (node))
    ? this->remove_and_do_closure (node)
    : this->visit_children (node);
}

bool
removing_closure_visitor::visit_gme_reference (DOMElement *node)
{
  const XMLCh *referred = node->getAttribute (X ("referred"));
  
  // Ports of component instanaces have a "derivedfrom" attribute
  // containing the id of the original port reference.
  return (this->id_ == X (referred) || this->derived_from (node)
    ? this->remove_and_do_closure (node)
    : true);
}

bool
removing_closure_visitor::visit_gme_connection (DOMElement *node)
{
  // This code and the code in check_endpoint() depends on the XML
  // mapping of a GME Connection staying the same - a safe bet.
  // If the id of either endpoint matches our member id, 'node'
  // is removed.
  
  DOMElement *dst = (DOMElement *) node->getLastChild ();
  this->check_endpoint (dst, node);

  // The called method checks if 'node' has already been released.
  DOMElement *src = (DOMElement *) dst->getPreviousSibling ();
  this->check_endpoint (src, node);

  return true;
}

void
removing_closure_visitor::check_endpoint (DOMElement *endpoint,
                                          DOMElement *node)
{
  // Quick check (see last line below) in case PICML ever
  // allows something to be connected to itself.
  if (0 == node)
    {
      return;
    }
    
  const XMLCh *target = endpoint->getAttribute (X ("target"));
  
  if (this->id_ == target)
    {
      // If an endpoint id matches the id of the removed element,
      // remove the connection element as well. No need to do
      // closure on a connection.
      be_global->release_node (node);
      node = 0;
    }
}

bool
removing_closure_visitor::derived_from (DOMElement *node)
{
  if (!node->hasAttribute (X ("derivedfrom")))
    {
      return false;
    }
    
  const XMLCh *derived = node->getAttribute (X ("derivedfrom"));
  return (this->id_ == derived);
}

bool
removing_closure_visitor::remove_and_do_closure (DOMElement *node)
{
  const XMLCh *id = node->getAttribute (X ("id"));
  be_global->release_node (node);
  node = 0;
  removing_closure_visitor rc_visitor (id);
  return rc_visitor.visit_root (be_global->root_folder ());
}

bool
removing_closure_visitor::visit_gme_set (DOMElement *node)
{
  const XMLCh *members_X = node->getAttribute (X ("members"));
  char *members_str = XMLString::transcode (members_X);
  ACE_CString members (members_str);
  char *id_str = XMLString::transcode (this->id_);
  
  int pos = members.find (id_str);
  bool found = (pos != ACE_CString::npos);
  const size_t ONE_ID_LEN = 16;
  const size_t TWO_ID_LEN = 2 * ONE_ID_LEN + 1;
      
  // The "members" attribute can have one or more id strings, each
  // 16 chars, separated by whitespace. If there is an id match:
  // - if there's only one id, remove the node
  // - otherwise, remove the matched string from the attribute
  bool remove = (found && (members.length () < TWO_ID_LEN));
  
  if (found && !remove)
    {
      if (0 == pos)
        {
          members = members.substr (ONE_ID_LEN + 1);
        }
      else if ((size_t) pos > (members.length () - TWO_ID_LEN))
        {
          // This should not be npos if 'remove' is false.
          int last_space = members.rfind (' ');
          members = members.substr (0, last_space - 1);
        }
      else
        {
          members = members.substr (0, pos - 1)
                    + members.substr (pos + ONE_ID_LEN);
        }
        
      node->setAttribute (X ("members"), X (members.c_str ()));
    }
  
  XMLString::release (&members_str);
  XMLString::release (&id_str);
  
  return (remove ? this->remove_and_do_closure (node) : true);
}