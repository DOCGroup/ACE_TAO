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
//    valuetype_obv_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Valuetypes
//    OBV_ class implementation
//    (see C++ mapping OMG 20.17)
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>,
//    derived from interface_ch.cpp from Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VALUETYPE_VALUETYPE_OBV_CS_H_
#define _BE_VALUETYPE_VALUETYPE_OBV_CS_H_

class be_visitor_valuetype_obv_cs : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_valuetype_obv_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the implementation for valuetype
  //   OBV_ class
  //
public:
  be_visitor_valuetype_obv_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_obv_cs (void);
  // destructor

  virtual int visit_valuetype (be_valuetype *node);

  virtual int visit_field (be_field *node);
};

#endif /* _BE_VALUETYPE_VALUETYPE_OBV_CS_H_ */

