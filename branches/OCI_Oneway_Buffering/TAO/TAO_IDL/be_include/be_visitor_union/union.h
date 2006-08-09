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
//    union.h
//
// = DESCRIPTION
//     Visitor for the Union class.
//     This one is a generic visitor.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_UNION_UNION_H_
#define _BE_VISITOR_UNION_UNION_H_

class be_visitor_union : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_union
  //
  // = DESCRIPTION
  //   This is the base visitor for union
  //
  //
public:
  be_visitor_union (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union (void);
  // destructor

  virtual int visit_union (be_union *node);
  // visit union. We provide code for this method in the derived class

  // =visit operations on syntactically valid elements in our scope

  virtual int visit_union_branch (be_union_branch *node);
  // visit union_branch
};

#endif /* _BE_VISITOR_UNION_UNION_H_ */
