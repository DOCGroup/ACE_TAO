/* -*- c++ -*- */
// $Id$

#include "message_visitor.h"
#include "XercesString.h"

#include "be_extern.h"

#include "ast_argument.h"
#include "ast_attribute.h"
#include "ast_component.h"
#include "ast_exception.h"
#include "ast_operation.h"
#include "ast_root.h"
#include "utl_identifier.h"
#include "nr_extern.h"

#include "ace/OS_NS_stdio.h"

message_visitor::message_visitor (DOMElement *sub_tree)
  : idl_to_wsdl_visitor (sub_tree),
    current_msg_ (0),
    current_response_msg_ (0)
{
}

message_visitor::~message_visitor (void)
{
}

int
message_visitor::visit_interface (AST_Interface *node)
{
  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "message_visitor::visit_interface - "
                          "codegen for scope failed\n"),
                        -1);
    }
    
  return 0;
}

int
message_visitor::visit_component (AST_Component *node)
{
  if  (0 != this->visit_interface (node))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "idl_to_wsdl_visitor::visit_component - "
                          "codegen for visit_interface failed\n"),
                        -1);
    }
    
  return 0;
}

int
message_visitor::visit_exception (AST_Exception *node)
{
  DOMElement *msg = this->doc_->createElement (X ("message"));
  
  ACE_CString fname ("_exception.");
  ACE_CString tname;
  this->type_name (tname, node, false);
  fname += tname;
  msg->setAttribute (X ("name"), X (fname.c_str ()));
  
  be_global->root_element ()->insertBefore (
    msg,
    be_global->msg_insert_point ());
  
  DOMElement *part = this->doc_->createElement (X ("part"));
  part->setAttribute (X ("name"), X ("exception"));
  tname = ACE_CString ("tns:") + tname;
  part->setAttribute (X ("type"), X (tname.c_str ()));
  
  msg->appendChild (part);
    
  return 0;
}

int
message_visitor::visit_operation (AST_Operation *node)
{
  AST_Decl::NodeType nt = ScopeAsDecl (node->defined_in ())->node_type ();
  
  // We don't want to generate anything for valuetype operations.
  if (AST_Decl::NT_eventtype == nt || AST_Decl::NT_valuetype == nt)
    {
      return 0;
    }
    
  this->gen_messages (node, 0);

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                          "message_visitor::visit_operation - "
                          "codegen for message scope failed\n"),
                        -1);
    }
    
  return 0;
}

int
message_visitor::visit_argument (AST_Argument *node)
{
  AST_Argument::Direction dir = node->direction ();
  const char *lname = node->local_name ()->get_string ();
  ACE_CString tname;
  this->type_name (tname, node->field_type ());
  
  if (AST_Argument::dir_OUT != dir)
    {
      DOMElement *arg = this->doc_->createElement (X ("part"));
      arg->setAttribute (X ("name"), X (lname));
      arg->setAttribute (X ("type"), X (tname.c_str ()));
      this->current_msg_->appendChild (arg);
    }
  
  if (AST_Argument::dir_IN != dir)
    {
      DOMElement *response_arg = this->doc_->createElement (X ("part"));
      response_arg->setAttribute (X ("name"), X (lname));
      response_arg->setAttribute (X ("type"), X (tname.c_str ()));
      this->current_response_msg_->appendChild (response_arg);
    }
    
  return 0;
}

int
message_visitor::visit_attribute (AST_Attribute *node)
{
  this->gen_messages (node, "_get_");
  
  if (!node->readonly ())
    {
      this->gen_messages (node, "_set_");
    }
   
  return 0;
}

void
message_visitor::gen_messages (AST_Decl *node,
                               const char *prefix)
{
  this->current_msg_ = this->doc_->createElement (X ("message"));
  ACE_CString name (ScopeAsDecl (node->defined_in ())->full_name ());
  be_global->to_wsdl_name (name);
  name += ".";
  name += (0 != prefix ? prefix : "");
  name += node->local_name ()->get_string ();
  this->current_msg_->setAttribute (X ("name"), X (name.c_str ()));
  
  be_global->root_element ()->insertBefore (
    this->current_msg_,
    be_global->msg_insert_point ());

  this->current_response_msg_ = this->doc_->createElement (X ("message"));
  name += "Response";
  this->current_response_msg_->setAttribute (X ("name"), X (name.c_str ()));
  
  be_global->root_element ()->insertBefore (
    this->current_response_msg_,
    be_global->msg_insert_point ());
  
  AST_Operation *op = AST_Operation::narrow_from_decl (node);
  AST_Attribute *attr = AST_Attribute::narrow_from_decl (node);
  ACE_CString prefix_str (prefix);
  ACE_CString part_name;
  AST_Type *rt = (0 != op ? op->return_type () : attr->field_type ());
  this->type_name (part_name, rt);
 
  if (0 != op && !op->void_return_type () || prefix_str == "_get_")
    {
      DOMElement *return_elem = this->doc_->createElement (X ("part"));
      return_elem->setAttribute (X ("name"), X ("_return"));
      return_elem->setAttribute (X ("type"), X (part_name.c_str ()));     
      this->current_response_msg_->appendChild (return_elem);
    }
  else if (prefix_str == "_set_")
    {
      DOMElement *arg_elem = this->doc_->createElement (X ("part"));
      arg_elem->setAttribute (X ("name"), X ("value"));
      arg_elem->setAttribute (X ("type"), X (part_name.c_str ()));
      this->current_msg_->appendChild (arg_elem);
    }
}

