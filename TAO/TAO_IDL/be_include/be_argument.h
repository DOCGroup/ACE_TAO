#if !defined(BE_ARGUMENT_H)
#define BE_ARGUMENT_H

/*
 * BE_Argument
 */
class	be_argument : public virtual AST_Argument {
public:
  // Operations
  be_argument();
  be_argument(AST_Argument::Direction d, AST_Type *ft, UTL_ScopedName *n,
	      UTL_StrList *p);

  // Narrowing
  DEF_NARROW_METHODS1(be_argument, AST_Argument);
  DEF_NARROW_FROM_DECL(be_argument);
};

#endif
