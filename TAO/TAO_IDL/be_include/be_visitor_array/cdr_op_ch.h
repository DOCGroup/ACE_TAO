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
//    Visitors for generation of code for Arrays in client header. This one
//    generates the CDR operators.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ARRAY_CDR_OP_CH_H_
#define _BE_VISITOR_ARRAY_CDR_OP_CH_H_

// ************************************************************
// class be_visitor_array_cdr_op_ch
// ************************************************************

class be_visitor_array_cdr_op_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_array_cdr_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for array that generates the CDR operator
  //   declarations
  //

public:
  be_visitor_array_cdr_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_array_cdr_op_ch (void);
  // destructor

  virtual int visit_array (be_array *node);
  // visit array
};

#endif /* _BE_VISITOR_ARRAY_CDR_OP_CH_H_ */
