/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    facet_svs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  This provides for code generation for the facet servant
 *  class definition
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_FACET_SVS_H_
#define _BE_COMPONENT_FACET_SVS_H_

/**
 * @class be_visitor_facet_svs
 *
 * @brief be_visitor_facet_svs
 *
 * This is a concrete visitor to generate the facet servant defn
 * for a component.
 */
class be_visitor_facet_svs : public be_visitor_component_scope
{
public:
  be_visitor_facet_svs (be_visitor_context *ctx);

  ~be_visitor_facet_svs (void);

  virtual int visit_provides (be_provides *node);
};

#endif /* _BE_COMPONENT_FACET_SVS_H_ */

