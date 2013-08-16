
//=============================================================================
/**
 *  @file    cdr_op_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for CDR operators for forward declared
 *  interfaces. This uses compiled marshaling.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "interface_fwd.h"

be_visitor_interface_fwd_cdr_op_ch::be_visitor_interface_fwd_cdr_op_ch (
    be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_interface_fwd_cdr_op_ch::~be_visitor_interface_fwd_cdr_op_ch (void)
{
}

int
be_visitor_interface_fwd_cdr_op_ch::visit_interface_fwd (be_interface_fwd *node)
{
  // If this forward declared interface is defined later in the file,
  // the CDR operator declaration (along with the corresponding
  // declarations for members of the interface's scope) will be
  // generated then.
  if (node->full_def_seen () || node->is_local ())
    {
      return 0;
    }

  // No CDR operations for locality constraint interfaces.
  if (node->cli_hdr_cdr_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << be_global->core_versioning_begin () << be_nl;

  *os << be_global->stub_export_macro () << " ::CORBA::Boolean "
      << "operator<< (TAO_OutputCDR &, const " << node->full_name ()
      << "_ptr );" << be_nl;
  *os << be_global->stub_export_macro () << " ::CORBA::Boolean "
      << "operator>> (TAO_InputCDR &, "
      << node->full_name () << "_ptr &);\n";

  *os << be_global->core_versioning_end () << be_nl;

  node->cli_hdr_cdr_op_gen (1);
  return 0;
}
