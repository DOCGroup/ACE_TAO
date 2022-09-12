
//=============================================================================
/**
 *  @file    any_op_ch.cpp
 *
 *  Visitor generating code for Any operators for a forward declared
 *  valuetype in the client header.
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "valuetype_fwd.h"

// ***************************************************************************
// Generates Any operator declarations in the client header
// ***************************************************************************

be_visitor_valuetype_fwd_any_op_ch::be_visitor_valuetype_fwd_any_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_valuetype_fwd_any_op_ch::~be_visitor_valuetype_fwd_any_op_ch ()
{
}

int
be_visitor_valuetype_fwd_any_op_ch::visit_valuetype_fwd (
    be_valuetype_fwd *node
  )
{
  AST_Interface *fd = node->full_definition ();

  // Only a forward declared interface that is not defined in the same file
  // needs to have this generated here. The Any operators are needed by
  // portable interceptor code if the interface is a parameter of an operation.
  if (fd->is_defined () != 0)
    {
      return 0;
    }

  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  const char *macro = this->ctx_->export_macro ();

  TAO_INSERT_COMMENT (os);

  *os << be_global->anyops_versioning_begin () << be_nl;

  *os << macro << " void"
      << " operator<<= (::CORBA::Any &, " << node->name ()
      << " *); // copying" << be_nl;
  *os << macro << " void"
      << " operator<<= (::CORBA::Any &, " << node->name ()
      << " **); // non-copying" << be_nl;
  *os << macro << " ::CORBA::Boolean"
      << " operator>>= (const ::CORBA::Any &, "
      << node->name () << " *&);";

  *os << be_global->anyops_versioning_end () << be_nl;

  node->cli_hdr_any_op_gen (true);
  return 0;
}

int
be_visitor_valuetype_fwd_any_op_ch::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_valuetype_fwd (node);
}
