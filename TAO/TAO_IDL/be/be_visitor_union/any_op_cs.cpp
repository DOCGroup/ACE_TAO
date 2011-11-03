
//=============================================================================
/**
 *  @file    any_op_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Any operators for Union.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


// ***************************************************************************
// Union visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_union_any_op_cs::be_visitor_union_any_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_union (ctx)
{
}

be_visitor_union_any_op_cs::~be_visitor_union_any_op_cs (void)
{
}

int
be_visitor_union_any_op_cs::visit_union (be_union *node)
{
  if (node->cli_stub_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2
      << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl_2;

  *os << be_global->core_versioning_begin () << be_nl;

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
          << "Any_Dual_Impl_T<" << node->name ()
          << ">::marshal_value (TAO_OutputCDR &)" << be_nl
          << "{" << be_idt_nl
          << "return false;" << be_uidt_nl
          << "}";

      *os << be_nl_2
          << "template<>" << be_nl
          << "::CORBA::Boolean" << be_nl
          << "Any_Dual_Impl_T<" << node->name ()
          << ">::demarshal_value (TAO_InputCDR &)" << be_nl
          << "{" << be_idt_nl
          << "return false;" << be_uidt_nl
          << "}" << be_uidt_nl
          << "}";
    }

  *os << be_global->core_versioning_end () << be_nl;

  be_module *module = 0;

  AST_Decl *decl = node;
  if (decl->is_nested ())
    {
      if (node->defined_in ()->scope_node_type () == AST_Decl::NT_interface)
        {
          be_interface *intf = 0;
          intf = be_interface::narrow_from_scope (node->defined_in ());
          decl = intf;
        }

      if (decl->defined_in ()->scope_node_type () == AST_Decl::NT_module)
        {
          module = be_module::narrow_from_scope (decl->defined_in ());

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

          // Copying insertion.
          *os << "// Copying insertion." << be_nl
              << "void operator<<= (" << be_idt << be_idt_nl
              << "::CORBA::Any &_tao_any," << be_nl
              << "const ::" << node->name () << " &_tao_elem" << be_uidt_nl
              << ")" << be_uidt_nl
              << "{" << be_idt_nl

              << "if (0 == &_tao_elem) // Trying to de-reference NULL object" << be_idt_nl
              << "_tao_any <<= static_cast< ::" << node->name () << " *>( 0 ); // Use non-copying insertion of a NULL" << be_uidt_nl
              << "else" << be_idt_nl

              << "TAO::Any_Dual_Impl_T< ::" << node->name () << ">::insert_copy ("
              << be_idt << be_idt_nl
              << "_tao_any," << be_nl
              << "::" << node->name () << "::_tao_any_destructor," << be_nl
              << "::" << node->tc_name () << "," << be_nl
              << "_tao_elem" << be_uidt_nl
              << ");" << be_uidt << be_uidt << be_uidt_nl
              << "}" << be_nl_2;

          // Non-copying insertion.
          *os << "// Non-copying insertion." << be_nl
              << "void operator<<= (" << be_idt << be_idt_nl
              << "::CORBA::Any &_tao_any," << be_nl
              << "::" << node->name () << " *_tao_elem" << be_uidt_nl
              << ")" << be_uidt_nl
              << "{" << be_idt_nl
              << "TAO::Any_Dual_Impl_T< ::" << node->name () << ">::insert ("
              << be_idt << be_idt_nl
              << "_tao_any," << be_nl
              << "::" << node->name () << "::_tao_any_destructor," << be_nl
              << "::" << node->tc_name () << "," << be_nl
              << "_tao_elem" << be_uidt_nl
              << ");" << be_uidt << be_uidt_nl
              << "}" << be_nl_2;

          // Extraction to non-const pointer (deprecated, just calls the other).
          *os << "// Extraction to non-const pointer (deprecated)." << be_nl
              << "::CORBA::Boolean operator>>= (" << be_idt << be_idt_nl
              << "const ::CORBA::Any &_tao_any," << be_nl
              << "::" << node->name () << " *&_tao_elem" << be_uidt_nl
              << ")" << be_uidt_nl
              << "{" << be_idt_nl
              << "return _tao_any >>= const_cast<" << be_idt << be_idt_nl
              << "const ::" << node->name () << " *&> (" << be_nl
              << "_tao_elem" << be_uidt_nl
              << ");" << be_uidt << be_uidt_nl
              << "}" << be_nl_2;

          // Extraction to const pointer.
          *os << "// Extraction to const pointer." << be_nl
              << "::CORBA::Boolean operator>>= (" << be_idt << be_idt_nl
              << "const ::CORBA::Any &_tao_any," << be_nl
              << "const ::" << node->name () << " *&_tao_elem" << be_uidt_nl
              << ")" << be_uidt_nl
              << "{" << be_idt_nl
              << "return" << be_idt_nl
              << "TAO::Any_Dual_Impl_T< ::" << node->name () << ">::extract ("
              << be_idt << be_idt_nl
              << "_tao_any," << be_nl
              << "::" << node->name () << "::_tao_any_destructor," << be_nl
              << "::" << node->tc_name () << "," << be_nl
              << "_tao_elem" << be_uidt_nl
              << ");" << be_uidt << be_uidt << be_uidt_nl
              << "}";

          be_util::gen_nested_namespace_end (os, module);

          // Emit #else.
          *os << be_nl_2
              << "#else\n";
        }
    }

  *os << be_global->core_versioning_begin () << be_nl;

  // Copying insertion.
  *os << "// Copying insertion." << be_nl
      << "void operator<<= (" << be_idt << be_idt_nl
      << "::CORBA::Any &_tao_any," << be_nl
      << "const " << node->name () << " &_tao_elem" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl

      << "if (0 == &_tao_elem) // Trying to de-reference NULL object" << be_idt_nl
      << "_tao_any <<= static_cast<" << node->name () << " *>( 0 ); // Use non-copying insertion of a NULL" << be_uidt_nl
      << "else" << be_idt_nl

      << "TAO::Any_Dual_Impl_T<" << node->name () << ">::insert_copy ("
      << be_idt << be_idt_nl
      << "_tao_any," << be_nl
      << node->name () << "::_tao_any_destructor," << be_nl
      << node->tc_name () << "," << be_nl
      << "_tao_elem" << be_uidt_nl
      << ");" << be_uidt << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  // Non-copying insertion.
  *os << "// Non-copying insertion." << be_nl
      << "void operator<<= (" << be_idt << be_idt_nl
      << "::CORBA::Any &_tao_any," << be_nl
      << node->name () << " *_tao_elem" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "TAO::Any_Dual_Impl_T<" << node->name () << ">::insert ("
      << be_idt << be_idt_nl
      << "_tao_any," << be_nl
      << node->name () << "::_tao_any_destructor," << be_nl
      << node->tc_name () << "," << be_nl
      << "_tao_elem" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  // Extraction to non-const pointer (deprecated, just calls the other).
  *os << "// Extraction to non-const pointer (deprecated)." << be_nl
      << "::CORBA::Boolean operator>>= (" << be_idt << be_idt_nl
      << "const ::CORBA::Any &_tao_any," << be_nl
      << node->name () << " *&_tao_elem" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return _tao_any >>= const_cast<" << be_idt << be_idt_nl
      << "const " << node->name () << " *&> (" << be_nl
      << "_tao_elem" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  // Extraction to const pointer.
  *os << "// Extraction to const pointer." << be_nl
      << "::CORBA::Boolean operator>>= (" << be_idt << be_idt_nl
      << "const ::CORBA::Any &_tao_any," << be_nl
      << "const " << node->name () << " *&_tao_elem" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return" << be_idt_nl
      << "TAO::Any_Dual_Impl_T<" << node->name () << ">::extract ("
      << be_idt << be_idt_nl
      << "_tao_any," << be_nl
      << node->name () << "::_tao_any_destructor," << be_nl
      << node->tc_name () << "," << be_nl
      << "_tao_elem" << be_uidt_nl
      << ");" << be_uidt << be_uidt << be_uidt_nl
      << "}";

  *os << be_global->core_versioning_end () << be_nl;

  if (module != 0)
    {
      *os << "\n\n#endif";
    }

  be_visitor_context ctx (*this->ctx_);
  for (UTL_ScopeActiveIterator si (node, UTL_Scope::IK_localtypes);
       !si.is_done ();
       si.next ())
    {
      AST_Decl *d = si.item ();

      be_enum *e = be_enum::narrow_from_decl (d);
      if (e != 0)
        {
          be_visitor_enum_any_op_cs visitor (&ctx);

          if (e->accept (&visitor) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          "(%N:%l) be_visitor_union_any_op_cs::visit_union"
                          " - codegen for enum failed\n"));
            }

          // Restore the union node in the enum visitor's context.
          ctx.node (this->ctx_->node ());
        }
    }

  // All we have to do is to visit the scope and generate code.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union::visit_union - "
                         "codegen for scope failed\n"),
                        -1);
    }

  node->cli_stub_any_op_gen (1);
  return 0;
}

int
be_visitor_union_any_op_cs::visit_union_branch (be_union_branch *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_any_op_cs::"
                         "visit_union_branch - "
                         "Bad field type\n"),
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_any_op_cs::"
                         "visit_union_branch - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_any_op_cs::visit_enum (be_enum *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  be_visitor_enum_any_op_cs visitor (this->ctx_);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_any_op_cs::"
                         "visit_enum - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_union_any_op_cs::visit_structure (be_structure *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  be_visitor_structure_any_op_cs visitor (this->ctx_);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_any_op_cs::"
                         "visit_structure - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}
