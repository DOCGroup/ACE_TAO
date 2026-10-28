
//=============================================================================
/**
 *  @file    any_op_ch.cpp
 *
 *  Visitor generating code for Any operators for a valuebox in the client
 *  header.
 *
 *  @author Gary Maxey
 */
//=============================================================================

#include "valuebox.h"

// ***************************************************************************
// Valuebox visitor for generating Any operator declarations.
// ***************************************************************************

be_visitor_valuebox_any_op_ch::be_visitor_valuebox_any_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_valuebox (ctx)
{
}

int
be_visitor_valuebox_any_op_ch::visit_valuebox (be_valuebox *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  std::string_view macro = this->ctx_->export_macro ();

  TAO_INSERT_COMMENT (os);

  *os << be_global->anyops_versioning_begin () << be_nl;

  *os << macro << " void operator<<= (::CORBA::Any &, " << node->name ()
      << " *); // copying" << be_nl;

  *os << macro << " void operator<<= (::CORBA::Any &, " << node->name ()
      << " **); // non-copying" << be_nl;

  *os << macro << " ::CORBA::Boolean operator>>= (::CORBA::Any const &, "
      << node->name () << " *&);";

  *os << be_global->anyops_versioning_end () << be_nl;

  node->cli_hdr_any_op_gen (true);
  return 0;
}
