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
//    thru_poa_collocated_ss.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This provides code generation for thru_poa collocated classes in server skeleton
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_INTERFACE_THRU_POA_COLLOCATED_SS_H_
#define _BE_INTERFACE_THRU_POA_COLLOCATED_SS_H_

class be_visitor_interface_thru_poa_collocated_ss : public be_visitor_interface
{
  // = TITLE
  //   Generate the "thru_poa collocated" class declaration.

public:
  be_visitor_interface_thru_poa_collocated_ss (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_interface_thru_poa_collocated_ss (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit interface

  static int collocated_ctor_helper (be_interface *,
                                     be_interface *,
                                     TAO_OutStream *os);
  // helper method passed to the template method to invoke ctors of all the
  // base classes.
};

#endif /* _BE_INTERFACE_THRU_POA_COLLOCATED_SS_H_ */
