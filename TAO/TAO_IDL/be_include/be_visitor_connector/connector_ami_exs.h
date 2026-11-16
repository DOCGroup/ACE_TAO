/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    connector_ami_exs.h
 *
 *  Concrete connector_ami_exs for the AMI4CCM Connector node.
 *  This provides for code generation in the exec impl source
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_CONNECTOR_CONNECTOR_AMI_EXS_H_
#define _BE_CONNECTOR_CONNECTOR_AMI_EXS_H_

class be_visitor_context;

/// Generates executor implementation source for AMI4CCM connectors.
class be_visitor_connector_ami_exs
  : public be_visitor_component_scope
{
public:
  explicit be_visitor_connector_ami_exs (be_visitor_context *ctx);

  ~be_visitor_connector_ami_exs () override;
  be_visitor_connector_ami_exs (be_visitor_connector_ami_exs const &) = delete;
  be_visitor_connector_ami_exs (be_visitor_connector_ami_exs &&) = delete;
  be_visitor_connector_ami_exs &operator= (be_visitor_connector_ami_exs const &) = delete;
  be_visitor_connector_ami_exs &operator= (be_visitor_connector_ami_exs &&) = delete;

  int visit_connector (be_connector *node) override;

private:
  void gen_entrypoint ();
};

#endif /* _BE_CONNECTOR_CONNECTOR_AMI_EXS_H_ */

