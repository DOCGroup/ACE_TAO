#if !defined(BE_EXCEPTION_H)
#define BE_EXCEPTION_H

/*
 * BE_Exception
 */
class	be_exception : public virtual AST_Exception {
public:
  // Operations
  be_exception();
  be_exception(UTL_ScopedName *n, UTL_StrList *p);

  // Narrowing
  DEF_NARROW_METHODS1(be_exception, AST_Exception);
  DEF_NARROW_FROM_DECL(be_exception);
  DEF_NARROW_FROM_SCOPE(be_exception);
};

#endif
