
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_obv_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Valuetypes
//    OBV_ class implementation
//    (see C++ mapping OMG 20.17)
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>,
//    derived from interface_ch.cpp from Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_valuetype, 
           valuetype_obv_cs, 
           "$Id$")

// ******************************************************
// Valuetype visitor for OBV_ class implementation
// ******************************************************

be_visitor_valuetype_obv_cs::be_visitor_valuetype_obv_cs (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_valuetype_obv_cs::~be_visitor_valuetype_obv_cs (void)
{
}


// OBV_ class must be in OBV_ namespace
int
be_visitor_valuetype_obv_cs::visit_valuetype (be_valuetype *node)
{
  // only visit non-abstract non-imported valuetype
  if (node->is_abstract_valuetype () || node->imported ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  // OBV_ class has no accessors or modifiers if we are optimizing
  // or the valuetype is abstract.
  if (!(node->opt_accessor () || node->is_abstract_valuetype () ))
    {
      os->indent ();
      *os << "CORBA::Boolean " << be_nl
          << node->full_obv_skel_name ()
          << "::_tao_marshal__" << node->flat_name ()
          <<    " (TAO_OutputCDR &strm)"
          << "{" << be_idt_nl
          <<   "return _tao_marshal_state (strm);" << be_nl
          << be_uidt_nl << "}\n";

      *os << "CORBA::Boolean "
          << node->full_obv_skel_name ()
          << "::_tao_unmarshal__" << node->flat_name ()
          <<    " (TAO_InputCDR &strm)"
          << "{" << be_idt_nl
          <<   "return _tao_unmarshal_state (strm);" << be_nl
          << be_uidt_nl << "}\n";

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_obv_cs::"
                             "visit_valuetype - "
                             "visit_scope failed\n"
                             ), -1);
        }
      }
  return 0;
}

int
be_visitor_valuetype_obv_cs::visit_field (be_field *node)
{
  be_visitor_context *ctx = new be_visitor_context (*this->ctx_);
  be_visitor_valuetype_field_cs *visitor =
    new be_visitor_valuetype_field_cs (ctx);
  visitor->in_obv_space_ = 1;
  if (visitor->visit_field (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_obv_cs::"
                         "visit_field - "
                         "visit_field failed\n"
                         ), -1);
    }
  delete visitor;
  return 0;
}
