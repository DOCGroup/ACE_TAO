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
//    facet_svh.h
//
// = DESCRIPTION
//    Concrete visitor for the Component node.
//    This provides for code generation for the facet servant
//    class declaration
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_FACET_SVH_H_
#define _BE_COMPONENT_FACET_SVH_H_

class be_visitor_facet_svh : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_facet_svh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the facet servant decl
  //   for a component.
  //
  //
public:
  be_visitor_facet_svh (be_visitor_context *ctx);
  
  ~be_visitor_facet_svh (void);
  
  virtual int visit_provides (be_provides *node);
};

#endif /* _BE_COMPONENT_FACET_SVH_H_ */

