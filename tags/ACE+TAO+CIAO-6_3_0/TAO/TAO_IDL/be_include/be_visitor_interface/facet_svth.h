/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    facet_svth.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Interface node.
 *  This provides for code generation for the facet servant
 *  class declaration
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_FACET_SVH_H_
#define _BE_COMPONENT_FACET_SVH_H_

/**
 * @class be_visitor_facet_svth
 *
 * @brief be_visitor_facet_svth
 *
 * This is a concrete visitor to generate the facet servant decl
 * for a component.
 */
class be_visitor_facet_svth : public be_visitor_interface
{
public:
  be_visitor_facet_svth (be_visitor_context *ctx);

  ~be_visitor_facet_svth (void);

  virtual int visit_interface (be_interface *node);
};

#endif /* _BE_COMPONENT_FACET_SVH_H_ */

