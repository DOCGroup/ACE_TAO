#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Sequence
 */
be_sequence::be_sequence()
{
}
be_sequence::be_sequence(AST_Expression *v, AST_Type *t)
	   : AST_Sequence(v, t),
    	     AST_Decl(AST_Decl::NT_sequence,
		      new UTL_ScopedName(
				new Identifier("sequence", 1, 0, I_FALSE),
				NULL),
		      NULL)
{
}

// Narrowing
IMPL_NARROW_METHODS1(be_sequence, AST_Sequence)
IMPL_NARROW_FROM_DECL(be_sequence)

