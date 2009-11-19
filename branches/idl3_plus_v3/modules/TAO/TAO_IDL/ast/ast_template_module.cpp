// $Id$

#include "ast_template_module.h"
#include "ast_visitor.h"

AST_Template_Module::AST_Template_Module (
      UTL_ScopedName *n,
      FE_Utils::T_PARAMLIST_INFO *template_params)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_module,
              n),
    UTL_Scope (AST_Decl::NT_module),
    AST_Module (n),
    template_params_ (template_params)
{
}

AST_Template_Module::~AST_Template_Module (void)
{
}

FE_Utils::T_PARAMLIST_INFO const * const
AST_Template_Module::template_params (void) const
{
  return this->template_params_;
}

void
AST_Template_Module::destroy (void)
{
  delete this->template_params_;
  this->template_params_ = 0;
  
  this->AST_Module::destroy ();
}

int
AST_Template_Module::ast_accept (ast_visitor *visitor)
{
  return 0;//visitor->visit_template_module (this);
}

void
AST_Template_Module::dump (ACE_OSTREAM_TYPE & /* o */)
{
}

IMPL_NARROW_FROM_DECL (AST_Template_Module)
IMPL_NARROW_FROM_SCOPE (AST_Template_Module)

