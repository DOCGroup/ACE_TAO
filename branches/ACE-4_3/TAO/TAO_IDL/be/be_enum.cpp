#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Enum
 */
be_enum::be_enum()
{
}
be_enum::be_enum(UTL_ScopedName *n, UTL_StrList *p)
       : AST_Enum(n, p),
	 AST_Decl(AST_Decl::NT_enum, n, p),
	 UTL_Scope(AST_Decl::NT_enum)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_enum, AST_Enum)
IMPL_NARROW_FROM_DECL(be_enum)
IMPL_NARROW_FROM_SCOPE(be_enum)

