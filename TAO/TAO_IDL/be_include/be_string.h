#if !defined(BE_STRING_H)
#define BE_STRING_H

/*
 * BE_String
 */
class	be_string : public virtual AST_String {
public:
  // Operations
  be_string();
  be_string(AST_Expression *v);
  be_string(AST_Expression *v, long wide);

  // Narrowing
  DEF_NARROW_METHODS1(be_string, AST_String);
  DEF_NARROW_FROM_DECL(be_string);
};

#endif
