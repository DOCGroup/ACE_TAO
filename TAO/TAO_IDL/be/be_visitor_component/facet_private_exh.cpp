
//=============================================================================
/**
 *  @file    facet_private_exh.cpp
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

be_visitor_facet_private_exh::be_visitor_facet_private_exh (
      be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_facet_private_exh::~be_visitor_facet_private_exh (void)
{
}

int
be_visitor_facet_private_exh::visit_provides (be_provides *node)
{
  be_type *impl = node->provides_type ();
  AST_Decl *i_scope = ScopeAsDecl (impl->defined_in ());
  bool is_global = (i_scope->node_type () == AST_Decl::NT_root);
  const char *smart_scope = (is_global ? "" : "::");
  ACE_CString scope_name =
    IdentifierHelper::orig_sn (i_scope->name (), false);

  os_ << be_nl
      << smart_scope << scope_name.c_str () << "::CCM_"
      << impl->original_local_name () << "_var ciao_"
      << this->ctx_->port_prefix ().c_str ()
      << node->original_local_name () << "_;";

  return 0;
}

