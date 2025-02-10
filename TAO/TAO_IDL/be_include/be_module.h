/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_module.h
 *
 *  Extension of class AST_Module that provides additional means for C++
 *  mapping.
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

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
  be_module (UTL_ScopedName *n, AST_Module *previous = 0);

  /// Cleanup method.
  virtual void destroy ();

  // Visiting
  virtual int accept (be_visitor *visitor);
};

#endif
