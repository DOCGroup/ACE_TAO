#if !defined(BE_INTERFACE_FWD_H)
#define BE_INTERFACE_FWD_H

/*
 * BE_InterfaceFwd
 */
class	be_interface_fwd : public virtual AST_InterfaceFwd {
public:
  // Operations
  be_interface_fwd();
  be_interface_fwd(UTL_ScopedName *n, UTL_StrList *p);

  // Narrowing
  DEF_NARROW_METHODS1(be_interface_fwd, AST_InterfaceFwd);
  DEF_NARROW_FROM_DECL(be_interface_fwd);
};

#endif
