//
// $Id$
//

// ================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for CDR operators for forward
//    declarations of interfaces. This uses compiled marshaling.
//
// = AUTHOR
//    Aniruddha Gokhale & Carlos O'Ryan
//
// ================================================================

ACE_RCSID (be_visitor_interface_fwd, 
           cdr_op_ci, 
           "$Id$")

// ****************************************************************
// Interface visitor for generating CDR operator declarations in the
// client header
// ****************************************************************

be_visitor_interface_fwd_cdr_op_ci::be_visitor_interface_fwd_cdr_op_ci (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_interface_fwd_cdr_op_ci::~be_visitor_interface_fwd_cdr_op_ci (
    void
  )
{
}

int
be_visitor_interface_fwd_cdr_op_ci::visit_interface_fwd (
    be_interface_fwd *node
  )
{
  if (node->cli_inline_cdr_decl_gen ()
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << be_global->stub_export_macro () << " CORBA::Boolean operator<< ("
      << be_idt << be_idt_nl
      << "TAO_OutputCDR &," << be_nl
      << "const " << node->name () << "_ptr" << be_uidt_nl
      << ");" << be_uidt_nl << be_nl;

  *os << be_global->stub_export_macro () << " CORBA::Boolean operator>> ("
      << be_idt << be_idt_nl
      << "TAO_InputCDR &," << be_nl
      << node->name () << "_ptr &" << be_uidt_nl
      << ");" << be_uidt;

  node->cli_inline_cdr_decl_gen (1);

  // Also set the state for our full definition.
  AST_Interface *fd = node->full_definition ();
  be_interface *bfd = be_interface::narrow_from_decl (fd);
  bfd->cli_inline_cdr_decl_gen (1);

  return 0;
}
