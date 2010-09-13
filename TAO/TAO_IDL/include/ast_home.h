// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef _AST_HOME_AST_HOME_HH
#define _AST_HOME_AST_HOME_HH

#include "ast_interface.h"

class AST_Home;
class AST_Component;
class AST_ValueType;

class TAO_IDL_FE_Export AST_Home : public virtual AST_Interface
{
public:
  AST_Home (UTL_ScopedName *n,
            AST_Home *base_home,
            AST_Component *managed_component,
            AST_Type *primary_key,
            AST_Type **supports,
            long n_supports,
            AST_Interface **supports_flat,
            long n_supports_flat);

  virtual ~AST_Home (void);

  // Extend lookup to the base home.
  virtual AST_Decl *look_in_inherited (UTL_ScopedName *e,
                                       bool full_def_only);

  // Extend lookup to the supported interfaces.
  virtual AST_Decl *look_in_supported (UTL_ScopedName *e,
                                       bool full_def_only);

  // Overridden for homes from the UTL_Scope method.
  virtual AST_Decl *special_lookup (UTL_ScopedName *,
                                    bool full_def_only);

  // Accessors.

  AST_Home *base_home (void) const;

  AST_Type **supports (void) const;

  long n_supports (void) const;

  AST_Component *managed_component (void) const;

  AST_Type *primary_key (void) const;

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL(AST_Home);
  DEF_NARROW_FROM_SCOPE(AST_Home);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  static AST_Decl::NodeType const NT;

private:
  // Scope Management Protocol.

  friend int tao_yyparse (void);
  friend class ast_visitor_tmpl_module_inst;

  virtual AST_Factory *fe_add_factory (AST_Factory *f);
  virtual AST_Finder *fe_add_finder (AST_Finder *f);

private:
  AST_Home *pd_base_home;
  AST_Component *pd_managed_component;
  AST_Type *pd_primary_key;
  bool owns_primary_key_;
};

#endif // _AST_HOME_AST_HOME_HH
