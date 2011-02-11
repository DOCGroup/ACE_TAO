

//=============================================================================
/**
 *  @file    valuetype_obv_ci.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Valuetypes
 *  OBV_ class implementation
 *  (see C++ mapping OMG 20.17)
 *
 *
 *  @author Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
 *  @author based on interface_ch.cpp from Aniruddha Gokhale
 */
//=============================================================================

// ******************************************************
// Valuetype visitor for OBV_ class implementation
// ******************************************************

be_visitor_valuetype_obv_ci::be_visitor_valuetype_obv_ci (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_obv_ci::~be_visitor_valuetype_obv_ci (void)
{
}


// OBV_ class must be in OBV_ namespace
int
be_visitor_valuetype_obv_ci::visit_valuetype (be_valuetype *node)
{
  // only visit non-abstract valuetype
  if (node->is_abstract ())
    {
      return 0;
    }

  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();
  os->indent ();

  // OBV_ class is only a typedef if we are optimizing accessors
  if (node->opt_accessor ())
    {
    }
  else
    {
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_obv_cs::"
                             "visit_valuetype - "
                             "visit_scope failed\n"
                             ), -1);
        }
    } // if !opt_accessor ()

  return 0;
}

int
be_visitor_valuetype_obv_ci::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int
be_visitor_valuetype_obv_ci::visit_field (be_field *node)
{
  be_visitor_context new_ctx (*this->ctx_);
  be_visitor_valuetype_field_ci visitor (&new_ctx);

  visitor.in_obv_space_ = 1;

  if (visitor.visit_field (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_obv_ci::"
                         "visit_field - "
                         "visit_field failed\n"),
                        -1);
    }

  return 0;
}
