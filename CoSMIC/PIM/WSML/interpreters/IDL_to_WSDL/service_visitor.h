// $Id$

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    IDL_TO_WSDL_BE_DLL
//
// = FILENAME
//    service_visitor.h
//
// = DESCRIPTION
//    Generates service elements in the WSDL document
//
// = AUTHOR
//    Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

#ifndef SERVICE_VISITOR_H
#define SERVICE_VISITOR_H

#include "idl_to_wsdl_visitor.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ast_component.h"

class service_visitor : public idl_to_wsdl_visitor
{
  //
  // = TITLE
  //    service_visitor.
  //
  // = DESCRIPTION
  //    Generates service elements in the WSDL document
  //
public:
  service_visitor (DOMElement *sub_tree);
  virtual ~service_visitor (void);
  
  virtual int visit_component (AST_Component *node);
  
private:
  void gen_port (AST_Component *node, const char *comp_port_name);
  void map_ports (AST_Component *node,
                  ACE_Unbounded_Queue<AST_Component::port_description> &list);
  
private:
  DOMElement *service_elem_;
};

#endif /* SERVICE_VISITOR_H */

