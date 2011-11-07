/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    marshal_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for valuetypes.
 *  This one provides code generation for marshalling.
 *
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUETYPE_MARSHAL_CH_H_
#define _BE_VISITOR_VALUETYPE_MARSHAL_CH_H_

/**
 * @class be_visitor_valuetype_marshal_ch
 *
 * @brief be_visitor_valuetype_marshal_ch
 *
 * This is a concrete visitor for interface that generates the CDR operator
 * declarations
 */
class be_visitor_valuetype_marshal_ch : public be_visitor_scope
{

public:
  /// constructor
  be_visitor_valuetype_marshal_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_marshal_ch (void);

  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_eventtype (be_eventtype *node);
};

#endif /* _BE_VISITOR_VALUETYPE_MARSHAL_CH_H_ */
