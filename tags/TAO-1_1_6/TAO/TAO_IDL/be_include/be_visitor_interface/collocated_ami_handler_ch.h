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
//    collocated_ami_handler_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This provides code generation for collocated classes in server header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_INTERFACE_COLLOCATED_AMI_HANDLER_CH_H_
#define _BE_INTERFACE_COLLOCATED_AMI_HANDLER_CH_H_

class be_visitor_interface_collocated_ami_handler_ch : public be_visitor_interface
{
  // = TITLE
  //   Generate the "collocated" class declaration.

public:
  be_visitor_interface_collocated_ami_handler_ch (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_interface_collocated_ami_handler_ch (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit an interface

};

#endif /* _BE_INTERFACE_COLLOCATED_AMI_HANDLER_CH_H_ */
