/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    component_exh.h
 *
 *  Concrete visitor for the Component node.
 *  This provides for code generation in the exec impl header
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
  explicit be_visitor_component_exh (be_visitor_context *ctx);

  ~be_visitor_component_exh () override;
  be_visitor_component_exh (be_visitor_component_exh const &) = delete;
  be_visitor_component_exh (be_visitor_component_exh &&) = delete;
  be_visitor_component_exh &operator= (be_visitor_component_exh const &) = delete;
  be_visitor_component_exh &operator= (be_visitor_component_exh &&) = delete;

  int visit_component (be_component *node) override;
};

#endif /* _BE_COMPONENT_COMPONENT_EXH_H_ */

