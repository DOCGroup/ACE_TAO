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
//    Visitor generating code for CDR operators for exceptions
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_exception, 
           cdr_op_ci, 
           "$Id$")

// ***************************************************************************
// Exception visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_exception_cdr_op_ci::be_visitor_exception_cdr_op_ci (
    be_visitor_context *ctx
  )
  : be_visitor_exception (ctx)
{
}

be_visitor_exception_cdr_op_ci::~be_visitor_exception_cdr_op_ci (void)
{
}

int
be_visitor_exception_cdr_op_ci::visit_exception (be_exception *node)
{
  // already generated and/or we are imported. Don't do anything.
  if (node->cli_inline_cdr_op_gen () 
      || node->imported () 
      || node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // First generate code for our children. The reason we do this first is
  // because the inlined code for our children must be available before we use
  // it in our parent

  // Set the substate as generating code for the types defined in our scope.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_SCOPE);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception_cdr_op_ci"
                         "::visit_exception - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  //  set the sub state as generating code for the output operator
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_OUTPUT);
  *os << "ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, "
      << "const " << node->name () << " &_tao_aggregate)" << be_nl
      << "{" << be_idt_nl;
  // do we have any members?
  if (node->nmembers () > 0)
    {
      be_visitor_context new_ctx (*this->ctx_);
      be_visitor_cdr_op_field_decl field_decl (&new_ctx);
      field_decl.visit_scope (node);

      // some members
      *os << "// first marshal the repository ID" << be_nl
          << "if (strm << _tao_aggregate._id ())" << be_nl
          << "{" << be_idt_nl
          << "// now marshal the members (if any)" << be_nl
          << "if (" << be_idt_nl;

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_exception_cdr_op_ci::"
                             "visit_exception - "
                             "codegen for scope failed\n"), 
                            -1);
        }

      *os << be_uidt_nl << ")"
          << be_idt_nl
          << "return 1;" << be_uidt_nl
          << "else" << be_idt_nl
          << "return 0;" << be_uidt << be_uidt_nl
          << "}" << be_nl
          << "else" << be_idt_nl
          << "return 0;" << be_uidt << be_uidt_nl;
    }
  else
    {
      // No members.
      *os << "// first marshal the repository ID" << be_nl
          << "if (strm << _tao_aggregate._id ())" << be_idt_nl
          << "return 1;" << be_uidt_nl
          << "else" << be_idt_nl
          << "return 0;" << be_uidt << be_uidt_nl;
    }

  *os << "}\n\n";

  // Set the substate as generating code for the input operator.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);

  *os << "ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &";

  if (node->nmembers () > 0)
    {
      *os << "strm,"
          << node->name () << " &_tao_aggregate)" << be_nl;
    }
  else
    {
      *os << ","
          << node->name () << "&)" << be_nl;
    }

  *os  << "{" << be_idt_nl;

  // WARNING: This method is not symmetric with respect to the
  // encoding function!
  // Exceptions are strange.... the repository ID is retrieved by the
  // caller, and they invoke this method only to demarshal the
  // members.  While the marshaling method encodes both...

  // Do we have any members?
  if (node->nmembers () > 0)
    {
      be_visitor_context new_ctx (*this->ctx_);
      be_visitor_cdr_op_field_decl field_decl (&new_ctx);
      field_decl.visit_scope (node);

      // Some members.
      *os << "// now marshal the members" << be_nl
          << "if (" << be_idt_nl;

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_exception_cdr_op_ci::"
                             "visit_exception - "
                             "codegen for scope failed\n"), 
                            -1);
        }
      *os << be_uidt_nl << ")"
          << be_idt_nl
          << "return 1;" << be_uidt_nl
          << "else" << be_idt_nl
          << "return 0;" << be_uidt << be_uidt_nl;
    }
  else
    {
      *os << "return 1;" << be_uidt_nl;
    }

  *os << "}\n\n";

  node->cli_inline_cdr_op_gen (1);
  return 0;
}

int
be_visitor_exception_cdr_op_ci::post_process (be_decl *bd)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (!this->last_node (bd))
    {
      switch (this->ctx_->sub_state ())
        {
        case TAO_CodeGen::TAO_CDR_OUTPUT:
        case TAO_CodeGen::TAO_CDR_INPUT:
          *os << " &&" << be_nl;
          break;
        case TAO_CodeGen::TAO_CDR_SCOPE:
        default:
          break;
        };
    }

  return 0;
}
