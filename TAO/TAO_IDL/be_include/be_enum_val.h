#if !defined(BE_ENUM_VAL_H)
#define BE_ENUM_VAL_H

/*
 * BE_EnumVal
 */
class	be_enum_val : public virtual AST_EnumVal {
public:
  // Operations
  be_enum_val();
  be_enum_val(unsigned long v, UTL_ScopedName *n, UTL_StrList *p);

  // Narrowing
  DEF_NARROW_METHODS1(be_enum_val, AST_EnumVal);
  DEF_NARROW_FROM_DECL(be_enum_val);
};

#endif
