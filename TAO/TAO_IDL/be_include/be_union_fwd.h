/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_union_fwd.h
//
// = DESCRIPTION
//    Extension of class AST_UnionFwd that provides additional
//    means for C++ mapping of a union.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef BE_UNION_FWD_H
#define BE_UNION_FWD_H

#include "be_type.h"
#include "ast_union_fwd.h"

class be_visitor;

class be_union_fwd : public virtual AST_UnionFwd,
                     public virtual be_type
{
  // =TITLE
  //   be_union_fwd
  //
  // =DESCRIPTION
  //   Extensions to the AST_UnionFwd class
public:
  be_union_fwd (void);
  // Default constructor.

  be_union_fwd (UTL_ScopedName *n);
  // Constructor.

  virtual ~be_union_fwd (void);
  // Destructor.

  virtual void destroy (void);
  // Cleanup function.

  // Visiting.
  virtual int accept (be_visitor* visitor);

  // Narrowing.
  DEF_NARROW_METHODS2 (be_union_fwd, AST_UnionFwd, be_type);
  DEF_NARROW_FROM_DECL (be_union_fwd);
};

#endif // if !defined
