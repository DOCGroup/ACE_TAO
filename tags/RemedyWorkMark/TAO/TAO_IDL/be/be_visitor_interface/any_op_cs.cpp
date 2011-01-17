// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    any_op_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Any operators for Interface in the stubs
//    file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

// ***************************************************************************
// Interface visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_interface_any_op_cs::be_visitor_interface_any_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_any_op_cs::~be_visitor_interface_any_op_cs (void)
{
}

int
be_visitor_interface_any_op_cs::visit_interface (be_interface *node)
{
  if (node->cli_stub_any_op_gen ()
      || node->imported ()
      || (node->is_local () && !be_global->gen_local_iface_anyops ()))
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2 << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  // All template specializations must be generated before the instantiations
  // in the operators.

  *os << be_global->core_versioning_begin () << be_nl;

  if (! node->is_abstract ())
    {
      *os << be_nl_2
          << "namespace TAO" << be_nl
          << "{" << be_idt_nl
          << "template<>" << be_nl
          << "::CORBA::Boolean" << be_nl
          << "Any_Impl_T<" << node->name () << ">::to_object ("
          << be_idt <<  be_idt_nl
          << "::CORBA::Object_ptr &_tao_elem" << be_uidt_nl
          << ") const" << be_uidt_nl
          << "{" << be_idt_nl
          << "_tao_elem = ::CORBA::Object::_duplicate (this->value_);" << be_nl
          << "return true;" << be_uidt_nl
          << "}" << be_uidt_nl
          << "}";
    }

  if (node->is_abstract () || node->has_mixed_parentage ())
    {
      *os << be_nl_2
          << "namespace TAO" << be_nl
          << "{" << be_idt_nl
          << "template<>" << be_nl
          << "::CORBA::Boolean" << be_nl
          << "Any_Impl_T<" << node->name ()
          << ">::to_abstract_base ("
          << be_idt <<  be_idt_nl
          << "::CORBA::AbstractBase_ptr &_tao_elem" << be_uidt_nl
          << ") const" << be_uidt_nl
          << "{" << be_idt_nl
          << "_tao_elem = ::CORBA::AbstractBase::_duplicate (this->value_);"
          << be_nl
          << "return true;" << be_uidt_nl
          << "}" << be_uidt_nl
          << "}";
    }

  // Since we don't generate CDR stream operators for types that
  // explicitly contain a local interface (at some level), we
  // must override these Any template class methods to avoid
  // calling the non-existent operators. The zero return value
  // will eventually cause CORBA::MARSHAL to be raised if this
  // type is inserted into an Any and then marshaled.
  if (node->is_local ())
    {
      *os << be_nl_2
          << "namespace TAO" << be_nl
          << "{" << be_idt_nl
          << "template<>" << be_nl
          << "::CORBA::Boolean" << be_nl
          << "Any_Impl_T<" << node->name ()
          << ">::marshal_value (TAO_OutputCDR &)" << be_nl
          << "{" << be_idt_nl
          << "return false;" << be_uidt_nl
          << "}";

      *os << be_nl_2
          << "template<>" << be_nl
          << "::CORBA::Boolean" << be_nl
          << "Any_Impl_T<" << node->name ()
          << ">::demarshal_value (TAO_InputCDR &)" << be_nl
          << "{" << be_idt_nl
          << "return false;" << be_uidt_nl
          << "}" << be_uidt_nl
          << "}";
    }

  *os << be_global->core_versioning_end () << be_nl;

  be_module *module = 0;

  if (node->is_nested ())
    {
      AST_Decl *d = node;
      AST_Decl::NodeType nt = d->node_type ();

      while (nt != AST_Decl::NT_root)
        {
          if (nt == AST_Decl::NT_module)
            {
              module = be_module::narrow_from_decl (d);
              break;
            }
          else
            {
              d = ScopeAsDecl (d->defined_in ());
              nt = d->node_type ();
            }
        }

      if (module != 0)
        {
          // Some compilers handle "any" operators in a namespace corresponding
          // to their module, others do not.
          *os << "\n\n#if defined (ACE_ANY_OPS_USE_NAMESPACE)\n";

          be_util::gen_nested_namespace_begin (os, module);

          // emit  nested variation of any operators
          *os << be_nl_2
              << "/// Copying insertion." << be_nl
              << "void" << be_nl
              << "operator<<= (" << be_idt << be_idt_nl
              << "::CORBA::Any &_tao_any," << be_nl
              << node->local_name () << "_ptr _tao_elem)" << be_uidt << be_uidt_nl
              << "{" << be_idt_nl
              << node->local_name () << "_ptr _tao_objptr =" << be_idt_nl
              << node->local_name () << "::_duplicate (_tao_elem);" << be_uidt_nl
              << "_tao_any <<= &_tao_objptr;" << be_uidt_nl
              << "}" << be_nl_2;

          *os << "/// Non-copying insertion." << be_nl
              << "void" << be_nl
              << "operator<<= (" << be_idt << be_idt_nl
              << "::CORBA::Any &_tao_any," << be_nl
              << node->local_name () << "_ptr *_tao_elem)" << be_uidt << be_uidt_nl
              << "{" << be_idt_nl
              << "TAO::Any_Impl_T<" << node->local_name () << ">::insert ("
              << be_idt << be_idt_nl
              << "_tao_any," << be_nl
              << node->local_name () << "::_tao_any_destructor," << be_nl
              << node->tc_name ()->last_component () << "," << be_nl
              << "*_tao_elem);" << be_uidt
              << be_uidt << be_uidt_nl
              << "}" << be_nl_2;

          *os << "::CORBA::Boolean" << be_nl
              << "operator>>= (" << be_idt << be_idt_nl
              << "const ::CORBA::Any &_tao_any," << be_nl
              << node->local_name () << "_ptr &_tao_elem)" << be_uidt << be_uidt_nl
              << "{" << be_idt_nl
              << "return" << be_idt_nl
              << "TAO::Any_Impl_T<" << node->local_name () << ">::extract ("
              << be_idt << be_idt_nl
              << "_tao_any," << be_nl
              << node->local_name () << "::_tao_any_destructor," << be_nl
              << node->tc_name ()->last_component () << "," << be_nl
              << "_tao_elem);" << be_uidt << be_uidt
              << be_uidt << be_uidt_nl
              << "}";

          be_util::gen_nested_namespace_end (os, module);

          // Emit #else.
          *os << be_nl_2
              << "#else\n";
        }
    }

  *os << be_global->core_versioning_begin () << be_nl;

  *os << be_nl_2
      << "/// Copying insertion." << be_nl
      << "void" << be_nl
      << "operator<<= (" << be_idt << be_idt_nl
      << "::CORBA::Any &_tao_any," << be_nl
      << node->full_name () << "_ptr _tao_elem)" << be_uidt << be_uidt_nl
      << "{" << be_idt_nl
      << node->full_name () << "_ptr _tao_objptr =" << be_idt_nl
      << node->full_name () << "::_duplicate (_tao_elem);" << be_uidt_nl
      << "_tao_any <<= &_tao_objptr;" << be_uidt_nl
      << "}" << be_nl_2;

  *os << "/// Non-copying insertion." << be_nl
      << "void" << be_nl
      << "operator<<= (" << be_idt << be_idt_nl
      << "::CORBA::Any &_tao_any," << be_nl
      << node->full_name () << "_ptr *_tao_elem)" << be_uidt << be_uidt_nl
      << "{" << be_idt_nl
      << "TAO::Any_Impl_T<" << node->name () << ">::insert ("
      << be_idt << be_idt_nl
      << "_tao_any," << be_nl
      << node->name () << "::_tao_any_destructor," << be_nl
      << node->tc_name () << "," << be_nl
      << "*_tao_elem);" << be_uidt
      << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  *os << "::CORBA::Boolean" << be_nl
      << "operator>>= (" << be_idt << be_idt_nl
      << "const ::CORBA::Any &_tao_any," << be_nl
      << node->full_name () << "_ptr &_tao_elem)" << be_uidt << be_uidt_nl
      << "{" << be_idt_nl
      << "return" << be_idt_nl
      << "TAO::Any_Impl_T<" << node->name () << ">::extract ("
      << be_idt << be_idt_nl
      << "_tao_any," << be_nl
      << node->name () << "::_tao_any_destructor," << be_nl
      << node->tc_name () << "," << be_nl
      << "_tao_elem);" << be_uidt
      << be_uidt << be_uidt << be_uidt_nl
      << "}" << be_nl;

  *os << be_global->core_versioning_end () << be_nl;

  if (module != 0)
    {
      *os << "\n\n#endif";
    }

  // All we have to do is to visit the scope and generate code.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  node->cli_stub_any_op_gen (1);
  return 0;
}

int
be_visitor_interface_any_op_cs::visit_component (
  be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_interface_any_op_cs::visit_connector (
  be_connector *node)
{
  return this->visit_interface (node);
}

