// $Id$

#include "be_template_module_ref.h"
#include "be_visitor.h"

#include "ast_template_module.h"

be_template_module_ref::be_template_module_ref (
      UTL_ScopedName *n,
      AST_Template_Module *ref,
      UTL_StrList *param_refs)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_module,
              n),
    AST_Field (AST_Decl::NT_module,
               ref,
               n),
    AST_Template_Module_Ref (n,
                             ref,
                             param_refs),
    be_decl (AST_Decl::NT_module,
             n),
    be_field (ref,
              n)
{
}

be_template_module_ref::~be_template_module_ref (void)
{
}

void
be_template_module_ref::destroy (void)
{
  this->AST_Template_Module_Ref::destroy ();
  this->be_field::destroy ();
}

int
be_template_module_ref::accept (be_visitor *visitor)
{
  return visitor->visit_template_module_ref (this);
}

IMPL_NARROW_FROM_DECL (be_template_module_ref)

