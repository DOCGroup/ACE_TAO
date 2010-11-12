
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

#include "nr_extern.h"

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

  /// Unset the flags in the port interfaces list so
  /// they can be used again in another connector.
  for (ACE_Unbounded_Queue<be_interface *>::ITERATOR iter (
         this->port_ifaces_);
       !iter.done ();
       iter.advance ())
    {
      be_interface **item = 0;
      iter.next (item);

      (*item)->dds_connector_traits_done (false);
    }

  /// Assumes parent connector exists and is either DDS_State
  /// or DDS_Event, so we generate inheritance from the
  /// corresponding template. May have to generalize this logic.
  os_ << be_nl_2
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
      os_ << "true>";
    }
  else
    {
      os_ << "false>";
    }

  os_ << be_uidt << be_uidt << be_uidt_nl
      << "{" << be_nl
      << "public:" << be_idt_nl
      << this->node_->local_name () << "_exec_i (void);" << be_nl
      << "virtual ~" << this->node_->local_name ()
      << "_exec_i (void);" << be_uidt_nl
      << "};";

  this->gen_exec_entrypoint_decl ();

  os_ << be_uidt_nl
      << "}";

  return 0;
}

int
be_visitor_connector_dds_exh::visit_provides (be_provides *node)
{
  be_interface *iface =
    be_interface::narrow_from_decl (node->provides_type ());

  this->gen_interface_connector_trait (iface, true);

  return 0;
}

int
be_visitor_connector_dds_exh::visit_uses (be_uses *node)
{
  be_interface *iface =
    be_interface::narrow_from_decl (node->uses_type ());

  this->gen_interface_connector_trait (iface, false);

  return 0;
}

void
be_visitor_connector_dds_exh::gen_dds_traits (void)
{
  /// We depend on the DDS datatype being the first template
  /// argument for now, this may change.
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

  AST_Decl *comp_scope =
    ScopeAsDecl (this->node_->defined_in ());

  UTL_ScopedName *dt_name = (*datatype)->name ();
  BE_GlobalData::DDS_IMPL the_dds_impl = be_global->dds_impl ();

  if (the_dds_impl != BE_GlobalData::NONE)
    {
      os_ << be_nl
          << "struct " << this->dds_traits_name_.c_str () << be_nl
          << "{" << be_idt_nl
          << "typedef ::" << dt_name << " value_type;" << be_nl
          << "typedef ::" << dt_name;

      if (the_dds_impl == BE_GlobalData::NDDS)
        {
          os_ << "RTI";
        }

      os_ << "Seq dds_seq_type;" << be_nl
          << "typedef ::" << dt_name << "Seq seq_type;" << be_nl
          << "typedef ::" << dt_name
          << "TypeSupport type_support;" << be_nl
          << "typedef ::DDS_SampleInfoSeq sampleinfo_seq_type;" << be_nl
          << "typedef ::DDS_SampleInfo sampleinfo_type;" << be_nl
          << "typedef ::" << dt_name
          << "DataWriter datawriter_type;" << be_nl
          << "typedef ::" << dt_name
          << "DataReader datareader_type;" << be_nl
          << "typedef ::" << comp_scope->full_name ()
          << "::DataWriter typed_writer_type;" << be_nl
          << "typedef ::" << comp_scope->full_name ()
          << "::DataReader typed_reader_type;" << be_uidt_nl
          << "};";
    }
}

void
be_visitor_connector_dds_exh::gen_connector_traits (void)
{
  AST_Decl **dt = 0;
  int status = this->t_args_->get (dt, 0UL);

  if (status != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("be_visitor_connector_dds_exh::")
                  ACE_TEXT ("gen_connector_traits - ")
                  ACE_TEXT ("first template arg not found\n ")));

      return;
    }

  AST_Decl **dt_seq = 0;
  status = this->t_args_->get (dt_seq, 1UL);

  if (status != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("be_visitor_connector_dds_exh::")
                  ACE_TEXT ("gen_connector_traits - ")
                  ACE_TEXT ("second template arg not found\n ")));

      return;
    }

  AST_Decl *comp_scope =
    ScopeAsDecl (this->node_->defined_in ());

  bool global_comp =
    (comp_scope->node_type () == AST_Decl::NT_root);

  os_ << be_nl_2
      << "struct DDS_" << this->node_->local_name ()
      << "_Traits" << be_nl
      << "{" << be_idt_nl
      << "typedef ::CIAO_" << this->node_->flat_name () << "_Impl::"
      << this->node_->local_name () << "_Exec base_type;" << be_nl
      << "typedef ::" << (*dt)->name () << " value_type;" << be_nl
      << "typedef ::" << (*dt_seq)->name () << " seq_type;" << be_nl
      << "typedef " << (global_comp ? "" : "::")
      << comp_scope->name () << "::CCM_"
      << this->node_->local_name () << "_Context context_type;" << be_nl;

  if (this->visit_component_scope (this->node_) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("be_visitor_connector_dds_exh::")
                  ACE_TEXT ("gen_connector_traits - ")
                  ACE_TEXT ("visit_scope_failed\n ")));

      return;
    }

  os_ << be_uidt_nl
      << "};";
}

void
be_visitor_connector_dds_exh::gen_interface_connector_trait (
  be_interface *iface,
  bool for_facet)
{
  if (!iface->dds_connector_traits_done ())
    {
      AST_Decl *scope = ScopeAsDecl (iface->defined_in ());
      bool global = (scope->node_type () == AST_Decl::NT_root);
      const char *smart_scope = (global ? "" : "::");
      const char *lname = iface->local_name ();

      os_ << be_nl
          << "typedef ::" << scope->name () << smart_scope
          << (for_facet ? "CCM_" : "") << lname
          << " " << tao_cg->downcase (lname) << "_type;";

      iface->dds_connector_traits_done (true);
      this->port_ifaces_.enqueue_tail (iface);
    }
}
