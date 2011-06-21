/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    connector_ami_exh.h
 *
 *  $Id$
 *
 *  Concrete visitor for the AMI4CCM Connector node.
 *  This provides for code generation in the exec impl header
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_CONNECTOR_CONNECTOR_AMI_EXH_H_
#define _BE_CONNECTOR_CONNECTOR_AMI_EXH_H_

class be_visitor_context;

class be_visitor_connector_ami_exh
  : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_connector_ami_exh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate
  //   the executor implementation header for
  //   AMI4CCM connectors.
  //
  //
public:
  be_visitor_connector_ami_exh (be_visitor_context *ctx);

  ~be_visitor_connector_ami_exh (void);

  virtual int visit_connector (be_connector *node);

private:
  void gen_entrypoint (void);
};

#endif /* _BE_CONNECTOR_CONNECTOR_AMI_EXH_H_ */

