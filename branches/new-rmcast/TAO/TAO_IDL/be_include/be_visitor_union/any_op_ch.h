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
//    any_op_ch.h
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

#ifndef _BE_VISITOR_UNION_ANY_OP_CH_H_
#define _BE_VISITOR_UNION_ANY_OP_CH_H_

class be_visitor_union_any_op_ch : public be_visitor_union
{
  //
  // = TITLE
  //   be_visitor_union_any_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for union that generates the Any operator
  //   declarations
  //

public:
  be_visitor_union_any_op_ch (be_visitor_context *ctx);

  ~be_visitor_union_any_op_ch (void);

  virtual int visit_union (be_union *node);

  virtual int visit_union_branch (be_union_branch *node);

  virtual int visit_enum (be_enum *node);

  virtual int visit_structure (be_structure *node);
};

#endif /* _BE_VISITOR_UNION_ANY_OP_CH_H_ */
