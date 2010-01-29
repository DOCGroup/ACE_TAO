//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    factory_svs.cpp
//
// = DESCRIPTION
//    Visitor generating code for home factores and finders
//    in the servant source.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

be_visitor_factory_svs::be_visitor_factory_svs (
      be_visitor_context *ctx,
      be_home *h_node,
      AST_Component *comp,
      bool for_finder)
  : be_visitor_scope (ctx),
    h_node_ (h_node),
    comp_ (comp),
    os_ (*ctx->stream ()),
    for_finder_ (for_finder),
    unused_args_(false)
{
}

be_visitor_factory_svs::~be_visitor_factory_svs (void)
{
}

int
be_visitor_factory_svs::visit_factory (be_factory *node)
{
  os_ << be_nl << be_nl
      << "::" << comp_->name () << "_ptr" << be_nl
      << h_node_->original_local_name ()->get_string ()
      << "_Servant::" << node->local_name ();
  
  return 0;
}

