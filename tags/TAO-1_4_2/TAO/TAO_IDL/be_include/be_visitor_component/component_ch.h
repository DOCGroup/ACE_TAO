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
//    component_ch.h
//
// = DESCRIPTION
//    Concrete visitor for the Component node.
//    This one provides code generation for components in the client header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_COMPONENT_CH_H_
#define _BE_COMPONENT_COMPONENT_CH_H_

class be_visitor_component_ch : public be_visitor_component
{
  //
  // = TITLE
  //   be_visitor_interface_ch
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client header for component.
  //
  //
public:
  be_visitor_component_ch (be_visitor_context *ctx);
  // constructor

  ~be_visitor_component_ch (void);
  // destructor

  virtual int visit_component (be_component *node);
  // set the right context and make a visitor
};

#endif /* _BE_COMPONENT_COMPONENT_CH_H_ */
