#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Structure
 */
be_structure::be_structure()
{
}
be_structure::be_structure(UTL_ScopedName *n, UTL_StrList *p)
	    : AST_Decl(AST_Decl::NT_struct, n, p),
	      UTL_Scope(AST_Decl::NT_struct)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_structure, AST_Structure)
IMPL_NARROW_FROM_DECL(be_structure)
IMPL_NARROW_FROM_SCOPE(be_structure)

