// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef TAO_IDL_AST_FINDER_H
#define TAO_IDL_AST_FINDER_H

// Representation of component home finder construct declaration.

#include "ast_factory.h"

class TAO_IDL_FE_Export AST_Finder : public virtual AST_Factory
{
public:

  AST_Finder (UTL_ScopedName *n);

  virtual ~AST_Finder (void);
  // Narrowing.

  DEF_NARROW_FROM_DECL(AST_Finder);
  DEF_NARROW_FROM_SCOPE(AST_Finder);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Cleanup function.
  virtual void destroy (void);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);

  static AST_Decl::NodeType const NT;
};

#endif // TAO_IDL_AST_FINDER_H
