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
//    cdr_op.h
//
// = DESCRIPTION
//    Concrete visitor for the Root class
//    This one provides code generation for the CDR operators for elements of
//    the Root scope.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ROOT_CDR_OP_H_
#define _BE_VISITOR_ROOT_CDR_OP_H_

class be_visitor_root_cdr_op : public be_visitor_root
{
  //
  // = TITLE
  //   be_visitor_root_cdr_op
  //
  // = DESCRIPTION
  //   This is a concrete visitor for root that generates the CDR operator
  //   declarations and implementations
  //

public:
  be_visitor_root_cdr_op (be_visitor_context *ctx);
  // constructor

  ~be_visitor_root_cdr_op (void);
  // destructor

  virtual int visit_root (be_root *node);
  // visit root
};

#endif /* _BE_VISITOR_ROOT_CDR_OP_H_ */
