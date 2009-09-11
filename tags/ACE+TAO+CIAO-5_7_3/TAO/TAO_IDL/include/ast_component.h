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

  // Extend lookup to the base component.
  virtual AST_Decl *look_in_inherited (UTL_ScopedName *e,
                                       bool treat_as_ref);

  // Extend lookup to the supported interfaces.
  virtual AST_Decl *look_in_supported (UTL_ScopedName *e,
                                       bool treat_as_ref);

  // Utility data structure for port declarations.
  struct port_description
  {
    port_description (void)
      : id (0),
        impl (0),
        is_multiple (false),
        line_number (0)
    {}

    Identifier *id;
    AST_Type *impl;
    bool is_multiple;
    
    // These structs are queued, in a separate queue for each port type,
    // this helps some backends get a total ordering for a component's 
    // ports and attributes.
    long line_number;
  };

  // Accessors.

  AST_Component *base_component (void) const;

  AST_Interface **supports (void) const;

  long n_supports (void) const;
  
  typedef ACE_Unbounded_Queue<port_description> PORTS;

  PORTS &provides (void);
  PORTS &uses (void);
  PORTS &emits (void);
  PORTS &publishes (void);
  PORTS &consumes (void);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL(AST_Component);
  DEF_NARROW_FROM_SCOPE(AST_Component);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

private:
  AST_Component *pd_base_component;
  PORTS pd_provides;
  PORTS pd_uses;
  PORTS pd_emits;
  PORTS pd_publishes;
  PORTS pd_consumes;
};

#endif // _AST_COMPONENT_AST_COMPONENT_HH
