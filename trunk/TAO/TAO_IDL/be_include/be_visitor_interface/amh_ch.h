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
//    amh_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the AMH-RH Interface node.
//    This one provides code generation for interfaces in the client header.
//
// = AUTHOR
//    Mayur Deshpande
//
// ============================================================================

#ifndef _BE_INTERFACE_AMH_INTERFACE_CH_H_
#define _BE_INTERFACE_AMH_INTERFACE_CH_H_

class be_visitor_amh_interface_ch : be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_amh_interface_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for AMH interface
  //
public:
  be_visitor_amh_interface_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_amh_interface_ch (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor
};

#endif /* _BE_INTERFACE_AMH_INTERFACE_CH_H_ */
