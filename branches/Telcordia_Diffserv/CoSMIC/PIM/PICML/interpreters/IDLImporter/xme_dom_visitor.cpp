/* -*- c++ -*- */
// $Id$

#include "xme_dom_visitor.h"
#include "be_extern.h"
#include "XercesString.h"

bool
xme_dom_visitor::visit_root (DOMElement *node)
{
  return this->visit_children (node);
}

bool
xme_dom_visitor::visit_children (DOMElement *node)
{
  DOMNodeList *children = node->getChildNodes ();
  bool is_good = true;
  XMLSize_t oldlength = children->getLength ();

  for (XMLSize_t index = 0; index < children->getLength (); ++index)
    {
      DOMNode *child = children->item (index);
      
      if (0 == child)
        {
          continue;
        }

      switch (child->getNodeType ())
        {
          case DOMNode::ELEMENT_NODE:
            is_good = this->visit_dom_element ((DOMElement *) child);
            break;
          case DOMNode::ATTRIBUTE_NODE:
            is_good = this->visit_dom_attribute ((DOMAttr *) child);
            break;
          case DOMNode::TEXT_NODE:
            is_good = this->visit_dom_text ((DOMText *) child);
            break;
          default:
            ACE_ERROR_RETURN ((LM_ERROR,
                               "xme_dom_visitor::visit_children - "
                               "unhandled DOM node type\n"),
                              false);
        }
      
      // No need for an error message - it is generated at a lower level.  
      if (!is_good)
        {
          return false;
        }
       
      // If we have removed an element, it will be reflected immediately
      // in the list of children, so we must adjust the iteration.  
      XMLSize_t newlength = children->getLength ();
      
      if (oldlength != newlength)
        {
          oldlength = newlength;
          --index;
        }
    }
    
  return is_good;
}

bool
xme_dom_visitor::visit_dom_element (DOMElement *node)
{
  const XMLCh *tag = node->getTagName ();
  
  if (X ("model") == tag)
    {
      return this->visit_gme_model (node);
    }
  else if (X ("folder") == tag)
    {
      return this->visit_gme_folder (node);
    }
  else if (X ("atom") == tag)
    {
      return this->visit_gme_atom (node);
    }
  else if (X ("reference") == tag)
    {
      return this->visit_gme_reference (node);
    }
  else if (X ("connection") == tag)
    {
      return this->visit_gme_connection (node);
    }
  else if (X ("attribute") == tag)
    {
      return this->visit_gme_attribute (node);
    }
  else if (X ("regnode") == tag)
    {
      return this->visit_gme_regnode (node);
    }
  else if (X ("name") == tag)
    {
      return this->visit_gme_name (node);
    }
  else if (X ("set") == tag)
    {
      return this->visit_gme_set (node);
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        "xme_dom_visitor::visit_dom_element - "
                        "node tag name not recognized\n"),
                        false);
    }
}

bool
xme_dom_visitor::visit_dom_text (DOMText *)
{
  return true;
}

bool
xme_dom_visitor::visit_dom_attribute (DOMAttr *)
{
  return true;
}

bool
xme_dom_visitor::visit_gme_folder (DOMElement *)
{
  return true;
}

bool
xme_dom_visitor::visit_gme_model (DOMElement *)
{
  return true;
}

bool
xme_dom_visitor::visit_gme_atom (DOMElement *)
{
  return true;
}

bool
xme_dom_visitor::visit_gme_reference (DOMElement *)
{
  return true;
}

bool
xme_dom_visitor::visit_gme_connection (DOMElement *)
{
  return true;
}

bool
xme_dom_visitor::visit_gme_attribute (DOMElement *)
{
  return true;
}

bool
xme_dom_visitor::visit_gme_regnode (DOMElement *)
{
  return true;
}

bool
xme_dom_visitor::visit_gme_name (DOMElement *)
{
  return true;
}

bool
xme_dom_visitor::visit_gme_set (DOMElement *)
{
  return true;
}


