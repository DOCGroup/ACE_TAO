#if !defined(BE_CONSTANT_H)
#define BE_CONSTANT_H

/*
 * BE_Constant
 */
class	be_constant : public virtual AST_Constant {
public:
  // Operations
  be_constant();
  be_constant(AST_Expression::ExprType et,
	      AST_Expression *v,
	      UTL_ScopedName *n,
	      UTL_StrList *p);

  // Narrowing
  DEF_NARROW_METHODS1(be_constant, AST_Constant);
  DEF_NARROW_FROM_DECL(be_constant);
};

#endif
