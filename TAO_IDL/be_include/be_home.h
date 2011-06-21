/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_home.h
 *
 *  $Id$
 *
 *  Extension of class AST_Home that provides additional
 *  means for C++ mapping of a component home.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef TAO_BE_HOME_H
#define TAO_BE_HOME_H

#include "be_interface.h"
#include "ast_home.h"

class be_home : public virtual AST_Home,
                public virtual be_interface
{
  // =TITLE
  //   be_home
  //
  // =DESCRIPTION
  //   Extensions to the AST_Home class
public:
  be_home (UTL_ScopedName *n,
           AST_Home *base_home,
           AST_Component *managed_component,
           AST_Type *primary_key,
           AST_Type **supports,
           long n_supports,
           AST_Interface **supports_flat,
           long n_supports_flat);

  virtual ~be_home (void);

  void scan (UTL_Scope *s);

  // Cleanup function.
  virtual void destroy (void);

  // Visiting.
  virtual int accept (be_visitor* visitor);

  // Narrowing.

  DEF_NARROW_FROM_DECL (be_home);
  DEF_NARROW_FROM_SCOPE (be_home);
};

#endif // if !defined
