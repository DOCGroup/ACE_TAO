//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for CDR operators for unions
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union.h"

ACE_RCSID(be_visitor_union, cdr_op_cs, "$Id$")

// ***************************************************************************
// Union visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_union_cdr_op_cs::be_visitor_union_cdr_op_cs
(be_visitor_context *ctx)
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
  if (node->cli_stub_cdr_op_gen () || node->imported ())
    return 0;

  // set the substate as generating code for the types defined in our scope
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_SCOPE);
  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_cs"
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
      << "{" << be_idt_nl
      << "if (!(strm << _tao_union._d ()))" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "CORBA::Boolean result = 0;" << be_nl
      << "switch (_tao_union._d ())" << be_nl
      << "{" << be_idt_nl;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_cs::"
                         "visit_union - "
                         "codegen for scope failed\n"), -1);
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
  *os << disc_type->fullname ()
      << " " << "_tao_discriminant;" << be_nl
      << "if ( !(strm >> _tao_discriminant) )" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "CORBA::Boolean result = 0;" << be_nl
      << "switch (_tao_discriminant)" << be_nl
      << "{" << be_idt_nl;

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_cs::"
                         "visit_union - "
                         "codegen for scope failed\n"), -1);
    }

  *os << be_uidt_nl << "}" << be_nl
      << "return result;" << be_uidt_nl
      << "}\n\n";

  node->cli_stub_cdr_op_gen (1);
  return 0;
}

int
be_visitor_union_cdr_op_cs::pre_process (be_decl *bd)
{
  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_SCOPE)
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  be_union_branch* b =
    be_union_branch::narrow_from_decl (bd);

  if (b->label ()->label_kind () == AST_UnionLabel::UL_default)
    {
      *os << "default:" << be_idt_nl
          << "result = ";
    }
  else
    {
      *os << "case ";
      b->gen_label_value (os);
      *os << ":" << be_idt_nl
          << "{" << be_idt_nl;
    }

  return 0;
}

int
be_visitor_union_cdr_op_cs::post_process (be_decl *bd)
{
  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_SCOPE)
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_uidt_nl << "}"
      << be_nl << "break;" << be_uidt_nl;

  return 0;
}
