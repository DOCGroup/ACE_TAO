
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

be_visitor_valuetype_obv_cs::be_visitor_valuetype_obv_cs (
    be_visitor_context *ctx
  )
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
  if (node->is_abstract () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl << be_nl << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  *os << node->full_obv_skel_name () << "::~";

  if (! node->is_nested ())
    {
      *os << "OBV_";
    }

  *os << node->local_name () << " (void)" << be_nl
      << "{}";

  // OBV_ class has no accessors or modifiers if we are optimizing
  // or the valuetype is abstract.
  if (!node->opt_accessor ())
    {
      *os << be_nl << be_nl << "CORBA::Boolean" << be_nl
          << node->full_obv_skel_name ()
          << "::_tao_marshal__" << node->flat_name ()
          <<    " (TAO_OutputCDR &strm)" << be_nl
          << "{" << be_idt_nl
          << "return _tao_marshal_state (strm);" << be_uidt_nl 
          << "}" << be_nl << be_nl;

      *os << "CORBA::Boolean" << be_nl
          << node->full_obv_skel_name ()
          << "::_tao_unmarshal__" << node->flat_name ()
          << " (TAO_InputCDR &strm)" << be_nl
          << "{" << be_idt_nl
          << "return _tao_unmarshal_state (strm);" << be_uidt_nl 
          << "}";

      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_obv_cs::"
                             "visit_valuetype - "
                             "visit_scope failed\n"), 
                            -1);
        }

      // If we inherit from both CORBA::ValueBase and CORBA::AbstractBase,
      // we have to add this to avoid ambiguity.
      if (node->supports_abstract ())
        {
          *os << be_nl << be_nl << "void" << be_nl
              << node->full_obv_skel_name () 
              << "::_add_ref (void)" << be_nl
              << "{" << be_idt_nl
              << "this->DefaultValueRefCountBase::_add_ref ();"
              << be_uidt_nl
              << "}" << be_nl;

          *os << be_nl << "void" << be_nl
              << node->full_obv_skel_name () 
              << "::_remove_ref (void)" << be_nl
              << "{" << be_idt_nl
              << "this->DefaultValueRefCountBase::_remove_ref ();"
              << be_uidt_nl
              << "}";
        }
    }

  return 0;
}

int
be_visitor_valuetype_obv_cs::visit_field (be_field *node)
{
  be_visitor_context ctx = (*this->ctx_);
  be_visitor_valuetype_field_cs visitor (&ctx);
  visitor.in_obv_space_ = 1;

  if (visitor.visit_field (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_obv_cs::"
                         "visit_field - "
                         "visit_field failed\n"
                         ), -1);
    }
 
  return 0;
}
