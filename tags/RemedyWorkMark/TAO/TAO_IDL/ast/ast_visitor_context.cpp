
//=============================================================================
/**
 *  @file    ast_visitor_context.cpp
 *
 *  $Id$
 *
 *   Maintains the context information for visitors
 *
 *
 *  @author Jeff Parsons & Aniruddha Gokhale
 */
//=============================================================================


#include "ast_visitor_context.h"

ast_visitor_context::ast_visitor_context (void)
  : template_params_ (0),
    template_args_ (0)
{
}

ast_visitor_context::ast_visitor_context (const ast_visitor_context &ctx)
  : template_params_ (ctx.template_params_),
    template_args_ (ctx.template_args_)
{
}

ast_visitor_context::~ast_visitor_context (void)
{
}

FE_Utils::T_PARAMLIST_INFO *
ast_visitor_context::template_params (void) const
{
  return this->template_params_;
}

void
ast_visitor_context::template_params (FE_Utils::T_PARAMLIST_INFO *params)
{
  this->template_params_ = params;
}

FE_Utils::T_ARGLIST const *
ast_visitor_context::template_args (void) const
{
  return this->template_args_;
}

void
ast_visitor_context::template_args (FE_Utils::T_ARGLIST const *args)
{
  this->template_args_ = args;
}

