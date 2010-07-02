
//=============================================================================
/**
 *  @file    any_op_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Enums for the Any operators
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


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
  const char *macro = this->ctx_->export_macro ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_global->core_versioning_begin () << be_nl;

  // Generate the Any <<= and >>= operators.
  *os << be_nl << be_nl
      << macro << " void operator<<= (::CORBA::Any &, " << node->name ()
      << ");" << be_nl;
  *os << macro << " ::CORBA::Boolean operator>>= (const ::CORBA::Any &, "
      << node->name () << " &);";

  *os << be_global->core_versioning_end () << be_nl;

  node->cli_hdr_any_op_gen (1);
  return 0;
}
