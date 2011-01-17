/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    any_op_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Sequence class
 *  This one provides code generation for the Sequence node in the client
 *  header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_SEQUENCE_ANY_OP_CS_H_
#define _BE_VISITOR_SEQUENCE_ANY_OP_CS_H_

/**
 * @class be_visitor_sequence_any_op_cs
 *
 * @brief be_visitor_sequence_any_op_cs
 *
 * This is a concrete visitor for sequence that generates the Any operator
 * implementations
 */
class be_visitor_sequence_any_op_cs : public be_visitor_decl
{

public:
  /// constructor
  be_visitor_sequence_any_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_sequence_any_op_cs (void);

  /// visit sequence
  virtual int visit_sequence (be_sequence *node);
};

#endif /* _BE_VISITOR_SEQUENCE_ANY_OP_CS_H_ */
