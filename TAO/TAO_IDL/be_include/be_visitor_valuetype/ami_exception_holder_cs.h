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
//    ami_exception_holder_cs.h
//
// = DESCRIPTION
//    Visitor to generate code for exception holders.
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de>
//
// ============================================================================

#ifndef _BE_VISITOR_VALUETYPE_AMI_EXCEPTION_HOLDER_CS_H_
#define _BE_VISITOR_VALUETYPE_AMI_EXCEPTION_HOLDER_CS_H_

class be_visitor_valuetype_ami_exception_holder_cs : public be_visitor_valuetype
{
  //
  // = TITLE
  //   be_visitor_valuetype_ami_exception_holder_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for interface
  //
  //
public:
  be_visitor_valuetype_ami_exception_holder_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_valuetype_ami_exception_holder_cs (void);
  // destructor

  virtual int visit_valuetype (be_valuetype *node);

  virtual int visit_operation (be_operation *node);
};

#endif /* _BE_VISITOR_VALUETYPE_AMI_EXCEPTION_HOLDER_CS_H_ */
