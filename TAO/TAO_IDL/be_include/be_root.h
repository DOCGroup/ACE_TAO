/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_root.h
 *
 *  $Id$
 *
 *  Extension of class AST_Root that provides the backend
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================


#ifndef TAO_BE_ROOT_H
#define TAO_BE_ROOT_H

#include "be_decl.h"
#include "be_scope.h"
#include "ast_root.h"

class be_visitor;

class be_root : public virtual AST_Root,
                public virtual be_scope,
                public virtual be_decl
{
  // = TITLE
  //    The back end extension of the AST_Root class
  //
  // = DESCRIPTION
  //
public:
  /// Default constructor.
  be_root (void);

  /// Constructor that sets its scoped name <n>, and any pragmas <p>.
  be_root (UTL_ScopedName *n);
  
  virtual ~be_root (void);

  /// Cleanup method.
  virtual void destroy (void);

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Narrowing.

  DEF_NARROW_FROM_DECL (be_root);
  DEF_NARROW_FROM_SCOPE (be_root);
};

#endif
