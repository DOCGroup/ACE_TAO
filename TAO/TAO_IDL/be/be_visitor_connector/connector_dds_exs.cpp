
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
  // This is initialized in the base class to svnt_export_macro()
  // or skel_export_macro(), since there are many more visitor
  // classes generating servant code. So we can just override
  // all that here.
  export_macro_ = be_global->conn_export_macro ();
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

  node_ = node;

  AST_Connector *base = node->base_connector ();

  // Shaky logic that will have to be improved. If our
  // base connector does not come from an instantiated
  // template module, we skip the code generation.
  this->process_template_args (base);

  if (this->t_args_ == 0)
    {
      this->process_template_args (node);
      
      if (this->t_args_ == 0)
        {
          return 0;
        }
    }

  /// CIDL-generated namespace used 'CIDL_' + composition name.
  /// Now we use 'CIAO_' + component's flat name.
  os_ << be_nl << be_nl
      << "namespace CIAO_" << node->flat_name ()
      << "_Impl" << be_nl
      << "{" << be_idt;

  ACE_CString lname = node->local_name ();  
  const char *base_tname = 0;
  
  if (lname == "DDS_Event" || lname == "DDS_State")
    {
      base_tname = lname.c_str ();
    }
  else
    {
      base_tname =
        node->base_connector ()->local_name ()->get_string ();
    }
    
  os_ << be_nl
      << this->node_->local_name () << "_exec_i::"
      << this->node_->local_name () << "_exec_i (void)"
      << be_idt_nl
      << ": " << base_tname << "_Connector_T";

  AST_Decl **datatype = 0;
  int status = this->t_args_->get (datatype, 0UL);

  if (status != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("be_visitor_connector_dds_exh::")
                  ACE_TEXT ("gen_dds_traits - ")
                  ACE_TEXT ("template arg not found\n ")));

      return -1;
    }

  AST_Type *ut = AST_Type::narrow_from_decl (*datatype);

  /// Assumes parent connector exists and is either DDS_State
  /// or DDS_Event, so we generate inheritance from the
  /// corresponding template. May have to generalize this logic.
  os_ << " <" << be_idt << be_idt_nl
      << this->dds_traits_name_.c_str () << "," << be_nl
      << "DDS" << this->node_->local_name () << "_Traits," << be_nl;

  if (ut->size_type () == AST_Type::FIXED)
    {
      os_ << "true> ";
    }
  else
    {
      os_ << "false> ";
    }
  os_ << "()"
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

