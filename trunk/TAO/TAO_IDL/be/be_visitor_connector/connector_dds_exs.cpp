
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

  if (!this->begin (node))
    {
      return -1;
    }

  os_ << be_nl
      << this->node_->local_name () << "_exec_i::"
      << this->node_->local_name () << "_exec_i (void)"
      << be_idt_nl
      << ": " << this->base_tname_ << "_Connector_T";

  AST_Decl **datatype = 0;
  int const status = this->t_args_->get (datatype, 0UL);

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
      << "DDS_" << this->node_->local_name () << "_Traits," << be_nl
      << this->dds_traits_name_.c_str () << "," << be_nl;

  if (ut->size_type () == AST_Type::FIXED)
    {
      os_ << "true>";
    }
  else
    {
      os_ << "false>";
    }

  os_ << "()"
      << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "}";

  os_ << be_nl_2
      << this->node_->local_name () << "_exec_i::~"
      << this->node_->local_name () << "_exec_i (void)" << be_nl
      << "{" << be_nl
      << "}";

  this->gen_exec_entrypoint_defn ();

  os_ << be_uidt_nl
      << "}";

  return 0;
}

