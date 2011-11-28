/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    ami4ccm_conn_ex_idl.h
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

#ifndef _AMI4CCM_CONN_EX_IDL_H_
#define _AMI4CCM_CONN_EX_IDL_H_

/**
 * @class be_visitor_ami4ccm_conn_ex_idl
 *
 * @brief be_visitor_ami4ccm_conn_ex_idl
 *
 * This is a concrete visitor to generate the implied
 * connector IDL in the local executor mapping IDL file.
 */
class be_visitor_ami4ccm_conn_ex_idl : public be_visitor_scope
{
public:
  be_visitor_ami4ccm_conn_ex_idl (be_visitor_context *ctx);

  ~be_visitor_ami4ccm_conn_ex_idl (void);

  virtual int visit_interface (be_interface *node);

private:
  TAO_OutStream &os_;
};

#endif /* _AMI4CCM_CONN_EX_IDL_H_ */
