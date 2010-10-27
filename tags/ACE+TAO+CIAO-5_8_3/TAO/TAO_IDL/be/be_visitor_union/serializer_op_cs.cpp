
//=============================================================================
/**
 *  @file    serializer_op_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for TAO::DCPS::Serializer operators for unions
 *
 *
 *  @author Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
 */
//=============================================================================


// ***************************************************************************
// Union visitor for generating Serializer operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_union_serializer_op_cs::be_visitor_union_serializer_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_union (ctx)
{
}

be_visitor_union_serializer_op_cs::~be_visitor_union_serializer_op_cs (void)
{
}

int
be_visitor_union_serializer_op_cs::visit_union (be_union *node)
{
  // already generated and/or we are imported. Don't do anything.
  if (node->cli_stub_serializer_op_gen ()
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
                         "(%N:%l) be_visitor_union_serializer_op_cs"
                         "::visit_union - "
                         "codegen for scope failed\n"),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  //---------------------------------------------------------------
  //  Set the sub state as generating code for _dcps_max_marshaled_size.
  this->ctx_->sub_state(TAO_CodeGen::TAO_MAX_MARSHALED_SIZE);

  *os << "size_t _dcps_max_marshaled_size (" << be_idt << be_idt_nl
      << "const " << node->name () << " &" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      ;
  *os << "// do not try getting the _dcps_max_marshaled_size" << be_nl
      << "// doing so may produce an access violation" << be_nl
      << "return 100000;" << be_uidt_nl
      << "}" << be_nl_2;

/*
      << "size_t max_size = 0;" << be_nl_2;

  *os << "// find the maximum field size." << be_nl;
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_serializer_op_cs::"
                         "visit_union - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << be_nl << "// now add in the size of the discriminant" << be_nl;
  switch (node->udisc_type ())
    {
      case AST_Expression::EV_bool:
        *os << "max_size += _dcps_max_marshaled_size ("
            << "ACE_OutputCDR::from_boolean (_tao_union._d ()));" << be_nl;
        break;
      case AST_Expression::EV_char:
        *os << "max_size += _dcps_max_marshaled_size ("
            << "ACE_OutputCDR::from_char (_tao_union._d ()));" << be_nl;

        break;
      case AST_Expression::EV_wchar:
        *os << "max_size += _dcps_max_marshaled_size ("
            << "ACE_OutputCDR::from_wchar (_tao_union._d ()));" << be_nl;

        break;
      default:
        // no need to cast the other dicriminant types
        *os << "max_size += _dcps_max_marshaled_size ("
            << "_tao_union._d ());" << be_nl;

        break;
    }

  *os << be_nl
      << "return max_size;" << be_uidt_nl
      << "}" << be_nl_2;
*/

  //---------------------------------------------------------------
  //  Set the sub state as generating code for _tao_is_bounded_size.
  this->ctx_->sub_state(TAO_CodeGen::TAO_IS_BOUNDED_SIZE);
  *os << "::CORBA::Boolean _tao_is_bounded_size (" << be_idt << be_idt_nl
      << "const " << node->name () << " &" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      ;
  *os << "// Always return false for a union because " << be_nl
      << "// _dcps_max_marshaled_size may produce an access violation."
      << be_nl
      << "return false;" << be_uidt_nl
      << "}" << be_nl_2;
/*
      << "CORBA::Boolean is_bounded = true;"
      << " // all discrimenent types are bounded" << be_nl_2;

  *os << "// bounded if all branches are bounded." << be_nl;
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_serializer_op_cs::"
                         "visit_union - "
                         "codegen for scope failed\n"),
                        -1);
    }

  *os << be_nl
      << "return is_bounded;" << be_uidt_nl
      << "}" << be_nl_2;
*/

  //---------------------------------------------------------------
  //  Set the sub state as generating code for _dcps_find_size.
  this->ctx_->sub_state(TAO_CodeGen::TAO_FIND_SIZE);
  *os << be_global->stub_export_macro ()
      << " size_t _dcps_find_size (const "
      << node->name() << "& _tao_union);" << be_nl;

  *os << "size_t _dcps_find_size (" << be_idt << be_idt_nl
      << "const " << node->name () << " &_tao_union" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "// all union discriminents are fixed size"
      <<     "so OK to use _dcps_max_marshaled_size" << be_nl
      << "size_t result = ";

  switch (node->udisc_type ())
    {
      case AST_Expression::EV_bool:
        *os << "_dcps_max_marshaled_size (::ACE_OutputCDR::from_boolean (_tao_union._d ()));" << be_nl;
        break;
      case AST_Expression::EV_char:
        *os << "_dcps_max_marshaled_size (::ACE_OutputCDR::from_char (_tao_union._d ()));" << be_nl;
        break;
      case AST_Expression::EV_wchar:
        *os << "_dcps_max_marshaled_size (::ACE_OutputCDR::from_wchar (_tao_union._d ()));" << be_nl;
        break;
      default:
        *os << "_dcps_max_marshaled_size (_tao_union._d ());" << be_nl;
        break;
    }

  *os << be_nl
      << "switch (_tao_union._d ())" << be_nl
      << "{" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_serializer_op_cs::"
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

    *os << be_uidt << be_uidt_nl << "}" << be_nl_2
      << "return result;" << be_uidt_nl
      << "}" << be_nl_2;

  //---------------------------------------------------------------
  //  Set the sub state as generating code for the output operator.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_OUTPUT);

  *os << "::CORBA::Boolean operator<< (" << be_idt << be_idt_nl
      << "TAO::DCPS::Serializer &strm," << be_nl
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
      << "return 0;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "::CORBA::Boolean result = true;" << be_nl_2
      << "switch (_tao_union._d ())" << be_nl
      << "{" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_serializer_op_cs::"
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

  //---------------------------------------------------------------
  // Set the substate as generating code for the input operator.
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);
  *os << "::CORBA::Boolean operator>> (" << be_idt << be_idt_nl
      << "TAO::DCPS::Serializer &strm," << be_nl
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
      << "return 0;" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl
      << "::CORBA::Boolean result = true;" << be_nl_2
      << "switch (_tao_discriminant)" << be_nl
      << "{" << be_idt;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_serializer_op_cs::"
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
      *os << "break;" << be_uidt;
    }

  *os << be_uidt_nl
      << "}" << be_nl_2
      << "return result;" << be_uidt_nl
      << "}";

  node->cli_stub_serializer_op_gen (1);
  return 0;
}

int
be_visitor_union_serializer_op_cs::pre_process (be_decl *bd)
{
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_SCOPE:
    case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
    case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
      return 0;
      break;

    case TAO_CodeGen::TAO_FIND_SIZE:
    case TAO_CodeGen::TAO_CDR_OUTPUT:
    case TAO_CodeGen::TAO_CDR_INPUT:
      {
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
      }
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                        "(%N:%l) be_visitor_union_serializer_op_cs::"
                        "pre_process - "
                        "bad sub_state.\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_serializer_op_cs::post_process (be_decl *bd)
{
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_SCOPE:
    case TAO_CodeGen::TAO_MAX_MARSHALED_SIZE:
    case TAO_CodeGen::TAO_IS_BOUNDED_SIZE:
      return 0;
      break;

    case TAO_CodeGen::TAO_FIND_SIZE:
    case TAO_CodeGen::TAO_CDR_OUTPUT:
    case TAO_CodeGen::TAO_CDR_INPUT:
      {

  if (bd->node_type () == AST_Decl::NT_enum_val)
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_uidt_nl << "}" << be_nl
      << "break;" << be_uidt;

      }
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                        "(%N:%l) be_visitor_union_serializer_op_cs::"
                        "pre_process - "
                        "bad sub_state.\n"),
                        -1);
    }

  return 0;
}
