// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    module_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Module in the client header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_module, 
           module_ch, 
           "$Id$")

// ******************************************************
// Module visitor for client header
// ******************************************************

be_visitor_module_ch::be_visitor_module_ch (be_visitor_context *ctx)
  : be_visitor_module (ctx)
{
}

be_visitor_module_ch::~be_visitor_module_ch (void)
{
}

int
be_visitor_module_ch::visit_module (be_module *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << "TAO_NAMESPACE "
      << " " << node->local_name () << be_nl
      << "{" << be_idt;

  // Generate code for the module definition by traversing thru the
  // elements of its scope. We depend on the front-end to have made sure
  // that only legal syntactic elements appear in our scope.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module_ch::"
                         "visit_module - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  *os << be_uidt_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl;

  *os << be_nl
      << "}\nTAO_NAMESPACE_CLOSE // module " << node->name ();

  return 0;
}
