// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_factory.h
//
// = DESCRIPTION
//    Extension of class AST_Factory that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Boris Kolpackov <bosk@ipmce.ru>
//
// ============================================================================

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
  be_factory (void);
  // Default constructor.

  be_factory (UTL_ScopedName *n);
  // Constructor

  ~be_factory (void);
  // Destructor.

  virtual void destroy (void);
  // Cleanup method.

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Narrowing
  DEF_NARROW_METHODS3 (be_factory, AST_Factory, be_scope, be_decl);
  DEF_NARROW_FROM_DECL (be_factory);
  DEF_NARROW_FROM_SCOPE (be_factory);
};

#endif
