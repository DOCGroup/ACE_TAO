//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_si.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the server inline file
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, interface_si, "$Id$")


// ************************************************************************
// Interface visitor for server inline
// ************************************************************************

be_visitor_interface_si::be_visitor_interface_si (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_si::~be_visitor_interface_si (void)
{
}

int
be_visitor_interface_si::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream

  if (node->srv_inline_gen () || node->imported ())
    return 0;

  os = this->ctx_->stream ();

  // determine if we are in some form of a multiple inheritance
  if (node->traverse_inheritance_graph
      (be_interface::in_mult_inheritance_helper, 0) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_si::visit_interface "
                         "error determining mult inheritance\n"),
                        -1);
    }

  os->indent (); // start with whatever indentation level we are at
  *os << "ACE_INLINE CORBA::Boolean" << be_nl;
  *os << node->full_skel_name () << "::in_mult_inheritance (void)" << be_nl
      << "{" << be_idt_nl;
  switch (node->in_mult_inheritance ())
    {
    case 0:
      *os << "return 0;";
      break;
    case 1:
      *os << "return 1;";
      break;
    default:
      // error
      return -1;
    }
  *os << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Generate skeletons for operations of our base classes. These skeletons
  // just cast the pointer to the appropriate type before invoking the
  // call. Hence we generate these in the inline file
  if (node->traverse_inheritance_graph (be_interface::gen_skel_helper, os)
      == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_si::"
                         "visit_interface - "
                         "codegen for base class skeletons failed\n"), -1);
    }

  // generate the TIE class
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_INTERFACE_TIE_SI);
  ctx.stream (tao_cg->server_template_inline ());
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_sh::"
                         "visit_interface - "
                         "codegen for TIE class failed\n"),
                        -1);
    }

  return 0;
}
