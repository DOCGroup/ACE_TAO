#if !defined(BE_ATTRIBUTE_H)
#define BE_ATTRIBUTE_H

/*
 * BE_Attribute
 */
class	be_attribute : public virtual AST_Attribute {
public:
  // Operations
  be_attribute();
  be_attribute(idl_bool ro, AST_Type *ft, UTL_ScopedName *n, UTL_StrList *p);

  // Narrowing
  DEF_NARROW_METHODS1(be_attribute, AST_Attribute);
  DEF_NARROW_FROM_DECL(be_attribute);
};

#endif
