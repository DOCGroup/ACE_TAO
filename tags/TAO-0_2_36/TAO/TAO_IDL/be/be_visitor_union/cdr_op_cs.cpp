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

  TAO_OutStream *os = this->ctx_->stream ();

  //  set the sub state as generating code for the output operator
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_OUTPUT);
  *os << "ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, "
      << "const " << node->name () << " &_tao_union)" << be_nl
      << "{" << be_idt_nl
      << "return 1;" << be_uidt_nl
      << "}\n\n";
#if 0 /* TO-DO */
      << "if (" << be_idt_nl;

  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_cs::"
                         "visit_union - "
                         "codegen for scope failed\n"), -1);
    }
  *os << be_uidt_nl << ")"
      << be_idt_nl
      << "return 1;" << be_uidt_nl
      << "else" << be_idt_nl
      << "return 0;" << be_uidt_nl << be_uidt_nl
      << "}\n\n";
#endif 

  // set the substate as generating code for the input operator
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);
  *os << "ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, "
      << node->name () << " &_tao_union)" << be_nl
      << "{" << be_idt_nl
      << "return 1;" << be_uidt_nl
      << "}\n\n";

#if 0 /* TO-DO */
      << "if (" << be_idt_nl;

  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_cs"
                         "::visit_union - "
                         "codegen for scope failed\n"), -1);
    }
  *os << be_uidt_nl << ")"
      << be_idt_nl
      << "return 1;" << be_uidt_nl
      << "else" << be_idt_nl
      << "return 0;" << be_uidt_nl << be_uidt_nl
      << "}\n\n";
#endif

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

  node->cli_stub_cdr_op_gen (1);
  return 0;
}

int
be_visitor_union_cdr_op_cs::post_process (be_decl *bd)
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
