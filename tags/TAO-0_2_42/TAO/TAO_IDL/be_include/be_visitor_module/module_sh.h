/* -*- c++ -*- */
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    module_sh.h
//
// = DESCRIPTION
//    Concrete visitor for the Module class
//    This provides code generation for the module in the server header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VISITOR_MODULE_MODULE_SH_H_
#define _BE_VISITOR_MODULE_MODULE_SH_H_

class be_visitor_module_sh : public be_visitor_module
{
  //
  // = TITLE
  //   be_visitor_module_sh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header for module
  //
  //
public:
  be_visitor_module_sh (be_visitor_context *ctx);
  // constructor

  ~be_visitor_module_sh (void);
  // destructor

  virtual int visit_module (be_module *node);
  // visit module. We provide code for this method in the derived class

};

#endif /* _BE_VISITOR_MODULE_MODULE_SH_H_ */
