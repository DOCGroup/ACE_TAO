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

  // Utility data structure for a 'uses' declaration.
  struct uses_description
  {
    AST_Interface *receptacle;
    idl_bool is_multiple;
  };

  // Accessors.

  AST_Component *base_component (void) const;

  AST_Interface **supports (void) const;
  
  long n_supports (void) const;

  ACE_Unbounded_Queue<AST_Interface *> &provides (void);
  void provides (AST_Interface *d);

  ACE_Unbounded_Queue<uses_description *> &uses (void);
  void uses (uses_description *d);

  ACE_Unbounded_Queue<AST_ValueType *> &emits (void);
  void emits (AST_ValueType *d);

  ACE_Unbounded_Queue<AST_ValueType *> &publishes (void);
  void publishes (AST_ValueType *d);

  ACE_Unbounded_Queue<AST_ValueType *> &consumes (void);
  void consumes (AST_ValueType *d);

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

private:
  AST_Component *pd_base_component;
  ACE_Unbounded_Queue<AST_Interface *> pd_provides;
  ACE_Unbounded_Queue<uses_description *> pd_uses;
  ACE_Unbounded_Queue<AST_ValueType *> pd_emits;
  ACE_Unbounded_Queue<AST_ValueType *> pd_publishes;
  ACE_Unbounded_Queue<AST_ValueType *> pd_consumes;
};

#endif // _AST_COMPONENT_AST_COMPONENT_HH
