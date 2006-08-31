/* -*- c++ -*- */
// $Id$

#include "binding_visitor.h"
#include "XercesString.h"
#include "be_extern.h"

#include "ast_attribute.h"
#include "ast_component.h"
#include "ast_exception.h"
#include "ast_operation.h"
#include "ast_root.h"
#include "utl_identifier.h"
#include "utl_idlist.h"
#include "utl_string.h"
#include "utl_exceptlist.h"
#include "global_extern.h"
#include "nr_extern.h"

#include "ace/OS_NS_stdio.h"

#include <xercesc/dom/DOM.hpp>

extern const char *SOAP_ENC;
extern const char *CORBA_NS;

binding_visitor::binding_visitor (DOMElement *sub_tree)
  : idl_to_wsdl_visitor (sub_tree),
    current_binding_ (0),
    current_comp_ (0),
    current_port_ (0)
{
}

binding_visitor::~binding_visitor (void)
{
}

int
binding_visitor::visit_component (AST_Component *node)
{
  this->current_comp_ = node;
  this->gen_binding (node);
  
  // Generate WSDL for our base component, if any.
  this->gen_inherited_comp (node);
  
  for (ACE_Unbounded_Queue_Iterator<AST_Component::port_description> i (
         node->provides ());
       !i.done ();
       i.advance ())
    {
      AST_Component::port_description *desc = 0;
      i.next (desc);
      
      this->current_port_ = desc->impl;
      this->gen_binding (node, desc->impl, desc->id->get_string ());
    }
  
  return 0;
}

int
binding_visitor::visit_operation (AST_Operation *node)
{
  DOMElement *operation =
    this->doc_->createElement (X ("operation"));
  this->current_binding_->appendChild (operation);
  this->finish_operation (node, operation, 0);                              
    
  return 0;
}

int
binding_visitor::visit_attribute (AST_Attribute *node)
{
  bool read_only = node->readonly ();

  DOMElement *get_attr =
    this->doc_->createElement (X ("operation"));
  this->current_binding_->appendChild (get_attr);
  this->finish_operation (node, get_attr, "_get_");
  
  if (!read_only)
    {
      DOMElement *set_attr =
        this->doc_->createElement (X ("operation"));
      this->finish_operation (node, set_attr, "_set_");          
      this->current_binding_->appendChild (set_attr);
    }
   
  return 0;
}

void
binding_visitor::finish_operation (AST_Decl *node,
                                   DOMElement *elem,
                                   const char *prefix)
{
  ACE_CString lname (0 != prefix ? prefix : "");
  lname += node->local_name ()->get_string ();
  elem->setAttribute (X ("name"), X (lname.c_str ()));
  
  DOMElement *soap_operation =
    this->doc_->createElement (X ("soap:operation"));
  soap_operation->setAttribute (X ("style"), X ("rpc"));
  elem->appendChild (soap_operation);

  DOMElement *binding_input =
    this->doc_->createElement (X ("input"));
  elem->appendChild (binding_input);
  
  DOMElement *soap_body =
    this->doc_->createElement (X ("soap:body"));
  soap_body->setAttribute (X ("use"), X ("encoded"));
  soap_body->setAttribute (X ("encodingStyle"),
                           X ("http://schemas.xmlsoap.org/soap/encoding/"));
  
  ACE_CString target_name_space ("urn:");
  target_name_space += be_global->output_file (); 
  soap_body->setAttribute (X ("namespace"),
                           X (target_name_space.c_str ()));
                           
  binding_input->appendChild (soap_body);
  
  AST_Operation *op = AST_Operation::narrow_from_decl (node);
  AST_Attribute *attr = AST_Attribute::narrow_from_decl (node);
  
  if (0 == op || AST_Operation::OP_oneway != op->flags ())
    {
      DOMElement *binding_output =
        this->doc_->createElement (X ("output"));
      elem->appendChild (binding_output);
 
      DOMElement *soap_body_clone =
        dynamic_cast<DOMElement *> (soap_body->cloneNode (false));
      binding_output->appendChild (soap_body_clone);
    }
    
  UTL_ExceptList *exceptions = 0;
  
  if (0 == prefix)
    {
      exceptions = op->exceptions ();
    }
  else if (0 == ACE_OS::strcmp (prefix, "_get_"))
    {
      exceptions = attr->get_get_exceptions ();
    }
  else
    {
      exceptions = attr->get_set_exceptions ();
    }
    
  for (UTL_ExceptlistActiveIterator i (exceptions);
        !i.is_done ();
        i.next ())
    {
      this->gen_fault (elem, i.item ());
    }

  // Map the ever-present CORBA::SystemException.
  this->gen_fault (elem, 0);
}

void
binding_visitor::gen_inherited_operations (AST_Interface *node)
{
  if (0 == node)
    {
      return;
    }

  for (long i = 0; i < node->n_inherits_flat (); ++i)
    {
      this->append_ops_and_attrs (node->inherits_flat ()[i]);
    }
}

void
binding_visitor::gen_inherited_comp (AST_Component *node)
{
  AST_Component *parent = node->base_component ();
  
  if (0 != parent)
    {
      this->gen_inherited_operations (parent);
      this->append_ops_and_attrs (parent);
      this->gen_inherited_comp (parent);
    }
}

void
binding_visitor::append_ops_and_attrs (AST_Interface *ancestor)
{
  for (UTL_ScopeActiveIterator i (ancestor, UTL_Scope::IK_decls);
       !i.is_done ();
       i.next ())
    {
      AST_Decl *d = i.item ();
      AST_Operation *op = AST_Operation::narrow_from_decl (d);
      AST_Attribute *attr = AST_Attribute::narrow_from_decl (d);
      
      if (0 != op)
        {
          DOMElement *op_elem =
            this->doc_->createElement (X ("operation"));
          this->current_binding_->appendChild (op_elem);
          this->finish_operation (op, op_elem, 0);
        }
      else if (0 != attr)
        {
          DOMElement *attr_elem =
            this->doc_->createElement (X ("operation"));
          this->current_binding_->appendChild (attr_elem);
          this->finish_operation (attr, attr_elem, "_get_");
          
          if (!attr->readonly ())
            {
              DOMElement *attr_set_elem =
                this->doc_->createElement (X ("operation"));
              this->current_binding_->appendChild (attr_set_elem);
              this->finish_operation (attr, attr_set_elem, "_set_");
            }
        }
    }
}

void
binding_visitor::gen_binding (AST_Component *node,
                              AST_Type *port_impl,
                              const char *port_name)
{
  this->current_binding_ = this->doc_->createElement (X ("binding"));
  
  be_global->root_element ()->insertBefore (
    this->current_binding_,
    be_global->binding_insert_point ());
    
  if (0 == be_global->port_type_insert_point ())
    {
      be_global->port_type_insert_point (this->current_binding_);
    }
  
  ACE_CString scope_name;
  AST_Decl *p = ScopeAsDecl (node->defined_in ());
  
  if (AST_Decl::NT_root != p->node_type ())
    {
      this->type_name (scope_name, p, false);
      scope_name += ".";
    }
    
  const char *lname = 0;
  const char *node_name = node->local_name ()->get_string ();
    
  if (0 == port_name)
    {
      lname = node_name;
    }
  else
    {
      scope_name += node_name;
      scope_name += ".";
      lname = port_name; 
    }
    
  scope_name += "_SE_";
  scope_name += lname;
  
  this->current_binding_->setAttribute (X ("name"),
                                        X (scope_name.c_str ()));
  ACE_CString tname (0 == port_impl
                       ? node->full_name ()
                       : port_impl->full_name ());
  be_global->to_wsdl_name (tname);
  tname = ACE_CString ("tns:") + tname;
  this->current_binding_->setAttribute (X ("type"),
                                        X (tname.c_str ()));
  
  DOMElement *soap_binding =
    this->doc_->createElement (X ("soap:binding"));
  soap_binding->setAttribute (X ("style"), X ("rpc"));
  soap_binding->setAttribute (X ("transport"),
                              X ("http://schemas.xmlsoap.org/soap/http"));
  this->current_binding_->appendChild (soap_binding);
  
  AST_Interface *scope = 0;
  
  // TODO - code generation for receptacles and events sinks/sources.
  if (0 == port_impl)
    {
      scope = node;
    }
  else if (AST_Decl::NT_interface == port_impl->node_type ())
    {
      scope = AST_Interface::narrow_from_decl (port_impl);
    }
  
  if (scope != 0 && this->visit_scope (scope) != 0)
    {
      ACE_ERROR((LM_ERROR,
                  "binding_visitor::gen_binding - "
                  "codegen for scope failed\n"));
    }
   
  // This also covers a component's supported interfaces.
  // A no-op if 'scope' is 0.
  this->gen_inherited_operations (scope);
}

void
binding_visitor::gen_fault (DOMElement *binding_op,
                            AST_Decl *user_exception)
{
  DOMElement *fault =
    this->doc_->createElement (X ("fault"));
  ACE_CString fname;
  
  if (0 == user_exception)
    {
      fname = "CORBA.SystemException";
    }
  else
    {
      fname = user_exception->full_name ();
      be_global->to_wsdl_name (fname);
    }
    
  fault->setAttribute (X ("name"), X (fname.c_str ()));
  
  DOMElement *soap_fault =
    this->doc_->createElement (X ("soap:fault"));
  soap_fault->setAttribute (X ("name"), X (fname.c_str ()));
  soap_fault->setAttribute (X ("use"), X ("encoded"));
  soap_fault->setAttribute (X ("encodingStyle"), X (SOAP_ENC));
  soap_fault->setAttribute (X ("namespace"), X (CORBA_NS));
  fault->appendChild (soap_fault);

  binding_op->appendChild (fault);
}

