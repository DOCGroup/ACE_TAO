
//=============================================================================
/**
 *  @file    connector_ami_exh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for AMI4CCM Connectors in the exec impl header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_connector_ami_exh::be_visitor_connector_ami_exh (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
  // This is initialized in the base class to svnt_export_macro()
  // or skel_export_macro(), since there are many more visitor
  // classes generating servant code. So we can just override
  // all that here.
  this->export_macro_ = be_global->conn_export_macro ();
}

be_visitor_connector_ami_exh::~be_visitor_connector_ami_exh (void)
{
}

int
be_visitor_connector_ami_exh::visit_connector (be_connector *node)
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

  be_visitor_facet_ami_exh facet_visitor (this->ctx_);

  /// We call visit_component() so the visitor base class member
  /// node_ can be set. If a facet is defined in a porttype, it
  /// complicates navigation to the component.
  if (facet_visitor.visit_component (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_connector_ami_exh::")
                         ACE_TEXT ("visit_connector - ")
                         ACE_TEXT ("facet visitor failed\n")),
                        -1);
    }

  be_visitor_executor_ami_exh exec_visitor (this->ctx_);

  if (exec_visitor.visit_connector (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_connector_ami_exh::")
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
      TAO_OutStream *new_stream = tao_cg->ciao_ami_rh_impl_header ();

      if (new_stream == 0)
        {
          int status =
            tao_cg->start_ciao_ami_rh_impl_header (
              be_global->be_get_ciao_ami_conn_impl_hdr_fname ());

           if (status == -1)
              {
                ACE_ERROR_RETURN ((LM_ERROR,
                                   ACE_TEXT ("be_visitor_connector_ami_exh")
                                   ACE_TEXT ("::visit_connector - ")
                                   ACE_TEXT ("Error opening CIAO AMI ")
                                   ACE_TEXT ("conn rh impl header file\n")),
                                  -1);
              }

            new_stream = tao_cg->ciao_ami_rh_impl_header ();
        }
    }

  return 0;
}

void
be_visitor_connector_ami_exh::gen_entrypoint (void)
{
  os_ << be_nl_2
      << "extern \"C\" " << this->export_macro_.c_str ()
      << " ::Components::EnterpriseComponent_ptr" << be_nl
      << "create_"
      << ScopeAsDecl (this->node_->defined_in ())->flat_name ()
      << "_Impl (void);";
}

