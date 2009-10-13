//
// $Id$
//

/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor_iface_inst.h
//
// = DESCRIPTION
//    This visitor creates the appropriate AST
//    (Abstract Syntax Tree) node corresponding to the instantiation
//    of a template interface, which may occur in an extended port
//    or mirrorport declaration.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef TAO_BE_VISITOR_IFACE_INST_H
#define TAO_BE_VISITOR_IFACE_INST_H

#include "be_visitor_scope.h"
#include "ast_component.h"
#include "ast_template_common.h"
#include "utl_identifier.h"

class be_exception;
class UTL_ExceptList;
class AST_Template_Interface;

class be_visitor_iface_inst : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_iface_inst
  //
  // = DESCRIPTION
  //   Adds instantiated template interface nodes to the AST.
  //
public:
  be_visitor_iface_inst (be_visitor_context *ctx,
                         be_interface *instance,
                         AST_Template_Common::T_ARGLIST *args);

  virtual ~be_visitor_iface_inst (void);
  
  virtual int visit_argument (be_argument *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_typedef (be_typedef *node);
  virtual int visit_exception (be_exception *node);

private:
  // Working node.
  be_interface *instance_;
  
  // Working arglist.
  AST_Template_Common::T_ARGLIST *template_args_;
};


#endif // TAO_BE_VISITOR_IFACE_INST_H
