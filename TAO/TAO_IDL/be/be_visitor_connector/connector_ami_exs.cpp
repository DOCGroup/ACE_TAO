
//=============================================================================
/**
 *  @file    connector_ami_exs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for AMI4CCM Connectors in the exec
 *  impl source.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_connector_ami_exs::be_visitor_connector_ami_exs (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_connector_ami_exs::~be_visitor_connector_ami_exs (void)
{
}

int
be_visitor_connector_ami_exs::visit_connector (be_connector *node)
{
  if (node->imported ())
    {
      return 0;
    }

  this->node_ = node;

  /// CIDL-generated namespace used 'CIDL_' + composition name.
  /// Now we use 'CIAO_' + component's flat name.
  os_ << be_nl_2
      << "namespace CIAO_" << node->flat_name ()
      << "_Impl" << be_nl
      << "{" << be_idt;

  be_visitor_facet_ami_exs facet_visitor (this->ctx_);

  if (facet_visitor.visit_component (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_connector_ami_exs::")
                         ACE_TEXT ("visit_connector - ")
                         ACE_TEXT ("facet visitor failed\n")),
                        -1);
    }

  be_visitor_executor_ami_exs exec_visitor (this->ctx_);

  if (exec_visitor.visit_connector (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_connector_ami_exs::")
                         ACE_TEXT ("visit_connector - ")
                         ACE_TEXT ("exec visitor failed\n")),
                        -1);
    }

  this->gen_entrypoint ();

  os_ << be_uidt_nl
      << "}";

  /// Presence of -Gex option here triggers generation of AMI4CCM
  /// reply handler impl class, with empty methods for user to
  /// complete, in its own set of files.
  if (be_global->gen_ciao_exec_impl ())
    {
      TAO_OutStream *new_stream = tao_cg->ciao_ami_rh_impl_source ();

      if (new_stream == 0)
        {
          int status =
            tao_cg->start_ciao_ami_rh_impl_source (
              be_global->be_get_ciao_ami_conn_impl_src_fname ());

           if (status == -1)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("be_visitor_connector_ami_exh")
                                   ACE_TEXT ("::visit_connector - ")
                                   ACE_TEXT ("Error opening CIAO AMI ")
                                   ACE_TEXT ("conn rh impl header file\n")),
                                  -1);
              }

            new_stream = tao_cg->ciao_ami_rh_impl_source ();
        }
    }

  return 0;
}

void
be_visitor_connector_ami_exs::gen_entrypoint (void)
{
  AST_Decl *s = ScopeAsDecl (this->node_->defined_in ());

  os_ << be_nl_2
      << "extern \"C\" ::Components::EnterpriseComponent_ptr"
      << be_nl
      << "create_" << s->flat_name () << "_Impl (void)" << be_nl
      << "{" << be_idt_nl
      << "::Components::EnterpriseComponent_ptr retval ="
      << be_idt_nl
      << "::Components::EnterpriseComponent::_nil ();"
      << be_uidt_nl << be_nl
      << "ACE_NEW_NORETURN (" << be_idt_nl
      << "retval," << be_nl
      << s->local_name () << "_exec_i);" << be_nl
      << be_uidt_nl
      << "return retval;" << be_uidt_nl
      << "}";
}

