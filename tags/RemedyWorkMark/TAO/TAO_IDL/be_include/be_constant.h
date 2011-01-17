/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_constant.h
 *
 *  $Id$
 *
 *  Extension of class AST_Constant that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#ifndef BE_CONSTANT_H
#define BE_CONSTANT_H

#include "be_decl.h"
#include "ast_constant.h"

class be_visitor;

class be_constant : public virtual AST_Constant,
                    public virtual be_decl
{
public:
  be_constant (AST_Expression::ExprType et,
               AST_Expression *v,
               UTL_ScopedName *n);

  // Visiting.
  virtual int accept (be_visitor *visitor);

  //Cleanup.
  virtual void destroy (void);

  // Narrowing.
  DEF_NARROW_FROM_DECL (be_constant);
};

#endif // if !defined
