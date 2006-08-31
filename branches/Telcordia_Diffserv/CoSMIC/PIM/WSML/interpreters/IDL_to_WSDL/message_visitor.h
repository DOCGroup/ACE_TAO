// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    IDL_TO_WSDL_BE_DLL
//
// = FILENAME
//    message_visitor.h
//
// = DESCRIPTION
//    Generates message elements in the WSDL document.
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef MESSAGE_VISITOR_H
#define MESSAGE_VISITOR_H

#include "idl_to_wsdl_visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class message_visitor : public idl_to_wsdl_visitor
{
  //
  // = TITLE
  //    message_visitor
  //
  // = DESCRIPTION
  //    Generates message elements in the WSDL document.
  //
public:
  message_visitor (DOMElement *sub_tree);
  virtual ~message_visitor (void);

  virtual int visit_interface (AST_Interface *node);
  virtual int visit_component (AST_Component *node);
  virtual int visit_exception (AST_Exception *node);
  virtual int visit_operation (AST_Operation *node);
  virtual int visit_argument (AST_Argument *node);
  virtual int visit_attribute (AST_Attribute *node);
  
private:
  void gen_messages (AST_Decl *node, const char *prefix);

private:  
  DOMElement *current_msg_;
  DOMElement *current_response_msg_;
};

#endif /* PORT_TYPE_VISITOR_H */
