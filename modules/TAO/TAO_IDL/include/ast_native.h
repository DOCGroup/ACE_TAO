// This may look like C, but it's really -*- C++ -*-
// $Id$

#ifndef _AST_NATIVE_H_
#define _AST_NATIVE_H_

#include "ast_exception.h"

// Representation of "native" IDL type. It may be used as a 
// return type, parameter type, or in an operation's
// exception list. This last usage creates special problems
// with both syntax checking and code generation. Letting
// this class inherit from AST_Exception is the most seamless
// way to handle it, and does not affect the other use cases.
class TAO_IDL_FE_Export AST_Native : public virtual AST_Exception
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
  
  virtual void destroy (void);
  // Cleanup.

  // Narrowing.

  DEF_NARROW_FROM_DECL(AST_Native);

  // AST Dumping.
  virtual void dump (ACE_OSTREAM_TYPE &o);

  // Visiting.
  virtual int ast_accept (ast_visitor *visitor);
};

#endif /* AST_NATIVE_H */
