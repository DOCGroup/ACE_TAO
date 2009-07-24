// $Id$

#include "ast_template_common.h"

ACE_RCSID (ast,
           ast_template_common,
           "$Id$")

AST_Template_Common::AST_Template_Common (
      FE_Utils::T_PARAMLIST_INFO *template_params)
  : template_params_ (template_params)
{
}

AST_Template_Common::~AST_Template_Common (void)
{
}

FE_Utils::T_PARAMLIST_INFO *
AST_Template_Common::template_params (void)
{
  return this->template_params_;
}

void
AST_Template_Common::destroy (void)
{
  delete this->template_params_;
  this->template_params_ = 0;
}

