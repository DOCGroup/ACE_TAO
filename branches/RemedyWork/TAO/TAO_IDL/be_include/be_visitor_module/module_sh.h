/* -*- c++ -*- */

//=============================================================================
/**
 *  @file    module_sh.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Module class
 *  This provides code generation for the module in the server header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#ifndef _BE_VISITOR_MODULE_MODULE_SH_H_
#define _BE_VISITOR_MODULE_MODULE_SH_H_

/**
 * @class be_visitor_module_sh
 *
 * @brief be_visitor_module_sh
 *
 * This is a concrete visitor to generate the server header for module
 */
class be_visitor_module_sh : public be_visitor_module
{
public:
  /// constructor
  be_visitor_module_sh (be_visitor_context *ctx);

  /// destructor
  ~be_visitor_module_sh (void);

  /// visit module. We provide code for this method in the derived class
  virtual int visit_module (be_module *node);
};

#endif /* _BE_VISITOR_MODULE_MODULE_SH_H_ */
