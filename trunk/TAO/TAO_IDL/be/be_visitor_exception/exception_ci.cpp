//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    exception_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Exception in the inline file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_exception, 
           exception_ci, 
           "$Id$")


// ******************************************************
// for client inline
// ******************************************************

be_visitor_exception_ci::be_visitor_exception_ci (be_visitor_context *ctx)
  : be_visitor_exception (ctx)
{
}

be_visitor_exception_ci::~be_visitor_exception_ci (void)
{
}

// visit the Exception node and its scope
int be_visitor_exception_ci::visit_exception (be_exception *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (!node->cli_inline_gen () && !node->imported ())
    {
      os = this->ctx_->stream ();
      os->indent ();

      *os << "// *************************************************************"
          << be_nl;
      *os << "// Inline operations for exception " << node->name () << be_nl;
      *os << "// *************************************************************\n\n";

      // Generate inline code  required of any anonymous types of members.
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_exception::"
                             "visit_exception -"
                             "code for inline failed\n"), 
                            -1);
        }

      node->cli_inline_gen (I_TRUE);
    }

  return 0;
}
