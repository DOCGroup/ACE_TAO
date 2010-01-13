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
//    be_visitor_tmpl_ module_inst.h
//
// = DESCRIPTION
//    This visitor creates, in a separate pass, nodes resulting
//    from the instantiation of a template module, or from a
//    reference to one inside another, and adds these nodes
//    to the AST.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef TAO_BE_VISITOR_TMPL_MODULE_INST_H
#define TAO_BE_VISITOR_TMPL_MODULE_INST_H

#include "be_visitor_scope.h"

#include "fe_utils.h"

class be_template_module_inst;
class AST_Type;
class UTL_ExceptList;

class be_visitor_tmpl_module_inst : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_tmpl_module_inst
  //
  // = DESCRIPTION
  //   Adds implied IDL code to the AST related to the
  //   instantiation or alias of a template module.
  //
public:
  be_visitor_tmpl_module_inst (be_visitor_context *ctx);

  virtual ~be_visitor_tmpl_module_inst (void);

  virtual int visit_root (be_root *node);
  virtual int visit_module (be_module *node);
  virtual int visit_template_module (be_template_module *node);
  virtual int visit_template_module_inst (be_template_module_inst *node);
  virtual int visit_interface (be_interface *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_argument (be_argument *node);
  virtual int visit_typedef (be_typedef *node);
  virtual int visit_constant (be_constant *node);
  virtual int visit_structure (be_structure *node);
  virtual int visit_field (be_field *node);
  
private:
  // If arg is a param holder, return the matching template arg,
  // otherwise return arg unchanged. If the arg is declared in
  // the scope of a template module, the returned node will be a
  // copy created in the scope of the template module instantiation.
  AST_Decl *reify_type (AST_Decl *d);
  
  // Utility method used for attributes and operations.
  UTL_ExceptList *reify_exception_list (UTL_ExceptList *orig);
};

#endif // TAO_BE_VISITOR_TMPL_MODULE_INST_H

