//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    eventtype_fwd_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for EventTypeFwd node in the client header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_eventtype_fwd, 
           eventtype_fwd_ch, 
           "$Id$")

be_visitor_eventtype_fwd_ch::be_visitor_eventtype_fwd_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_eventtype_fwd_ch::~be_visitor_eventtype_fwd_ch (void)
{
}

int
be_visitor_eventtype_fwd_ch::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // All we do in this is generate a forward declaration of the 
  // corresponding consumer interface class.
  *os << be_nl << be_nl << "class " << node->local_name () << "Consumer;";

  // Generate the ifdefined macro for the _ptr type.
  os->gen_ifdef_macro (node->flat_name (), "Consumer_ptr");

  // Generate the _ptr typedef.
  *os << be_nl << be_nl 
      << "typedef " << node->local_name () << "Consumer *" << node->local_name ()
      << "Consumer_ptr;";

  os->gen_endif ();

  be_visitor_context ctx (*this->ctx_);
  be_visitor_valuetype_fwd_ch visitor (&ctx);
  return visitor.visit_valuetype_fwd (node);
}
