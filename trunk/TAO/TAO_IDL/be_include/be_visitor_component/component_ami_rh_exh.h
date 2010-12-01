/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    component_ami_rh_exh.h
 *
 *  $Id$
 *
 *  Concrete visitor for the AMI4CCM Component node.
 *  This provides for code generation of the AMI4CCM reply
 *  handler implementation header, triggered by the presence
 *  of a '#pragma ciao ami4ccm receptacle' directive in
 *  the component's IDL file.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_AMI_EXH_RH_H_
#define _BE_COMPONENT_AMI_EXH_RH_H_

class be_visitor_context;

/// This class inherits from the AMI connector visitor that its
/// code generation tracks closely.
class be_visitor_component_ami_rh_exh
  : public be_visitor_scope
{
  //
  // = TITLE
  //   be_visitor_component_ami_rh_exh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate
  //   the implementation header for
  //   AMI4CCM reply handers.
  //
  //
public:
  be_visitor_component_ami_rh_exh (be_visitor_context *ctx);

  ~be_visitor_component_ami_rh_exh (void);

  virtual int visit_uses (be_uses *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_argument (be_argument *node);

  virtual int post_process (be_decl *bd);

private:
  void init (void);

  void gen_excep_op (const char *prefix, be_decl *node);

private:
  be_interface *iface_;
  be_interface *callback_iface_;
  ACE_CString handler_str_;
  const char *scope_name_;
  const char *iface_name_;
  const char *callback_name_;
  const char *smart_scope_;
  ACE_CString export_macro_;
};

// ======================================================

/// Worker class passed to traverse_inheritance_graph(),
/// collects operations and attributes.
class Exec_ReplyHandler_Op_Attr_Decl_Generator
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  Exec_ReplyHandler_Op_Attr_Decl_Generator (
    be_visitor_scope * visitor);

  virtual int emit (be_interface * derived_interface,
                    TAO_OutStream * os,
                    be_interface * base_interface);

private:
  be_visitor_scope * visitor_;
};

#endif /* _BE_COMPONENT_AMI_EXH_RH_H_ */

