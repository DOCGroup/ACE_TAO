/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    component_ami_rh_exs.h
 *
 *  $Id$
 *
 *  Concrete visitor for the AMI4CCM component receptacle.
 *  This provides for code generation of the AMI4CCM reply
 *  handler implementation source.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_AMI_EXS_RH_H_
#define _BE_COMPONENT_AMI_EXS_RH_H_

class be_visitor_context;

class be_visitor_component_ami_rh_exs
  : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_component_ami_rh_exs
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate
  //   the stencil of the implementation source for
  //   AMI4CCM reply handers.
  //
  //
public:
  be_visitor_component_ami_rh_exs (be_visitor_context *ctx);

  ~be_visitor_component_ami_rh_exs (void);

  virtual int visit_uses (be_uses *node);

  virtual int visit_operation (be_operation *node);

  virtual int visit_attribute (be_attribute *node);

private:
  void init (void);

private:
  be_interface *iface_;
  be_interface *callback_iface_;
  ACE_CString handler_str_;
  const char *scope_name_;
  const char *iface_name_;
  const char *callback_name_;
  const char *smart_scope_;
};

// ======================================================

/// Worker class passed to traverse_inheritance_graph(),
/// collects operations and attributes.
class Exec_ReplyHandler_Op_Attr_Defn_Generator
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  Exec_ReplyHandler_Op_Attr_Defn_Generator (
    be_visitor_scope * visitor);

  virtual int emit (be_interface * derived_interface,
                    TAO_OutStream * os,
                    be_interface * base_interface);

private:
  be_visitor_scope * visitor_;
};

#endif /* _BE_COMPONENT_AMI_EXS_RH_H_ */

