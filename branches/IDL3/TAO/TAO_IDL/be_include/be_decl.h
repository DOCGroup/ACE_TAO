/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_decl.h
//
// = DESCRIPTION
//    Extension of the AST_Decl class.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef TAO_BE_DECL_H
#define TAO_BE_DECL_H

#include "ast_decl.h"

class be_scope;
class be_visitor;

class be_decl : public virtual AST_Decl
{
  // = TITLE
  //    be_decl
  // = DESCRIPTION
  //    The back end extension of the AST_Decl class. Provides an abstract
  //    interface.
  //
public:
  be_decl (void);
  // Default constructor.

  be_decl (AST_Decl::NodeType type,
           UTL_ScopedName *n);
  // Constructor that sets the node type.

  ~be_decl (void);
  // Destructor.

  // Methods used by the interface type strategy.
  void compute_full_name  (const char *prefix,
                           const char *suffix,
                           char *&name);
  // Both the arguments should be non-null!!!. Applies prefix and
  // suffix to the local name and makes a flat name.

  void compute_flat_name (const char *prefix,
                          const char *suffix,
                          char *& name);
  // Both the arguments should be non-null!!!. Applies prefix and
  // suffix to the local name and makes a flat name.

  // End of Methods use by the interface type strategy.

  virtual be_scope *scope (void);
  // Return the scope created by this node (if one exists).

  // Visiting
  virtual int accept (be_visitor *visitor);

  virtual void destroy (void);
  // Cleanup function.

  // Narrowing
  DEF_NARROW_METHODS1 (be_decl, AST_Decl);
  DEF_NARROW_FROM_DECL (be_decl);
};

#endif // if !defined
