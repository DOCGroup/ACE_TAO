// $Id$

#include "ast_uses.h"
#include "ast_interface.h"
#include "ast_visitor.h"

AST_Uses::AST_Uses (UTL_ScopedName *n,
                    AST_Interface *uses_type,
                    bool is_multiple)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_uses,
              n),
    AST_Field (AST_Decl::NT_uses,
               uses_type,
               n),
    is_multiple_ (is_multiple)
{
}

AST_Uses::~AST_Uses (void)
{
}

AST_Interface *
AST_Uses::uses_type (void) const
{
  return AST_Interface::narrow_from_decl (this->field_type ());
}

bool
AST_Uses::is_multiple (void) const
{
  return this->is_multiple_;
}

void
AST_Uses::dump (ACE_OSTREAM_TYPE &o)
{
  this->dump_i (o, "uses ");
  this->AST_Field::dump (o);
}

int
AST_Uses::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_uses (this);
}

void
AST_Uses::destroy (void)
{
  this->AST_Field::destroy ();
}

IMPL_NARROW_FROM_DECL(AST_Uses)
