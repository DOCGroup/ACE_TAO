#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_InterfaceFwd
 */
be_interface_fwd::be_interface_fwd()
{
}
be_interface_fwd::be_interface_fwd(UTL_ScopedName *n, UTL_StrList *p)
		: AST_InterfaceFwd(n, p),
		  AST_Decl(AST_Decl::NT_interface_fwd, n, p)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_interface_fwd, AST_InterfaceFwd)
IMPL_NARROW_FROM_DECL(be_interface_fwd)

