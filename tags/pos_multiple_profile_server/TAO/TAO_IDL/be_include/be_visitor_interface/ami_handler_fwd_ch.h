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
//    interface_fwd_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface Forward node.
//    This one provides code generation for interface forward node.
//
// = AUTHOR
//    Aniruddha Gokhale and Alexander Babu Arulanthu
//    <alex@cs.wustl.edu> 
//
// ============================================================================

#ifndef _BE_INTERFACE_AMI_HANDLER_FWD_CH_H_
#define _BE_INTERFACE_AMI_HANDLER_FWD_CH_H_

class be_visitor_interface_ami_handler_fwd_ch : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_interface_ami_handler_fwd_ch
  //
  // = DESCRIPTION
  //   This visitor visits the an interface and generates fwd
  //   declaration code for the AMI Handler interface.
  //
  //
public:
  be_visitor_interface_ami_handler_fwd_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_ami_handler_fwd_ch (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit interface.
};

#endif /* _BE_INTERFACE_AMI_HANDLER_FWD_CH_H_ */
