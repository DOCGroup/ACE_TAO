/* -*- c++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    public_assign_cs.h
//
// = DESCRIPTION
//     Visitor for the Union class.
//     This one generates code for the assignment operator of the union class
//     for the union members in the client stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#if !defined (_BE_VISITOR_UNION_BRANCH_PUBLIC_ASSIGN_CS_H_)
#define _BE_VISITOR_UNION_BRANCH_PUBLIC_ASSIGN_CS_H_

class be_visitor_union_branch_public_assign_cs : public be_visitor_decl
{
  //
  // = TITLE
  //    be_visitor_union_branch_public_assign_cs
  //
  // = DESCRIPTION
  //    This is used to generate the body of the assignment operator and the
  //    copy constructor of the union class
  //
public:
  be_visitor_union_branch_public_assign_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_branch_public_assign_cs (void);
  // destructor

  virtual int visit_union_branch (be_union_branch *node);
  // visit the union_branch node

};

#endif /* _BE_VISITOR_UNION_BRANCH_PUBLIC_ASSIGN_CS_H_ */
