#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_String
 */
be_string::be_string()
{
}
be_string::be_string(AST_Expression *v)
	 : AST_String(v),
	   AST_Decl(AST_Decl::NT_string,
		    new UTL_ScopedName(new Identifier("string", 1, 0, I_FALSE),
				       NULL),
		    NULL)
{
}
be_string::be_string(AST_Expression *v, long wide)
	 : AST_String(v, wide),
	   AST_Decl(AST_Decl::NT_string,
		    wide == 1
		    ? new UTL_ScopedName(new Identifier("string",1,0,I_FALSE),
					 NULL)
		    : new UTL_ScopedName(new Identifier("wstring_t",
                                                        1,
                                                        0,
                                                        I_FALSE),
					 NULL),
		    NULL)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_string, AST_String)
IMPL_NARROW_FROM_DECL(be_string)

