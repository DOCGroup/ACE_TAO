/* -*- c++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    module_ih.h
//
// = DESCRIPTION
//    Concrete visitor for the Module class
//    This provides code generation for the module in the implementation header.
//
// = AUTHOR
//   Yamuna Krishnamurthy (yamuna@cs.wustl.edu)
//
// ============================================================================

#ifndef _BE_VISITOR_MODULE_MODULE_IH_H_
#define _BE_VISITOR_MODULE_MODULE_IH_H_

class be_visitor_module_ih : public be_visitor_module
{
  //
  // = TITLE
  //   be_visitor_module_ih
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the implementation header for module
  //
  //
public:
  be_visitor_module_ih (be_visitor_context *ctx);
  // constructor

  ~be_visitor_module_ih (void);
  // destructor

  virtual int visit_module (be_module *node);
  // visit module. We provide code for this method in the derived class
};

#endif /* _BE_VISITOR_MODULE_MODULE_SH_H_ */
