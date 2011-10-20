
/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    ast_visitor_tmpl_module_ref.h
 *
 *  $Id$
 *
 *  This visitor creates nodes resulting from the reference
 *  to one inside another, and adds these nodes to the AST.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef TAO_AST_VISITOR_TMPL_MODULE_REF_H
#define TAO_AST_VISITOR_TMPL_MODULE_REF_H

#include "ast_visitor_tmpl_module_inst.h"

/**
 * Adds implied IDL code to the AST related to the
 * alias of a template module. This class reuses all
 * the methods from the base class except the no-op
 * visit_template_module_ref(), which it overrides.
 */
class ast_visitor_tmpl_module_ref
  : public ast_visitor_tmpl_module_inst
{
public:
  ast_visitor_tmpl_module_ref (ast_visitor_context *ctx);

  virtual ~ast_visitor_tmpl_module_ref (void);

  virtual int visit_template_module_ref (AST_Template_Module_Ref *node);
};

#endif // TAO_AST_VISITOR_TMPL_MODULE_INST_H

