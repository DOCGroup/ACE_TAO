#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Field
 */
be_field::be_field()
{
}
be_field::be_field(AST_Type *ft, UTL_ScopedName *n, UTL_StrList *p)
	: AST_Field(ft, n, p),
	  AST_Decl(AST_Decl::NT_field, n, p)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_field, AST_Field)
IMPL_NARROW_FROM_DECL(be_field)

