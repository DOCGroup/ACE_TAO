#include "ast_fixed.h"

#include "ast_visitor.h"

AST_Fixed::AST_Fixed (UTL_ScopedName *name,
                      AST_Expression *digits,
                      AST_Expression *scale)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_fixed, name, true),
    AST_Type (AST_Decl::NT_fixed, name),
    AST_ConcreteType (AST_Decl::NT_fixed, name),
    pd_digits (digits),
    pd_scale (scale)
{
}

AST_Fixed::~AST_Fixed ()
{
}

void AST_Fixed::dump (ACE_OSTREAM_TYPE &os)
{
  dump_i (os, "fixed<");
  pd_digits->dump (os);
  dump_i (os, ", ");
  pd_scale->dump (os);
  dump_i (os, ">");
}

void AST_Fixed::destroy ()
{
  pd_digits->destroy ();
  delete pd_digits;
  pd_digits = 0;

  pd_scale->destroy ();
  delete pd_scale;
  pd_scale = 0;

  AST_ConcreteType::destroy ();
}

int AST_Fixed::ast_accept (ast_visitor *visitor)
{
  return visitor->visit_fixed (this);
}

AST_Expression *AST_Fixed::digits ()
{
  return pd_digits;
}

AST_Expression *AST_Fixed::scale ()
{
  return pd_scale;
}

IMPL_NARROW_FROM_DECL(AST_Fixed)
