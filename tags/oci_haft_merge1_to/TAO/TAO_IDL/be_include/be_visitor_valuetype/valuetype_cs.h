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
//    valuetype_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the Valuetype node.
//    This one provides code generation for valuetypes in the client stubs.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    based on code from Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_VALUETYPE_VALUETYPE_CS_H_
#define _BE_VALUETYPE_VALUETYPE_CS_H_

class be_visitor_valuetype_cs : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_valuetype_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for valuetype
  //
  //
public:
  be_visitor_valuetype_cs (be_visitor_context *ctx);

  ~be_visitor_valuetype_cs (void);

  virtual int is_amh_exception_holder (be_valuetype *node);
  // Test if valuetype is an AMH ExceptionHolder.  Special generation
  // of marshalling/unmarshalling tests if it is an AMH
  // ExceptionHolder

  virtual int visit_valuetype (be_valuetype *node);
  // Set the right context and make a visitor.l

  virtual int visit_operation (be_operation *node);
  // Called only by AMH exceptions.
};

#endif /* _BE_VALUETYPE_VALUETYPE_CS_H_ */
