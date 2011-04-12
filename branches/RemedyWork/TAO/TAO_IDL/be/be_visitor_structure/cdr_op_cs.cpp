
//=============================================================================
/**
 *  @file    cdr_op_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for CDR operators for structures
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


be_visitor_structure_cdr_op_cs::be_visitor_structure_cdr_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_structure (ctx)
{
}

be_visitor_structure_cdr_op_cs::~be_visitor_structure_cdr_op_cs (void)
{
}

int
be_visitor_structure_cdr_op_cs::visit_structure (be_structure *node)
{
  if (node->cli_stub_cdr_op_gen ()
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  // Set the substate as generating code for the types defined in our scope.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_SCOPE);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_cdr_op_cs::"
                         "visit_structure - "
                         "codegen for scope failed\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << be_global->core_versioning_begin () << be_nl;

  //  Set the sub state as generating code for the output operator.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);

  *os << "::CORBA::Boolean operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &strm," << be_nl
      << "const " << node->name () << " &_tao_aggregate" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  be_visitor_context new_ctx (*this->ctx_);
  be_visitor_cdr_op_field_decl field_decl (&new_ctx);
  field_decl.visit_scope (node);

  *os << "return" << be_idt_nl;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_cdr_op_cs::"
                         "visit_structure - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << ";" << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  // Set the substate as generating code for the input operator.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_INPUT);

  *os << "::CORBA::Boolean operator>> (" << be_idt << be_idt_nl
      << "TAO_InputCDR &";

  if (! node->is_local ())
    {
      *os << "strm";
    }

  *os << "," << be_nl
      << node->name () << " &";

  if (! node->is_local ())
    {
      *os << "_tao_aggregate";
    }

  *os << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  if (node->is_local ())
    {
      *os << "return false;";
    }
  else
    {
      new_ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);

      if (field_decl.visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_structure_cdr_op_cs::"
                             "visit_structure - "
                             "codegen for field decl scope failed\n"),
                            -1);
        }

      *os << "return" << be_idt_nl;

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_structure_cdr_op_cs"
                             "::visit_structure - "
                             "codegen for scope failed\n"),
                            -1);
        }

      *os << ";" << be_uidt << be_uidt;
    }

  *os << be_uidt_nl << "}" << be_nl;

  if (be_global->gen_ostream_operators ())
    {
      node->gen_ostream_operator (os, false);
    }

  *os << be_global->core_versioning_end () << be_nl;

  node->cli_stub_cdr_op_gen (true);
  return 0;
}

int
be_visitor_structure_cdr_op_cs::post_process (be_decl *bd)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (!this->last_node (bd)
      && bd->node_type () != AST_Decl::NT_enum_val)
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
