// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_union_branch.h
//
// = DESCRIPTION
//    Extension of class AST_UnionBranch that provides additional means for C++
//    mapping.
//
// = AUTHOR
//    Copyright 1994-1995 by Sun Microsystems, Inc.
//    and
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef BE_UNION_BRANCH_H
#define BE_UNION_BRANCH_H

/*
 * BE_UnionBranch
 */
class be_union_branch : public virtual AST_UnionBranch,
                        public virtual be_decl
{
public:
  // Operations
  be_union_branch (void);
  // default constructor

  be_union_branch (UTL_LabelList *ll, AST_Type *ft, UTL_ScopedName *n,
                   UTL_StrList *p);
  // constructor

  int gen_label_value (TAO_OutStream *os, unsigned long index = 0);
  // Generate the label value (as in a switch/case statement).

  // Visiting
  virtual int accept (be_visitor *visitor);

  // Narrowing
  DEF_NARROW_METHODS2 (be_union_branch, AST_UnionBranch, be_decl);
  DEF_NARROW_FROM_DECL (be_union_branch);

protected:
  virtual int compute_size_type (void);
  // compute the size type if it is unknown

};

#endif
