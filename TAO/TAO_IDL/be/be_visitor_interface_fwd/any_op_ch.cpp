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
//    Visitor generating code for Any operators for a forward declared interface
//    in the client header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_interface_fwd, 
           any_op_ch, 
           "$Id$")


// ***************************************************************************
// Generates Any operator declarations in the client header
// ***************************************************************************

be_visitor_interface_fwd_any_op_ch::be_visitor_interface_fwd_any_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_interface_fwd_any_op_ch::~be_visitor_interface_fwd_any_op_ch (void)
{
}

int
be_visitor_interface_fwd_any_op_ch::visit_interface_fwd (
    be_interface_fwd *node
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
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Generate the Any <<= and >>= operator declarations.
  os->indent ();
  *os << "// Any operators for interface " << node->name () << be_nl;
  *os << be_global->stub_export_macro () << " void"
      << " operator<<= (CORBA::Any &, " << node->name ()
      << "_ptr);" << be_nl;
  *os << be_global->stub_export_macro () << " CORBA::Boolean"
      << " operator>>= (const CORBA::Any &, "
      << node->name () << " *&);\n";

  node->cli_hdr_any_op_gen (1);
  return 0;
}
