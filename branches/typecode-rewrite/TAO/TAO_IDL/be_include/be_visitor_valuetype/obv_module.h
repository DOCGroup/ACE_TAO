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
//    module_obv.h
//
// = DESCRIPTION
//    Concrete visitor for the Module class
//    This provides code generation for the module in the header
//    for the OBV_ namespace
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//
// ============================================================================

#ifndef _BE_VISITOR_VALUETYPE_OBV_MODULE_H_
#define _BE_VISITOR_VALUETYPE_OBV_MODULE_H_

class be_visitor_obv_module : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_obv_module
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the header for module
  //   OBV_ namespace
  //
public:
  be_visitor_obv_module (be_visitor_context *ctx);
  // constructor

  ~be_visitor_obv_module (void);
  // destructor

  virtual int visit_module (be_module *node);

  virtual int visit_valuetype (be_valuetype *node);

  virtual int visit_eventtype (be_eventtype *node);
};

#endif /* _BE_VISITOR_VALUETYPE_OBV_MODULE_H_ */
