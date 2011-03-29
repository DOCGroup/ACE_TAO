/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    component_ami_rh_ex_base.h
 *
 *  $Id$
 *
 *  Base class visitor for the AMI4CCM reply handler
 *  implementation concrete visitors.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_AMI_RH_EX_BASE_H_
#define _BE_COMPONENT_AMI_RH_EX_BASE_H_

class be_visitor_context;

/// This class inherits from the AMI connector visitor that its
/// code generation tracks closely.
class be_visitor_component_ami_rh_ex_base
  : public be_visitor_scope
{
  // = DESCRIPTION
  //   This is a base class visitor for the AMI4CCM reply
  //   handler implementation concrete visitors.
  //
  //
public:
  be_visitor_component_ami_rh_ex_base (be_visitor_context *ctx);
  ~be_visitor_component_ami_rh_ex_base (void);

  virtual int post_process (be_decl *node);

protected:
  void init (void);
  void gen_excep_op (const char *prefix,
                     be_decl *node,
                     bool for_defn);
  virtual void gen_op_body (void);
  int gen_attr_op (be_attribute *node, bool for_defn);

protected:
  be_interface *iface_;
  be_uses *port_;
  ACE_CString class_name_;
  ACE_CString base_class_name_;
};

// ======================================================

/// Worker class passed to traverse_inheritance_graph(),
/// collects operations and attributes.
class Exec_ReplyHandler_Op_Attr_Generator
  : public TAO_IDL_Inheritance_Hierarchy_Worker
{
public:
  Exec_ReplyHandler_Op_Attr_Generator (
    be_visitor_scope * visitor);

  virtual int emit (be_interface * derived_interface,
                    TAO_OutStream * os,
                    be_interface * base_interface);

private:
  be_visitor_scope * visitor_;
};

#endif /* _BE_COMPONENT_AMI_RH_EX_BASE_H_ */

