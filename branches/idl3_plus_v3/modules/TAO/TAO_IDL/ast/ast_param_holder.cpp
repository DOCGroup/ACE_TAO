// $Id$

#include "ast_param_holder.h"

#include "utl_identifier.h"

AST_Param_Holder::AST_Param_Holder (Identifier *parameter_name)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_param_holder,
              0),
    param_name_ (parameter_name)
{
}

AST_Param_Holder::~AST_Param_Holder (void)
{
}

Identifier const * const
AST_Param_Holder::param_name (void) const
{
  return this->param_name_;
}

void
AST_Param_Holder::destroy (void)
{
  this->param_name_->destroy ();
  delete this->param_name_;
  this->param_name_ = 0;
  
  this->AST_Decl::destroy ();
}

int
AST_Param_Holder::ast_accept (ast_visitor *)
{
  // This node isn't part of the AST, but managed by
  // its reference holder.
  return 0;
}

void
AST_Param_Holder::dump (ACE_OSTREAM_TYPE & /* o */)
{
}

IMPL_NARROW_FROM_DECL (AST_Param_Holder)

