/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_enum.h
//
// = DESCRIPTION
//    Extension of class AST_Enum that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef BE_ENUM_H
#define BE_ENUM_H

#include "be_scope.h"
#include "be_type.h"
#include "ast_enum.h"

class be_visitor;

class be_enum : public virtual AST_Enum,
                public virtual be_scope,
                public virtual be_type
{
public:
  be_enum (void);
  // Default constructor.

  be_enum (UTL_ScopedName *n,
           bool local,
           bool abstract);
  // Constructor.
  
  virtual void gen_ostream_operator (TAO_OutStream *os);
  // Overridden from class be_type.

  virtual void destroy (void);
  // Cleanup method.

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Narrowing.

  DEF_NARROW_FROM_DECL (be_enum);
  DEF_NARROW_FROM_SCOPE (be_enum);
};

#endif
