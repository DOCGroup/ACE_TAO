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
//    cdr_op_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Union class
//    This one provides code generation for the CDR operators for the union
//    in the client stub.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_UNION_CDR_OP_CS_H_
#define _BE_VISITOR_UNION_CDR_OP_CS_H_

class be_visitor_union_cdr_op_cs : public be_visitor_union
{
  //
  // = TITLE
  //   be_visitor_union_cdr_op_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor for union that generates the CDR operator
  //   implementations
  //

public:
  be_visitor_union_cdr_op_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_union_cdr_op_cs (void);
  // destructor

  virtual int visit_union (be_union *node);
  // visit union

  virtual int pre_process (be_decl *);
  virtual int post_process (be_decl *);
};

#endif /* _BE_VISITOR_UNION_CDR_OP_CS_H_ */
