#include "be_fixed.h"
#include "be_visitor.h"


be_fixed::be_fixed (UTL_ScopedName *name,
                    AST_Expression *digits,
                    AST_Expression *scale)
  : COMMON_Base (),
    AST_Decl (AST_Decl::NT_fixed, name, true),
    AST_Type (AST_Decl::NT_fixed,name),
    AST_ConcreteType (AST_Decl::NT_fixed, name),
    AST_Fixed (name, digits, scale),
    be_decl (AST_Decl::NT_fixed, name),
    be_type (AST_Decl::NT_fixed, name)
{}

int be_fixed::accept (be_visitor *visitor)
{
  return visitor->visit_fixed (this);
}

void be_fixed::destroy ()
{
  be_type::destroy ();
  AST_Fixed::destroy ();
}

IMPL_NARROW_FROM_DECL (be_fixed)
