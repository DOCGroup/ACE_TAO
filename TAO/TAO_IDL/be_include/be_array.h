#if !defined(BE_ARRAY_H)
#define BE_ARRAY_H

/*
 * BE_Array
 */
class	be_array : public virtual AST_Array {
public:
  // Operations
  be_array();
  be_array(UTL_ScopedName *n, unsigned long ndims, UTL_ExprList *dims);

  // Narrowing
  DEF_NARROW_METHODS1(be_array, AST_Array);
  DEF_NARROW_FROM_DECL(be_array);
};

#endif
