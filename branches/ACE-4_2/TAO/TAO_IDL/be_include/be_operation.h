#if !defined(BE_OPERATION_H)
#define BE_OPERATION_H

/*
 * BE_Operation
 */
class	be_operation : public virtual AST_Operation {
public:
  // Operations
  be_operation();
  be_operation(AST_Type *rt, AST_Operation::Flags fl, UTL_ScopedName *n,
	       UTL_StrList *p);

  // Narrowing
  DEF_NARROW_METHODS1(be_operation, AST_Operation);
  DEF_NARROW_FROM_DECL(be_operation);
  DEF_NARROW_FROM_SCOPE(be_operation);
};

#endif
