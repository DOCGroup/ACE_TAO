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

ACE_RCSID (be_visitor_operation, 
           tie_si, 
           "$Id$")

// ************************************************************
// Operation visitor for server inline for TIE class operations.
// ************************************************************

be_visitor_operation_tie_si::be_visitor_operation_tie_si (
    be_visitor_context *ctx
  )
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_tie_si::~be_visitor_operation_tie_si (void)
{
}

int be_visitor_operation_tie_si::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  be_interface *intf = this->ctx_->interface ();

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_tie_si::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  // Retrieve the operation return type.
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

  // Generate the return type mapping (same as in the header file).
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_OTHERS);
  be_visitor_operation_rettype oro_visitor (&ctx);

  if (bt->accept (&oro_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_tie_si::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }

  *os << " " << intf->full_skel_name () << "_tie<T>::"
      << node->local_name () << " ";

  // STEP 4: generate the argument list with the appropriate mapping (same as
  // in the header file)
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_OTHERS);
  be_visitor_operation_arglist oao_visitor (&ctx);

  if (node->accept (&oao_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  *os << "{" << be_idt_nl;

  be_predefined_type *pdt = be_predefined_type::narrow_from_decl (bt);

  if (pdt == 0 || pdt->pt () != AST_PredefinedType::PT_void)
    {
      *os << "return ";
    }

  *os << "this->ptr_->" << node->local_name () << " (" << be_idt << "\n";

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_COLLOCATED_ARG_UPCALL_SS);
  be_visitor_operation_argument ocau_visitor (&ctx);

  if (node->accept (&ocau_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ss::"
                         "visit_operation - "
                         "codegen for making upcall failed\n"),
                        -1);
    }

  *os << be_uidt_nl;
  *os << ");" << be_uidt_nl;
  *os << "}\n\n";

  return 0;
}
