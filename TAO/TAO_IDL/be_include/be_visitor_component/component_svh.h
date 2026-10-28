/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    component_svh.h
 *
 *  Concrete visitor for the Component node.
 *  This provides for code generation in the servant header
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_COMPONENT_SVH_H_
#define _BE_COMPONENT_COMPONENT_SVH_H_

#include <string_view>

/**
 * @class be_visitor_component_svh
 *
 * @brief be_visitor_component_svh
 *
 * This is a concrete visitor to generate the servant header for component.
 */
class be_visitor_component_svh : public be_visitor_component
{
public:
  explicit be_visitor_component_svh (be_visitor_context *ctx);

  ~be_visitor_component_svh () override;
  be_visitor_component_svh (be_visitor_component_svh const &) = delete;
  be_visitor_component_svh (be_visitor_component_svh &&) = delete;
  be_visitor_component_svh &operator= (be_visitor_component_svh const &) = delete;
  be_visitor_component_svh &operator= (be_visitor_component_svh &&) = delete;

  int visit_component (be_component *node) override;
  int visit_connector (be_connector *node) override;

private:
  void gen_entrypoint (be_component *node);

private:
  TAO_OutStream &os_;
  std::string_view export_macro_;
};

#endif /* _BE_COMPONENT_COMPONENT_SVH_H_ */

