//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    tie_si.cpp
//
// = DESCRIPTION
//    Visitor generating code for operations for the TIE class.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, tie_si, "$Id$")


// ************************************************************
// Operation visitor for server inline for TIE class operations
// ************************************************************

be_visitor_operation_tie_si::be_visitor_operation_tie_si
(be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_tie_si::~be_visitor_operation_tie_si (void)
{
}

int be_visitor_operation_tie_si::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context
  be_interface *intf;
  intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (node->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_tie_si::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  // retrieve the operation return type
  be_type *bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_tie_si::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  os->indent ();
  *os << "template <class T> ACE_INLINE\n";

  // generate the return type mapping (same as in the header file)
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_OTHERS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_tie_si::"
                         "visit_operation - "
                         "Bad visitor for return type\n"),
                        -1);
    }

  if (bt->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_tie_si::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }
  delete visitor;

  *os << " " << intf->full_skel_name () << "_tie<T>::"
      << node->local_name () << " ";

  // STEP 4: generate the argument list with the appropriate mapping (same as
  // in the header file)
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_OTHERS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_cs::"
                         "visit_operation - "
                         "Bad visitor for argument list\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;

  *os << "{" << be_idt << "\n";

  os->indent ();
  if (bt->node_type () != AST_Decl::NT_pre_defined
      || be_predefined_type::narrow_from_decl (bt)->pt () != AST_PredefinedType::PT_void)
    {
      *os << "return ";
    }

  *os << "this->ptr_->" << node->local_name () << " (" << be_idt << "\n";

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_COLLOCATED_ARG_UPCALL_SS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "codegen for making upcall failed\n"),
                        -1);
    }
  // last argument is the environment
  if (node->argument_count () > 0)
    *os << ",\n";
  os->indent ();
  *os << "_tao_environment";
  // end the upcall
  *os << be_uidt_nl;
  *os << ");" << be_uidt_nl;
  *os << "}\n\n";

  return 0;
}
