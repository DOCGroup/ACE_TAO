#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Module
 */
be_module::be_module()
{
}
be_module::be_module(UTL_ScopedName *n, UTL_StrList *p)
	 : AST_Decl(AST_Decl::NT_module, n, p),
	   UTL_Scope(AST_Decl::NT_module)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_module, AST_Module)
IMPL_NARROW_FROM_DECL(be_module)
IMPL_NARROW_FROM_SCOPE(be_module)

