// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef _AST_COMPONENT_AST_COMPONENT_HH
#define _AST_COMPONENT_AST_COMPONENT_HH

#include "ast_interface.h"
#include "ace/Unbounded_Queue.h"

class AST_Provides;
class AST_Uses;
class AST_Publishes;
class AST_Emits;
class AST_Consumes;
class AST_Extended_Port;
class AST_Mirror_Port;

class TAO_IDL_FE_Export AST_Component
  : public virtual AST_Interface
{
public:
  AST_Component (UTL_ScopedName *n,
                 AST_Component *base_component,
                 AST_Type **supports,
                 long n_supports,
                 AST_Interface **supports_flat,
                 long n_supports_flat);

  virtual ~AST_Component (void);

  // This also calls the base class version.
  virtual void redefine (AST_Interface *from);

  // Extend lookup to the base component.
  virtual AST_Decl *look_in_inherited (UTL_ScopedName *e,
                                       bool full_def_only);

  // Extend lookup to the supported interfaces.
  virtual AST_Decl *look_in_supported (UTL_ScopedName *e,
                                       bool full_def_only);

  // Accessors.

  AST_Component *base_component (void) const;

  AST_Type **supports (void) const;

  long n_supports (void) const;

  // Override for component of UTL_Scope method.
  virtual AST_Decl *special_lookup (UTL_ScopedName *e,
                                    bool full_def_only,
                                    AST_Decl *&final_parent_decl);

  // Cleanup function.
  virtual void destroy (void);

  // Allows adding an uses to a later point
  // The port i is inserted after port ix, if
  // ix is not null.
  int be_add_uses (AST_Uses *i,
                   AST_Uses *ix = 0);

  // Narrowing.

  DEF_NARROW_FROM_DECL (AST_Component);
  DEF_NARROW_FROM_SCOPE (AST_Component);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  static AST_Decl::NodeType const NT;
  typedef AST_ComponentFwd FWD_TYPE;

protected:
  virtual AST_Provides *fe_add_provides (AST_Provides *p);

  virtual AST_Uses *fe_add_uses (AST_Uses *p);

  virtual AST_Publishes *fe_add_publishes (AST_Publishes *p);

  virtual AST_Emits *fe_add_emits (AST_Emits *p);

  virtual AST_Consumes *fe_add_consumes (AST_Consumes *p);

  virtual AST_Extended_Port *fe_add_extended_port (AST_Extended_Port *p);

  virtual AST_Mirror_Port *fe_add_mirror_port (AST_Mirror_Port *p);

protected:
  AST_Component *pd_base_component;
};

#endif // _AST_COMPONENT_AST_COMPONENT_HH
