
//=============================================================================
/**
 *  @file    connector_dds_exh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for DDS Connectors in the exec impl header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_connector_dds_exh::be_visitor_connector_dds_exh (
      be_visitor_context *ctx)
  : be_visitor_connector_dds_ex_base (ctx)
{
  // This is initialized in the base class to svnt_export_macro()
  // or skel_export_macro(), since there are many more visitor
  // classes generating servant code. So we can just override
  // all that here.
  export_macro_ = be_global->conn_export_macro ();
}

be_visitor_connector_dds_exh::~be_visitor_connector_dds_exh (void)
{
}

int
be_visitor_connector_dds_exh::visit_connector (be_connector *node)
{
  if (node->imported ())
    {
      return 0;
    }

  if (!this->begin (node))
    {
      return -1;
    }
    
  this->gen_dds_traits ();
  this->gen_connector_traits ();
  
  /// Assumes parent connector exists and is either DDS_State
  /// or DDS_Event, so we generate inheritance from the
  /// corresponding template. May have to generalize this logic.
  os_ << be_nl << be_nl
      << "class " << this->export_macro_.c_str () << " "
      << this->node_->local_name () << "_exec_i" << be_idt_nl
      << ": public " << this->base_tname_ << "_Connector_T";

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

  os_ << " <" << be_idt << be_idt_nl
      << this->dds_traits_name_.c_str () << "," << be_nl
      << "DDS_" << this->node_->local_name ()
      << "_Traits," << be_nl;

  if (ut->size_type () == AST_Type::FIXED)
    {
      os_ << "true> ";
    }
  else
    {
      os_ << "false> ";
    }

  os_ << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << this->node_->local_name () << "_exec_i (void);" << be_nl
      << "virtual ~" << this->node_->local_name ()
      << "_exec_i (void);";

  if (this->visit_scope (node) != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_connector_dds_exh::")
                         ACE_TEXT ("visit_connector - ")
                         ACE_TEXT ("visit_scope() failed\n ")),
                        -1);
    }

  os_ << be_uidt_nl
      << "};";

  this->gen_exec_entrypoint_decl ();

  os_ << be_uidt_nl
      << "}";

  return 0;
}

void
be_visitor_connector_dds_exh::gen_dds_traits (void)
{
  // We depend on the DDS datatype being the first template
  // argument for now, this may change.
  AST_Decl **datatype = 0;
  int const status = this->t_args_->get (datatype, 0UL);

  if (status != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("be_visitor_connector_dds_exh::")
                  ACE_TEXT ("gen_dds_traits - ")
                  ACE_TEXT ("template arg not found\n ")));

      return;
    }

  UTL_ScopedName *dt_name = (*datatype)->name ();

  if (be_global->dds_impl () != ::BE_GlobalData::NONE)
    {
      os_ << be_nl
          << "typedef ::CIAO::DDS4CCM::Type_Traits <"
          << be_idt_nl
          << "::" << dt_name << "," << be_nl;

      if (be_global->dds_impl () == ::BE_GlobalData::NDDS)
        {
          os_ << "::" << dt_name << "RTISeq," << be_nl;
        }
      else
        {
          os_ << "::" << dt_name << "Seq," << be_nl;
        }

      os_ << "::" << dt_name << "TypeSupport," << be_nl
          << "::" << dt_name << "DataWriter," << be_nl
          << "::" << dt_name << "DataReader> ";
      os_ << this->dds_traits_name_.c_str () << ";" << be_uidt;
    }
}

void
be_visitor_connector_dds_exh::gen_connector_traits (void)
{
  AST_Decl **item = 0;
  int status = this->t_args_->get (item, 1UL);

  if (status != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("be_visitor_connector_dds_exh::")
                  ACE_TEXT ("gen_connector_traits - ")
                  ACE_TEXT ("template arg not found\n ")));

      return;
    }

  AST_Decl *comp_scope =
    ScopeAsDecl (this->node_->defined_in ());

  bool global_comp =
    (comp_scope->node_type () == AST_Decl::NT_root);

  os_ << be_nl << be_nl
      << "typedef ::CIAO::DDS4CCM::Connector_Traits <"
      << be_idt_nl
      << "::CIAO_" << this->node_->flat_name () << "_Impl::"
      << this->node_->local_name () << "_Exec," << be_nl
      << "::" << (*item)->name () << "," << be_nl
      << "::" << this->t_inst_->name ()
      << "::CCM_Writer," << be_nl
      << "::" << this->t_inst_->name ()
      << "::CCM_Updater," << be_nl
      << "::" << this->t_inst_->name ()
      << "::CCM_Getter," << be_nl
      << "::" << this->t_inst_->name ()
      << "::CCM_Reader," << be_nl
      << (global_comp ? "" : "::")
      << comp_scope->name ()
      << "::CCM_" << this->node_->local_name ()
      << "_Context," << be_nl
      << "::" << this->t_inst_->name ()
      << "::Listener," << be_nl
      << "::" << this->t_inst_->name ()
      << "::StateListener," << be_nl
      << "::CCM_DDS::ConnectorStatusListener> DDS_"
      << this->node_->local_name () << "_Traits;"
      << be_uidt;
}


