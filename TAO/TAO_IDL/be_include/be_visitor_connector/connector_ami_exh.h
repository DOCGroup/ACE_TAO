/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    connector_ami_exh.h
 *
 *  Concrete visitor for the AMI4CCM Connector node.
 *  This provides for code generation in the exec impl header
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_CONNECTOR_CONNECTOR_AMI_EXH_H_
#define _BE_CONNECTOR_CONNECTOR_AMI_EXH_H_

class be_visitor_context;

/// Generates executor implementation headers for AMI4CCM connectors.
class be_visitor_connector_ami_exh
  : public be_visitor_component_scope
{
public:
  explicit be_visitor_connector_ami_exh (be_visitor_context *ctx);

  ~be_visitor_connector_ami_exh () override;
  be_visitor_connector_ami_exh (be_visitor_connector_ami_exh const &) = delete;
  be_visitor_connector_ami_exh (be_visitor_connector_ami_exh &&) = delete;
  be_visitor_connector_ami_exh &operator= (be_visitor_connector_ami_exh const &) = delete;
  be_visitor_connector_ami_exh &operator= (be_visitor_connector_ami_exh &&) = delete;

  int visit_connector (be_connector *node) override;

private:
  void gen_entrypoint ();
};

#endif /* _BE_CONNECTOR_CONNECTOR_AMI_EXH_H_ */

