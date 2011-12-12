
//=============================================================================
/**
 *  @file    be_union_branch.h
 *
 *  $Id$
 *
 *  Extension of class AST_UnionBranch that provides additional means for C++
 *  mapping.
 *
 *
 *  @author Copyright 1994-1995 by Sun Microsystems
 *  @author Inc. and Aniruddha Gokhale
 */
//=============================================================================

#ifndef BE_UNION_BRANCH_H
#define BE_UNION_BRANCH_H

#include "be_field.h"
#include "ast_union_branch.h"

class UTL_LabelList;
class AST_Type;
class be_visitor;
class TAO_OutStream;
class be_union;

class be_union_branch : public virtual AST_UnionBranch,
                        public virtual be_field
{
public:
  be_union_branch (UTL_LabelList *ll,
                   AST_Type *ft,
                   UTL_ScopedName *n);

  /// Generate the label value (as in a switch/case statement).
  int gen_label_value (TAO_OutStream *os,
                       unsigned long index = 0);

  /// Generate the default label value (as in a switch/case statement).
  int gen_default_label_value (TAO_OutStream *os,
                               be_union *bu);

  // Visiting.
  virtual int accept (be_visitor *visitor);

  // Cleanup.
  virtual void destroy (void);

  // Narrowing.
  DEF_NARROW_FROM_DECL (be_union_branch);
};

#endif
