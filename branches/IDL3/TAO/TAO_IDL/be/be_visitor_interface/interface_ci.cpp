//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the client inline file
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_interface, 
           interface_ci, 
           "$Id$")


// **************************************************
// Interface visitor for client inline
// **************************************************

be_visitor_interface_ci::be_visitor_interface_ci (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ci::~be_visitor_interface_ci (void)
{
}

int
be_visitor_interface_ci::visit_interface (be_interface *node)
{
  if (node->imported ())
    {
      return 0;
    }

  // Generate inline methods for elements of our scope.
  // This will always be done here for any non-imported node,
  // whether the _var and _out code has been generated already
  // at a forward declaration or not.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ci::"
                         "visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  // A forward declared interface may have set this flag.
  if (node->cli_inline_gen ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl;

  os->gen_ifdef_macro (node->flat_name (), "");

  if (node->is_abstract ())
    {
      *os << "ACE_INLINE" << be_nl
          << node->name () << "::" << node->local_name () 
          << " (void)" << be_idt_nl
          << ": CORBA_AbstractBase ()" << be_uidt_nl
          << "{}" << be_nl << be_nl;

      *os << "ACE_INLINE" << be_nl
          << node->name () << "::" << node->local_name () 
          << " (const " << node->local_name () << " &rhs)" << be_idt_nl
          << ": CORBA_AbstractBase (rhs)" << be_uidt_nl
          << "{}" << be_nl << be_nl;

      *os << "ACE_INLINE" << be_nl
          << node->name () << "::" << node->local_name ()
          << " (" << be_idt << be_idt_nl
          << "TAO_Stub *objref," << be_nl
          << "CORBA::Boolean _tao_collocated," << be_nl
          << "TAO_Abstract_ServantBase *servant" << be_uidt_nl
          << ")" << be_nl
          << ": CORBA_AbstractBase (objref, _tao_collocated, servant)"
          << be_uidt_nl
          << "{}" << be_nl;
    }
  else
    {
      // Generate the constructor from stub and servant.
      node->gen_stub_ctor (os);
    }

  os->gen_endif ();

  return 0;
}
