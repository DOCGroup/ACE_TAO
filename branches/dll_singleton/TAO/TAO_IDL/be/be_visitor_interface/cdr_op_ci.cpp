//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for CDR operators for interfaces
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, cdr_op_ci, "$Id$")

// ***************************************************************************
// Interface visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_interface_cdr_op_ci::be_visitor_interface_cdr_op_ci
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_cdr_op_ci::~be_visitor_interface_cdr_op_ci (void)
{
}

int
be_visitor_interface_cdr_op_ci::visit_interface (be_interface *node)
{
  // Already generated and/or we are imported. Don't do anything.
  // No CDR operations for locality constrained interfaces.
  if (node->cli_inline_cdr_op_gen ()
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  if (! node->cli_inline_cdr_decl_gen ())
    {
      TAO_OutStream *os = this->ctx_->stream ();

      // First generate code for our children. The reason we do this first is
      // because the inlined code for our children must be available before
      // it in our parent, but we must forward declare the parent
      // we use operators, so code like this:
      //
      // // IDL
      // interface Foo {
      //   exception Bar {Foo foo_member; };
      // };
      //
      // can work properly (the Foo::Bar operators need the Foo operators
      // defined).
      //

      // Generate the CDR << and >> operator declarations.
      os->indent ();
      *os << be_global->stub_export_macro ()
          << " CORBA::Boolean operator<< ("
          << be_idt << be_idt_nl
          << "TAO_OutputCDR &," << be_nl
          << "const " << node->full_name () << "_ptr" << be_uidt_nl
          << ");" << be_uidt_nl << be_nl;

      *os << be_global->stub_export_macro ()
          << " CORBA::Boolean operator>> ("
          << be_idt << be_idt_nl
          << "TAO_InputCDR &," << be_nl
          << node->full_name () << "_ptr &" << be_uidt_nl
          << ");" << be_uidt << "\n\n";

      node->cli_inline_cdr_decl_gen (1);
    }

  // Set the substate as generating code for the types defined in our scope.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_SCOPE);

  // All we have to do is to visit the scope and generate code.
  // CDR operators for the interface itself are generated in the .cpp file.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_cdr_op_ci"
                         "::visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  node->cli_inline_cdr_op_gen (1);

  return 0;
}
