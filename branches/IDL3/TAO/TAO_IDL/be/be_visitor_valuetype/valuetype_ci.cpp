//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Valuetypes in the client inline file
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    based on code from Aniruddha Gokhale
//
// ============================================================================

#include "be.h"
#include "be_visitor_valuetype.h"

ACE_RCSID (be_visitor_valuetype, 
           valuetype_ci, 
           "$Id$")


// **************************************************
// Valuetype visitor for client inline.
// **************************************************
be_visitor_valuetype_ci::be_visitor_valuetype_ci (be_visitor_context *ctx)
  : be_visitor_valuetype (ctx),
    opt_accessor_ (0)
{
}

be_visitor_valuetype_ci::~be_visitor_valuetype_ci (void)
{
}

int
be_visitor_valuetype_ci::visit_valuetype (be_valuetype *node)
{
  if (node->cli_inline_gen () || node->imported ())
    {
      return 0;
    }

  if (node->opt_accessor ())
    {
      this->opt_accessor_ = 1;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  os->indent ();

  *os << be_nl << "// TAO_IDL - Generated from "
      << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << "ACE_INLINE" << be_nl;
  *os << node->name () << "::" << node->local_name () <<
    " () // default constructor" << be_nl;
  *os << "{}" << be_nl << be_nl;

  *os << "ACE_INLINE" << be_nl;
  *os << node->name () << "::~" << node->local_name () <<
    " () // destructor" << be_nl;
  *os << "{}\n" << be_nl;

  *os << "ACE_INLINE const char* " << be_nl
      << node->name() << "::_tao_obv_static_repository_id ()" << be_nl
      <<  "{" << be_idt_nl
      <<     "return \"" << node->repoID () << "\";" << be_uidt_nl
      <<  "}\n\n";

  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ci::"
                         "visit_valuetype - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  // Generate the _init-related code.
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_VALUETYPE_INIT_CI);
  be_visitor_valuetype_init_ci visitor (&ctx);

  if (visitor.visit_valuetype (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_ci::"
                         "visit_valuetype - "
                         "failed to generate _init construct.\n"),  
                        -1);
    }

  return 0;
}


int
be_visitor_valuetype_ci::visit_field (be_field *node)
{
  if (opt_accessor_)
    {
      be_visitor_context ctx (*this->ctx_);
      be_visitor_valuetype_field_cs visitor (&ctx);
      visitor.in_obv_space_ = 0;
      visitor.setenclosings ("ACE_INLINE ");

      if (visitor.visit_field (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_ci::"
                             "visit_field - "
                             "visit_field failed\n"), 
                            -1);
        }
    }

  return 0;
}
