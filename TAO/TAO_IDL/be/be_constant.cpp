#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Constant
 */
be_constant::be_constant()
{
}
be_constant::be_constant(AST_Expression::ExprType et,
			 AST_Expression *v,
			 UTL_ScopedName *n,
			 UTL_StrList *p)
	   : AST_Constant(et, v, n, p),
	     AST_Decl(AST_Decl::NT_const, n, p)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_constant, AST_Constant)
IMPL_NARROW_FROM_DECL(be_constant)

