#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Union
 */
be_union::be_union()
{
}
be_union::be_union(AST_ConcreteType *dt, UTL_ScopedName *n, UTL_StrList *p)
	: AST_Union(dt, n, p),
	  AST_Decl(AST_Decl::NT_union, n, p),
          AST_Structure(AST_Decl::NT_union, n, p),
	  UTL_Scope(AST_Decl::NT_union)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_union, AST_Union)
IMPL_NARROW_FROM_DECL(be_union)
IMPL_NARROW_FROM_SCOPE(be_union)

