//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the client inline file
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, interface_ci, "$Id$")


// **************************************************
// Interface visitor for client inline
// **************************************************
be_visitor_interface_ci::be_visitor_interface_ci (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ci::~be_visitor_interface_ci (void)
{
}

int
be_visitor_interface_ci::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream

  if (node->cli_inline_gen () || node->imported ())
    return 0;

  os = this->ctx_->stream ();

  os->indent (); // start from the current indentation level

  // generate the constructors and destructor
  *os << "ACE_INLINE" << be_nl;
  *os << node->name () << "::" << node->local_name () <<
    " (void) // default constructor" << be_nl;
  *os << "{}" << be_nl << be_nl;

  *os << "ACE_INLINE" << be_nl;
  *os << node->name () << "::" << node->local_name () <<
    " (TAO_Stub *objref, TAO_ServantBase *_tao_servant, "
      << "CORBA::Boolean _tao_collocated) // constructor" << be_nl;
  *os << "  : CORBA_Object (objref, _tao_servant, _tao_collocated)" << be_nl;
  *os << "{}" << be_nl << be_nl;

  *os << "ACE_INLINE" << be_nl;
  *os << node->name () << "::~" << node->local_name () <<
    " (void) // destructor" << be_nl;
  *os << "{}\n\n";

  // _nil method
  *os << "ACE_INLINE "
      << node->name () << "_ptr" << be_nl
      << node->name () << "::_nil (void)" << be_nl
      << "{" << be_idt_nl
      << "return (" << node->name () << "_ptr)0;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // generate the ifdefined macro for  the _var type
  os->gen_ifdef_macro (node->flatname (), "_var");
  if (node->gen_var_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ci::"
                         "visit_interface - "
                         "codegen for _var failed\n"), -1);
    }
  os->gen_endif ();

  // generate the ifdefined macro for  the _out type
  os->gen_ifdef_macro (node->flatname (), "_out");
  if (node->gen_out_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ci::"
                         "visit_interface - "
                         "codegen for _out failed\n"), -1);
    }
  os->gen_endif ();

  // Generate inline methods for elements of our scope
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ci::"
                         "visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  // If AMI is enabled, generate all the inline stuff for the
  // AMI_<interface_name>_Handler interface.

  if (idl_global->ami_call_back () == I_TRUE)
    {
      // Set the context.
      be_visitor_context ctx (*this->ctx_);
      
      // Set the state.
      ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_FWD_CI);
      
      // Create the visitor.
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_ci::"
                             "visit_interface - "
                             "Bad visitor\n"),
                            -1);
        }
      
      // Call the visitor on this interface.
      if (node->accept (visitor) == -1)
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_interface_ch::"
                             "visit_interface - "
                             "code gen for ami handler fwd failed\n"),
                            -1);
        }
      delete visitor;
    }

  return 0;
}
