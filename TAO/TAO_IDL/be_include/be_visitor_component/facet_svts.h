/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    facet_svts.h
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
 * @class be_visitor_facet_svts
 *
 * @brief be_visitor_facet_svts
 *
 * This is a concrete visitor to generate the facet servant defn
 * for a component.
 */
class be_visitor_facet_svts : public be_visitor_component_scope
{
public:
  be_visitor_facet_svts (be_visitor_context *ctx);

  ~be_visitor_facet_svts (void);

  virtual int visit_connector (be_connector *node);
  virtual int visit_component (be_component *node);
  virtual int visit_provides (be_provides *node);
};

#endif /* _BE_COMPONENT_FACET_SVS_H_ */

