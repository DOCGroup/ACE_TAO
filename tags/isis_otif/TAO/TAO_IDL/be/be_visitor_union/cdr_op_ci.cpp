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
//    Visitor generating code for CDR operators for unions
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_union,
           cdr_op_ci,
           "$Id$")

// ***************************************************************************
// Union visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_union_cdr_op_ci::be_visitor_union_cdr_op_ci (
    be_visitor_context *ctx
  )
  : be_visitor_union (ctx)
{
}

be_visitor_union_cdr_op_ci::~be_visitor_union_cdr_op_ci (void)
{
}

int
be_visitor_union_cdr_op_ci::visit_union (be_union *node)
{
  // There is no check for is_local here because we have no way of
  // knowing which member will be active at marshaling time. So we
  // generate the CDR operators and let the local interface member
  // (if any) be caught at runtime.
  if (node->cli_inline_cdr_op_gen () 
      || node->imported ())
    {
      return 0;
    }

  // Set the substate as generating code for the types defined in our scope.
  this->ctx_->sub_state (TAO_CodeGen::TAO_CDR_SCOPE);

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_ci"
                         "::visit_union - "
                         "codegen for scope failed\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  //  Set the sub state as generating code for the output operator.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_OUTPUT);

  *os << "ACE_INLINE" << be_nl
      << "CORBA::Boolean operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &strm," << be_nl
      << "const " << node->name () << " &_tao_union" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  switch (node->udisc_type ())
    {
      case AST_Expression::EV_bool:
        *os << "CORBA::Any::from_boolean tmp (_tao_union._d ());" << be_nl
            << "if ( !(strm << tmp) )" << be_idt_nl;

        break;
      case AST_Expression::EV_char:
        *os << "CORBA::Any::from_char tmp (_tao_union._d ());" << be_nl
            << "if ( !(strm << tmp) )" << be_idt_nl;

        break;
      case AST_Expression::EV_wchar:
        *os << "CORBA::Any::from_wchar tmp (_tao_union._d ());" << be_nl
            << "if ( !(strm << tmp) )" << be_idt_nl;

        break;
      default:
        *os << "if ( !(strm << _tao_union._d ()) )" << be_idt_nl;

        break;
    }

  *os << "{" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "CORBA::Boolean result = 1;" << be_nl << be_nl
      << "switch (_tao_union._d ())" << be_nl
      << "{" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_ci::"
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
  if (node->default_index () == -1)
    {
      *os << be_nl << "default:" << be_idt_nl;
      *os << "break;"<< be_uidt;
    }

  *os << be_uidt_nl << "}" << be_nl << be_nl
      << "return result;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  // Set the substate as generating code for the input operator.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);
  *os << "ACE_INLINE" << be_nl
      << "CORBA::Boolean operator>> (" << be_idt << be_idt_nl
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
        *os << "CORBA::Any::to_boolean tmp (_tao_discriminant);" << be_nl
            << "if ( !(strm >> tmp) )" << be_idt_nl;

        break;
      case AST_Expression::EV_char:
        *os << "CORBA::Any::to_char tmp (_tao_discriminant);" << be_nl
            << "if ( !(strm >> tmp) )" << be_idt_nl;

        break;
      case AST_Expression::EV_wchar:
        *os << "CORBA::Any::to_wchar tmp (_tao_discriminant);" << be_nl
            << "if ( !(strm >> tmp) )" << be_idt_nl;

        break;
      default:
        *os << "if ( !(strm >> _tao_discriminant) )" << be_idt_nl;

        break;
    }

  *os << "{" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "CORBA::Boolean result = 1;" << be_nl << be_nl
      << "switch (_tao_discriminant)" << be_nl
      << "{" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_ci::"
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
  if (node->default_index () == -1)
    {
      *os << be_nl;
      *os << "default:" << be_idt_nl;
      *os << "_tao_union._d (_tao_discriminant);" << be_nl;
      *os << "break;" << be_uidt;
    }

  *os << be_uidt_nl
      << "}" << be_nl << be_nl
      << "return result;" << be_uidt_nl
      << "}";

  node->cli_inline_cdr_op_gen (1);
  return 0;
}

int
be_visitor_union_cdr_op_ci::pre_process (be_decl *bd)
{
  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_SCOPE)
    {
      return 0;
    }

  // Enum val nodes are added just to help check reference
  // clashes, since an enum declared in our scope is not itself
  // a scope.
  if (bd->node_type () == AST_Decl::NT_enum_val)
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  be_union_branch* b =
    be_union_branch::narrow_from_decl (bd);

  *os << be_nl;

  for (unsigned long i = 0; i < b->label_list_length (); ++i)
    {
      // check if we are printing the default case
      if (b->label (i)->label_kind () == AST_UnionLabel::UL_default)
        {
          *os << "default:";
        }
      else
        {
          *os << "case ";

          b->gen_label_value (os, i);

          *os << ":";
        }

      if (i == (b->label_list_length () - 1))
        {
          *os << be_idt_nl;
        }
      else
        {
          *os << be_nl;
        }
    }

  *os << "{" << be_idt_nl;

  return 0;
}

int
be_visitor_union_cdr_op_ci::post_process (be_decl *bd)
{
  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_SCOPE)
    {
      return 0;
    }

  if (bd->node_type () == AST_Decl::NT_enum_val)
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_uidt_nl << "}" << be_nl
      << "break;" << be_uidt;

  return 0;
}
