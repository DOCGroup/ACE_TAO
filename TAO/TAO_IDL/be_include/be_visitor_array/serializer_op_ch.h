
//=============================================================================
/**
 *  @file    serializer_op_ch.h
 *
 *  $Id$
 *
 *  Visitors for generation of TAO::DCPS::Serializer operators code
 *  for Arrays in client header. This one
 *  generates the TAO::DCPS::Serializer operators.
 *
 *
 *  @author Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_ARRAY_serializer_op_CH_H_
#define _BE_VISITOR_ARRAY_serializer_op_CH_H_

// ************************************************************
// class be_visitor_array_serializer_op_ch
// ************************************************************

/**
 * @class be_visitor_array_serializer_op_ch
 *
 * @brief be_visitor_array_serializer_op_ch
 *
 * This is a concrete visitor for array that generates the Serializer operator
 * declarations
 */
class be_visitor_array_serializer_op_ch : public be_visitor_decl
{

public:
  /// constructor
  be_visitor_array_serializer_op_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_array_serializer_op_ch (void);

  /// helper - common code
  void gen_arg_ref(be_array *node);

  /// visit array
  virtual int visit_array (be_array *node);
};

#endif /* _BE_VISITOR_ARRAY_serializer_op_CH_H_ */
