/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_module.h
//
// = DESCRIPTION
//    Extension of class AST_Module that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_MODULE_H
#define _BE_MODULE_H

#include "be_scope.h"
#include "be_decl.h"
#include "ast_module.h"

class be_visitor;

class be_module : public virtual AST_Module,
                  public virtual be_scope,
                  public virtual be_decl
{
public:
  be_module (void);
  // Default constructor.

  be_module (UTL_ScopedName *n);
  // Constructor.

  virtual void destroy (void);
  // Cleanup method.

  // Visiting
  virtual int accept (be_visitor *visitor);

  // Narrowing.
  DEF_NARROW_METHODS3 (be_module, AST_Module, be_scope, be_decl);
  DEF_NARROW_FROM_DECL (be_module);
  DEF_NARROW_FROM_SCOPE (be_module);

protected:
  virtual int compute_size_type (void);
  // Compute the size type if it is unknown.
};

#endif
