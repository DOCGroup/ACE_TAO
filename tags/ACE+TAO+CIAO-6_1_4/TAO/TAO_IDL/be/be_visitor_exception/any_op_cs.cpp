
//=============================================================================
/**
 *  @file    any_op_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for the Any operators for Exceptions.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "exception.h"

be_visitor_exception_any_op_cs::be_visitor_exception_any_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_scope (ctx)
{
}

be_visitor_exception_any_op_cs::~be_visitor_exception_any_op_cs (void)
{
}

int
be_visitor_exception_any_op_cs::visit_exception (be_exception *node)
{
  if (node->cli_stub_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2
      << "// TAO_IDL - Generated from" << be_nl
      << "// " << __FILE__ << ":" << __LINE__;

  *os << be_global->core_versioning_begin () << be_nl;

  if (!node->is_local ())
    {
      *os << be_nl_2
          << "namespace TAO" << be_nl
          << "{" << be_idt_nl
          << "template<>" << be_nl
          << "::CORBA::Boolean" << be_nl
          << "Any_Dual_Impl_T<" << node->name ()
          << ">::demarshal_value ("
          << "TAO_InputCDR & cdr"
          << ")" << be_nl
          << "{" << be_idt_nl
          << "::CORBA::String_var id;" << be_nl_2
          << "if (!(cdr >> id.out ()))" << be_idt_nl
          << "{" << be_idt_nl
          << "return false;" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "try"
          << be_idt_nl
          << "{" << be_idt_nl
          << "this->value_->_tao_decode (cdr);" << be_uidt_nl
          << "}" << be_uidt_nl
          << "catch (const ::CORBA::Exception &)"
          << be_idt_nl
          << "{" << be_idt_nl
          << "return false;" << be_uidt_nl
          << "}" << be_uidt
          << be_nl_2
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
  else
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

          // Copying insertion operator.

          *os << be_nl_2
              << "// Copying insertion." << be_nl
              << "void operator<<= (" << be_idt << be_idt_nl
              << "::CORBA::Any &_tao_any," << be_nl
              << "const ::" << node->name () << " &_tao_elem)" << be_uidt
              << be_uidt_nl
              << "{" << be_idt_nl
              << "TAO::Any_Dual_Impl_T< ::" << node->name () << ">::insert_copy ("
              << be_idt << be_idt_nl
              << "_tao_any," << be_nl
              << "::" << node->name () << "::_tao_any_destructor," << be_nl
              << "::" << node->tc_name () << "," << be_nl
              << "_tao_elem);" << be_uidt
              << be_uidt << be_uidt_nl
              << "}" << be_nl_2;

          // Non-copying insertion operator."
          *os << "// Non-copying insertion." << be_nl
              << "void operator<<= (" << be_idt << be_idt_nl
              << "::CORBA::Any &_tao_any," << be_nl
              << "::" << node->name () << " *_tao_elem)" << be_uidt
              << be_uidt_nl
              << "{" << be_idt_nl
              << "TAO::Any_Dual_Impl_T< ::" << node->name () << ">::insert ("
              << be_idt << be_idt_nl
              << "_tao_any," << be_nl
              << "::" << node->name () << "::_tao_any_destructor," << be_nl
              << "::" << node->tc_name () << "," << be_nl
              << "_tao_elem);" << be_uidt
              << be_uidt << be_uidt_nl
              << "}" << be_nl_2;

          // Extraction to non-const pointer operator.
          *os << "// Extraction to non-const pointer (deprecated)." << be_nl
              << "::CORBA::Boolean operator>>= (" << be_idt << be_idt_nl
              << "const ::CORBA::Any &_tao_any," << be_nl
              << "::" << node->name () << " *&_tao_elem)" << be_uidt
              << be_uidt_nl
              << "{" << be_idt_nl
              << "return _tao_any >>= const_cast<" << be_idt << be_idt_nl
              << "const ::" << node->name () << " *&> (" << be_nl
              << "_tao_elem);" << be_uidt
              << be_uidt << be_uidt_nl
              << "}" << be_nl_2;

          // Extraction to const pointer operator.
          *os << "// Extraction to const pointer." << be_nl
              << "::CORBA::Boolean operator>>= (" << be_idt << be_idt_nl
              << "const ::CORBA::Any &_tao_any," << be_nl
              << "const ::" << node->name () << " *&_tao_elem)" << be_uidt
              << be_uidt_nl
              << "{" << be_idt_nl
              << "return" << be_idt_nl
              << "TAO::Any_Dual_Impl_T< ::" << node->name () << ">::extract ("
              << be_idt << be_idt_nl
              << "_tao_any," << be_nl
              << "::" << node->name () << "::_tao_any_destructor," << be_nl
              << "::" << node->tc_name () << "," << be_nl
              << "_tao_elem);" << be_uidt
              << be_uidt << be_uidt << be_uidt_nl
              << "}";

          be_util::gen_nested_namespace_end (os, module);

          // Emit #else.
          *os << be_nl_2
              << "#else\n";
        }
    }


  *os << be_global->core_versioning_begin () << be_nl;

  // Copying insertion operator.

  *os << be_nl_2
      << "// Copying insertion." << be_nl
      << "void operator<<= (" << be_idt << be_idt_nl
      << "::CORBA::Any &_tao_any," << be_nl
      << "const " << node->name () << " &_tao_elem)" << be_uidt
      << be_uidt_nl
      << "{" << be_idt_nl
      << "TAO::Any_Dual_Impl_T<" << node->name () << ">::insert_copy ("
      << be_idt << be_idt_nl
      << "_tao_any," << be_nl
      << node->name () << "::_tao_any_destructor," << be_nl
      << node->tc_name () << "," << be_nl
      << "_tao_elem" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  // Non-copying insertion operator."
  *os << "// Non-copying insertion." << be_nl
      << "void operator<<= (" << be_idt << be_idt_nl
      << "::CORBA::Any &_tao_any," << be_nl
      << node->name () << " *_tao_elem)" << be_uidt
      << be_uidt_nl
      << "{" << be_idt_nl
      << "TAO::Any_Dual_Impl_T<" << node->name () << ">::insert ("
      << be_idt << be_idt_nl
      << "_tao_any," << be_nl
      << node->name () << "::_tao_any_destructor," << be_nl
      << node->tc_name () << "," << be_nl
      << "_tao_elem" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  // Extraction to non-const pointer operator.
  *os << "// Extraction to non-const pointer (deprecated)." << be_nl
      << "::CORBA::Boolean operator>>= (" << be_idt << be_idt_nl
      << "const ::CORBA::Any &_tao_any," << be_nl
      << node->name () << " *&_tao_elem)" << be_uidt
      << be_uidt_nl
      << "{" << be_idt_nl
      << "return _tao_any >>= const_cast<" << be_idt << be_idt_nl
      << "const " << node->name () << " *&> (" << be_nl
      << "_tao_elem);" << be_uidt
      << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  // Extraction to const pointer operator.
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
      << "_tao_elem);" << be_uidt
      << be_uidt << be_uidt << be_uidt_nl
      << "}";

  *os << be_global->core_versioning_end () << be_nl;

  if (module != 0)
    {
      *os << "\n\n#endif";
    }

  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception::visit_exception - "
                         "codegen for scope failed\n"),
                        -1);
    }

  node->cli_stub_any_op_gen (1);
  return 0;
}

int
be_visitor_exception_any_op_cs::visit_field (be_field *node)
{
  // First generate the type information.
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception_any_op_cs::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception_any_op_cs::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_exception_any_op_cs::visit_enum (be_enum *node)
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
                         "(%N:%l) be_visitor_exception_any_op_cs::"
                         "visit_enum - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_exception_any_op_cs::visit_structure (be_structure *node)
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
                         "(%N:%l) be_visitor_exception_any_op_cs::"
                         "visit_structure - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_exception_any_op_cs::visit_union (be_union *node)
{
  if (node->cli_hdr_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  be_visitor_union_any_op_cs visitor (this->ctx_);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception_any_op_cs::"
                         "visit_union - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}
