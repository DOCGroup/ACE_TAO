//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for CDR operators for interfaces. This uses
//    compiled marshaling.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface_fwd.h"

ACE_RCSID(be_visitor_interface_fwd, cdr_op_ch, "$Id$")

// ***************************************************************************
// Forward declared interface visitor for generating CDR operator declarations 
// in the client header. Called if this node is not later defined in the file.
// ***************************************************************************

be_visitor_interface_fwd_cdr_op_ch::be_visitor_interface_fwd_cdr_op_ch
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_interface_fwd_cdr_op_ch::~be_visitor_interface_fwd_cdr_op_ch (void)
{
}

int
be_visitor_interface_fwd_cdr_op_ch::visit_interface_fwd (be_interface_fwd *node)
{
  // No CDR operations for locality constraint interfaces.
  if (idl_global->gen_locality_constraint ())
    return 0;
  if (node->cli_hdr_cdr_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  // generate the CDR << and >> operator declarations
  os->indent ();
  *os << idl_global->stub_export_macro () << " CORBA::Boolean" << be_nl
      << "operator<< (TAO_OutputCDR &, const " << node->full_name ()
      << "_ptr );" << be_nl;
  *os << idl_global->stub_export_macro () << " CORBA::Boolean" << be_nl
      << "operator>> (TAO_InputCDR &, "
      << node->full_name () << "_ptr &);\n";

  node->cli_hdr_cdr_op_gen (1);

  return 0;
}
