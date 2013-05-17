/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    valuetype_obv_cs.h
 *
 *  $Id$
 *
 *  Visitor generating code for Valuetypes
 *  OBV_ class implementation
 *  (see C++ mapping OMG 20.17)
 *
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
 *  @author derived from interface_ch.cpp from Aniruddha Gokhale
 */
//=============================================================================

#ifndef _BE_VALUETYPE_VALUETYPE_OBV_CS_H_
#define _BE_VALUETYPE_VALUETYPE_OBV_CS_H_

/**
 * @class be_visitor_valuetype_obv_cs
 *
 * @brief be_visitor_valuetype_obv_cs
 *
 * This is a concrete visitor to generate the implementation for valuetype
 * OBV_ class
 */
class be_visitor_valuetype_obv_cs : public be_visitor_valuetype
{
public:
  /// constructor
  be_visitor_valuetype_obv_cs (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_valuetype_obv_cs (void);

  virtual int visit_valuetype (be_valuetype *node);
  virtual int visit_eventtype (be_eventtype *node);
  virtual int visit_field (be_field *node);

private:
  /// Generate member assignments for the initializing constructor.
  void gen_obv_init_constructor_inits (be_valuetype *node);

  /// Called by method above to generate base class constructor call.
  void gen_obv_init_base_constructor_args (be_valuetype *node,
                                           unsigned long &index);

  /// Called to generate the initalizing constructor call for the
  /// generated _copy_value method.
  void gen_obv_call_base_constructor_args (be_valuetype *node,
                                           unsigned long &index);
};

#endif /* _BE_VALUETYPE_VALUETYPE_OBV_CS_H_ */
