#if !defined(BE_TYPEDEF_H)
#define BE_TYPEDEF_H

/*
 * BE_Typedef
 */
class	be_typedef : public virtual AST_Typedef {
public:
  // Operations
  be_typedef();
  be_typedef(AST_Type *bt, UTL_ScopedName *n, UTL_StrList *p);

  // Narrowing
  DEF_NARROW_METHODS1(be_typedef, AST_Typedef);
  DEF_NARROW_FROM_DECL(be_typedef);
};

#endif
