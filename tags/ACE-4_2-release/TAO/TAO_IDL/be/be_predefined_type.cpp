#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_PredefinedType
 */
be_predefined_type::be_predefined_type()
{
}
be_predefined_type::be_predefined_type(AST_PredefinedType::PredefinedType t,
				       UTL_ScopedName *n, UTL_StrList *p)
		  : AST_PredefinedType(t, n, p),
		    AST_Decl(AST_Decl::NT_pre_defined, n, p)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_predefined_type, AST_PredefinedType)
IMPL_NARROW_FROM_DECL(be_predefined_type)

