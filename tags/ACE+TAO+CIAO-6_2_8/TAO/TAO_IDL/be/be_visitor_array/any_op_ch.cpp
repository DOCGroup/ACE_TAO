
//=============================================================================
/**
 *  @file    any_op_ch.cpp
 *
 *  $Id$
 *
 *  Visitor for code generation of Arrays for the Any operators in the client
 *  header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "array.h"

be_visitor_array_any_op_ch::be_visitor_array_any_op_ch (
    be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_array_any_op_ch::~be_visitor_array_any_op_ch (void)
{
}

int
be_visitor_array_any_op_ch::visit_array (be_array *node)
{
  if (node->cli_hdr_any_op_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  const char *macro = this->ctx_->export_macro ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << be_global->core_versioning_begin () << be_nl;

  *os << macro << " void operator<<= (::CORBA::Any &, const " << node->name ()
      << "_forany &);" << be_nl;
  *os << macro << " ::CORBA::Boolean operator>>= (const ::CORBA::Any &, "
      << node->name () << "_forany &);";

  *os << be_global->core_versioning_end () << be_nl;

  node->cli_hdr_any_op_gen (1);
  return 0;
}
