// $Id$

#include "be_template_module.h"
#include "be_visitor.h"

be_template_module::be_template_module (
      UTL_ScopedName *n,
      FE_Utils::T_PARAMLIST_INFO *template_params)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_module,
              n),
    UTL_Scope (AST_Decl::NT_module),
    AST_Module (n),
    AST_Template_Module (n,
                         template_params),
    be_scope (AST_Decl::NT_module),
    be_decl (AST_Decl::NT_module,
             n)
{
}

be_template_module::~be_template_module (void)
{
}

void
be_template_module::destroy (void)
{
  this->AST_Template_Module::destroy ();
  this->be_scope::destroy ();
  this->be_decl::destroy ();
}

int
be_template_module::accept(be_visitor *visitor)
{
  return 0;//visitor->visit_template_module (this);
}

IMPL_NARROW_FROM_DECL (be_template_module)
IMPL_NARROW_FROM_SCOPE (be_template_module)
