// $Id$

#include "be_template_module_inst.h"
#include "be_visitor.h"

#include "ast_template_module.h"

be_template_module_inst::be_template_module_inst (
      UTL_ScopedName *n,
      AST_Template_Module *ref,
      FE_Utils::T_ARGLIST *template_args)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_module,
              n),
    AST_Field (AST_Decl::NT_module,
               ref,
               n),
    AST_Template_Module_Inst (n,
                              ref,
                              template_args),
    be_decl (AST_Decl::NT_module,
             n),
    be_field (ref,
              n)
{
}

be_template_module_inst::~be_template_module_inst (void)
{
}

void
be_template_module_inst::destroy (void)
{
  this->AST_Template_Module_Inst::destroy ();
}

int
be_template_module_inst::accept (be_visitor *visitor)
{
  return visitor->visit_template_module_inst (this);
}

IMPL_NARROW_FROM_DECL (be_template_module_inst)

