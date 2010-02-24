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
//    facet_svs.h
//
// = DESCRIPTION
//    Concrete visitor for the Component node.
//    This provides for code generation for the facet servant
//    class definition
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#ifndef _BE_COMPONENT_FACET_SVS_H_
#define _BE_COMPONENT_FACET_SVS_H_

class be_visitor_facet_svs : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_facet_svs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate the facet servant defn
  //   for a component.
  //
  //
public:
  be_visitor_facet_svs (be_visitor_context *ctx);
  
  ~be_visitor_facet_svs (void);
  
  virtual int visit_provides (be_provides *node);
};

#endif /* _BE_COMPONENT_FACET_SVS_H_ */

