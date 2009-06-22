// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef _AST_INTERFACE_AST_VALUETYPE_HH
#define _AST_INTERFACE_AST_VALUETYPE_HH

#include "ast_interface.h"

class TAO_IDL_FE_Export AST_ValueType : public virtual AST_Interface
{
public:
  AST_ValueType (void);

  AST_ValueType (UTL_ScopedName *n,
                 AST_Interface **inherits,
                 long n_inherits,
                 AST_ValueType *inherits_concrete,
                 AST_Interface **inherits_flat,
                 long n_inherits_flat,
                 AST_Interface **supports,
                 long n_supports,
                 AST_Interface *supports_concrete,
                 bool abstract,
                 bool truncatable,
                 bool custom);

  virtual ~AST_ValueType (void);

  virtual bool in_recursion (ACE_Unbounded_Queue<AST_Type *> &list);
  // Are we or the node represented by node involved in recursion.

  // This also calls the base class version.
  virtual void redefine (AST_Interface *from);

  AST_Interface **supports (void) const;

  long n_supports (void) const;

  AST_ValueType *inherits_concrete (void) const;

  AST_Interface *supports_concrete (void) const;

  bool truncatable (void) const;
  bool custom (void) const;

  virtual bool will_have_factory (void);
  // Called from y.tab.cpp to set the factory decl seen bit.

  // Look up a scoped name in the supported interface list.
  virtual AST_Decl *look_in_supported (UTL_ScopedName *,
                                       bool treat_as_ref);

  // Recursively called on valuetype to check for legal use as
  // a primary key. Overridden for valuetype, struct, sequence,
  // union, array, typedef, and interface.
  virtual bool legal_for_primary_key (void) const;

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL(AST_ValueType);
  DEF_NARROW_FROM_SCOPE(AST_ValueType);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

protected:
  AST_Interface **pd_supports;
  long pd_n_supports;
  AST_ValueType *pd_inherits_concrete;
  AST_Interface *pd_supports_concrete;

  bool pd_truncatable;
  bool pd_custom;

protected:
  virtual AST_Factory *fe_add_factory (AST_Factory *f);
  bool derived_from_primary_key_base (const AST_ValueType *node,
                                      const AST_ValueType *pk_base) const;
  AST_ValueType *lookup_primary_key_base (void) const;
};

#endif           // _AST_INTERFACE_AST_VALUETYPE_HH
