#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

/*
 * BE_Expression
 */
be_expr_val::be_expr_val(UTL_ScopedName *n) : AST_Expression(n) {}
be_expr_val::be_expr_val(AST_Expression *b, AST_Expression::ExprType t)
	   : AST_Expression(b, t) 
{
}
be_expr_val::be_expr_val(AST_Expression::ExprComb c,
			 AST_Expression *v1,
			 AST_Expression *v2)
	   : AST_Expression(c, v1, v2)
{
}
be_expr_val::be_expr_val(long l) : AST_Expression(l) {}
be_expr_val::be_expr_val(long l, AST_Expression::ExprType t)
	   : AST_Expression(l, t)
{
}
be_expr_val::be_expr_val(unsigned long l) : AST_Expression(l) {}
be_expr_val::be_expr_val(String *s) : AST_Expression(s) {}
be_expr_val::be_expr_val(char c) : AST_Expression(c) {}
be_expr_val::be_expr_val(double d) : AST_Expression(d) {}


