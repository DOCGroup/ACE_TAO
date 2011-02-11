/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    valuetype_obv_ci.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Valuetype node.
 *  This one provides code generation for valuetype OBV_ class
 *  in the (client) header.
 *
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
 *  @author based on code from Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VALUETYPE_VALUETYPE_OBV_CI_H_
#define _BE_VALUETYPE_VALUETYPE_OBV_CI_H_

// we need derived valuetype visitors for the client and server header files. For
// the others, they use the default valuetype visitor

/**
 * @class be_visitor_valuetype_obv_ci
 *
 * @brief be_visitor_valuetype_ci
 *
 * This is a concrete visitor to generate the implementation for valuetype
 * OBV_ class
 */
class be_visitor_valuetype_obv_ci : public be_visitor_valuetype
{
public:
  /// constructor
  be_visitor_valuetype_obv_ci (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_obv_ci (void);

  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_eventtype (be_eventtype *node);

  virtual int visit_field (be_field *node);
};

#endif /* _BE_VALUETYPE_VALUETYPE_OBV_CI_H_ */
