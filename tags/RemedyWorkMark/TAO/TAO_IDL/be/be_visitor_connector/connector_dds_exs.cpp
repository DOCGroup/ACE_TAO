
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

  // If we have a connector within a templated module
  if (! this->t_args_.is_empty ())
    {
      os_ << be_nl
          << this->node_->local_name () << "_exec_i::"
          << this->node_->local_name () << "_exec_i (void)"
          << be_idt_nl
          << ": " << this->base_tname_ << "_Connector_T";

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

      os_ << "> ()"
          << be_uidt << be_uidt << be_uidt_nl
          << "{" << be_nl
          << "}";

      os_ << be_nl_2
          << this->node_->local_name () << "_exec_i::~"
          << this->node_->local_name () << "_exec_i (void)" << be_nl
          << "{" << be_nl
          << "}";

      this->gen_exec_entrypoint_defn ();

    }

  os_ << be_uidt_nl
      << "}";

  return 0;
}

