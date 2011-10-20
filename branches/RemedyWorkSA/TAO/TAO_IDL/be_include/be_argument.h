
//=============================================================================
/**
 *  @file    be_argument.h
 *
 *  $Id$
 *
 *  Extension of class AST_Argument that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#ifndef BE_ARGUMENT_H
#define BE_ARGUMENT_H

#include "be_decl.h"
#include "ast_argument.h"

class AST_Type;
class be_visitor;

class be_argument : public virtual AST_Argument,
                    public virtual be_decl
{
public:
  be_argument (AST_Argument::Direction d,
               AST_Type *ft,
               UTL_ScopedName *n);

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Cleanup.
  virtual void destroy (void);

  // Narrowing.
  DEF_NARROW_FROM_DECL (be_argument);
};

#endif
