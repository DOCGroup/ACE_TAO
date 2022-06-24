
//=============================================================================
/**
 *  @file    any_op_ch.cpp
 *
 *  Visitor generating code for Enums for the Any operators
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "enum.h"

be_visitor_enum_any_op_ch::be_visitor_enum_any_op_ch (
    be_visitor_context *ctx)
  : be_visitor_scope (ctx)
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

  TAO_INSERT_COMMENT (os);

  *os << be_global->anyops_versioning_begin () << be_nl;

  // Generate the Any <<= and >>= operators.
  *os << be_nl_2
      << macro << " void operator<<= (::CORBA::Any &, " << node->name ()
      << ");" << be_nl;
  *os << macro << " ::CORBA::Boolean operator>>= (const ::CORBA::Any &, "
      << node->name () << " &);";

  *os << be_global->anyops_versioning_end () << be_nl;

  node->cli_hdr_any_op_gen (true);
  return 0;
}
