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
//    Visitor generating code for Enums for the Any operators
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_enum, 
           any_op_ch, 
           "$Id$")

// ***************************************************************************
// Enum visitor for generating Any operator declarations in the client header
// ***************************************************************************

be_visitor_enum_any_op_ch::be_visitor_enum_any_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_scope (ctx)
{
}

be_visitor_enum_any_op_ch::~be_visitor_enum_any_op_ch (void)
{
}

int
be_visitor_enum_any_op_ch::visit_enum (be_enum *node)
{
  if (node->cli_hdr_any_op_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // Generate the Any <<= and >>= operators.
  *os << be_nl << be_nl 
      << be_global->stub_export_macro () << " void"
      << " operator<<= (CORBA::Any &, " << node->name ()
      << ");" << be_nl;
  *os << be_global->stub_export_macro () << " CORBA::Boolean"
      << " operator>>= (const CORBA::Any &, "
      << node->name () << " &);";

  node->cli_hdr_any_op_gen (1);
  return 0;
}
