#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_UnionBranch
 */
be_union_branch::be_union_branch()
{
}
be_union_branch::be_union_branch(AST_UnionLabel *lab, AST_Type *ft,
				 UTL_ScopedName *n, UTL_StrList *p)
	       : AST_UnionBranch(lab, ft, n, p),
		 AST_Field(AST_Decl::NT_union_branch, ft, n, p),
		 AST_Decl(AST_Decl::NT_union_branch, n, p)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_union_branch, AST_UnionBranch)
IMPL_NARROW_FROM_DECL(be_union_branch)

