// $Id$

#include "ast_consumes.h"
#include "ast_visitor.h"

AST_Decl::NodeType const
AST_Consumes::NT = AST_Decl::NT_consumes;

AST_Consumes::AST_Consumes (UTL_ScopedName *n,
                            AST_Type *consumes_type)
  : COMMON_Base (false,
                 false),
    AST_Decl (AST_Decl::NT_consumes,
              n),
    AST_Field (AST_Decl::NT_consumes,
               consumes_type,
               n)
{
}

AST_Consumes::~AST_Consumes (void)
{
}

AST_Type *
AST_Consumes::consumes_type (void) const
{
  return this->field_type ();
}

void
AST_Consumes::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "consumes ");
  this->AST_Field::dump (o);
}

int
AST_Consumes::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_consumes (this);
}

void
AST_Consumes::destroy (void)
{
  this->AST_Field::destroy ();
}

IMPL_NARROW_FROM_DECL(AST_Consumes)
