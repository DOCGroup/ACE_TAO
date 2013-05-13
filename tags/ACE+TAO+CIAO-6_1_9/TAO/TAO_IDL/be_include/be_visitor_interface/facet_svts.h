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
class be_visitor_facet_svts : public be_visitor_interface
{
public:
  be_visitor_facet_svts (be_visitor_context *ctx);

  ~be_visitor_facet_svts (void);

  virtual int visit_interface (be_interface *node);
};

// ====================================================

class be_facet_op_attr_defn_helper
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  be_facet_op_attr_defn_helper (be_interface *op_scope);

  virtual int emit (be_interface *derived_interface,
                    TAO_OutStream *os,
                    be_interface *base_interface);

private:
  be_interface *op_scope_;
};

#endif /* _BE_COMPONENT_FACET_SVS_H_ */

