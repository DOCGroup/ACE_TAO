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
//    Visitor generating code for Any operators for a valuetype in the client
//    header.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//    Boris Kolpackov <bosk@ipmce.ru>
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "be_visitor_valuetype.h"

ACE_RCSID(be_visitor_valuetype, any_op_ch, "$Id$")


// ***************************************************************************
// Valuetype visitor for generating Any operator declarations.
// ***************************************************************************

be_visitor_valuetype_any_op_ch::be_visitor_valuetype_any_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_any_op_ch::~be_visitor_valuetype_any_op_ch (void)
{
}

int
be_visitor_valuetype_any_op_ch::visit_valuetype (be_valuetype *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Generate the Any <<= and >>= operator declarations.
  os->indent ();
  *os << "// Any operators for valuetype " << node->name () << be_nl;

  *os << be_global->stub_export_macro () << " void"
      << " operator<<= (CORBA::Any &, " << node->name ()
      << " *); // copying" << be_nl;

  *os << be_global->stub_export_macro () << " void"
      << " operator<<= (CORBA::Any &, " << node->name ()
      << " **); // non-copying" << be_nl;

  *os << be_global->stub_export_macro () << " CORBA::Boolean"
      << " operator>>= (const CORBA::Any &, "
      << node->name () << " *&);\n";

  node->cli_hdr_any_op_gen (1);

  return 0;
}
