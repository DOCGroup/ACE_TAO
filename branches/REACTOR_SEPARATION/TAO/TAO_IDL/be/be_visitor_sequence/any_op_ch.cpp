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
//    Visitor generating code for Any operators for the Sequence
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_sequence, 
           any_op_ch, 
           "$Id$")

// ***************************************************************************
// Sequence visitor for generating Any operator declarations in the client header
// ***************************************************************************

be_visitor_sequence_any_op_ch::be_visitor_sequence_any_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_any_op_ch::~be_visitor_sequence_any_op_ch (void)
{
}

int
be_visitor_sequence_any_op_ch::visit_sequence (be_sequence *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Generate the Any <<= and >>= operators.
  *os << be_global->stub_export_macro ();
  *os << " void"
      << " operator<<= (CORBA::Any &, const ";
  *os << node->name ();
  *os << " &); // copying version" << be_nl;
  *os << be_global->stub_export_macro ();
  *os << " void"
      << " operator<<= (CORBA::Any &, ";
  *os << node->name ();
  *os << "*); // noncopying version" << be_nl;
  *os << be_global->stub_export_macro ();
  *os << " CORBA::Boolean"
      << " operator>>= (const CORBA::Any &, ";
  *os << node->name ();
  *os << " *&); // deprecated" << be_nl;
  *os << be_global->stub_export_macro ();
  *os << " CORBA::Boolean"
      << " operator>>= (const CORBA::Any &, const ";
  *os << node->name ();
  *os << " *&);";

  node->cli_hdr_any_op_gen (1);
  return 0;
}
