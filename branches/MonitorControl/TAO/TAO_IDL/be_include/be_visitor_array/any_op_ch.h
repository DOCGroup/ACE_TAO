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
//    Visitors for generation of code for Arrays in client header. This one
//    generates the Any operators.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ARRAY_ANY_OP_CH_H_
#define _BE_VISITOR_ARRAY_ANY_OP_CH_H_

// ************************************************************
// class be_visitor_array_any_op_ch
// ************************************************************

class be_visitor_array_any_op_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_array_any_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for array that generates the Any operator
  //   declarations
  //

public:
  be_visitor_array_any_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_array_any_op_ch (void);
  // destructor

  virtual int visit_array (be_array *node);
  // visit array

};

#endif /* _BE_VISITOR_ARRAY_ANY_OP_CH_H_ */
