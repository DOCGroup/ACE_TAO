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
//    ami_handler_servant_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This provides for code generation in the server skeleton
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_INTERFACE_INTERFACE_AMI_HANDLER_SERVANT_CS_H_
#define _BE_INTERFACE_INTERFACE_AMI_HANDLER_SERVANT_CS_H_

class be_visitor_interface_ami_handler_servant_cs : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_ami_handler_servant_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server skeletons for interface
  //
  //
public:
  be_visitor_interface_ami_handler_servant_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_ami_handler_servant_cs (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor
};

#endif /* _BE_INTERFACE_INTERFACE_AMI_HANDLER_SERVANT_CS_H_ */
