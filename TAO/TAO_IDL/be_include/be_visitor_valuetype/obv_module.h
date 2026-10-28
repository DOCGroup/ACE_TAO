/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    obv_module.h
 *
 *  Concrete visitor for the Module class
 *  This provides code generation for the module in the header
 *  for the OBV_ namespace
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
 */
//=============================================================================


#ifndef _BE_VISITOR_VALUETYPE_OBV_MODULE_H_
#define _BE_VISITOR_VALUETYPE_OBV_MODULE_H_

/**
 * @class be_visitor_obv_module
 *
 * @brief be_visitor_obv_module
 *
 * This is a concrete visitor to generate the header for module
 * OBV_ namespace
 */
class be_visitor_obv_module : public be_visitor_scope
{
public:
  /// constructor
  explicit be_visitor_obv_module (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_obv_module () override = default;
  be_visitor_obv_module (be_visitor_obv_module const &) = delete;
  be_visitor_obv_module (be_visitor_obv_module &&) = delete;
  be_visitor_obv_module &operator= (be_visitor_obv_module const &) = delete;
  be_visitor_obv_module &operator= (be_visitor_obv_module &&) = delete;

  int visit_module (be_module *node) override;

  int visit_valuetype (be_valuetype *node) override;

  int visit_eventtype (be_eventtype *node) override;
};

#endif /* _BE_VISITOR_VALUETYPE_OBV_MODULE_H_ */
