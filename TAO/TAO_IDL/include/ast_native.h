// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef _AST_NATIVE_H_
#define _AST_NATIVE_H_

#include "ast_type.h"

// Representation of "native" IDL type added by the POA spec.

class TAO_IDL_FE_Export AST_Native : public virtual AST_Type
{
public:
  // Operations.

  // Constructor(s).

  AST_Native (void);
  // Default constructor.

  AST_Native (UTL_ScopedName *n);
  // Constructor that initializes the scoped name.

  virtual ~AST_Native (void);
  // Destructor.

  // Narrowing.
  DEF_NARROW_METHODS1(AST_Native, AST_Type);
  DEF_NARROW_FROM_DECL(AST_Native);

  // AST Dumping.
  virtual void dump(ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);
};

#endif /* AST_NATIVE_H */
