
/* -*- C++ -*- */
//=============================================================================
/**
 *  @file    ast_visitor_context.h
 *
 *  $Id$
 *
 *  This class serves to provide the necessary context information to the
 *  visitor as it generates code. This may include the scope inside which the
 *  code is generated, the output file that is being used, and others. This
 *  context information will be passed to the abstract/concrete visitor
 *  factory which in turn will use it to construct the right visitor.
 *
 *
 *  @author Jeff Parsons & Aniruddha Gokhale
 */
//=============================================================================


#ifndef AST_VISITOR_CONTEXT_H
#define AST_VISITOR_CONTEXT_H

#include "fe_utils.h"

class TAO_IDL_FE_Export ast_visitor_context
{
public:
  ast_visitor_context (void);

  ast_visitor_context (const ast_visitor_context &ctx);

  ~ast_visitor_context (void);

  // Accessors/mutators for template-related storage.

  FE_Utils::T_PARAMLIST_INFO *template_params (void) const;
  void template_params (FE_Utils::T_PARAMLIST_INFO *args);

  FE_Utils::T_ARGLIST *template_args (void) const;
  void template_args (FE_Utils::T_ARGLIST *args);

protected:
  // Storage for the current template module params and args.
  FE_Utils::T_PARAMLIST_INFO *template_params_;
  FE_Utils::T_ARGLIST *template_args_;
};

#endif /* AST_VISITOR_CONTEXT_H */
