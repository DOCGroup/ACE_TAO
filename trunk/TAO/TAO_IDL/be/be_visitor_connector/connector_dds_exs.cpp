
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
  : be_visitor_connector_dds_ex_base (ctx)
{
}

be_visitor_connector_dds_exs::~be_visitor_connector_dds_exs (void)
{
}

int
be_visitor_connector_dds_exs::visit_connector (be_connector *node)
{
  if (node->imported ())
    {
      return 0;
    }
    
  AST_Connector *base = node->base_connector ();
  
  // Hack for the time being to skip codegen for DDS_State
  // and DDS_Event, both of which come along with the template
  // module instantiation.
  if (base == 0)
    {
      return 0;
    }
    
  node_ = node;

  // Stores several values in base class members for later use.  
  this->process_template_args (base);
      
  /// CIDL-generated namespace used 'CIDL_' + composition name.
  /// Now we use 'CIAO_' + component's flat name.
  os_ << be_nl << be_nl
      << "namespace CIAO_" << node->flat_name ()
      << "_Impl" << be_nl
      << "{" << be_idt;
      
  os_ << be_nl
      << this->node_->local_name () << "_exec_i::"
      << this->node_->local_name () << "_exec_i (void)"
      << be_idt_nl
      << ": ";
      
  // Placeholder for forthcoming real-world logic.
  bool dds_event_connector = true;
  
  if (dds_event_connector)
    {
      os_ << "DDS_Event_Connector_T";
    }
    
  os_ << " <" << be_idt << be_idt_nl
      << this->dds_traits_name_.c_str () << "," << be_nl
      << this->node_->local_name () << "_Traits> ()"
      << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "}";
      
  os_ << be_nl << be_nl
      << this->node_->local_name () << "_exec_i::~"
      << this->node_->local_name () << "_exec_i (void)" << be_nl
      << "{" << be_nl
      << "}";
    
  this->gen_exec_entrypoint_defn ();
     
  os_ << be_uidt_nl
      << "}";
     
  return 0;
}

