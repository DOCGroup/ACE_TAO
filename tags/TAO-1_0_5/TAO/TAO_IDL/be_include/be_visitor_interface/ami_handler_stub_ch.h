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
//    ami_handler_stub_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This one provides code generation for interfaces in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale and Alexander Babu Arulanthu
//    <alex@cs.wustl.edu>  
//
// ============================================================================

#ifndef _BE_INTERFACE_AMI_HANDLER_STUB_CH_H_
#define _BE_INTERFACE_AMI_HANDLER_STUB_CH_H_

// we need derived interface visitors for the client and server header files. For
// the others, they use the default interface visitor

class be_visitor_interface_ami_handler_stub_ch : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_ami_handler_stub_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for
  //   interface.
  //
public:
  be_visitor_interface_ami_handler_stub_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_ami_handler_stub_ch (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor
};

#endif /* _BE_INTERFACE_AMI_HANDLER_STUB_CH_H_ */
