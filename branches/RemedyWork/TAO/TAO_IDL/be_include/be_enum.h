/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_enum.h
 *
 *  $Id$
 *
 *  Extension of class AST_Enum that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

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
  be_enum (UTL_ScopedName *n,
           bool local,
           bool abstract);

  /// Overridden from class be_type.
  virtual void gen_ostream_operator (TAO_OutStream *os,
                                     bool use_underscore);

  /// Cleanup method.
  virtual void destroy (void);

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Narrowing.
  DEF_NARROW_FROM_DECL (be_enum);
  DEF_NARROW_FROM_SCOPE (be_enum);
};

#endif
