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
//    ami_handler_fwd_ci.h
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

#ifndef _BE_INTERFACE_AMI_HANDLER_FWD_CI_H_
#define _BE_INTERFACE_AMI_HANDLER_FWD_CI_H_

class be_visitor_ami_handler_fwd_ci : public be_visitor_decl
{
  //
  // = TITLE
  //   be_visitor_ami_handler_fwd_ci
  //
  // = DESCRIPTION
  //   This visitor visits the interface to generate the forward
  //   declration code for the AMI reply handler.
  //
  //
public:
  be_visitor_ami_handler_fwd_ci (be_visitor_context *ctx);
  // constructor

  ~be_visitor_ami_handler_fwd_ci (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit interface.

};

#endif /* _BE_INTERFACE_INTERFACE_FWD_CH_I_ */
