/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    valuetype_ci.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Valuetype node.
 *  This one provides code generation for valuetypes in the client inline.
 *
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de> based on code from Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VALUETYPE_VALUETYPE_CI_H_
#define _BE_VALUETYPE_VALUETYPE_CI_H_

/**
 * @class be_visitor_valuetype_ci
 *
 * @brief be_visitor_valuetype_ci
 *
 * This is a concrete visitor to generate the client inline for valuetype
 */
class be_visitor_valuetype_ci : public be_visitor_valuetype
{
public:
  /// constructor
  be_visitor_valuetype_ci (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_ci (void);

  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_eventtype (be_eventtype *node);

  virtual int visit_field (be_field *node);
private:
  bool opt_accessor_;
};

#endif /* _BE_VALUETYPE_VALUETYPE_CI_H_ */
