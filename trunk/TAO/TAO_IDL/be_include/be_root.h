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

#include "be_module.h"
#include "ast_root.h"

class be_visitor;

/**
 * The back end extension of the AST_Root class
 */
class be_root : public virtual AST_Root,
                public virtual be_module
{
public:
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
