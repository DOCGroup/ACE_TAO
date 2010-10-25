
//=============================================================================
/**
 *  @file    executor_exs_attr_init.cpp
 *
 *  $Id$
 *
 *  Visitor generating code in the exec impl source for
 *  the initialization of attribute members in the constructor.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_executor_exs_attr_init::be_visitor_executor_exs_attr_init (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx),
    semicolon_generated_ (false)
{
}

be_visitor_executor_exs_attr_init::~be_visitor_executor_exs_attr_init (void)
{
}

int
be_visitor_executor_exs_attr_init::visit_attribute (
  be_attribute *node)
{
  AST_Decl::NodeType nt = this->node_->node_type ();

  // Executor attribute code generated for porttype attributes
  // always in connectors and only for mirrorports in components.
  if (this->in_ext_port_ && nt == AST_Decl::NT_component)
    {
      return 0;
    }

  os_ << this->ctx_->port_prefix ().c_str ()
      << node->local_name () << "_ (";

  be_visitor_attr_init nrt_visitor (this->ctx_);

  int status =
    node->field_type ()->accept (&nrt_visitor);

   if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_executor_exs_attr_init::")
                         ACE_TEXT ("visit_attribute - ")
                         ACE_TEXT ("accept with null value ")
                         ACE_TEXT ("visitor failed\n")),
                        -1);
    }

  os_ << ")";

  return 0;
}

int
be_visitor_executor_exs_attr_init::pre_process (be_decl *node)
{
  AST_Decl::NodeType nt = this->node_->node_type ();

  // Executor attribute code generated for porttype attributes
  // always in connectors and only for mirrorports in components.
  if (this->in_ext_port_ && nt == AST_Decl::NT_component)
    {
      return 0;
    }

  if (node->node_type () == AST_Decl::NT_attr)
    {
      if (this->semicolon_generated_)
        {
          os_ << be_nl
              << ", ";
        }
      else
        {
          os_ << be_idt_nl
              << ": " << be_idt;

          this->semicolon_generated_ = true;
        }
    }

  return 0;
}

bool
be_visitor_executor_exs_attr_init::attr_generated (void) const
{
  return this->semicolon_generated_;
}

