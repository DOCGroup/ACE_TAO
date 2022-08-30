/* -*- c++ -*- */


//=============================================================================
/**
 *  @file    module_ih.h
 *
 *  Concrete visitor for the Module class
 *  This provides code generation for the module in the implementation header.
 *
 *  @author Yamuna Krishnamurthy (yamuna@cs.wustl.edu)
 */
//=============================================================================


#ifndef _BE_VISITOR_MODULE_MODULE_IH_H_
#define _BE_VISITOR_MODULE_MODULE_IH_H_

/**
 * @class be_visitor_module_ih
 *
 * @brief be_visitor_module_ih
 *
 * This is a concrete visitor to generate the implementation header for module
 */
class be_visitor_module_ih : public be_visitor_module
{
public:
  /// constructor
  be_visitor_module_ih (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_module_ih ();

  /// visit module. We provide code for this method in the derived class
  int visit_module (be_module *node) override;
};

#endif /* _BE_VISITOR_MODULE_MODULE_SH_H_ */
