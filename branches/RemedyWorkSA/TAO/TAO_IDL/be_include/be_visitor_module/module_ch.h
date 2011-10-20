/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    module_ch.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Module class
 *  This provides code generation for the module in the client header.
 *
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
  be_visitor_module_ch (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_module_ch (void);

  /// visit module. We provide code for this method in the derived class
  virtual int visit_module (be_module *node);
};

#endif /* _BE_VISITOR_MODULE_MODULE_CH_H_ */
