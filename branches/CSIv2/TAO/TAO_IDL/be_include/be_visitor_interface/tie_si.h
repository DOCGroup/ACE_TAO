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
//    tie_si.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This one provides the code generation for TIE classes
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_INTERFACE_TIE_SI_H_
#define _BE_INTERFACE_TIE_SI_H_

class be_visitor_interface_tie_si : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_tie_si
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the server inline for operations
  //   of the TIE class
  //
  //
public:
  be_visitor_interface_tie_si (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_tie_si (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor

  virtual int visit_component (be_component *node);
  // set the right context and make a visitor

  static int method_helper (be_interface *,
                            be_interface *,
                            TAO_OutStream *os);
  // helper method to generate method declaration in the TIE classes.
};

#endif /* _BE_INTERFACE_TIE_SI_H_ */
