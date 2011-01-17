// $Id$

#include "ast_publishes.h"
#include "ast_visitor.h"

AST_Decl::NodeType const
AST_Publishes::NT = AST_Decl::NT_publishes;

AST_Publishes::AST_Publishes (UTL_ScopedName *n,
                              AST_Type *publishes_type)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_publishes,
              n),
    AST_Field (AST_Decl::NT_publishes,
               publishes_type,
               n)
{
}

AST_Publishes::~AST_Publishes (void)
{
}

AST_Type *
AST_Publishes::publishes_type (void) const
{
  return this->field_type ();
}

void
AST_Publishes::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "publishes ");
  this->AST_Field::dump (o);
}

int
AST_Publishes::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_publishes (this);
}

void
AST_Publishes::destroy (void)
{
  this->AST_Field::destroy ();
}

IMPL_NARROW_FROM_DECL(AST_Publishes)
