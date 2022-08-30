#ifndef _AST_INTERFACE_AST_VALUETYPE_HH
#define _AST_INTERFACE_AST_VALUETYPE_HH

#include "ast_interface.h"

class TAO_IDL_FE_Export AST_ValueType
  : public virtual AST_Interface
{
public:
  AST_ValueType (UTL_ScopedName *n,
                 AST_Type **inherits,
                 long n_inherits,
                 AST_Type *inherits_concrete,
                 AST_Interface **inherits_flat,
                 long n_inherits_flat,
                 AST_Type **supports,
                 long n_supports,
                 AST_Type *supports_concrete,
                 bool abstract,
                 bool truncatable,
                 bool custom);

  ~AST_ValueType () override;

  bool in_recursion (ACE_Unbounded_Queue<AST_Type *> &list) override;
  // Are we or the node represented by node involved in recursion.

  // This also calls the base class version.
  void redefine (AST_Interface *from) override;

  AST_Type **supports () const;

  long n_supports () const;

  AST_Type *inherits_concrete () const;

  AST_Type *supports_concrete () const;

  bool truncatable () const;
  bool custom () const;

  virtual bool will_have_factory ();
  // Called from y.tab.cpp to set the factory decl seen bit.

  // Look up a scoped name in the supported interface list.
  AST_Decl *look_in_supported (UTL_ScopedName *,
                               bool full_def_only) override;

  // Overridden for valuetype from UTL_Scope method.
  AST_Decl *special_lookup (UTL_ScopedName *,
                            bool full_def_only,
                            AST_Decl *&final_parent_decl) override;

  // Recursively called on valuetype to check for legal use as
  // a primary key. Overridden for valuetype, struct, sequence,
  // union, array, typedef, and interface.
  bool legal_for_primary_key () const override;

  // Cleanup function.
  void destroy () override;

  // AST Dumping.
  void dump (ACE_OSTREAM_TYPE &o) override;

  // Visiting.
  int ast_accept (ast_visitor *visitor) override;

  static AST_Decl::NodeType const NT;
  typedef AST_ValueTypeFwd FWD_TYPE;

protected:
  AST_Type **pd_supports;
  long pd_n_supports;
  AST_Type *pd_inherits_concrete;
  AST_Type *pd_supports_concrete;

  bool pd_truncatable;
  bool pd_custom;

protected:
  AST_Factory *fe_add_factory (AST_Factory *f) override;
  AST_Field *fe_add_field (AST_Field *o) override;

  bool derived_from_primary_key_base (const AST_ValueType *node,
                                      const AST_ValueType *pk_base) const;
  AST_ValueType *lookup_primary_key_base () const;
};

#endif           // _AST_INTERFACE_AST_VALUETYPE_HH
