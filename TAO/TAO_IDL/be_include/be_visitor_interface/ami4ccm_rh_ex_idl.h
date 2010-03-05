/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    ami4ccm_rh_ex_idl.h
 *
 *  $Id$
 *
 *  Concrete visitor for the AMI4CCM Interface node.
 *  This one provides code generation for interfaces in the executor IDL.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#ifndef _AMI4CCM_RH_EX_IDL_H_
#define _AMI4CCM_RH_EX_IDL_H_

/**
 * @class be_visitor_ami4ccm_rh_ex_idl
 *
 * @brief be_visitor_ami4ccm_rh_ex_idl
 *
 * This is a concrete visitor to generate the reply handler
 * interface IDL in the local executor mapping IDL file.
 */
class be_visitor_ami4ccm_rh_ex_idl : public be_visitor_scope
{
public:
  be_visitor_ami4ccm_rh_ex_idl (be_visitor_context *ctx);

  ~be_visitor_ami4ccm_rh_ex_idl (void);

  virtual int visit_interface (be_interface *node);
  virtual int visit_operation (be_operation *node);
  virtual int visit_attribute (be_attribute *node);
  virtual int visit_argument (be_argument *node);
};

#endif /* _AMI4CCM_RH_EX_IDL_H_ */
