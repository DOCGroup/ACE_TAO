// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    IDL_TO_WSDL_BE_DLL
//
// = FILENAME
//    binding_visitor.h
//
// = DESCRIPTION
//    Generates binding elements in the WSDL document.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef BINDING_VISITOR_H
#define BINDING_VISITOR_H

#include "idl_to_wsdl_visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class binding_visitor : public idl_to_wsdl_visitor
{
  //
  // = TITLE
  //    binding_visitor
  //
  // = DESCRIPTION
  //    Generates binding elements in the WSDL document.
  //
public:
  binding_visitor (DOMElement *sub_tree);
  virtual ~binding_visitor (void);

  virtual int visit_component (AST_Component *node);
  virtual int visit_operation (AST_Operation *node);
  virtual int visit_attribute (AST_Attribute *node);
  
private:
  void gen_binding (AST_Component *node,
                    AST_Type *port_impl = 0,
                    const char *port_name = 0);
  void finish_operation (AST_Decl *node,
                         DOMElement *elem,
                         const char *prefix);
  void gen_inherited_operations (AST_Interface *node);
  void gen_inherited_comp (AST_Component *node);
  void append_ops_and_attrs (AST_Interface *ancestor);
  void gen_fault (DOMElement *binding_op, AST_Decl *user_exception);
  
private:
  DOMElement *current_binding_;
  AST_Component *current_comp_;
  AST_Type *current_port_;
};

#endif /* BINDING_VISITOR_H */
