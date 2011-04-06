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

#ifndef _BE_COMPONENT_AMI_RH_EXH_H_
#define _BE_COMPONENT_AMI_RH_EXH_H_

class be_visitor_context;

/**
 * This class inherits from the AMI connector visitor that its
 * code generation tracks closely.
 *
 * This is a concrete visitor to generate
 * the implementation header for AMI4CCM reply handlers.
 */

class be_visitor_component_ami_rh_exh
  : public be_visitor_component_ami_rh_ex_base
{
public:
  be_visitor_component_ami_rh_exh (be_visitor_context *ctx);
  ~be_visitor_component_ami_rh_exh (void);

  virtual int visit_uses (be_uses *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_argument (be_argument *node);

private:
  ACE_CString export_macro_;
};

#endif /* _BE_COMPONENT_AMI_RH_EXH_H_ */

