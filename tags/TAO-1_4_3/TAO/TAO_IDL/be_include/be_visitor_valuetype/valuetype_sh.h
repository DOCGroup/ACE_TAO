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
//    valuetype_sh.h
//
// = DESCRIPTION
//    Concrete visitor for the valuetype node.
//    This provides for code generation in the server header
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VALUETYPE_VALUETYPE_SH_H_
#define _BE_VALUETYPE_VALUETYPE_SH_H_

class be_visitor_valuetype_sh : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_valuetype_sh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header for valuetype
  //
  //
public:
  be_visitor_valuetype_sh (be_visitor_context *ctx);
  // constructor
  
  ~be_visitor_valuetype_sh (void);
  // destructor
  
  virtual int visit_valuetype (be_valuetype *node);
  // set the right context and make a visitor
};

#endif /* _BE_VALUETYPE_VALUETYPE_SH_H_ */
