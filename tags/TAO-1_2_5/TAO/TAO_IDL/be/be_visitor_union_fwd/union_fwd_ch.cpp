//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    union_fwd_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for be_union_fwd node in the client header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_union_fwd, 
           union_fwd_ch, 
           "$Id$")

be_visitor_union_fwd_ch::be_visitor_union_fwd_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_union_fwd_ch::~be_visitor_union_fwd_ch (void)
{
}

// Visit the interface_fwd_ch node and its scope.
int
be_visitor_union_fwd_ch::visit_union_fwd (be_union_fwd *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // Generate a forward declaration of the class.
  *os << be_nl << be_nl
      << "class " << node->local_name () << ";";

  node->cli_hdr_gen (I_TRUE);
  return 0;
}
