#include "ast_param_holder.h"
#include "ast_visitor.h"

#include "utl_identifier.h"

AST_Decl::NodeType const
AST_Param_Holder::NT = AST_Decl::NT_param_holder;

AST_Param_Holder::AST_Param_Holder (UTL_ScopedName *parameter_name,
                                    FE_Utils::T_Param_Info *info)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_param_holder,
              parameter_name),
    AST_Type (AST_Decl::NT_param_holder,
              parameter_name),
    info_ (info)
{
}

AST_Param_Holder::~AST_Param_Holder ()
{
}

FE_Utils::T_Param_Info const *
AST_Param_Holder::info () const
{
  return this->info_;
}

void
AST_Param_Holder::destroy ()
{
  this->AST_Type::destroy ();
}

int
AST_Param_Holder::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_param_holder (this);
}

void
AST_Param_Holder::dump (ACE_OSTREAM_TYPE & /* o */)
{
}
