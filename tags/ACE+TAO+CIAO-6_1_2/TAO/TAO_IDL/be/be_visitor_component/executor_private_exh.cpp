
//=============================================================================
/**
 *  @file    executor_private_exh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for private facet members
 *  in the exec impl header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "component.h"

be_visitor_executor_private_exh::be_visitor_executor_private_exh (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx),
    do_provides_ (true),
    do_attribute_(true)
{
}

be_visitor_executor_private_exh::~be_visitor_executor_private_exh (void)
{
}

int
be_visitor_executor_private_exh::visit_provides (be_provides *node)
{
  if (!this->do_provides_)
    return 0;

  be_type *impl = node->provides_type ();
  AST_Decl *i_scope = ScopeAsDecl (impl->defined_in ());
  bool is_global = (i_scope->node_type () == AST_Decl::NT_root);
  const char *smart_scope = (is_global ? "" : "::");
  ACE_CString scope_name =
    IdentifierHelper::orig_sn (i_scope->name (), false);

  os_ << be_nl_2;
  os_ << "/// Object reference to "<< this->ctx_->port_prefix ().c_str ()
      << node->original_local_name () << " facet";

  os_ << be_nl
      << smart_scope << scope_name.c_str () << "::CCM_"
      << impl->original_local_name () << "_var ciao_"
      << this->ctx_->port_prefix ().c_str ()
      << node->original_local_name () << "_;";

  return 0;
}

int
be_visitor_executor_private_exh::visit_attribute (be_attribute *node)
{
  if (!this->do_attribute_)
    return 0;

  AST_Decl::NodeType nt = this->node_->node_type ();

  // Executor attribute code generated for porttype attributes
  // always in connectors and only for mirrorports in components.
  if (this->in_ext_port_ && nt == AST_Decl::NT_component)
    {
      return 0;
    }

  os_ << be_nl_2;

  os_ << "/// Class member storing value of "<< node->local_name () << " attribute" << be_nl;

  be_visitor_member_type_decl v (this->ctx_);

  if (node->field_type ()->accept (&v) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_executor_private_exh::")
                         ACE_TEXT ("visit_attribute - ")
                         ACE_TEXT ("accept on field type failed\n")),
                        -1);
    }

  os_ << this->ctx_->port_prefix ().c_str ()
      << node->local_name () << "_;";

  return 0;
}

void
be_visitor_executor_private_exh::set_flags (bool do_provides,
                                            bool do_attribute)
{
  this->do_provides_ = do_provides;
  this->do_attribute_ = do_attribute;
}
