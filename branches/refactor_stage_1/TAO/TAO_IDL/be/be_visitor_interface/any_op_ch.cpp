//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    any_op_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Any operators for an Interface in the client
//    header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_interface, 
           any_op_ch, 
           "$Id$")

// ***************************************************************************
// Interface visitor for generating Any operator declarations in the client header
// ***************************************************************************

be_visitor_interface_any_op_ch::be_visitor_interface_any_op_ch
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_any_op_ch::~be_visitor_interface_any_op_ch (void)
{
}

int
be_visitor_interface_any_op_ch::visit_interface (be_interface *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << be_global->stub_export_macro () << " void"
      << " operator<<= (CORBA::Any &, " << node->name ()
      << "_ptr); // copying" << be_nl;
  *os << be_global->stub_export_macro () << " void"
      << " operator<<= (CORBA::Any &, " << node->name ()
      << "_ptr *); // non-copying" << be_nl;
  *os << be_global->stub_export_macro () << " CORBA::Boolean"
      << " operator>>= (const CORBA::Any &, "
      << node->name () << "_ptr &);";

  // All we have to do is to visit the scope and generate code.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_any_op_ch::"
                         "visit_interface - "
                         "codegen for scope failed\n"), 
                        -1);
    }


  node->cli_hdr_any_op_gen (1);
  return 0;
}
