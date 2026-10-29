/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    module_ch.h
 *
 *  Concrete visitor for the Module class
 *  This provides code generation for the module in the client header.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_MODULE_MODULE_CH_H_
#define _BE_VISITOR_MODULE_MODULE_CH_H_

/**
 * @class be_visitor_module_ch
 *
 * @brief be_visitor_module_ch
 *
 * This is a concrete visitor to generate the client header for module
 */
class be_visitor_module_ch : public be_visitor_module
{
public:
  /// constructor
  explicit be_visitor_module_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_module_ch () override = default;

  be_visitor_module_ch (const be_visitor_module_ch &) = delete;
  be_visitor_module_ch (be_visitor_module_ch &&) = delete;
  be_visitor_module_ch &operator= (const be_visitor_module_ch &) = delete;
  be_visitor_module_ch &operator= (be_visitor_module_ch &&) = delete;

  /// visit module. We provide code for this method in the derived class
  int visit_module (be_module *node) override;
};

#endif /* _BE_VISITOR_MODULE_MODULE_CH_H_ */
