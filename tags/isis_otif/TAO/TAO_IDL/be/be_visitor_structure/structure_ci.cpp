//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    structure_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Structure in the inline file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_structure, 
           structure_ci, 
           "$Id$")

// ******************************************************
// for client inline
// ******************************************************

be_visitor_structure_ci::be_visitor_structure_ci (be_visitor_context *ctx)
  : be_visitor_structure (ctx)
{
}

be_visitor_structure_ci::~be_visitor_structure_ci (void)
{
}

// visit the Structure node and its scope
int be_visitor_structure_ci::visit_structure (be_structure *node)
{
  if (node->cli_inline_gen () || node->imported ())
    {
      return 0;
    }

  if (node->gen_var_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_ci::"
                         "visit_structure - "
                         "codegen for _var failed\n"), 
                        -1);
    }

  if (node->size_type () == AST_Type::VARIABLE &&
      node->gen_out_impl () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_ci::"
                         "visit_structure - "
                         "codegen for _out failed\n"), 
                        -1);
    }

  // all we have to do is to visit the scope
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_ci::"
                         "visit_structure - "
                         "codegen for scope failed\n"), 
                        -1);
    }

  // Since we don't generate CDR stream operators for types that
  // explicitly contain a local interface (at some level), we 
  // must override these Any template class methods to avoid
  // calling the non-existent operators. The zero return value
  // will eventually cause CORBA::MARSHAL to be raised if this
  // type is inserted into an Any and then marshaled.
  if (node->is_local () && be_global->any_support ())
    {
      TAO_OutStream *os = this->ctx_->stream ();

      *os << be_nl << be_nl
          << "template<>" << be_nl
          << "ACE_INLINE" << be_nl
          << "CORBA::Boolean" << be_nl
          << "TAO::Any_Dual_Impl_T<" << node->name ()
          << ">::marshal_value (TAO_OutputCDR &)" << be_nl
          << "{" << be_idt_nl
          << "return 0;" << be_uidt_nl
          << "}";

      *os << be_nl << be_nl
          << "template<>" << be_nl
          << "ACE_INLINE" << be_nl
          << "CORBA::Boolean" << be_nl
          << "TAO::Any_Dual_Impl_T<" << node->name ()
          << ">::demarshal_value (TAO_InputCDR &)" << be_nl
          << "{" << be_idt_nl
          << "return 0;" << be_uidt_nl
          << "}";
    }

  node->cli_inline_gen (I_TRUE);
  return 0;
}
