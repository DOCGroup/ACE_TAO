
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
  
  os_ << be_nl
      << smart_scope << i_scope->full_name () << "::CCM_"
      << impl->local_name () << "_var ciao_"
      << this->port_prefix_.c_str () << node->local_name ()
      << "_;";
  
  return 0;
}

