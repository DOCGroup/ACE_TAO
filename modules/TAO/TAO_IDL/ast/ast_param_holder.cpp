// $Id$

#include "ast_param_holder.h"

#include "utl_identifier.h"

AST_Param_Holder::AST_Param_Holder (UTL_ScopedName *parameter_name)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_param_holder,
              parameter_name),
    AST_Type (AST_Decl::NT_param_holder,
              parameter_name)
{
}

AST_Param_Holder::~AST_Param_Holder (void)
{
}

void
AST_Param_Holder::destroy (void)
{
  this->AST_Type::destroy ();
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

