/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    any_op_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Sequence class
 *  This one provides code generation for the Any operators for the Sequence
 *  node in the client header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_SEQUENCE_ANY_OP_CH_H_
#define _BE_VISITOR_SEQUENCE_ANY_OP_CH_H_

/**
 * @class be_visitor_sequence_any_op_ch
 *
 * @brief be_visitor_sequence_any_op_ch
 *
 * This is a concrete visitor for sequence that generates the Any operator
 * declarations
 */
class be_visitor_sequence_any_op_ch : public be_visitor_decl
{

public:
  /// constructor
  be_visitor_sequence_any_op_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_sequence_any_op_ch (void);

  /// visit sequence
  virtual int visit_sequence (be_sequence *node);
};

#endif /* _BE_VISITOR_SEQUENCE_ANY_OP_CH_H_ */
