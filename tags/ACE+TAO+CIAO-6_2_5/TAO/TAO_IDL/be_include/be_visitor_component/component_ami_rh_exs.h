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

#ifndef _BE_COMPONENT_AMI_RH_EXS_H_
#define _BE_COMPONENT_AMI_RH_EXS_H_

class be_visitor_context;

/**
 * This is a concrete visitor to generate
 * the stencil of the implementation source for
 * AMI4CCM reply handlers.
 */
class be_visitor_component_ami_rh_exs
  : public be_visitor_component_ami_rh_ex_base
{
public:
  be_visitor_component_ami_rh_exs (be_visitor_context *ctx);
  ~be_visitor_component_ami_rh_exs (void);

  virtual int visit_uses (be_uses *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_argument (be_argument *node);

private:
  virtual void gen_op_body (void);

private:
  const char *your_code_here_;
};

#endif /* _BE_COMPONENT_AMI_RH_EXS_H_ */

