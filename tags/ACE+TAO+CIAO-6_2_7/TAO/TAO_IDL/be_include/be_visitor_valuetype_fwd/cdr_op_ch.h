/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    cdr_op_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the valuetype_fwd class
 *  This one provides code generation for the CDR operators
 *  (actually just their declarations) for the valuetype forward
 *  declaration in the client inline.
 *
 *
 *  @author Boris Kolpackov <bosk@ipmce.ru> based on code from Torsten Kuepper based on code from Aniruddha Gokhale & Carlos O'Ryan
 */
//=============================================================================


#ifndef BE_VISITOR_VALUETYPE_FWD_CDR_OP_CH_H
#define BE_VISITOR_VALUETYPE_FWD_CDR_OP_CH_H

/**
 * @class be_visitor_valuetype_fwd_cdr_op_ch
 *
 * @brief be_visitor_valuetype_fwd_cdr_op_ch
 *
 * This is a concrete visitor for valuetype that generates the CDR
 * operator forward declarations.
 */
class be_visitor_valuetype_fwd_cdr_op_ch : public be_visitor_decl
{

public:
  /// constructor
  be_visitor_valuetype_fwd_cdr_op_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_fwd_cdr_op_ch (void);

  virtual int visit_valuetype_fwd (be_valuetype_fwd *node);
  virtual int visit_eventtype_fwd (be_eventtype_fwd *node);
};

#endif /* BE_VISITOR_VALUETYPE_FWD_CDR_OP_CH_H */
