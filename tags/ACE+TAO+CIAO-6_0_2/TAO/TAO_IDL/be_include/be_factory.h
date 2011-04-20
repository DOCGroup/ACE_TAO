
//=============================================================================
/**
 *  @file    be_factory.h
 *
 *  $Id$
 *
 *  Extension of class AST_Factory that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Boris Kolpackov <bosk@ipmce.ru>
 */
//=============================================================================

#ifndef BE_FACTORY_H
#define BE_FACTORY_H

#include "be_scope.h"
#include "be_decl.h"
#include "ast_factory.h"

class AST_Type;
class UTL_StrList;
class be_visitor;
class be_argument;

class be_factory : public virtual AST_Factory,
                   public virtual be_scope,
                   public virtual be_decl
{
public:
  be_factory (UTL_ScopedName *n);

  ~be_factory (void);

  /// Cleanup method.
  virtual void destroy (void);

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Add an argument to the scope.
  virtual AST_Argument *be_add_argument (AST_Argument *arg);

  // Narrowing
  DEF_NARROW_FROM_DECL (be_factory);
  DEF_NARROW_FROM_SCOPE (be_factory);
};

#endif // BE_FACTORY_H
