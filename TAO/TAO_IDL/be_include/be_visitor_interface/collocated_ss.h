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
//    collocated_ss.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This provides code generation for collocated classes in server skeleton
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _BE_INTERFACE_COLLOCATED_SS_H_
#define _BE_INTERFACE_COLLOCATED_SS_H_

class be_visitor_interface_collocated_ss : public be_visitor_interface
{
  // = TITLE
  //   Generate the "collocated" class declaration.

public:
  be_visitor_interface_collocated_ss (be_visitor_context *ctx);
  // constructor

  virtual ~be_visitor_interface_collocated_ss (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // visit interface

};

#endif /* _BE_INTERFACE_COLLOCATED_SS_H_ */
