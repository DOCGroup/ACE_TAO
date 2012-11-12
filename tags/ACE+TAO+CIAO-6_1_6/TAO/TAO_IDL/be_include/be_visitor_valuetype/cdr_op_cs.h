/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op_cs.h
 *
 *  $Id$
 *
 *  Concrete visitor for valuetypes.
 *  This one provides code generation for the CDR operators.
 *
 *
 *  @author Boris Kolpackov <bosk@ipmce.ru> based on code from Torsten Kuepper <kuepper2@lfa.uni-wuppertal.de>
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUETYPE_CDR_OP_CS_H_
#define _BE_VISITOR_VALUETYPE_CDR_OP_CS_H_

/**
 * @class be_visitor_valuetype_cdr_op_cs
 *
 * @brief be_visitor_valuetype_cdr_op_cs
 *
 * This is a concrete visitor for valuetype that generates the CDR operator
 * implementations
 */
class be_visitor_valuetype_cdr_op_cs : public be_visitor_valuetype
{

public:
  /// constructor
  be_visitor_valuetype_cdr_op_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_cdr_op_cs (void);

  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_eventtype (be_eventtype *node);

  /// visit field
  virtual int visit_field (be_field *node);

  /// visit sequence
  virtual int visit_sequence (be_sequence *node);
};

#endif /* _BE_VISITOR_VALUETYPE_CDR_OP_CS_H_ */
