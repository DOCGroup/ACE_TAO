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
//    Visitor generating code for CDR operators for exceptions
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_exception.h"
#include "be_visitor_field.h"

ACE_RCSID(be_visitor_exception, cdr_op_cs, "$Id$")

// ***************************************************************************
// Exception visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_exception_cdr_op_cs::be_visitor_exception_cdr_op_cs
(be_visitor_context *ctx)
  : be_visitor_exception (ctx)
{
}

be_visitor_exception_cdr_op_cs::~be_visitor_exception_cdr_op_cs (void)
{
}

int
be_visitor_exception_cdr_op_cs::visit_exception (be_exception *node)
{
  // already generated and/or we are imported. Don't do anything.
  if (node->cli_stub_cdr_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  // First generate code for our children. The reason we do this first is
  // because the inlined code for our children must be available before we use
  // it in our parent

  // set the substate as generating code for the types defined in our scope
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_SCOPE);
  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception_cdr_op_cs"
                         "::visit_exception - "
                         "codegen for scope failed\n"), -1);
    }

  //  set the sub state as generating code for the output operator
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_OUTPUT);
  *os << "ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, "
      << "const " << node->name () << " &_tao_aggregate)" << be_nl
      << "{" << be_idt_nl;
  // do we have any members?
  if (node->nmembers () > 0)
    {
      be_visitor_context* new_ctx =
        new be_visitor_context (*this->ctx_);
      be_visitor_cdr_op_field_decl field_decl (new_ctx);
      field_decl.visit_scope (node);

      // some members
      *os << "// first marshal the repository ID" << be_nl
          << "if (strm << _tao_aggregate._id ())" << be_nl
          << "{" << be_idt_nl
          << "// now marshal the members (if any)" << be_nl
          << "if (" << be_idt_nl;
      
      // all we have to do is to visit the scope and generate code
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_exception_cdr_op_cs::"
                             "visit_exception - "
                             "codegen for scope failed\n"), -1);
        }
      *os << be_uidt_nl << ")"
          << be_idt_nl
          << "return 1;" << be_uidt_nl
          << "else" << be_idt_nl
          << "return 0;" << be_uidt << be_uidt_nl
          << "}" << be_nl
          << "else" << be_idt_nl
          << "return 0;" << be_uidt << be_uidt_nl;
    }
  else
    {
      // no members
      *os << "// first marshal the repository ID" << be_nl
          << "if (strm << _tao_aggregate._id ())" << be_idt_nl
          << "return 1;" << be_uidt_nl
          << "else" << be_idt_nl
          << "return 0;" << be_uidt << be_uidt_nl;
    }
  *os << "}\n\n";

  // set the substate as generating code for the input operator
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);
  *os << "ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, "
      << node->name () << " &_tao_aggregate)" << be_nl
      << "{" << be_idt_nl;
  // retrieve the repository ID and compare with what we have
  *os << "// retrieve  RepoID and verify if we are of that type" << be_nl;
  *os << "char *_tao_repoID;" << be_nl;
  *os << "if ((strm >> _tao_repoID) &&" << be_nl
      << "    (_tao_aggregate._is_a (_tao_repoID)))" << be_nl
      << "{" << be_idt_nl;
  // do we have any members?
  if (node->nmembers () > 0)
    {
      be_visitor_context* new_ctx =
        new be_visitor_context (*this->ctx_);
      be_visitor_cdr_op_field_decl field_decl (new_ctx);
      field_decl.visit_scope (node);

      // some members
      *os << "// now marshal the members" << be_nl
          << "if (" << be_idt_nl;
      
      // all we have to do is to visit the scope and generate code
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_exception_cdr_op_cs::"
                             "visit_exception - "
                             "codegen for scope failed\n"), -1);
        }
      *os << be_uidt_nl << ")"
          << be_idt_nl
          << "return 1;" << be_uidt_nl
          << "else" << be_idt_nl
          << "return 0;" << be_uidt << be_uidt_nl;
    }
  else
    {
      *os << "return 1;" << be_uidt_nl;
    }
  *os << "}" << be_nl
      << "else" << be_idt_nl
      << "return 0;" << be_uidt << be_uidt_nl
      << "}\n\n";

  node->cli_stub_cdr_op_gen (1);
  return 0;
}

int
be_visitor_exception_cdr_op_cs::post_process (be_decl *bd)
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
