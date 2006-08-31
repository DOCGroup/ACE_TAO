// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    IDL_TO_WSDL_BE_DLL
//
// = FILENAME
//    port_type_visitor.h
//
// = DESCRIPTION
//    Generates portType elements in the WSDL document.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef PORT_TYPE_VISITOR_H
#define PORT_TYPE_VISITOR_H

#include "idl_to_wsdl_visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class port_type_visitor : public idl_to_wsdl_visitor
{
  //
  // = TITLE
  //    port_type_visitor
  //
  // = DESCRIPTION
  //    Generates portType elements in the WSDL document.
  //
public:
  port_type_visitor (DOMElement *sub_tree);
  virtual ~port_type_visitor (void);

  virtual int visit_interface (AST_Interface *node);
  virtual int visit_component (AST_Component *node);
  virtual int visit_operation (AST_Operation *node);
  virtual int visit_attribute (AST_Attribute *node);
  
private:
  void finish_operation (AST_Decl *node,
                         DOMElement *elem,
                         const char *prefix);
  void gen_inherited_operations (AST_Interface *node);
  void gen_inherited_comp (AST_Component *node);
  void append_ops_and_attrs (AST_Interface *ancestor);
  void gen_fault (DOMElement *port_op, AST_Decl *user_exception);

private:
  DOMElement *current_port_type_;
  DOMElement *current_response_op_;
};

#endif /* PORT_TYPE_VISITOR_H */
