//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    serializer_op_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Enums for the TAO::DCPS::Serializer operators
//
// = AUTHOR
//    Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_enum, 
           serializer_op_ch, 
           "$Id$")


// ***************************************************************************
// Enum visitor for generating Serializer operator declarations in the client header
// ***************************************************************************

be_visitor_enum_serializer_op_ch::be_visitor_enum_serializer_op_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_enum_serializer_op_ch::~be_visitor_enum_serializer_op_ch (void)
{
}

int
be_visitor_enum_serializer_op_ch::visit_enum (be_enum *node)
{
  if (node->cli_hdr_serializer_op_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // generate the Serializer << and >> operators
  *os << be_global->stub_export_macro () << " ::CORBA::Boolean"
      << " operator<< (TAO::DCPS::Serializer &, const " << node->name ()
      << " &);" << be_nl;
  *os << be_global->stub_export_macro () << " ::CORBA::Boolean"
      << " operator>> (TAO::DCPS::Serializer &, "
      << node->name () << " &);";

  node->cli_hdr_serializer_op_gen (1);
  return 0;
}
