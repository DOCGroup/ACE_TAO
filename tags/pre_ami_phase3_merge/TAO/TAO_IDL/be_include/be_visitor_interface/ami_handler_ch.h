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
//    ami_handler_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This provides for code generation in the server header
//
// = AUTHOR
//    Aniruddha Gokhale and Alexander Babu Arulanthu
//    <alex@cs.wustl.edu>  
//
// ============================================================================

#ifndef _BE_INTERFACE_AMI_HANDLER_CH_H_
#define _BE_INTERFACE_AMI_HANDLER_CH_H_

class be_visitor_interface_ami_handler_ch : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_ami_handler_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server header for interface
  //
  //
public:
  be_visitor_interface_ami_handler_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_ami_handler_ch (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor
};

#endif /* _BE_INTERFACE_AMI_HANDLER_CH_H_ */
