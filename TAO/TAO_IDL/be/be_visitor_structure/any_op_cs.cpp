
//=============================================================================
/**
 *  @file    any_op_cs.cpp
 *
 *  Visitor generating code for Any operators for structures
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "structure.h"

// ***************************************************************************
// Structure visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_structure_any_op_cs::be_visitor_structure_any_op_cs (
                                                                be_visitor_context *ctx
                                                                )
  : be_visitor_structure (ctx)
{
}

int
be_visitor_structure_any_op_cs::visit_structure (be_structure *node)
{
  if (node->cli_stub_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  TAO_INSERT_COMMENT (os);

  // Since we don't generate CDR stream operators for types that
  // explicitly contain a local interface (at some level), we
  // must override these Any template class methods to avoid
  // calling the non-existent operators. The zero return value
  // will eventually cause CORBA::MARSHAL to be raised if this
  // type is inserted into an Any and then marshaled.
  if (node->is_local ())
    {
      *os << be_global->core_versioning_begin () << be_nl;

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

      *os << be_global->core_versioning_end () << be_nl;
    }

  *os << be_global->anyops_versioning_begin () << be_nl;

  // Copying insertion.
  *os << "/// Copying insertion." << be_nl
      << "void operator<<= (" << be_idt_nl
      << "::CORBA::Any &_tao_any," << be_nl
      << "const ::" << node->name () << " &_tao_elem)"
      << be_uidt_nl
      << "{" << be_idt_nl
      << "TAO::Any_Dual_Impl_T< ::" << node->name () << ">::insert_copy ("
      << be_idt_nl
      << "_tao_any," << be_nl
      << "::" << node->name () << "::_tao_any_destructor," << be_nl
      << node->tc_name () << "," << be_nl
      << "_tao_elem);"
      << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  // Non-copying insertion.
  *os << "/// Non-copying insertion." << be_nl
      << "void operator<<= (" << be_idt_nl
      << "::CORBA::Any &_tao_any," << be_nl
      << "::" << node->name () << " *_tao_elem)"
      << be_uidt_nl
      << "{" << be_idt_nl
      << "TAO::Any_Dual_Impl_T< ::" << node->name () << ">::insert ("
      << be_idt_nl
      << "_tao_any," << be_nl
      << "::" << node->name () << "::_tao_any_destructor," << be_nl
      << node->tc_name () << "," << be_nl
      << "_tao_elem);"
      << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  // Extraction to const pointer.
  *os << "/// Extraction to const pointer." << be_nl
      << "::CORBA::Boolean operator>>= (" << be_idt_nl
      << "const ::CORBA::Any &_tao_any," << be_nl
      << "const ::" << node->name () << " *&_tao_elem)"
      << be_uidt_nl
      << "{" << be_idt_nl
      << "return TAO::Any_Dual_Impl_T< ::" << node->name () << ">::extract ("
      << be_idt_nl
      << "_tao_any," << be_nl
      << "::" << node->name () << "::_tao_any_destructor," << be_nl
      << node->tc_name () << "," << be_nl
      << "_tao_elem);"
      << be_uidt << be_uidt_nl
      << "}";

  *os << be_global->anyops_versioning_end () << be_nl;

  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure::visit_structure - "
                         "codegen for scope failed\n"),
                        -1);
    }

  node->cli_stub_any_op_gen (true);
  return 0;
}

int
be_visitor_structure_any_op_cs::visit_field (be_field *node)
{
  // First generate the type information.
  be_type *bt = dynamic_cast<be_type*> (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_any_op_cs::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_any_op_cs::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_structure_any_op_cs::visit_union (be_union *node)
{
  if (node->cli_stub_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  be_visitor_union_any_op_cs visitor (this->ctx_);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_any_op_cs::"
                         "visit_union - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_structure_any_op_cs::visit_enum (be_enum *node)
{
  if (node->cli_stub_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  be_visitor_enum_any_op_cs visitor (this->ctx_);

  if (node->accept (&visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_any_op_cs::"
                         "visit_enum - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}
