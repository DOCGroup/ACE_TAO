#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Exception
 */
be_exception::be_exception()
{
}
be_exception::be_exception(UTL_ScopedName *n, UTL_StrList *p)
	    : AST_Decl(AST_Decl::NT_except, n, p),
              AST_Structure(AST_Decl::NT_except, n, p),
	      UTL_Scope(AST_Decl::NT_except)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_exception, AST_Exception)
IMPL_NARROW_FROM_DECL(be_exception)
IMPL_NARROW_FROM_SCOPE(be_exception)

