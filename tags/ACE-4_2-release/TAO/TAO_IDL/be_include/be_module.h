#if !defined(_BE_MODULE_H)
#define _BE_MODULE_H

/*
 * BE_Module
 */

class	be_module : public virtual AST_Module {
public:
  // Operations
  be_module();
  be_module(UTL_ScopedName *n, UTL_StrList *p);

  // Narrowing
  DEF_NARROW_METHODS1(be_module, AST_Module);
  DEF_NARROW_FROM_DECL(be_module);
  DEF_NARROW_FROM_SCOPE(be_module);
};

#endif
