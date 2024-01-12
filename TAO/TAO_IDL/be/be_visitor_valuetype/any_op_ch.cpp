
//=============================================================================
/**
 *  @file    any_op_ch.cpp
 *
 *  Visitor generating code for Any operators for a valuetype in the client
 *  header.
 *
 *  @author Jeff Parsons <parsons@cs.wustl.edu> Boris Kolpackov <bosk@ipmce.ru>
 */
//=============================================================================

#include "valuetype.h"

// ***************************************************************************
// Valuetype visitor for generating Any operator declarations.
// ***************************************************************************

be_visitor_valuetype_any_op_ch::be_visitor_valuetype_any_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_any_op_ch::~be_visitor_valuetype_any_op_ch ()
{
}

int
be_visitor_valuetype_any_op_ch::visit_valuetype (be_valuetype *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  const char *macro = this->ctx_->export_macro ();

  TAO_INSERT_COMMENT (os);

  *os << be_global->anyops_versioning_begin () << be_nl;

  *os << macro << " void operator<<= (::CORBA::Any &, " << node->name ()
      << " *); // copying" << be_nl;

  *os << macro << " void operator<<= (::CORBA::Any &, " << node->name ()
      << " **); // non-copying" << be_nl;

  *os << macro << " ::CORBA::Boolean operator>>= (const ::CORBA::Any &, "
      << node->name () << " *&);";

  *os << be_global->anyops_versioning_end () << be_nl;

  node->cli_hdr_any_op_gen (true);
  return 0;
}

int
be_visitor_valuetype_any_op_ch::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}
