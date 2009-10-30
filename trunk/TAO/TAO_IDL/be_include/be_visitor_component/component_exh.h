//
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    component_exh.h
//
// = DESCRIPTION
//    Concrete visitor for the Component node.
//    This provides for code generation in the exec impl header
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_COMPONENT_EXH_H_
#define _BE_COMPONENT_COMPONENT_EXH_H_

class be_visitor_component_exh
  : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_component_exh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate
  //   the executor implementation header for components.
  //
  //
public:
  be_visitor_component_exh (be_visitor_context *ctx);
  
  ~be_visitor_component_exh (void);
  
  virtual int visit_component (be_component *node);
};

#endif /* _BE_COMPONENT_COMPONENT_EXH_H_ */

