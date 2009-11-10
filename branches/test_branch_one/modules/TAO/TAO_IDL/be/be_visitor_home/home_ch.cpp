//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    home_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Component Home node in the client header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_home, 
           home_ch, 
           "$Id$")

be_visitor_home_ch::be_visitor_home_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_home_ch::~be_visitor_home_ch (void)
{
}

int
be_visitor_home_ch::visit_home (be_home *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // All we do in this is generate a forward declaration of the class.
  *os << be_nl << be_nl << "class " << node->local_name () << ";";

  // Generate the _ptr typedef.
  *os << be_nl << be_nl 
      << "typedef " << node->local_name () << " *" << node->local_name ()
      << "_ptr;";

    if (be_global->tc_support ())
      {
        be_visitor_context ctx (*this->ctx_);
        ctx.node (node);
        be_visitor_typecode_decl tc_visitor (&ctx);

        if (node->accept (&tc_visitor) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                              "(%N:%l) be_visitor_home_ch::"
                              "visit_home - "
                              "TypeCode definition failed\n"),
                              -1);
          }
      }

  node->cli_hdr_gen (true);
  return 0;
}
