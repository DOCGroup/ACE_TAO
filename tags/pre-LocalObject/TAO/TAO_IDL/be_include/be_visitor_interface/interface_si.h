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
//    interface_si.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This provides for code generation in the server inline
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_INTERFACE_INTERFACE_SI_H_
#define _BE_INTERFACE_INTERFACE_SI_H_

class be_visitor_interface_si : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_si
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server inline for interface
  //
  //
public:
  be_visitor_interface_si (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_si (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor
};

#endif /* _BE_INTERFACE_INTERFACE_SI_H_ */
