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
//    Concrete visitor for the Structure class
//    This one provides code generation for the CDR operators for the structure
//    in the client stub.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_STRUCTURE_CDR_OP_CI_H_
#define _BE_VISITOR_STRUCTURE_CDR_OP_CI_H_

class be_visitor_structure_cdr_op_ci : public be_visitor_structure
{
  //
  // = TITLE
  //   be_visitor_structure_cdr_op_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor for structure that generates the CDR operator
  //   implementations
  //

public:
  be_visitor_structure_cdr_op_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_structure_cdr_op_ci (void);
  // destructor

  virtual int visit_structure (be_structure *node);
  // visit structure

  virtual int post_process (be_decl *);
  // any post processing that needs to be done after a scope element is handled
};

#endif /* _BE_VISITOR_STRUCTURE_CDR_OP_CI_H_ */
