/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_component.h
//
// = DESCRIPTION
//    Extension of class AST_Component that provides additional means for C++
//    mapping of an interface.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef TAO_BE_COMPONENT_H
#define TAO_BE_COMPONENT_H

#include "be_interface.h"
#include "ast_component.h"

class be_component : public virtual AST_Component,
                     public virtual be_interface
{
  // = TITLE
  //   The back end extension of the AST_Component class
  //
  // = DESCRIPTION
  //
public:
  be_component (void);

  be_component (UTL_ScopedName *n,
                AST_Component *base_component,
                AST_Interface **supports,
                long n_supports,
                AST_Interface **supports_flat,
                long n_supports_flat);

  ~be_component (void);

  virtual void destroy (void);
  // Cleanup function.

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Narrowing.
  DEF_NARROW_METHODS2 (be_component, be_interface, AST_Component);
  DEF_NARROW_FROM_DECL (be_component);
  DEF_NARROW_FROM_SCOPE (be_component);
};

#endif  // if !defined
