/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    facet_ami_exh.h
 *
 *  $Id$
 *
 *  This provides code generation for facets in the AMI connector
 *  impl header,
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _BE_COMPONENT_FACET_AMI_EXH_H_
#define _BE_COMPONENT_FACET_AMI_EXH_H_

class be_visitor_facet_ami_exh
  : public be_visitor_component_scope
{
  //
  // = TITLE
  //   be_visitor_facet_ami_exh
  //
  // = DESCRIPTION
  //   This is a concrete visitor to generate
  //   the AMI connector implementation header for facets.
  //
  //
public:
  be_visitor_facet_ami_exh (be_visitor_context *ctx);

  ~be_visitor_facet_ami_exh (void);

  virtual int visit_component (be_component *node);
  virtual int visit_provides (be_provides *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);

protected:
  /// Common the this class and derived class.
  void init (bool for_impl);

protected:
  be_interface *iface_;
  be_interface *callback_iface_;
  ACE_CString handler_str_;
  const char *scope_name_;
  const char *iface_name_;
  const char *smart_scope_;
  bool sync_;

private:
  int gen_reply_handler_class (void);
  int gen_facet_executor_class (void);
};
  //==============================================================

  /// Worker class passed to traverse_inheritance_graph(),
  /// collects all operations and attributes.
  class Facet_AMI_ExecH_Op_Attr_Generator
    : public TAO_IDL_Inheritance_Hierarchy_Worker
  {
  public:
    Facet_AMI_ExecH_Op_Attr_Generator (be_visitor_scope * visitor);

    virtual int emit (be_interface * derived_interface,
                      TAO_OutStream * os,
                      be_interface * base_interface);

  private:
    be_visitor_scope * visitor_;
  };
#endif /* _BE_COMPONENT_FACET_AMI_EXH_H_ */

