
//=============================================================================
/**
 *  @file    any_op_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Any operators for valueboxes in the stubs
 *  file.
 *
 *
 *  @author Gary Maxey
 */
//=============================================================================


// ***************************************************************************
// Valuebox visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_valuebox_any_op_cs::be_visitor_valuebox_any_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_valuebox (ctx)
{
}

be_visitor_valuebox_any_op_cs::~be_visitor_valuebox_any_op_cs (void)
{
}

int
be_visitor_valuebox_any_op_cs::visit_valuebox (be_valuebox *node)
{
  if (node->cli_stub_any_op_gen ()
      || node->imported ()
      || node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Generate the Any <<= and >>= operator declarations

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_global->core_versioning_begin () << be_nl;

  // All template specializations must be generated before the instantiations
  // in the operators.
  *os << be_nl_2
      << "namespace TAO" << be_nl
      << "{" << be_idt_nl
      << "template<>" << be_nl
      << "::CORBA::Boolean" << be_nl
      << "Any_Impl_T<" << node->name () << ">::to_value ("
      << be_idt <<  be_idt_nl
      << "::CORBA::ValueBase *&_tao_elem" << be_uidt_nl
      << ") const" << be_uidt_nl
      << "{" << be_idt_nl
      << "::CORBA::add_ref (this->value_);" << be_nl
      << "_tao_elem = this->value_;" << be_nl
      << "return true;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl_2;

  *os << be_global->core_versioning_end () << be_nl;

  be_module *module = 0;

  if (node->is_nested () &&
      node->defined_in ()->scope_node_type () == AST_Decl::NT_module)
    {
      module = be_module::narrow_from_scope (node->defined_in ());

      if (!module)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_valuebox_any_op_cs::"
                             "visit_valuebox - "
                             "Error parsing nested name\n"),
                            -1);
        }

      // Some compilers handle "any" operators in a namespace corresponding
      // to their module, others do not.
      *os << "\n\n#if defined (ACE_ANY_OPS_USE_NAMESPACE)\n";

      be_util::gen_nested_namespace_begin (os, module);

      // emit  nested variation of any operators
      *os << "// Copying insertion." << be_nl
          << "void" << be_nl
          << "operator<<= (" << be_idt << be_idt_nl
          << "::CORBA::Any &_tao_any," << be_nl
          << node->local_name () << " *_tao_elem)" << be_uidt << be_uidt_nl
          << "{" << be_idt_nl
          << "::CORBA::add_ref (_tao_elem);" << be_nl
          << "_tao_any <<= &_tao_elem;" << be_uidt_nl
          << "}" << be_nl_2;

      *os << "// Non-copying insertion." << be_nl
          << "void" << be_nl
          << "operator<<= (" << be_idt << be_idt_nl
          << "::CORBA::Any &_tao_any," << be_nl
          << node->local_name () << " **_tao_elem)" << be_uidt << be_uidt_nl
          << "{" << be_idt_nl
          << "TAO::Any_Impl_T<" << node->local_name () << ">::insert ("
          << be_idt << be_idt_nl
          << "_tao_any," << be_nl
          << node->local_name () << "::_tao_any_destructor," << be_nl
          << node->tc_name ()->last_component () << "," << be_nl
          << "*_tao_elem);" << be_uidt << be_uidt << be_uidt_nl
          << "}" << be_nl_2;

      *os << "::CORBA::Boolean" << be_nl
          << "operator>>= (" << be_idt << be_idt_nl
          << "const ::CORBA::Any &_tao_any," << be_nl
          << node->local_name () << " *&_tao_elem" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "return" << be_idt_nl
          << "TAO::Any_Impl_T<" << node->local_name () << ">::extract ("
          << be_idt << be_idt_nl
          << "_tao_any," << be_nl
          << node->local_name () << "::_tao_any_destructor," << be_nl
          << node->tc_name ()->last_component () << "," << be_nl
          << "_tao_elem" << be_uidt_nl
          << ");" << be_uidt << be_uidt << be_uidt_nl
          << "}" << be_nl_2;

      be_util::gen_nested_namespace_end (os, module);

      // Emit #else.
      *os << be_nl_2
          << "#else\n\n";
    }

  *os << be_global->core_versioning_begin () << be_nl;

  *os << "// Copying insertion." << be_nl
      << "void" << be_nl
      << "operator<<= (" << be_idt << be_idt_nl
      << "::CORBA::Any &_tao_any," << be_nl
      << node->full_name () << " *_tao_elem" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "::CORBA::add_ref (_tao_elem);" << be_nl
      << "_tao_any <<= &_tao_elem;" << be_uidt_nl
      << "}" << be_nl_2;

  *os << "// Non-copying insertion." << be_nl
      << "void" << be_nl
      << "operator<<= (" << be_idt << be_idt_nl
      << "::CORBA::Any &_tao_any," << be_nl
      << node->full_name () << " **_tao_elem" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "TAO::Any_Impl_T<" << node->name () << ">::insert ("
      << be_idt << be_idt_nl
      << "_tao_any," << be_nl
      << node->name () << "::_tao_any_destructor," << be_nl
      << node->tc_name () << "," << be_nl
      << "*_tao_elem" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  *os << "::CORBA::Boolean" << be_nl
      << "operator>>= (" << be_idt << be_idt_nl
      << "const ::CORBA::Any &_tao_any," << be_nl
      << node->full_name () << " *&_tao_elem" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return" << be_idt_nl
      << "TAO::Any_Impl_T<" << node->name () << ">::extract ("
      << be_idt << be_idt_nl
      << "_tao_any," << be_nl
      << node->name () << "::_tao_any_destructor," << be_nl
      << node->tc_name () << "," << be_nl
      << "_tao_elem" << be_uidt_nl
      << ");" << be_uidt << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  *os << be_global->core_versioning_end () << be_nl;

  if (module != 0)
    {
      *os << "\n\n#endif";
    }

  node->cli_stub_any_op_gen (1);
  return 0;
}
