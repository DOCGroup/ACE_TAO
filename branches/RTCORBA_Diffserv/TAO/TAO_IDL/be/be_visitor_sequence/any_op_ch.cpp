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

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "be_visitor_sequence.h"

ACE_RCSID(be_visitor_sequence, any_op_ch, "$Id$")


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

  // generate the Any <<= and >>= operators
  os->indent ();
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
  *os << " *&); // deprecated\n";
  *os << be_global->stub_export_macro ();
  *os << " CORBA::Boolean"
      << " operator>>= (const CORBA::Any &, const ";
  *os << node->name ();
  *os << " *&);\n";

  node->cli_hdr_any_op_gen (1);
  return 0;
}
