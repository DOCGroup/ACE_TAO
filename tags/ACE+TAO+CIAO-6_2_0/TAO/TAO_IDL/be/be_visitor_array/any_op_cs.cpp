
//=============================================================================
/**
 *  @file    any_op_cs.cpp
 *
 *  $Id$
 *
 *  Visitor for code generation of Arrays for the Any operators in the client
 *  stubs.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "array.h"

be_visitor_array_any_op_cs::be_visitor_array_any_op_cs (
    be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_array_any_op_cs::~be_visitor_array_any_op_cs (void)
{
}

int
be_visitor_array_any_op_cs::visit_array (be_array *node)
{
  if (node->cli_stub_any_op_gen () || node->imported ())
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
          << "Any_Array_Impl_T<" << be_idt << be_idt_nl
          << node->name () << "_slice," << be_nl
          << node->name () << "_forany" << be_uidt_nl
          << ">::marshal_value (TAO_OutputCDR &)" << be_uidt_nl
          << "{" << be_idt_nl
          << "return false;" << be_uidt_nl
          << "}";

      *os << be_nl_2
          << "template<>" << be_nl
          << "::CORBA::Boolean" << be_nl
          << "Any_Array_Impl_T<" << be_idt << be_idt_nl
          << node->name () << "_slice," << be_nl
          << node->name () << "_forany" << be_uidt_nl
          << ">::demarshal_value (TAO_OutputCDR &)" << be_uidt_nl
          << "{" << be_idt_nl
          << "return false;" << be_uidt_nl
          << "}" << be_uidt_nl
          << "}";
    }

  // If this is non-zero, we want to call its tc_name()
  // for the TypeCode to pass to the Any operator impls.
  be_typedef *td = this->ctx_->tdef ();

  *os << "void operator<<= (" << be_idt << be_idt_nl
      << "::CORBA::Any &_tao_any," << be_nl
      << "const " << node->name () << "_forany &_tao_elem" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "TAO::Any_Array_Impl_T<" << be_idt << be_idt_nl
      << node->name () << "_slice," << be_nl
      << node->name () << "_forany" << be_uidt_nl
      << ">::insert ("
      << be_idt << be_idt_nl
      << "_tao_any," << be_nl
      << node->name () << "_forany::_tao_any_destructor," << be_nl
      << (td != 0 ? td->tc_name () : node->tc_name ()) << "," << be_nl
      << "_tao_elem.nocopy ()" << be_idt_nl
      << "? _tao_elem.ptr ()" << be_nl
      << ": "
      << node->name () << "_dup (_tao_elem.in ())" << be_uidt << be_uidt_nl
      << ");" << be_uidt << be_uidt << be_uidt_nl
      << "}" << be_nl_2;

  *os << "::CORBA::Boolean operator>>= (" << be_idt << be_idt_nl
      << "const ::CORBA::Any &_tao_any," << be_nl
      << node->name () << "_forany &_tao_elem" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "return" << be_idt_nl
      << "TAO::Any_Array_Impl_T<" << be_idt << be_idt_nl
      << node->name () << "_slice," << be_nl
      << node->name () << "_forany" << be_uidt_nl
      << ">::extract (" << be_idt << be_idt_nl
      << "_tao_any," << be_nl
      << node->name () << "_forany::_tao_any_destructor," << be_nl
      << (td != 0 ? td->tc_name () : node->tc_name ()) << "," << be_nl
      << "_tao_elem.out ()" << be_uidt_nl
      << ");" << be_uidt << be_uidt << be_uidt << be_uidt_nl
      << "}";

  *os << be_global->core_versioning_end () << be_nl;

  node->cli_stub_any_op_gen (1);
  return 0;
}
