
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

#include "global_extern.h"
#include "utl_strlist.h"
#include "utl_string.h"

#include "ast_structure.h"
#include "ast_union.h"
#include "ast_template_module_ref.h"
#include "ast_template_module.h"

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

  // If we have a connector within a templated module
  if (! this->t_args_.is_empty ())
    {
      // Generate all needed dds_traits
      for (FE_Utils::T_ARGLIST::CONST_ITERATOR i (this->t_args_);
          !i.done ();
          i.advance ())
        {
          AST_Decl **item = 0;
          i.next (item);
          AST_Decl *d = *item;

          if (this->is_dds_type (node, d))
            {
              this->gen_dds_traits (d);
            }
        }

      // Generate connector traits
      this->gen_connector_traits ();

      os_ << be_nl_2
          << "class " << this->export_macro_.c_str () << " "
          << this->node_->local_name () << "_exec_i" << be_idt_nl
          << ": public " << this->base_tname_ << "_Connector_T";

      os_ << " <" << be_idt << be_idt_nl;

      os_ << "CCM_" << this->node_->flat_name ()
          << "_Traits," << be_nl;

      size_t slot = 1UL;

      for (FE_Utils::T_ARGLIST::CONST_ITERATOR i (this->t_args_);
          !i.done ();
          i.advance (), ++slot)
        {
          AST_Decl **item = 0;
          i.next (item);
          AST_Decl *d = *item;

          if (this->is_dds_type (node, d))
            {
              os_ << d->flat_name ()
                  << "_DDS_Traits";
            }
          else
            {
              os_ << d->name ();
            }

          bool needs_bool = false;
          bool is_fixed = false;
          FE_Utils::T_Param_Info *param = 0;

          if (this->t_params_->get (param, slot - 1) != 0)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 ACE_TEXT ("be_visitor_connector_dds_exh::")
                                 ACE_TEXT ("visit_connector - ")
                                 ACE_TEXT ("template param fetch failed\n ")),
                                -1);
            }

          if (d->node_type () == AST_Decl::NT_typedef)
            {
              /// Strip away all layers of typedef before narrowing.
              AST_Typedef *td = AST_Typedef::narrow_from_decl (d);
              d = td->primitive_base_type ();
            }

          /// No need to check if this is 0, but must narrow
          /// to call virtual function size_type() below.
          AST_Type *t = AST_Type::narrow_from_decl (d);

          switch (param->type_)
            {
              case AST_Decl::NT_type:
              case AST_Decl::NT_struct:
              case AST_Decl::NT_union:
                needs_bool = true;
                is_fixed = (t->size_type () == AST_Type::FIXED);
                break;
              default:
                break;
            }

          if (needs_bool)
            {
              os_ << "," << be_nl
                  << (is_fixed ? "true" : "false");
            }

          if (slot < this->t_args_.size ())
            {
              os_ << "," << be_nl;
            }
        }

      os_ << ">";

      os_ << be_uidt << be_uidt << be_uidt_nl
          << "{" << be_nl
          << "public:" << be_idt_nl
          << this->node_->local_name () << "_exec_i (void);" << be_nl
          << "virtual ~" << this->node_->local_name ()
          << "_exec_i (void);" << be_uidt_nl
          << "};";

      this->gen_exec_entrypoint_decl ();

    }

  os_ << be_uidt_nl
      << "}";

  /// Unset the flags in the port interfaces list. This is
  /// also done in visit_mirror_port(), but we must also do
  /// it here to catch a port interface that didn't come to
  /// us from an extended port or mirror port.
  for (ACE_Unbounded_Queue<be_interface *>::ITERATOR iter (
        this->port_ifaces_);
      !iter.done ();
      iter.advance ())
    {
      be_interface **item = 0;
      iter.next (item);

      (*item)->dds_connector_traits_done (false);
    }

  return 0;
}

int
be_visitor_connector_dds_exh::visit_mirror_port (
  be_mirror_port *node)
{
  os_ << be_nl
      << "struct " << node->local_name ()->get_string ()
      << "_traits" << be_nl
      << "{" << be_idt;

  /// Reuse the code in the base class to flip the
  /// facet/receptacle elements in the scope. We will end up
  /// in visit_provides() and visit_uses() below.
  int status =
    this->be_visitor_component_scope::visit_mirror_port (node);

  if (status != 0)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_connector_dds_exh::")
                         ACE_TEXT ("visit_mirror_port - ")
                         ACE_TEXT ("base class traversal failed\n ")),
                        -1);
    }

  os_ << be_uidt_nl
      << "};" << be_nl;

  /// Unset the flags in the port interfaces list so
  /// they can be used again in another port.
  for (ACE_Unbounded_Queue<be_interface *>::ITERATOR iter (
         this->port_ifaces_);
       !iter.done ();
       iter.advance ())
    {
      be_interface **item = 0;
      iter.next (item);

      (*item)->dds_connector_traits_done (false);
    }

  return 0;
}

int
be_visitor_connector_dds_exh::visit_provides (be_provides *node)
{
  be_interface *iface =
    be_interface::narrow_from_decl (node->provides_type ());

  this->gen_interface_connector_trait (iface, node, true);

  return 0;
}

int
be_visitor_connector_dds_exh::visit_uses (be_uses *node)
{
  be_interface *iface =
    be_interface::narrow_from_decl (node->uses_type ());

  this->gen_interface_connector_trait (iface, node, false);

  return 0;
}

int
be_visitor_connector_dds_exh::visit_attribute (
  be_attribute *node)
{
  AST_Decl::NodeType nt =
    ScopeAsDecl (node->defined_in ())->node_type ();

  /// We are interested in attributes in extended ports
  /// and connectors, skip all others.
  if (nt == AST_Decl::NT_component)
    {
      return 0;
    }

  os_ << be_nl
      << "typedef "
      << node->field_type ()->full_name () << " "
      << node->local_name ()->get_string () << "_type;";

  return 0;
}

void
be_visitor_connector_dds_exh::gen_dds_traits (AST_Decl *datatype)
{
  AST_Decl *comp_scope =
    ScopeAsDecl (this->node_->defined_in ());

  bool const global_comp =
    (comp_scope->node_type () == AST_Decl::NT_root);

  UTL_ScopedName *dt_name = datatype->name ();
  BE_GlobalData::DDS_IMPL the_dds_impl = be_global->dds_impl ();

  if (the_dds_impl != BE_GlobalData::NONE)
    {
      os_ << be_nl
          << "struct " << datatype->flat_name () << "_DDS_Traits" << be_nl
          << "{" << be_idt_nl
          << "typedef ::" << dt_name << " value_type;" << be_nl
          << "typedef ::" << dt_name;

      if (the_dds_impl == BE_GlobalData::NDDS)
        {
          os_ << "RTI";
        }
      else if (the_dds_impl == BE_GlobalData::OPENDDS)
        {
          os_ << "DDS";
        }

      os_ << "Seq dds_seq_type;" << be_nl
          << "typedef ::" << dt_name
          << "TypeSupport type_support;" << be_nl
          << "typedef ::DDS_SampleInfoSeq sampleinfo_seq_type;" << be_nl
          << "typedef ::DDS_SampleInfo sampleinfo_type;" << be_nl
          << "typedef ::" << dt_name
          << "DataWriter datawriter_type;" << be_nl
          << "typedef ::" << dt_name
          << "DataReader datareader_type;" << be_nl
          << "typedef " << (global_comp ? "" : "::") << comp_scope->full_name ()
          << "::DataWriter typed_writer_type;" << be_nl
          << "typedef " << (global_comp ? "" : "::") << comp_scope->full_name ()
          << "::DataReader typed_reader_type;" << be_uidt_nl
          << "};";
    }
}

void
be_visitor_connector_dds_exh::gen_connector_traits (void)
{
  AST_Decl *comp_scope =
    ScopeAsDecl (this->node_->defined_in ());

  bool global_comp =
    (comp_scope->node_type () == AST_Decl::NT_root);

  os_ << be_nl_2
      << "struct CCM_" << this->node_->flat_name ()
      << "_Traits" << be_nl
      << "{" << be_idt_nl
      << "typedef ::CIAO_" << this->node_->flat_name () << "_Impl::"
      << this->node_->local_name () << "_Exec base_type;" << be_nl
      << "typedef " << (global_comp ? "" : "::")
      << comp_scope->name () << "::CCM_"
      << this->node_->local_name () << "_Context context_type;"
      << be_nl;

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
  be_field *port_elem,
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
          << (!for_facet ? "" : "CCM_") << lname
          << " " << port_elem->local_name ()->get_string ()
          << "_type;";

      iface->dds_connector_traits_done (true);
      this->port_ifaces_.enqueue_tail (iface);
    }
}
