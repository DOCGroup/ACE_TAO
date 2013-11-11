
//=============================================================================
/**
 *  @file    cdr_op_ch.cpp
 *
 *  $Id$
 *
 *  Concrete visitor for valueboxes.
 *  This one provides code generation for the CDR operators.
 *
 *
 *  @author Gary Maxey
 */
//=============================================================================

#include "valuebox.h"

be_visitor_valuebox_cdr_op_ch::be_visitor_valuebox_cdr_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_valuebox (ctx)
{
}

be_visitor_valuebox_cdr_op_ch::~be_visitor_valuebox_cdr_op_ch (void)
{
}

int
be_visitor_valuebox_cdr_op_ch::visit_valuebox (be_valuebox *node)
{
  if (node->imported ()
      || node->cli_hdr_cdr_op_gen ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2
      << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << be_global->core_versioning_begin () << be_nl;

  *os << be_global->stub_export_macro () << " "
      << "::CORBA::Boolean operator<< (TAO_OutputCDR &, const "
      << node->full_name () << " *);" << be_nl;

  *os << be_global->stub_export_macro () << " "
      << "::CORBA::Boolean operator>> (TAO_InputCDR &, "
      << node->full_name () << " *&);";

  *os << be_global->core_versioning_end () << be_nl;

  node->cli_hdr_cdr_op_gen (1);
  return 0;
}
