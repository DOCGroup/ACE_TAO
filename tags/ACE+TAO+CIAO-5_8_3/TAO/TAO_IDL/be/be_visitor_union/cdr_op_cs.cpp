
//=============================================================================
/**
 *  @file    cdr_op_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for CDR operators for unions
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


// ***************************************************************************
// Union visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_union_cdr_op_cs::be_visitor_union_cdr_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_union (ctx)
{
}

be_visitor_union_cdr_op_cs::~be_visitor_union_cdr_op_cs (void)
{
}

int
be_visitor_union_cdr_op_cs::visit_union (be_union *node)
{
  // already generated and/or we are imported. Don't do anything.
  if (node->cli_stub_cdr_op_gen ()
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  // Set the substate as generating code for the types defined in our scope
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_SCOPE);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_cs"
                         "::visit_union - "
                         "codegen for scope failed\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << be_global->core_versioning_begin () << be_nl;

  //  Set the sub state as generating code for the output operator.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_OUTPUT);

  *os << "::CORBA::Boolean operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &strm," << be_nl
      << "const " << node->name () << " &_tao_union" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  switch (node->udisc_type ())
    {
      case AST_Expression::EV_bool:
        *os << "::ACE_OutputCDR::from_boolean tmp (_tao_union._d ());" << be_nl
            << "if ( !(strm << tmp) )" << be_idt_nl;

        break;
      case AST_Expression::EV_char:
        *os << "::ACE_OutputCDR::from_char tmp (_tao_union._d ());" << be_nl
            << "if ( !(strm << tmp) )" << be_idt_nl;

        break;
      case AST_Expression::EV_wchar:
        *os << "::ACE_OutputCDR::from_wchar tmp (_tao_union._d ());" << be_nl
            << "if ( !(strm << tmp) )" << be_idt_nl;

        break;
      default:
        *os << "if ( !(strm << _tao_union._d ()) )" << be_idt_nl;

        break;
    }

  *os << "{" << be_idt_nl
      << "return false;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "::CORBA::Boolean result = true;" << be_nl_2
      << "switch (_tao_union._d ())" << be_nl
      << "{" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_cs::"
                         "visit_union - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // If there is no explicit default case, but there
  // is an implicit one, and the discriminant is an enum,
  // we need this to avert warnings in some compilers that
  // not all case values are included. If there is no
  // implicit default case, or the discriminator is not
  // an enum, this does no harm.
  if (node->gen_empty_default_label ())
    {
      *os << be_nl << "default:" << be_idt_nl;
      *os << "break;"<< be_uidt;
    }

  *os << be_uidt_nl << "}" << be_nl_2
      << "return result;" << be_uidt_nl
      << "}" << be_nl_2;

  // Set the substate as generating code for the input operator.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);
  *os << "::CORBA::Boolean operator>> (" << be_idt << be_idt_nl
      << "TAO_InputCDR &strm," << be_nl
      << node->name () << " &_tao_union" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  be_type* disc_type =
    be_type::narrow_from_decl (node->disc_type ());

  // Generate a temporary to store the discriminant
  *os << disc_type->full_name ()
      << " " << "_tao_discriminant;" << be_nl;

  switch (node->udisc_type ())
    {
      case AST_Expression::EV_bool:
        *os << "::ACE_InputCDR::to_boolean tmp (_tao_discriminant);" << be_nl
            << "if ( !(strm >> tmp) )" << be_idt_nl;

        break;
      case AST_Expression::EV_char:
        *os << "::ACE_InputCDR::to_char tmp (_tao_discriminant);" << be_nl
            << "if ( !(strm >> tmp) )" << be_idt_nl;

        break;
      case AST_Expression::EV_wchar:
        *os << "::ACE_InputCDR::to_wchar tmp (_tao_discriminant);" << be_nl
            << "if ( !(strm >> tmp) )" << be_idt_nl;

        break;
      default:
        *os << "if ( !(strm >> _tao_discriminant) )" << be_idt_nl;

        break;
    }

  *os << "{" << be_idt_nl
      << "return false;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "::CORBA::Boolean result = true;" << be_nl_2
      << "switch (_tao_discriminant)" << be_nl
      << "{" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_cs::"
                         "visit_union - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // If there is no explicit default case, but there
  // is an implicit one, and the discriminant is an enum,
  // we need this to avert warnings in some compilers that
  // not all case values are included. If there is no
  // implicit default case, or the discriminator is not
  // an enum, this does no harm.
  if (node->gen_empty_default_label ())
    {
      *os << be_nl;
      *os << "default:" << be_idt_nl;
      *os << "_tao_union._default ();" << be_nl;
      *os << "// For maximum interop compatability, force the same value as transmitted" << be_nl;
      *os << "_tao_union._d (_tao_discriminant);" << be_nl;
      *os << "break;" << be_uidt;
    }

  *os << be_uidt_nl
      << "}" << be_nl_2
      << "return result;" << be_uidt_nl
      << "}" << be_nl;

  bool use_underscore = (this->ctx_->tdef () == 0);

  if (be_global->gen_ostream_operators ())
    {
      node->gen_ostream_operator (os, use_underscore);
    }

  *os << be_global->core_versioning_end () << be_nl;

  node->cli_stub_cdr_op_gen (true);
  return 0;
}
