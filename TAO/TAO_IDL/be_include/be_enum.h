#if !defined(BE_ENUM_H)
#define BE_ENUM_H

/*
 * BE_Enum
 */
class	be_enum : public virtual AST_Enum {
public:
  // Operations
  be_enum();
  be_enum(UTL_ScopedName *n, UTL_StrList *p);

  // Narrowing
  DEF_NARROW_METHODS1(be_enum, AST_Enum);
  DEF_NARROW_FROM_DECL(be_enum);
  DEF_NARROW_FROM_SCOPE(be_enum);
};

#endif
