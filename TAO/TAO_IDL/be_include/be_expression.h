#if !defined(BE_EXPRESSION_H)
#define BE_EXPRESSION_H

/*
 * BE_Expression
 */
class	be_expr_val : public virtual AST_Expression {
public:
  // Operations
  be_expr_val(UTL_ScopedName *n);
  be_expr_val(AST_Expression *b, AST_Expression::ExprType t);
  be_expr_val(AST_Expression::ExprComb c,
	      AST_Expression *v1,
	      AST_Expression *v2);
  be_expr_val(long l);
  be_expr_val(long l, AST_Expression::ExprType t);
  be_expr_val(unsigned long l);
  be_expr_val(String *s);
  be_expr_val(char c);
  be_expr_val(double d);
};

#endif
