#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Argument
 */
be_argument::be_argument()
{
}
be_argument::be_argument(AST_Argument::Direction d, AST_Type *ft,
			 UTL_ScopedName *n, UTL_StrList *p)
	   : AST_Argument(d, ft, n, p),
	     AST_Field(AST_Decl::NT_argument, ft, n, p),
	     AST_Decl(AST_Decl::NT_argument, n, p)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_argument, AST_Argument)
IMPL_NARROW_FROM_DECL(be_argument)

