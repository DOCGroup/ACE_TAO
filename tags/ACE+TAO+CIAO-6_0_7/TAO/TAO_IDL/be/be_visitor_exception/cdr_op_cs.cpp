
//=============================================================================
/**
 *  @file    cdr_op_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for CDR operators for exceptions
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

be_visitor_exception_cdr_op_cs::be_visitor_exception_cdr_op_cs (
      be_visitor_context *ctx)
  : be_visitor_exception (ctx)
{
}

be_visitor_exception_cdr_op_cs::~be_visitor_exception_cdr_op_cs (void)
{
}

int
be_visitor_exception_cdr_op_cs::visit_exception (be_exception *node)
{
  if (node->cli_stub_cdr_op_gen ()
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_SCOPE);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(be_visitor_exception_cdr_op_cs::")
                         ACE_TEXT ("visit_exception - ")
                         ACE_TEXT ("codegen for scope failed\n")),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_global->core_versioning_begin () << be_nl;

  //  Set the sub state as generating code for the output operator.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);

  *os << be_nl_2
      << "::CORBA::Boolean operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &strm," << be_nl
      << "const " << node->name () << " &_tao_aggregate" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  // Do we have any members?
  if (node->nmembers () > 0)
    {
      be_visitor_context new_ctx (*this->ctx_);
      be_visitor_cdr_op_field_decl field_decl (&new_ctx);

      if (field_decl.visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_exception_cdr_op_cs::")
                             ACE_TEXT ("visit_exception - ")
                             ACE_TEXT ("codegen for field decl scope failed\n")),
                            -1);
        }

      // some members
      *os << "// First marshal the repository ID." << be_nl
          << "if (strm << _tao_aggregate._rep_id ())" << be_idt_nl
          << "{" << be_idt_nl
          << "// Now marshal the members (if any)." << be_nl
          << "return (" << be_idt_nl;

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_exception_cdr_op_cs::")
                             ACE_TEXT ("visit_exception - ")
                             ACE_TEXT ("codegen for scope failed\n")),
                            -1);
        }

      *os << be_uidt_nl << " );" << be_uidt_nl
          << "}" << be_uidt_nl
          << "else" << be_idt_nl
          << "{" << be_idt_nl
          << "return false;" << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl;
    }
  else
    {
      // No members.
      *os << "// Marshal the repository ID." << be_nl
          << "return (strm << _tao_aggregate._rep_id ());" << be_uidt_nl;
    }

  *os << "}" << be_nl_2;

  // Set the substate as generating code for the input operator.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);

  *os << "::CORBA::Boolean operator>> (" << be_idt << be_idt_nl
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

      if (field_decl.visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_exception_cdr_op_cs::")
                             ACE_TEXT ("visit_exception - ")
                             ACE_TEXT ("codegen for field decl failed\n")),
                            -1);
        }

      // Some members.
      *os << "// Demarshal the members." << be_nl
          << "return (" << be_idt_nl;

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("be_visitor_exception_cdr_op_cs::")
                             ACE_TEXT ("visit_exception - ")
                             ACE_TEXT ("codegen for scope failed\n")),
                            -1);
        }

      *os << be_uidt_nl << ");" << be_uidt_nl;
    }
  else
    {
      *os << "return true;" << be_uidt_nl;
    }

  *os << "}" << be_nl;

  if (be_global->gen_ostream_operators ())
    {
      node->gen_ostream_operator (os, false);
    }

  *os << be_global->core_versioning_end () << be_nl;

  node->cli_stub_cdr_op_gen (1);
  return 0;
}

int
be_visitor_exception_cdr_op_cs::post_process (be_decl *bd)
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
