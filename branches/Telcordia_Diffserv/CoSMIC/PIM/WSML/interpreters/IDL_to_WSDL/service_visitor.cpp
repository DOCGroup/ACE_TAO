/* -*- c++ -*- */
// $Id$

#include "service_visitor.h"
#include "XercesString.h"

#include "be_extern.h"

#include "nr_extern.h"
#include "utl_identifier.h"

service_visitor::service_visitor (DOMElement *sub_tree)
  : idl_to_wsdl_visitor (sub_tree),
    service_elem_ (0)
{
}

service_visitor::~service_visitor (void)
{
}

int
service_visitor::visit_component (AST_Component *node)
{
  this->service_elem_ = this->doc_->createElement (X ("service"));
  be_global->root_element ()->insertBefore (this->service_elem_, 0);
  
  if (0 == be_global->binding_insert_point ())
    {
      be_global->binding_insert_point (this->service_elem_);
    }
    
  ACE_CString name (node->full_name ());
  be_global->to_wsdl_name (name);
  
  this->service_elem_->setAttribute (X ("name"), X (name.c_str ()));
  
  this->gen_port (node, 0);
  this->map_ports (node, node->provides ());
  
  return 0;
}

void
service_visitor::gen_port (AST_Component *node, const char *comp_port_name)
{
  DOMElement *port = this->doc_->createElement (X ("port"));
  ACE_CString name (node->full_name ());
  be_global->to_wsdl_name (name);
  
  if (0 != comp_port_name)
    {
      name += ".";
      name += comp_port_name;
    }
    
  port->setAttribute (X ("name"), X (name.c_str ()));
  this->service_elem_->appendChild (port);
  
  AST_Decl *scope = ScopeAsDecl (node->defined_in ());
  ACE_CString binding_val;
  
  if (AST_Decl::NT_root == scope->node_type ())
    {
      binding_val = "tns:";
    }
  else
    {
      this->type_name (binding_val, scope);
    }
    
  binding_val += ".";

  if (0 == comp_port_name)
    {
      binding_val += "_SE_";
      binding_val += node->local_name ()->get_string ();
    }
  else
    {
      binding_val += node->local_name ()->get_string ();
      binding_val += "._SE_";
      binding_val += comp_port_name;
    }
  
  port->setAttribute (X ("binding"), X (binding_val.c_str ()));
  
  DOMElement *address = this->doc_->createElement (X ("soap:address"));
  
  // This value will force the modeler to set the location.
  address->setAttribute (X ("location"), X ("http://tempuri.org"));
  
  port->appendChild (address);
}

void
service_visitor::map_ports (
  AST_Component *node,
  ACE_Unbounded_Queue<AST_Component::port_description> &list)
{
  for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> i (list);
       !i.done ();
       i.advance ())
    {
      AST_Component::port_description *desc = 0;
      i.next (desc);
      
      this->gen_port (node, desc->id->get_string ());
    }
}

