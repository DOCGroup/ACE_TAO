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
//    ami_exception_holder_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This one provides code generation for AMI exception holders
//    in client headers.
//
// = AUTHOR
//    Michael Kircher <Michael.Kircher@mchp.siemens.de> 
//
// ============================================================================

#ifndef _BE_INTERFACE_AMI_EXCEPTION_HOLDER_CH_H_
#define _BE_INTERFACE_AMI_EXCEPTION_HOLDER_CH_H_

// we need derived interface visitors for the client and server header files. For
// the others, they use the default interface visitor

class be_visitor_interface_ami_exception_holder_ch : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_ami_exception_holder_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for
  //   interface.
  //
public:
  be_visitor_interface_ami_exception_holder_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_ami_exception_holder_ch (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor
};

#endif /* _BE_INTERFACE_AMI_EXCEPTIUON_HOLER_CH_H_ */
