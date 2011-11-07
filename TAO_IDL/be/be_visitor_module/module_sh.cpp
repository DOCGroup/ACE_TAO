
//=============================================================================
/**
 *  @file    module_sh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Module in the server header
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


// ************************************************************
// Module visitor for server header
// ************************************************************

be_visitor_module_sh::be_visitor_module_sh (be_visitor_context *ctx)
  : be_visitor_module (ctx)
{
}

be_visitor_module_sh::~be_visitor_module_sh (void)
{
}

int
be_visitor_module_sh::visit_module (be_module *node)
{
  // Not generated and not imported.
  if (node->srv_hdr_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  // Generate the skeleton class name.

  // Now generate the class definition. The prefix POA_ is prepended to our
  // name only if we are the outermost module.
  *os << "namespace ";

  if (!node->is_nested ())
    {
      // We are outermost module.
      *os << "POA_" << node->local_name () << be_nl;
    }
  else
    {
      // We are inside another module.
      *os << node->local_name () << be_nl;
    }

  *os << "{" << be_idt_nl;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_module_sh::"
                         "visit_module - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << be_uidt_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "} // module "
      << node->name ();

  return 0;

}
