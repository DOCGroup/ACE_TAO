
//=============================================================================
/**
 *  @file    ami4ccm_conn_ex_idl.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for AMI4CCM Interfaces in the executor IDL
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_ami4ccm_conn_ex_idl::be_visitor_ami4ccm_conn_ex_idl (
      be_visitor_context *ctx)
  : be_visitor_scope (ctx),
    os_ (*ctx->stream ())
{
}

be_visitor_ami4ccm_conn_ex_idl::~be_visitor_ami4ccm_conn_ex_idl (void)
{
}

int
be_visitor_ami4ccm_conn_ex_idl::visit_interface (be_interface *node)
{
  Identifier *oln = node->original_local_name ();

  os_ << be_nl << be_nl
      << "connector AMI_" << oln << "_Connector" << be_nl
      << "{" << be_idt_nl
      << "uses " << oln << " uses_" << oln << ";" << be_nl
      << "provides AMI_" << oln << " provides_" << oln
      << ";" << be_uidt_nl
      << "};";

  return 0;
}

