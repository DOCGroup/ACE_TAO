
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

  AST_Connector *base = node->base_connector ();

  // Hack for the time being to skip codegen for DDS_State
  // and DDS_Event, both of which come along with the template
  // module instantiation.
  if (base == 0)
    {
      return 0;
    }

  node_ = node;

  // More shaky logic that will have to be improved. If our
  // base connector does not come from an instantiated
  // template module, we skip the code generation.
  this->process_template_args (base);

  if (this->t_args_ == 0)
    {
      return 0;
    }

  /// CIDL-generated namespace used 'CIDL_' + composition name.
  /// Now we use 'CIAO_' + component's flat name.
  os_ << be_nl << be_nl
      << "namespace CIAO_" << node->flat_name ()
      << "_Impl" << be_nl
      << "{" << be_idt;

  this->gen_dds_traits ();
  this->gen_connector_traits ();

  if (this->t_args_ == 0)
    {
      return 0;
    }

  os_ << be_nl << be_nl
      << "class " << this->export_macro_.c_str () << " "
      << this->node_->local_name () << "_exec_i" << be_idt_nl
      << ": public ";

  // Placeholder for forthcoming real-world logic.
  bool dds_event_connector = true;

  if (dds_event_connector)
    {
      os_ << "DDS_Event_Connector_T";
    }

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

  UTL_ScopedName *dt_name = (*datatype)->name ();

  AST_Type *ut = AST_Type::narrow_from_decl (*datatype);

  os_ << " <" << be_idt << be_idt_nl
      << this->dds_traits_name_.c_str () << "," << be_nl
      << "DDS" << this->node_->local_name ()
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
  int status = this->t_args_->get (datatype, 0UL);

  if (status != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("be_visitor_connector_dds_exh::")
                  ACE_TEXT ("gen_dds_traits - ")
                  ACE_TEXT ("template arg not found\n ")));

      return;
    }

  UTL_ScopedName *dt_name = (*datatype)->name ();

  // More generic logic TBI.
  bool rti_dds_connector = true;

  if (rti_dds_connector)
    {
      os_ << be_nl
          << "typedef CIAO::DDS4CCM::RTI::Type_Traits <"
          << be_idt_nl
          << "::" << dt_name << "," << be_nl
          << "::" << dt_name << "Seq," << be_nl
          << "::" << dt_name << "TypeSupport," << be_nl
          << "::" << dt_name << "DataWriter," << be_nl
          << "::" << dt_name << "DataReader>";
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
      << "::" << ScopeAsDecl (this->node_->defined_in ())->name ()
      << "::CCM_" << this->node_->local_name ()
      << "_Context," << be_nl
      << "::" << this->t_inst_->name ()
      << "::Listener," << be_nl
      << "::" << this->t_inst_->name ()
      << "::StateListener," << be_nl
      << "::CCM_DDS::ConnectorStatusListener> DDS"
      << this->node_->local_name () << "_Traits;";
}


