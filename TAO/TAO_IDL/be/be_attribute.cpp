#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Attribute
 */
be_attribute::be_attribute()
{
}
be_attribute::be_attribute(idl_bool ro, AST_Type *ft, UTL_ScopedName *n,
			   UTL_StrList *p)
	    : AST_Attribute(ro, ft, n, p),
	      AST_Field(AST_Decl::NT_attr, ft, n, p),
	      AST_Decl(AST_Decl::NT_attr, n, p)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_attribute, AST_Attribute)
IMPL_NARROW_FROM_DECL(be_attribute)

