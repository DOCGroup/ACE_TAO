// $id: any_op_cs.cpp,v 1.12 1998/10/30 19:46:55 levine Exp $

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    any_op_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Any operators for valuetypes in the stubs
//    file.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_valuetype.h"

ACE_RCSID(be_visitor_valuetype, any_op_cs, "$Id$")


// ***************************************************************************
// Valuetype visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_valuetype_any_op_cs::be_visitor_valuetype_any_op_cs
(be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_any_op_cs::~be_visitor_valuetype_any_op_cs (void)
{
}

int
be_visitor_valuetype_any_op_cs::visit_valuetype (be_valuetype *node)
{
  if (node->cli_stub_any_op_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  os->indent ();

  // TODO

  *os << be_nl;

  *os << "#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)  || \\"
      << be_idt_nl
      << "  defined (ACE_HAS_GNU_REPO)" << be_idt_nl
      << "template class TAO_Valuetype_Manager<"
      << node->full_name () << ", "
      << node->full_name () << "_var>;" << be_uidt_nl
      << "#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)" << be_nl
      << "#  pragma instantiate TAO_Valuetype_Manager<"
      << node->full_name () << ", "
      << node->full_name () << "_var>" << be_uidt_nl
      << "#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */\n\n";

  node->cli_stub_any_op_gen (1);

  return 0;
}
