/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    be_union_fwd.h
 *
 *  Extension of class AST_UnionFwd that provides additional
 *  means for C++ mapping of a union.
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef BE_UNION_FWD_H
#define BE_UNION_FWD_H

#include "be_structure_fwd.h"
#include "ast_union_fwd.h"

class be_visitor;

/// Extends AST_UnionFwd with additional means for C++ mapping of a union.
class be_union_fwd : public virtual AST_UnionFwd,
                     public virtual be_structure_fwd
{
public:
  be_union_fwd (AST_Union *dummy,
                UTL_ScopedName *n);

  ~be_union_fwd () override;

  /// Cleanup function.
  void destroy () override;

  // Visiting.
  int accept (be_visitor* visitor) override;
};

#endif // if !defined
