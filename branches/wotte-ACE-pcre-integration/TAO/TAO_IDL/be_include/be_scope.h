/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_scope.h
//
// = DESCRIPTION
//    Extension of the UTL_Scope CFE class
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef TAO_BE_SCOPE_H
#define TAO_BE_SCOPE_H

#include "utl_scope.h"
#include "ast_decl.h"

class be_decl;
class be_visitor;

class be_scope : public virtual UTL_Scope
{
  // = TITLE
  //    be_scope
  //
  // = DESCRIPTION
  //    The back end extension of the UTL_Scope class
  //
public:
  be_scope (void);
  // Default constructor.

  be_scope (AST_Decl::NodeType nt);
  // Constructor that sets the node type.

  virtual ~be_scope (void);
  // Destructor.

  virtual void comma (unsigned short set);
  // Set the comma producing state.

  int comma (void) const;
  // Get the comma producing state.

  virtual be_decl *decl (void);
  // Return the be_decl node corresponding to this scope node.

  virtual void destroy (void);
  // Recursively clean up scope members.

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Narrowing.
  DEF_NARROW_METHODS1 (be_scope, UTL_Scope);
  DEF_NARROW_FROM_SCOPE (be_scope);

private:
  unsigned short comma_;
  // If set, generate a comma after every element is handled.
};

#endif // if !defined
