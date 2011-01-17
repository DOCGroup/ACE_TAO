
//=============================================================================
/**
 *  @file    valuetype_ci.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Valuetypes in the client inline file
 *
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de> based on code from Aniruddha Gokhale
 */
//=============================================================================


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

  *os << be_nl_2 << "// TAO_IDL - Generated from " << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << "ACE_INLINE" << be_nl;
  *os << node->name () << "::" << node->local_name () << " (void)" << be_nl;

  if (node->truncatable())
    {
      *os << "{" << be_idt_nl
          << "this->is_truncatable_ = true;" << be_uidt_nl
          << "}" << be_nl_2;
    }
  else
    *os << "{}" << be_nl_2;


  *os << "ACE_INLINE const char* " << be_nl
      << node->name () << "::_tao_obv_static_repository_id ()" << be_nl
      <<  "{" << be_idt_nl
      <<     "return \"" << node->repoID () << "\";" << be_uidt_nl
      <<  "}";

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
be_visitor_valuetype_ci::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
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
