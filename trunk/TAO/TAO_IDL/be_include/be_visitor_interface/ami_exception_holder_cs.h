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

#ifndef _BE_INTERFACE_INTERFACE_AMI_EXCEPTION_HOLDER_CS_H_
#define _BE_INTERFACE_INTERFACE_AMI_EXCEPTION_HOLDER_CS_H_

class be_visitor_interface_ami_exception_holder_cs : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stubs for interface
  //
  //
public:
  be_visitor_interface_ami_exception_holder_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_ami_exception_holder_cs (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor
};

#endif /* _BE_INTERFACE_INTERFACE_AMI_EXCEPTION_HOLDER_CS_H_ */
