/* -*- c++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    component_ih.h
//
// = DESCRIPTION
//    Concrete visitor for the Component node.
//    This provides for code generation in the implementation header
//
// = AUTHOR
//   Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_COMPONENT_IH_H_
#define _BE_COMPONENT_COMPONENT_IH_H_

class be_visitor_component_ih : public be_visitor_component
{
  //
  // = TITLE
  //   be_visitor_component_ih
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate 
  //   the implementation header for component
  //
  //
public:
  be_visitor_component_ih (be_visitor_context *ctx);
  // constructor

  ~be_visitor_component_ih (void);
  // destructor

  virtual int visit_component (be_component *node);
  // set the right context and make a visitor
};

#endif /* _BE_COMPONENT_COMPONENT_IH_H_ */
