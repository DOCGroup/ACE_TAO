#if !defined(BE_UNION_BRANCH_H)
#define BE_UNION_BRANCH_H

/*
 * BE_UnionBranch
 */
class	be_union_branch : public virtual AST_UnionBranch {
public:
  // Operations
  be_union_branch();
  be_union_branch(AST_UnionLabel *lab, AST_Type *ft, UTL_ScopedName *n,
		  UTL_StrList *p);

  // Narrowing
  DEF_NARROW_METHODS1(be_union_branch, AST_UnionBranch);
  DEF_NARROW_FROM_DECL(be_union_branch);
};

#endif
