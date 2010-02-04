
//=============================================================================
/**
 *  @file    connector_dds_exs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Connectors in the exec impl source.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


be_visitor_connector_dds_exs::be_visitor_connector_dds_exs (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
  // This is initialized in the base class to svnt_export_macro()
  // or skel_export_macro(), since there are many more visitor
  // classes generating servant code. So we can just override
  // all that here.
  export_macro_ = be_global->exec_export_macro ();
}

be_visitor_connector_dds_exs::~be_visitor_connector_dds_exs (void)
{
}

int
be_visitor_connector_dds_exs::visit_connector (be_connector *node)
{
  node_ = node;

  /// CIDL-generated namespace used 'CIDL_' + composition name.
  /// Now we use 'CIAO_' + component's flat name.
  os_ << be_nl << be_nl
      << "namespace CIAO_" << node->flat_name ()
      << "_Impl" << be_nl
      << "{" << be_idt;
      
  // TODO - template class constructor and destructor.
      
  this->gen_exec_entrypoint_defn ();
     
  os_ << be_uidt_nl
      << "}";
     
  return 0;
}

