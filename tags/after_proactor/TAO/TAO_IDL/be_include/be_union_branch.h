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

  be_union_branch (AST_UnionLabel *lab, AST_Type *ft, UTL_ScopedName *n,
                   UTL_StrList *p);
  // constructor

  virtual int gen_encapsulation (void);
  // generate the encapsulation

  virtual long tc_encap_len (void);
  // return the total byte length of ourselves represented as an encapsulation

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
