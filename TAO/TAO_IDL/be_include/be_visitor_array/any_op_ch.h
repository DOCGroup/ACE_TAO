
//=============================================================================
/**
 *  @file    any_op_ch.h
 *
 *  Visitors for generation of code for Arrays in client header. This one
 *  generates the Any operators.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_ARRAY_ANY_OP_CH_H_
#define _BE_VISITOR_ARRAY_ANY_OP_CH_H_

/**
 * @class be_visitor_array_any_op_ch
 *
 * @brief be_visitor_array_any_op_ch
 *
 * This is a concrete visitor for array that generates the Any operator
 * declarations
 */
class be_visitor_array_any_op_ch : public be_visitor_decl
{
public:
  /// constructor
  be_visitor_array_any_op_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_array_any_op_ch () override = default;

  /// visit array
  int visit_array (be_array *node) override;
};

#endif /* _BE_VISITOR_ARRAY_ANY_OP_CH_H_ */
