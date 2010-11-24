
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
  if (this->t_args_->size () > 0)
    {
      os_ << be_nl
          << this->node_->local_name () << "_exec_i::"
          << this->node_->local_name () << "_exec_i (void)"
          << be_idt_nl
          << ": " << this->base_tname_ << "_Connector_T";

      os_ << " <" << be_idt << be_idt_nl;

      os_ << "DDS_" << this->node_->local_name ()
          << "_Traits," << be_nl;
      size_t slot = 1UL;

      for (FE_Utils::T_ARGLIST::CONST_ITERATOR i (*this->t_args_);
          !i.done ();
          i.advance (), ++slot)
        {
          AST_Decl **item = 0;
          i.next (item);
          AST_Decl *d = *item;

          UTL_ScopedName *dt_name = d->name ();
          if (this->is_dds_type (node, d))
            {
              os_ << dt_name
                  << "_DDS_Traits,";
            }
          else
            {
              os_ << dt_name << ",";
            }

          AST_Structure *s = AST_Structure::narrow_from_decl (d);
          if (s == 0)
            {
              AST_Typedef *td = AST_Typedef::narrow_from_decl (d);

              if (td != 0)
                {
                  s = AST_Structure::narrow_from_decl (td->primitive_base_type ());
                }
            }
          if (s && s->size_type () == AST_Type::FIXED)
            {
              os_ << be_nl << "true";
            }
          else
            {
              os_ << be_nl << "false";
            }
          if (slot < this->t_args_->size ())
            {
              os_ << "," << be_nl;
            }
        }

      os_ << ">";

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

    }

  os_ << be_uidt_nl
      << "}";

  return 0;
}

