#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Operation
 */
be_operation::be_operation()
{
}
be_operation::be_operation(AST_Type *rt, AST_Operation::Flags fl,
			   UTL_ScopedName *n, UTL_StrList *p)
	    : AST_Operation(rt, fl, n, p),
	      AST_Decl(AST_Decl::NT_op, n, p),
	      UTL_Scope(AST_Decl::NT_op)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_operation, AST_Operation)
IMPL_NARROW_FROM_DECL(be_operation)
IMPL_NARROW_FROM_SCOPE(be_operation)

