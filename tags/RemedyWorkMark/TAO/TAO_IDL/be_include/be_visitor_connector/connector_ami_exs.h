/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    connector_ami_exs.h
 *
 *  $Id$
 *
 *  Concrete connector_ami_exs for the AMI4CCM Connector node.
 *  This provides for code generation in the exec impl source
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#ifndef _BE_CONNECTOR_CONNECTOR_AMI_EXS_H_
#define _BE_CONNECTOR_CONNECTOR_AMI_EXS_H_

class be_visitor_context;

class be_visitor_connector_ami_exs
  : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_connector_ami_exs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate
  //   the executor implementation source for
  //   AMI4CCM connectors.
  //
  //
public:
  be_visitor_connector_ami_exs (be_visitor_context *ctx);

  ~be_visitor_connector_ami_exs (void);

  virtual int visit_connector (be_connector *node);

private:
  void gen_entrypoint (void);
};

#endif /* _BE_CONNECTOR_CONNECTOR_AMI_EXS_H_ */

