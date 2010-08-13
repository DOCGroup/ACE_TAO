/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Structure class
 *  This one provides code generation for the CDR operators for the structure
 *  in the client header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_STRUCTURE_CDR_OP_CH_H_
#define _BE_VISITOR_STRUCTURE_CDR_OP_CH_H_

/**
 * @class be_visitor_structure_cdr_op_ch
 *
 * @brief be_visitor_structure_cdr_op_ch
 *
 * This is a concrete visitor for structure that generates the CDR operator
 * declarations
 */
class be_visitor_structure_cdr_op_ch : public be_visitor_structure
{

public:
  /// constructor
  be_visitor_structure_cdr_op_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_structure_cdr_op_ch (void);

  /// visit structure
  virtual int visit_structure (be_structure *node);
};

#endif /* _BE_VISITOR_STRUCTURE_CDR_OP_CH_H_ */
