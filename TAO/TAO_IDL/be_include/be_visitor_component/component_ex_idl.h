/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    component_ex_idl.h
 *
 *  Concrete visitor for the Component node.
 *  Provides code generation for components in the CIAO executor IDL
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_COMPONENT_EX_IDL_H_
#define _BE_COMPONENT_COMPONENT_EX_IDL_H_

/**
 * @class be_visitor_component_ex_idl
 *
 * @brief be_visitor_component_ex_idl
 *
 * This is a concrete visitor to generate the
 * CIAO executor IDL for component.
 */
class be_visitor_component_ex_idl : public be_visitor_decl
{
public:
  explicit be_visitor_component_ex_idl (be_visitor_context *ctx);

  ~be_visitor_component_ex_idl () override;
  be_visitor_component_ex_idl (be_visitor_component_ex_idl const &) = delete;
  be_visitor_component_ex_idl (be_visitor_component_ex_idl &&) = delete;
  be_visitor_component_ex_idl &operator= (be_visitor_component_ex_idl const &) = delete;
  be_visitor_component_ex_idl &operator= (be_visitor_component_ex_idl &&) = delete;

  int visit_component (be_component *node) override;
  int visit_connector (be_connector *node) override;

private:
  TAO_OutStream &os_;
};

#endif /* _BE_COMPONENT_COMPONENT_EX_IDL_H_ */
