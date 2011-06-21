/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    component_exh.h
 *
 *  $Id$
 *
 *  Concrete visitor for the Component node.
 *  This provides for code generation in the exec impl header
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_COMPONENT_COMPONENT_EXH_H_
#define _BE_COMPONENT_COMPONENT_EXH_H_

/**
 * This is a concrete visitor to generate
 * the executor implementation header for components.
 */
class be_visitor_component_exh
  : public be_visitor_component_scope
{
public:
  be_visitor_component_exh (be_visitor_context *ctx);

  ~be_visitor_component_exh (void);

  virtual int visit_component (be_component *node);
};

#endif /* _BE_COMPONENT_COMPONENT_EXH_H_ */

