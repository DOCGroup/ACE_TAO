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

  ~AST_Home () override;

  // Extend lookup to the base home.
  AST_Decl *look_in_inherited (UTL_ScopedName *e,
                                       bool full_def_only) override;

  // Extend lookup to the supported interfaces.
  AST_Decl *look_in_supported (UTL_ScopedName *e,
                                       bool full_def_only) override;

  // Overridden for homes from the UTL_Scope method.
  AST_Decl *special_lookup (UTL_ScopedName *,
                                    bool full_def_only,
                                    AST_Decl *&final_parent_decl) override;

  // Accessors.

  AST_Home *base_home () const;

  AST_Type **supports () const;

  long n_supports () const;

  AST_Component *managed_component () const;

  AST_Type *primary_key () const;

  void transfer_scope_elements (AST_Interface *dst);

  // Cleanup function.
  void destroy () override;

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &o) override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  static AST_Decl::NodeType const NT;

private:
  // Scope Management Protocol.

  friend int tao_yyparse ();
  friend class ast_visitor_tmpl_module_inst;

  AST_Factory *fe_add_factory (AST_Factory *f) override;
  AST_Finder *fe_add_finder (AST_Finder *f) override;

private:
  AST_Home *pd_base_home;
  AST_Component *pd_managed_component;
  AST_Type *pd_primary_key;
  bool owns_primary_key_;
};

#endif // _AST_HOME_AST_HOME_HH
