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
//    Visitor generating code for CDR operators for structures
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_structure, 
           cdr_op_ci, 
           "$Id$")

// ***************************************************************************
// Structure visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_structure_cdr_op_ci::be_visitor_structure_cdr_op_ci (
    be_visitor_context *ctx
  )
  : be_visitor_structure (ctx)
{
}

be_visitor_structure_cdr_op_ci::~be_visitor_structure_cdr_op_ci (void)
{
}

int
be_visitor_structure_cdr_op_ci::visit_structure (be_structure *node)
{
  // already generated and/or we are imported. Don't do anything.
  if (node->cli_inline_cdr_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // First generate code for our children. The reason we do this first is
  // because the inlined code for our children must be available before we use
  // it in our parent.

  // Set the substate as generating code for the types defined in our scope.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_SCOPE);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_cdr_op_ci"
                         "::visit_structure - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  //  Set the sub state as generating code for the output operator.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_OUTPUT);

  *os << "ACE_INLINE" << be_nl
      << "CORBA::Boolean operator<< (" << be_idt << be_idt_nl
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
                         "(%N:%l) be_visitor_structure_cdr_op_ci::"
                         "visit_structure - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  *os << ";" << be_uidt << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Set the substate as generating code for the input operator.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_INPUT);

  *os << "ACE_INLINE" << be_nl
      << "CORBA::Boolean operator>> (" << be_idt << be_idt_nl
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
      *os << "return 0;";
    }
  else
    {
      new_ctx.sub_state (TAO_CodeGen::TAO_CDR_INPUT);
      field_decl.visit_scope (node);

      *os << "return" << be_idt_nl;

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_structure_cdr_op_ci"
                             "::visit_structure - "
                             "codegen for scope failed\n"), 
                            -1);
        }

      *os << ";" << be_uidt << be_uidt;
    }

  *os << be_uidt_nl << "}";

  node->cli_inline_cdr_op_gen (1);
  return 0;
}

int
be_visitor_structure_cdr_op_ci::post_process (be_decl *bd)
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
