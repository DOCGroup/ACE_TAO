#if !defined(BE_STRUCTURE_H)
#define BE_STRUCTURE_H

/*
 * BE_Structure
 */
class	be_structure : public virtual AST_Structure {
public:
  // Operations
  be_structure();
  be_structure(UTL_ScopedName *n, UTL_StrList *p);

  // Narrowing
  DEF_NARROW_METHODS1(be_structure, AST_Structure);
  DEF_NARROW_FROM_DECL(be_structure);
  DEF_NARROW_FROM_SCOPE(be_structure);
};

#endif
