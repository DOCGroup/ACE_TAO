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

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_union.h"

ACE_RCSID(be_visitor_union, cdr_op_ci, "$Id$")

// ***************************************************************************
// Union visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_union_cdr_op_ci::be_visitor_union_cdr_op_ci
(be_visitor_context *ctx)
  : be_visitor_union (ctx)
{
}

be_visitor_union_cdr_op_ci::~be_visitor_union_cdr_op_ci (void)
{
}

int
be_visitor_union_cdr_op_ci::visit_union (be_union *node)
{
  // already generated and/or we are imported. Don't do anything.
  if (node->cli_inline_cdr_op_gen () ||
      node->imported () ||
      node->is_local ())
    return 0;

  // set the substate as generating code for the types defined in our scope
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_SCOPE);
  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_ci"
                         "::visit_union - "
                         "codegen for scope failed\n"), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  //  set the sub state as generating code for the output operator
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_OUTPUT);

  *os << "ACE_INLINE CORBA::Boolean operator<< (" << be_idt << be_idt_nl
      << "TAO_OutputCDR &strm," << be_nl
      << "const " << node->name () << " &_tao_union" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  switch (node->udisc_type ())
    {
      case AST_Expression::EV_bool:
        *os << "CORBA_Any::from_boolean tmp (_tao_union._d ());" << be_nl
            << "if ( !(strm << tmp) )" << be_idt_nl;
        break;

      case AST_Expression::EV_char:
        *os << "CORBA_Any::from_char tmp (_tao_union._d ());" << be_nl
            << "if ( !(strm << tmp) )" << be_idt_nl;
        break;

      case AST_Expression::EV_wchar:
        *os << "CORBA_Any::from_wchar tmp (_tao_union._d ());" << be_nl
            << "if ( !(strm << tmp) )" << be_idt_nl;
        break;

      default:
        *os << "if ( !(strm << _tao_union._d ()) )" << be_idt_nl;
        break;
    }

  *os << "{" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "CORBA::Boolean result = 1;" << be_nl
      << "switch (_tao_union._d ())" << be_nl
      << "{" << be_idt_nl;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_ci::"
                         "visit_union - "
                         "codegen for scope failed\n"), -1);
    }

  // If there is no explicit default case, but there
  // is an implicit one, and the discriminant is an enum,
  // we need this to avert warnings in some compilers that
  // not all case values are included. If there is no
  // implicit default case, or the discriminator is not
  // an enum, this does no harm.
  if (node->default_index () == -1)
    {
      os->decr_indent (0);
      *os << "default:" << be_nl;
      os->incr_indent ();
      *os << "break;";
    }

  *os << be_uidt_nl << "}" << be_nl
      << "return result;" << be_uidt_nl
      << "}\n\n";

  // set the substate as generating code for the input operator
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);
  *os << "ACE_INLINE CORBA::Boolean operator>> (" << be_idt << be_idt_nl
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
        *os << "CORBA_Any::to_boolean tmp (_tao_discriminant);" << be_nl
            << "if ( !(strm >> tmp) )" << be_idt_nl;
        break;

      case AST_Expression::EV_char:
        *os << "CORBA_Any::to_char tmp (_tao_discriminant);" << be_nl
            << "if ( !(strm >> tmp) )" << be_idt_nl;
        break;

      case AST_Expression::EV_wchar:
        *os << "CORBA_Any::to_wchar tmp (_tao_discriminant);" << be_nl
            << "if ( !(strm >> tmp) )" << be_idt_nl;
        break;

      default:
        *os << "if ( !(strm >> _tao_discriminant) )" << be_idt_nl;
        break;
    }

  *os << "{" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "CORBA::Boolean result = 1;" << be_nl
      << "switch (_tao_discriminant)" << be_nl
      << "{" << be_idt_nl;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_ci::"
                         "visit_union - "
                         "codegen for scope failed\n"), -1);
    }

  // If there is no explicit default case, but there
  // is an implicit one, and the discriminant is an enum,
  // we need this to avert warnings in some compilers that
  // not all case values are included. If there is no
  // implicit default case, or the discriminator is not
  // an enum, this does no harm.
  if (node->default_index () == -1)
    {
      os->decr_indent (0);
      *os << "default:" << be_nl;
      os->incr_indent ();
      *os << "_tao_union._d (_tao_discriminant);" << be_idt_nl;
      *os << "break;" << be_uidt << be_uidt_nl;
    }

  *os << "}" << be_nl
      << "return result;" << be_uidt_nl
      << "}\n\n";

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

  TAO_OutStream *os = this->ctx_->stream ();

  be_union_branch* b =
    be_union_branch::narrow_from_decl (bd);

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
be_visitor_union_cdr_op_ci::post_process (be_decl *)
{
  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_SCOPE)
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_uidt_nl << "}" << be_nl
      << "break;" << be_uidt_nl;

  return 0;
}
