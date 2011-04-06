/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    facet_ex_idl.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  Provides code generation for facets in the CIAO executor IDL
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_FACET_EX_IDL_H_
#define _BE_COMPONENT_FACET_EX_IDL_H_

/**
 * This is a concrete visitor to generate the
 * CIAO executor IDL for facets.
 */
class be_visitor_facet_ex_idl
  : public be_visitor_component_scope
{
public:
  be_visitor_facet_ex_idl (be_visitor_context *ctx);

  ~be_visitor_facet_ex_idl (void);

  virtual int visit_provides (be_provides *node);
};

#endif /* _BE_COMPONENT_FACET_EX_IDL_H_ */

