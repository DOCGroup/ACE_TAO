/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    connector_ami_rh_exs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the AMI4CCM Connector node.
 *  This provides for code generation of the AMI4CCM reply
 *  handler implementation source.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_CONNECTOR_CONNECTOR_AMI_EXS_RH_H_
#define _BE_CONNECTOR_CONNECTOR_AMI_EXS_RH_H_

class be_visitor_context;

/// This class inherits from the AMI connector visitor that its
/// code generation tracks closely.
class be_visitor_connector_ami_rh_exs
  : public be_visitor_facet_ami_exh
{
  //
  // = TITLE
  //   be_visitor_connector_ami_rh_exs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate
  //   the stencil of the implementation source for
  //   AMI4CCM reply handers.
  //
  //
public:
  be_visitor_connector_ami_rh_exs (be_visitor_context *ctx);

  ~be_visitor_connector_ami_rh_exs (void);

  virtual int visit_component (be_component *node);

  virtual int visit_provides (be_provides *node);

  virtual int visit_operation (be_operation *node);

  virtual int visit_attribute (be_attribute *node);
};

// ======================================================

/// Worker class passed to traverse_inheritance_graph(),
/// collects operations and attributes.
class ReplyHandler_Op_Attr_Defn_Generator
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  ReplyHandler_Op_Attr_Defn_Generator (be_visitor_scope * visitor);

  virtual int emit (be_interface * derived_interface,
                    TAO_OutStream * os,
                    be_interface * base_interface);

private:
  be_visitor_scope * visitor_;
};

#endif /* _BE_CONNECTOR_CONNECTOR_AMI_EXS_RH_H_ */

