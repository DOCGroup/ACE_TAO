#if !defined(BE_FIELD_H)
#define BE_FIELD_H

/*
 * BE_Field
 */
class	be_field : public virtual AST_Field {
public:
  // Operations
  be_field();
  be_field(AST_Type *ft, UTL_ScopedName *n, UTL_StrList *p);

  // Narrowing
  DEF_NARROW_METHODS1(be_field, AST_Field);
  DEF_NARROW_FROM_DECL(be_field);
};

#endif
