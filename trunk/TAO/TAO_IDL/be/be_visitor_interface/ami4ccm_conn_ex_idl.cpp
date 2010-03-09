
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
  AST_Decl *scope = ScopeAsDecl (node->defined_in ());
  bool global = (scope->node_type () == AST_Decl::NT_root);
  const char *smart_scope = (global ? "" : "::");

  os_ << be_nl << be_nl
      << "connector AMI_" << oln << "_Connector" << be_nl
      << "{" << be_idt_nl
      << "uses " << oln << " uses_" << oln << ";" << be_nl
      << "provides AMI_" << oln << " provides_" << oln
      << ";" << be_uidt_nl
      << "};";
      
  os_ << be_nl << be_nl
      << "local interface CCM_AMI_CONN_" << oln << be_idt_nl
      << ": ::Components::EnterpriseComponent" << be_uidt_nl
      << "{" << be_idt_nl
      << smart_scope << scope->full_name () << "::CCM_AMI_"
      << oln << " get_provides_" << oln << " ();" << be_uidt_nl
      << "};";
      
  os_ << be_nl << be_nl
      << "local interface CCM_AMI_CONN_" << oln
      << "_Context" << be_idt_nl
      << ": ::Components::SessionContext" << be_uidt_nl
      << "{" << be_idt_nl
      << "::" << node->full_name ()
      << " get_connection_uses_" << oln << " ();" << be_nl
      << smart_scope << scope->full_name () << "::AMI_"
      << oln << " get_connection_provides_" << oln
      << " ();" << be_uidt_nl
      << "};";

  return 0;
}

