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
//    interface_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This one provides code generation for interfaces in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_INTERFACE_INTERFACE_CH_H_
#define _BE_INTERFACE_INTERFACE_CH_H_

// we need derived interface visitors for the client and server header files. For
// the others, they use the default interface visitor

class be_visitor_interface_ch : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for interface
  //
  //
public:
  be_visitor_interface_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_ch (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor
};

#endif /* _BE_INTERFACE_INTERFACE_CH_H_ */
