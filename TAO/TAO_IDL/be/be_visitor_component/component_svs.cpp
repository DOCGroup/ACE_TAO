
//=============================================================================
/**
 *  @file    component_svs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Components in the servant source.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

// ******************************************************
// Component visitor for servant source
// ******************************************************

be_visitor_component_svs::be_visitor_component_svs (be_visitor_context *ctx)
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

be_visitor_component_svs::~be_visitor_component_svs (void)
{
}

int
be_visitor_component_svs::visit_component (be_component *node)
{
  if (node->imported ())
    {
      return 0;
    }

  be_visitor_facet_svs facet_visitor (this->ctx_);

  if (facet_visitor.visit_component_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_svs::")
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

  be_visitor_context_svs context_visitor (this->ctx_);

  if (context_visitor.visit_component (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_svs::")
                         ACE_TEXT ("visit_component - ")
                         ACE_TEXT ("context visitor failed\n")),
                        -1);
    }

  be_visitor_servant_svs servant_visitor (this->ctx_);

  if (servant_visitor.visit_component (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_component_svs::")
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
be_visitor_component_svs::visit_connector (be_connector *node)
{
  return this->visit_component (node);
}

void
be_visitor_component_svs::gen_entrypoint (AST_Component *node)
{
  ACE_CString sname_str (
    ScopeAsDecl (node->defined_in ())->full_name ());
  const char *sname = sname_str.c_str ();
  const char *lname = node->local_name ()->get_string ();
  const char *global = (sname_str == "" ? "" : "::");

  os_ << be_nl << be_nl
      << "extern \"C\" " << export_macro_.c_str ()
      << " ::PortableServer::Servant" << be_nl
      << "create_" << node->flat_name ()
      << "_Servant (" << be_idt_nl
      << "::Components::EnterpriseComponent_ptr p," << be_nl
      << "::CIAO::Session_Container_ptr c," << be_nl
      << "const char * ins_name)" << be_uidt_nl
      << "{" << be_idt_nl
      << global << sname << "::CCM_" << lname
      << "_var x =" << be_idt_nl
      << global << sname << "::CCM_" << lname
      << "::_narrow (p);" << be_uidt_nl << be_nl
      << "::PortableServer::Servant retval = 0;" << be_nl
      << "if (! ::CORBA::is_nil (x.in ()))" << be_idt_nl
      << "{" << be_idt_nl
      << "ACE_NEW_NORETURN (retval," << be_nl
      << "                  " << lname << "_Servant (" << be_idt_nl
      << "                  x.in ()," << be_nl
      << "                  ::Components::CCMHome::_nil ()," << be_nl
      << "                  ins_name," << be_nl
      << "                  0," << be_nl
      << "                  c));" << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "return retval;" << be_uidt_nl
      << "}";
}

