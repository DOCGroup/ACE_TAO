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
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Since we don't generate CDR stream operators for types that
  // explicitly contain a local interface (at some level), we 
  // must override these Any template class methods to avoid
  // calling the non-existent operators. The zero return value
  // will eventually cause CORBA::MARSHAL to be raised if this
  // type is inserted into an Any and then marshaled.
  if (node->is_local ())
    {
      if (be_global->any_support ())
        {
          *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
              << "// " << __FILE__ << ":" << __LINE__;

          *os << be_nl << be_nl
              << "ACE_INLINE" << be_nl
              << "CORBA::Boolean" << be_nl
              << "TAO::Any_Dual_Impl_T<" << node->name ()
              << ">::marshal_value (TAO_OutputCDR &)" << be_nl
              << "{" << be_idt_nl
              << "return 0;" << be_uidt_nl
              << "}";

          *os << be_nl << be_nl
              << "ACE_INLINE" << be_nl
              << "CORBA::Boolean" << be_nl
              << "TAO::Any_Dual_Impl_T<" << node->name ()
              << ">::demarshal_value (TAO_InputCDR &)" << be_nl
              << "{" << be_idt_nl
              << "return 0;" << be_uidt_nl
              << "}";
        }

      return 0;
    }

  // First generate code for our children. The reason we do this first is
  // because the inlined code for our children must be available before we use
  // it in our parent.

  // Set the substate as generating code for the types defined in our scope.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_SCOPE);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception_cdr_op_ci"
                         "::visit_exception - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  //  Set the sub state as generating code for the output operator.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);

  *os << be_nl << be_nl
      << "ACE_INLINE" << be_nl
      << "CORBA::Boolean operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &strm," << be_nl
      << "const " << node->name () << " &_tao_aggregate" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  // Do we have any members?
  if (node->nmembers () > 0)
    {
      be_visitor_context new_ctx (*this->ctx_);
      be_visitor_cdr_op_field_decl field_decl (&new_ctx);
      field_decl.visit_scope (node);

      // some members
      *os << "// First marshal the repository ID." << be_nl
          << "if (strm << _tao_aggregate._rep_id ())" << be_idt_nl
          << "{" << be_idt_nl
          << "// Now marshal the members (if any)." << be_nl
          << "if (" << be_idt_nl;

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_exception_cdr_op_ci::"
                             "visit_exception - "
                             "codegen for scope failed\n"), 
                            -1);
        }

      *os << be_uidt_nl << " )"
          << be_idt_nl
          << "{" << be_idt_nl
          << "return 1;" << be_uidt_nl
          << "}" << be_uidt_nl
          << "else" << be_idt_nl
          << "{" << be_idt_nl
          << "return 0;" << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl
          << "}" << be_uidt_nl
          << "else" << be_idt_nl
          << "{" << be_idt_nl
          << "return 0;" << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl;
    }
  else
    {
      // No members.
      *os << "// First marshal the repository ID." << be_nl
          << "if (strm << _tao_aggregate._rep_id ())" << be_idt_nl
          << "{" << be_idt_nl
          << "return 1;" << be_uidt_nl
          << "}" << be_uidt_nl
          << "else" << be_idt_nl
          << "{" << be_idt_nl
          << "return 0;" << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl;
    }

  *os << "}" << be_nl << be_nl;

  // Set the substate as generating code for the input operator.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);

  *os << "ACE_INLINE" << be_nl
      << "CORBA::Boolean operator>> (" << be_idt << be_idt_nl
      << "TAO_InputCDR &";

  if (node->nmembers () > 0)
    {
      *os << "strm," << be_nl
          << node->name () << " &_tao_aggregate" << be_uidt_nl
          << ")" << be_uidt_nl;
    }
  else
    {
      *os << "," << be_nl
          << node->name () << "&" << be_uidt_nl
          << ")" << be_uidt_nl;
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
      *os << "// Demarshal the members." << be_nl
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
          << "{" << be_idt_nl
          << "return 1;" << be_uidt_nl
          << "}" << be_uidt_nl
          << "else" << be_idt_nl
          << "{" << be_idt_nl
          << "return 0;" << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl;
    }
  else
    {
      *os << "return 1;" << be_uidt_nl;
    }

  *os << "}";

  node->cli_inline_cdr_op_gen (1);
  return 0;
}

int
be_visitor_exception_cdr_op_ci::post_process (be_decl *bd)
{
  // This checks for members of an enum 'non-scope' declared inside
  // the exception. If an enum val is actually a member, it will 
  // have node type NT_field.
  if (bd->node_type () == AST_Decl::NT_enum_val)
    {
      return 0;
    }

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
