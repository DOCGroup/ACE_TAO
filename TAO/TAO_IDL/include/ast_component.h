// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef _AST_COMPONENT_AST_COMPONENT_HH
#define _AST_COMPONENT_AST_COMPONENT_HH

#include "ast_interface.h"
#include "ace/Unbounded_Queue.h"

class TAO_IDL_FE_Export AST_Component : public virtual AST_Interface
{
public:
  AST_Component (void);

  AST_Component (UTL_ScopedName *n,
                 AST_Component *base_component,
                 AST_Interface **supports,
                 long n_supports,
                 AST_Interface **supports_flat,
                 long n_supports_flat);

  virtual ~AST_Component (void);

  // This also calls the base class version.
  virtual void redefine (AST_Interface *from);

  // Utility data structure for port declarations.
  struct port_description
  {
    Identifier *id;
    AST_Type *impl;
    idl_bool is_multiple;
  };

  // Accessors.

  AST_Component *base_component (void) const;

  AST_Interface **supports (void) const;
  
  long n_supports (void) const;

  ACE_Unbounded_Queue<port_description> &provides (void);
  ACE_Unbounded_Queue<port_description> &uses (void);
  ACE_Unbounded_Queue<port_description> &emits (void);
  ACE_Unbounded_Queue<port_description> &publishes (void);
  ACE_Unbounded_Queue<port_description> &consumes (void);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.
  DEF_NARROW_METHODS1(AST_Component, AST_Interface);
  DEF_NARROW_FROM_DECL(AST_Component);
  DEF_NARROW_FROM_SCOPE(AST_Component);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  // Check if we have redefined any of our parents' operations or attributes,
  // and check if there is such a clash among the parents
  virtual idl_bool redef_clash (void);

private:
  AST_Component *pd_base_component;
  ACE_Unbounded_Queue<port_description> pd_provides;
  ACE_Unbounded_Queue<port_description> pd_uses;
  ACE_Unbounded_Queue<port_description> pd_emits;
  ACE_Unbounded_Queue<port_description> pd_publishes;
  ACE_Unbounded_Queue<port_description> pd_consumes;
};

#endif // _AST_COMPONENT_AST_COMPONENT_HH
