/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_home.h
//
// = DESCRIPTION
//    Extension of class AST_Home that provides additional
//    means for C++ mapping of a component home.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

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
  be_home (void);

  be_home (UTL_ScopedName *n,
           AST_Home *base_home,
           AST_Component *managed_component,
           AST_ValueType *primary_key);

  virtual ~be_home (void);

  // Cleanup function.
  virtual void destroy (void);

  // Visiting.
  virtual int accept (be_visitor* visitor);

  // Narrowing.
  DEF_NARROW_METHODS2 (be_home, be_interface, AST_Home);
  DEF_NARROW_FROM_DECL (be_home);
  DEF_NARROW_FROM_SCOPE (be_home);
};

#endif // if !defined
