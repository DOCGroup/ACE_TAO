#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Array
 */
be_array::be_array()
{
}
be_array::be_array(UTL_ScopedName *n, unsigned long ndims, UTL_ExprList *dims)
	: AST_Array(n, ndims, dims),
	  AST_Decl(AST_Decl::NT_array, n, NULL)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_array, AST_Array)
IMPL_NARROW_FROM_DECL(be_array)

