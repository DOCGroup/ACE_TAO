
/* -*- c++ -*- */

// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_ih.h
//
// = DESCRIPTION
//    Concrete visitor for the Interface node.
//    This provides for code generation in the implementation header
//
// = AUTHOR
//   Yamuna Krishnamurthy (yamuna@cs.wustl.edu)
//
// ============================================================================

#ifndef _BE_INTERFACE_INTERFACE_IH_H_
#define _BE_INTERFACE_INTERFACE_IH_H_

class be_visitor_interface_ih : public be_visitor_interface
{
  //
  // = TITLE
  //   be_visitor_interface_ih
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the implementation header for interface
  //
  //
public:
  be_visitor_interface_ih (be_visitor_context *ctx);
  // constructor

  ~be_visitor_interface_ih (void);
  // destructor

  virtual int visit_interface (be_interface *node);
  // set the right context and make a visitor
};

#endif /* _BE_INTERFACE_INTERFACE_IH_H_ */
