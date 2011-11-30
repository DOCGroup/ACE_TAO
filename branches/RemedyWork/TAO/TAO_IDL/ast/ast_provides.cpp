// $Id$

#include "ast_provides.h"
#include "ast_interface.h"
#include "ast_visitor.h"

AST_Decl::NodeType const
AST_Provides::NT = AST_Decl::NT_provides;

AST_Provides::AST_Provides (UTL_ScopedName *n,
                            AST_Type *provides_type)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_provides,
              n),
    AST_Field (AST_Decl::NT_provides,
               provides_type,
               n)
{
}

AST_Provides::~AST_Provides (void)
{
}

AST_Type *
AST_Provides::provides_type (void) const
{
  return this->field_type ();
}

void
AST_Provides::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "provides ");
  this->AST_Field::dump (o);
}

int
AST_Provides::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_provides (this);
}

void
AST_Provides::destroy (void)
{
  this->AST_Field::destroy ();
}

IMPL_NARROW_FROM_DECL(AST_Provides)
