#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

be_enum_val::be_enum_val()
{
}
be_enum_val::be_enum_val(unsigned long v, UTL_ScopedName *n, UTL_StrList *p)
	   : AST_Constant(AST_Expression::EV_ulong,
			  AST_Decl::NT_enum_val,
			  new AST_Expression(v),
			  n,
			  p),
	     AST_Decl(AST_Decl::NT_enum_val, n, p)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_enum_val, AST_EnumVal)
IMPL_NARROW_FROM_DECL(be_enum_val)

