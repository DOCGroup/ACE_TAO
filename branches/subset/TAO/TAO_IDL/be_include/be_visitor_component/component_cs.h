// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    component_cs.h
//
// = DESCRIPTION
//    Concrete visitor for the Component node.
//    This one provides code generation for components in the client stub.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_COMPONENT_CS_H_
#define _BE_COMPONENT_COMPONENT_CS_H_

class be_visitor_component_cs : public be_visitor_component
{
  //
  // = TITLE
  //   be_visitor_component_cs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the client stub for component.
  //
  //
public:
  be_visitor_component_cs (be_visitor_context *ctx);
  // constructor

  ~be_visitor_component_cs (void);
  // destructor

  virtual int visit_component (be_component *node);
  // set the right context and make a visitor

protected:
  void gen_unchecked_narrow (be_component *node,
                             be_type *bt,
                             TAO_OutStream *os);
};

#endif /* _BE_COMPONENT_COMPONENT_CS_H_ */
