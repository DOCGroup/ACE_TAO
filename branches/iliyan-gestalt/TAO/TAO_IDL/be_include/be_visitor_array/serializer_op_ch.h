//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    serializer_op_ch.h
//
// = DESCRIPTION
//    Visitors for generation of TAO::DCPS::Serializer operators code 
//    for Arrays in client header. This one
//    generates the TAO::DCPS::Serializer operators.
//
// = AUTHOR
//    Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_ARRAY_serializer_op_CH_H_
#define _BE_VISITOR_ARRAY_serializer_op_CH_H_

// ************************************************************
// class be_visitor_array_serializer_op_ch
// ************************************************************

class be_visitor_array_serializer_op_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_array_serializer_op_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor for array that generates the Serializer operator
  //   declarations
  //

public:
  be_visitor_array_serializer_op_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_array_serializer_op_ch (void);
  // destructor

  void gen_arg_ref(be_array *node);
  // helper - common code

  virtual int visit_array (be_array *node);
  // visit array
};

#endif /* _BE_VISITOR_ARRAY_serializer_op_CH_H_ */
