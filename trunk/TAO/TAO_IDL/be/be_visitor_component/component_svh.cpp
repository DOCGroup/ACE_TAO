
//=============================================================================
/**
 *  @file    component_svh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Components in the servant header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

// ******************************************************
// Component visitor for server header
// ******************************************************

be_visitor_component_svh::be_visitor_component_svh (be_visitor_context *ctx)
  : be_visitor_component (ctx),
    os_ (*ctx->stream ()),
    export_macro_ (be_global->svnt_export_macro ())
{
  /// All existing CIAO examples set the servant export values in the CIDL
  /// compiler to equal the IDL compiler's skel export values. Below is a
  /// partial effort to decouple them, should be completely decoupled
  /// sometime. See comment in codegen.cpp, line 1173.
  if (export_macro_ == "")
    {
      export_macro_ = be_global->skel_export_macro ();
    }
}

be_visitor_component_svh::~be_visitor_component_svh (void)
{
}

int
be_visitor_component_svh::visit_component (be_component *node)
{
  if (node->imported ())
    {
      return 0;
    }

  /// Fills in the node's has_* members, for use in minimizing
  /// code generation.
  node->scan (node);

  // Generate the facet servant class declaration.
  be_visitor_facet_svh facet_visitor (this->ctx_);

  if (facet_visitor.visit_component_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_svh::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("facet visitor failed\n")),
                        -1);
    }

  /// CIDL-generated namespace used 'CIDL_' + composition name.
  /// Now we use 'CIAO_' + component's flat name.
  os_ << be_nl << be_nl
      << "namespace CIAO_" << node->flat_name ()
      << "_Impl" << be_nl
      << "{" << be_idt;

  // Generate the context class declaration.
  be_visitor_context_svh context_visitor (this->ctx_);

  if (context_visitor.visit_component (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_svh::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("context visitor failed\n")),
                        -1);
    }

  // Generate the servant class declaration.
  be_visitor_servant_svh servant_visitor (this->ctx_);

  if (servant_visitor.visit_component (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_svh::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("servant visitor failed\n")),
                        -1);
    }

  this->gen_entrypoint (node);

  os_ << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_component_svh::visit_connector (be_connector *node)
{
  return this->visit_component (node);
}

void
be_visitor_component_svh::gen_entrypoint (be_component *node)
{
  os_ << be_nl << be_nl
      << "extern \"C\" " << export_macro_.c_str ()
      << " ::PortableServer::Servant" << be_nl
      << "create_" << node->flat_name ()
      << "_Servant (" << be_idt_nl
      << "::Components::EnterpriseComponent_ptr p," << be_nl
      << "::CIAO::" << be_global->ciao_container_type ()
      << "_Container_ptr c," << be_nl
      << "const char * ins_name);" << be_uidt;
}

