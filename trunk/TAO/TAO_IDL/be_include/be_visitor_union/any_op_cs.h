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
//    any_op_cs.h
//
// = DESCRIPTION
//     Visitor for the Union class.
//     This one generates code for the Any operators for the Union in the client
//     header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_UNION_ANY_OP_CS_H_
#define _BE_VISITOR_UNION_ANY_OP_CS_H_

class be_visitor_union_any_op_cs : public be_visitor_union
{
  //
  // = TITLE
  //   be_visitor_union_any_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for union that generates the Any operator
  //   implementations
  //

public:
  be_visitor_union_any_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_any_op_cs (void);
  // destructor

  virtual int visit_union (be_union *node);
  // visit union

  virtual int visit_union_branch (be_union_branch *node);
  // visit union_branch
};

#endif /* _BE_VISITOR_UNION_ANY_OP_CS_H_ */
