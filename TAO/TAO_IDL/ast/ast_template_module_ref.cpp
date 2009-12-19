// $Id$

#include "ast_template_module_ref.h"
#include "ast_template_module.h"

#include "utl_strlist.h"

AST_Template_Module_Ref::AST_Template_Module_Ref (
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
    param_refs_ (param_refs)
{
}

AST_Template_Module_Ref::~AST_Template_Module_Ref (void)
{
}

AST_Template_Module *
AST_Template_Module_Ref::ref (void) const
{
  return
    AST_Template_Module::narrow_from_decl (this->field_type ());
}

UTL_StrList const *
AST_Template_Module_Ref::param_refs (void) const
{
  return this->param_refs_;
}

void
AST_Template_Module_Ref::dump (ACE_OSTREAM_TYPE &)
{
  // TODO
}

void
AST_Template_Module_Ref::destroy (void)
{
  this->param_refs_->destroy ();
  delete this->param_refs_;
  this->param_refs_ = 0;

  this->AST_Field::destroy ();
}

int
AST_Template_Module_Ref::ast_accept (ast_visitor *)
{
  return 0;//visitor->visit_template_module_ref (this);
}

IMPL_NARROW_FROM_DECL (AST_Template_Module_Ref)

