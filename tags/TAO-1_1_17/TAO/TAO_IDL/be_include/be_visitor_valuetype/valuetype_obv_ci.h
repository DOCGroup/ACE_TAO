//
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_ci.h
//
// = DESCRIPTION
//    Concrete visitor for the Valuetype node.
//    This one provides code generation for valuetype OBV_ class
//    in the (client) header.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>,
//    based on code from Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VALUETYPE_VALUETYPE_OBV_CI_H_
#define _BE_VALUETYPE_VALUETYPE_OBV_CI_H_

// we need derived valuetype visitors for the client and server header files. For
// the others, they use the default valuetype visitor

class be_visitor_valuetype_obv_ci : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_valuetype_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the implementation for valuetype
  //   OBV_ class
  //
public:
  be_visitor_valuetype_obv_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_obv_ci (void);
  // destructor

  virtual int visit_valuetype (be_valuetype *node);

  virtual int visit_field (be_field *node);
};

#endif /* _BE_VALUETYPE_VALUETYPE_OBV_CI_H_ */
