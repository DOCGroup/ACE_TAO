#if !defined(BE_UNION_H)
#define BE_UNION_H

/*
 * BE_Union
 */
class	be_union : public virtual AST_Union {
public:
  // Operations
  be_union();
  be_union(AST_ConcreteType *dt, UTL_ScopedName *n, UTL_StrList *p);

  // Narrowing
  DEF_NARROW_METHODS1(be_union, AST_Union);
  DEF_NARROW_FROM_DECL(be_union);
  DEF_NARROW_FROM_SCOPE(be_union);
};

#endif
