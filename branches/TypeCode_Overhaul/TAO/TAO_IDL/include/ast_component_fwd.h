// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef _AST_COMPONENT_FWD_AST_COMPONENT_FWD_HH
#define _AST_COMPONENT_FWD_AST_COMPONENT_FWD_HH

#include "ast_interface_fwd.h"

// Representation of a forward component declaration.

class TAO_IDL_FE_Export AST_ComponentFwd : public virtual AST_InterfaceFwd
{
public:
  AST_ComponentFwd (void);

  AST_ComponentFwd (AST_Interface *dummy,
                    UTL_ScopedName *n);

  virtual ~AST_ComponentFwd (void);

  // Cleanup function.
  virtual void destroy (void);

  // Narrowing.
  DEF_NARROW_METHODS1(AST_ComponentFwd, AST_InterfaceFwd);
  DEF_NARROW_FROM_DECL(AST_ComponentFwd);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);
};

#endif           // _AST_COMPONENT_FWD_AST_COMPONENT_FWD_HH
