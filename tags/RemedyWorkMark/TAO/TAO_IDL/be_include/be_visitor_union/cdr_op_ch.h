/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Union class
 *  This one provides code generation for the CDR operators for the union
 *  in the client header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_UNION_CDR_OP_CH_H_
#define _BE_VISITOR_UNION_CDR_OP_CH_H_

/**
 * @class be_visitor_union_cdr_op_ch
 *
 * @brief be_visitor_union_cdr_op_ch
 *
 * This is a concrete visitor for union that generates the CDR operator
 * declarations
 */
class be_visitor_union_cdr_op_ch : public be_visitor_union
{

public:
  /// constructor
  be_visitor_union_cdr_op_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_union_cdr_op_ch (void);

  /// visit union
  virtual int visit_union (be_union *node);

};

#endif /* _BE_VISITOR_UNION_CDR_OP_CH_H_ */
