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
//    valuetype_si.h
//
// = DESCRIPTION
//    Concrete visitor for the valuetype node.
//    This provides for code generation in the server inline
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_VALUETYPE_VALUETYPE_SI_H_
#define _BE_VALUETYPE_VALUETYPE_SI_H_

class be_visitor_valuetype_si : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_valuetype_si
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server inline for valuetype
  //
  //
public:
  be_visitor_valuetype_si (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_si (void);
  // destructor

  virtual int visit_valuetype (be_valuetype *node);
  // set the right context and make a visitor
};

#endif /* _BE_VALUETYPE_VALUETYPE_SI_H_ */
