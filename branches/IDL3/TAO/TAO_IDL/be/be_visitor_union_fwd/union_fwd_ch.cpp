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
  TAO_OutStream *os = this->ctx_->stream ();

  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  // Generate a forward declaration of the class.
  *os << "class " << node->local_name () << ";" << be_nl;

  node->cli_hdr_gen (I_TRUE);

  return 0;
}
