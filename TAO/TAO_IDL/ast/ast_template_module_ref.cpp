// $Id$

#include "ast_template_module_ref.h"
#include "ast_template_module.h"
#include "ast_visitor.h"

#include "utl_strlist.h"

AST_Decl::NodeType const
AST_Template_Module_Ref::NT = AST_Decl::NT_module;

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
    param_refs_ (param_refs),
    processed_ (false)
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
AST_Template_Module_Ref::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_template_module_ref (this);
}

// If IDL module has been created in a scope corresponding
// to this node. That's the one we want to match, NOT this
// AST_Template_Module_Ref, and it occurs in the scope right
// after this node, so we'll match what we're looking for on
// the next iteration of the search. So for this immediate
// adjustment we return no match.
AST_Decl *
AST_Template_Module_Ref::adjust_found (
  bool ignore_fwd,
  bool /*full_def_only*/)
{
  return ignore_fwd ? 0 : this;
}

bool
AST_Template_Module_Ref::processed (void) const
{
  return this->processed_;
}

void
AST_Template_Module_Ref::processed (bool val)
{
  this->processed_ = val;
}

IMPL_NARROW_FROM_DECL (AST_Template_Module_Ref)

