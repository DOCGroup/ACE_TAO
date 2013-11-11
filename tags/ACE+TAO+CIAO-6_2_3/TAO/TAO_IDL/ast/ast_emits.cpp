// $Id$

#include "ast_emits.h"
#include "ast_visitor.h"

AST_Decl::NodeType const
AST_Emits::NT = AST_Decl::NT_emits;

AST_Emits::AST_Emits (UTL_ScopedName *n,
                      AST_Type *emits_type)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_emits,
              n),
    AST_Field (AST_Decl::NT_emits,
               emits_type,
               n)
{
}

AST_Emits::~AST_Emits (void)
{
}

AST_Type *
AST_Emits::emits_type (void) const
{
  return this->field_type ();
}

void
AST_Emits::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "emits ");
  this->AST_Field::dump (o);
}

int
AST_Emits::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_emits (this);
}

void
AST_Emits::destroy (void)
{
  this->AST_Field::destroy ();
}

IMPL_NARROW_FROM_DECL(AST_Emits)
