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
//    union_ci.h
//
// = DESCRIPTION
//     Visitor for the Union class.
//     This one generates code in the client inline file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_UNION_UNION_CI_H_
#define _BE_VISITOR_UNION_UNION_CI_H_

class be_visitor_union_ci : public be_visitor_union
{
  //
  // = TITLE
  //   be_visitor_union_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client inline for union
  //
  //
public:
  be_visitor_union_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_ci (void);
  // destructor

  virtual int visit_union (be_union *node);
  // visit union. We provide code for this method in the derived class

};

#endif /* _BE_VISITOR_UNION_UNION_CI_H_ */
