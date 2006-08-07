/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    component_is.h
//
// = DESCRIPTION
//    Concrete visitor for the Component node.
//    This provides for code generation in the implementation skeleton
//
// = AUTHOR
//   Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_COMPONENT_IS_H_
#define _BE_COMPONENT_COMPONENT_IS_H_

class be_visitor_component_is : public be_visitor_component
{
  //
  // = TITLE
  //   be_visitor_component_ih
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate 
  //   the implementation skeleton for component
  //
  //
public:
  be_visitor_component_is (be_visitor_context *ctx);
  // constructor

  ~be_visitor_component_is (void);
  // destructor

  virtual int visit_component (be_component *node);
  // set the right context and make a visitor
};

#endif /* _BE_COMPONENT_COMPONENT_IS_H_ */
