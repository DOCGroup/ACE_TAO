#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

be_typedef::be_typedef()
{
}
be_typedef::be_typedef(AST_Type *bt, UTL_ScopedName *n, UTL_StrList *p)
	  : AST_Typedef(bt, n, p),
	    AST_Decl(AST_Decl::NT_typedef, n, p)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_typedef, AST_Typedef)
IMPL_NARROW_FROM_DECL(be_typedef)

