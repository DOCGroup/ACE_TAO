
//=============================================================================
/**
 *  @file    be_union.h
 *
 *  $Id$
 *
 *  Extension of class AST_Union that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#ifndef BE_UNION_H
#define BE_UNION_H

#include "be_type.h"
#include "be_scope.h"
#include "ast_union.h"

class AST_ConcreteType;
class be_visitor;

class be_union : public virtual AST_Union,
                 public virtual be_scope,
                 public virtual be_type
{
public:
  be_union (AST_ConcreteType *dt,
            UTL_ScopedName *n,
            bool local,
            bool abstract);

  /// Catch BE-specific member values before delegating to the base class.
  virtual void redefine (AST_Structure *from);

  /// Do we have at least one member with multiple case labels?
  virtual bool has_duplicate_case_labels (void);

  /// Overridden from class be_type.
  virtual void gen_ostream_operator (TAO_OutStream *os,
                                     bool use_underscore);

  /// Cleanup function.
  virtual void destroy (void);

  /// Visiting.
  virtual int accept (be_visitor *visitor);

  /// Decides whether a default switch case label in the generated copy
  /// constructor, assignment operator, etc. is needed.
  bool gen_empty_default_label (void);

  /// Just a way to get at fe_add_union_branch() from the backend.
  AST_UnionBranch *be_add_union_branch (AST_UnionBranch *b);

  /// Returns total number of labels, useful when the union has
  /// multiple case labels.
  ACE_UINT64 nlabels (void);

  // Narrowing.

  DEF_NARROW_FROM_DECL(be_union);
  DEF_NARROW_FROM_SCOPE(be_union);
};

#endif
