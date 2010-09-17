
//=============================================================================
/**
 *  @file    cdr_op_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for CDR operators for interfaces. This uses
 *  compiled marshaling.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

// ***************************************************************************
// Interface visitor for generating CDR operator declarations in the client header
// ***************************************************************************

be_visitor_interface_cdr_op_ch::be_visitor_interface_cdr_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_cdr_op_ch::~be_visitor_interface_cdr_op_ch (void)
{
}

int
be_visitor_interface_cdr_op_ch::visit_interface (be_interface *node)
{
  // No CDR operations for locality constraint interfaces.
  if (node->cli_hdr_cdr_op_gen ()
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl;

  *os << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_nl;

  *os << be_global->core_versioning_begin () << be_nl;

  // Generate the CDR << and >> operator declarations.
  *os << be_global->stub_export_macro () << " ::CORBA::Boolean "
      << "operator<< (TAO_OutputCDR &, const " << node->full_name ()
      << "_ptr );" << be_nl;
  *os << be_global->stub_export_macro () << " ::CORBA::Boolean "
      << "operator>> (TAO_InputCDR &, "
      << node->full_name () << "_ptr &);" << be_nl;

  if (be_global->gen_ostream_operators ())
    {
      *os << be_global->stub_export_macro () << " std::ostream&"
          << " operator<< (std::ostream &strm, const "
          << node->full_name () << "_ptr);" << be_nl;
    }

  *os << be_global->core_versioning_end () << be_nl;

  // Set the substate as generating code for the types defined in our scope.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_SCOPE);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_interface_cdr_op_ch::")
                         ACE_TEXT ("visit_interface - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  this->ctx_->sub_state (TAO_CodeGen::TAO_SUB_STATE_UNKNOWN);
  node->cli_hdr_cdr_op_gen (1);
  return 0;
}

int
be_visitor_interface_cdr_op_ch::visit_component (
  be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_cdr_op_ch::visit_connector (
  be_connector *node)
{
  return this->visit_interface (node);
}

