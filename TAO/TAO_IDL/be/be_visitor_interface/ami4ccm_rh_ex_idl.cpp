
//=============================================================================
/**
 *  @file    ami4ccm_rh_ex_idl.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for AMI4CCM Interfaces in the executor IDL
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_ami4ccm_rh_ex_idl::be_visitor_ami4ccm_rh_ex_idl (
    be_visitor_context *ctx
  )
  : be_visitor_scope (ctx)
{
}

be_visitor_ami4ccm_rh_ex_idl::~be_visitor_ami4ccm_rh_ex_idl (void)
{
}

int
be_visitor_ami4ccm_rh_ex_idl::visit_interface (be_interface *)
{
  return 0;
}

int
be_visitor_ami4ccm_rh_ex_idl::visit_operation (be_operation *)
{
  return 0;
}

int
be_visitor_ami4ccm_rh_ex_idl::visit_attribute (be_attribute *)
{
  return 0;
}

int
be_visitor_ami4ccm_rh_ex_idl::visit_argument (be_argument *)
{
  return 0;
}
