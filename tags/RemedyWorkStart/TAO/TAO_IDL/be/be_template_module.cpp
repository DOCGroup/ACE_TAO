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
    AST_Type (AST_Decl::NT_module,
              n),
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
be_template_module::accept (be_visitor *)
{
  // Disables visitation when this node is encountered as part
  // of a normal scope traversal. We will call
  // visit_template_module() directly when the template module
  // is instantiated, and match template args with template params
  // to generate implied IDL.
  return 0;
}

IMPL_NARROW_FROM_DECL (be_template_module)
IMPL_NARROW_FROM_SCOPE (be_template_module)
