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
//    This one provides code generation for valuetypes in the client inline.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    based on code from Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VALUETYPE_VALUETYPE_CI_H_
#define _BE_VALUETYPE_VALUETYPE_CI_H_

class be_visitor_valuetype_ci : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_valuetype_ci
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client inline for valuetype
  //
  //
public:
  be_visitor_valuetype_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_ci (void);
  // destructor

  virtual int visit_valuetype (be_valuetype *node);
  // set the right context and make a visitor

  virtual int visit_field (be_field *node);
private:
  idl_bool opt_accessor_;
};

#endif /* _BE_VALUETYPE_VALUETYPE_CI_H_ */
