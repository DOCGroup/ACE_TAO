#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_UnionLabel
 */
be_union_label::be_union_label()
{
}
be_union_label::be_union_label(AST_UnionLabel::UnionLabel ul,
			       AST_Expression *v)
	      : AST_UnionLabel(ul, v)
{
}

