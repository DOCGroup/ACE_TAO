//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    any_op_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Any operators for structures
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_structure.h"

ACE_RCSID(be_visitor_structure, any_op_cs, "$Id$")


// ***************************************************************************
// Structure visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_structure_any_op_cs::be_visitor_structure_any_op_cs
(be_visitor_context *ctx)
  : be_visitor_structure (ctx)
{
}

be_visitor_structure_any_op_cs::~be_visitor_structure_any_op_cs (void)
{
}

int
be_visitor_structure_any_op_cs::visit_structure (be_structure *node)
{
  if (node->cli_stub_any_op_gen () ||
      node->imported () ||
      node->is_local ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  // generate the Any <<= and >>= operator declarations
  // Any <<= and >>= operators
  os->indent ();
  *os << "void operator<<= (CORBA::Any &_tao_any, const "
      << node->name () << " &_tao_elem) // copying" << be_nl
      << "{" << be_idt_nl
      << "TAO_OutputCDR stream;" << be_nl
      << "stream << _tao_elem;" << be_nl
      << "_tao_any._tao_replace (" << be_idt << be_idt_nl
      << node->tc_name () << "," << be_nl
      << "TAO_ENCAP_BYTE_ORDER," << be_nl
      << "stream.begin ()" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}\n" << be_nl;

  *os << "void operator<<= (CORBA::Any &_tao_any, "
      << node->name () << " *_tao_elem) // non copying" << be_nl
      << "{" << be_idt_nl
      << "TAO_OutputCDR stream;" << be_nl
      << "stream << *_tao_elem;" << be_nl
      << "_tao_any._tao_replace (" << be_idt << be_idt_nl
      << node->tc_name () << "," << be_nl
      << "TAO_ENCAP_BYTE_ORDER," << be_nl
      << "stream.begin ()," << be_nl
      << "1," << be_nl
      << "_tao_elem," << be_nl
      << node->name () << "::_tao_any_destructor" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}\n" << be_nl;

  *os << "CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, "
      << node->name () << " *&_tao_elem)" << be_nl
      << "{" << be_idt_nl
      << "return _tao_any >>= ACE_const_cast("
      << "const " << node->name () << "*&,_tao_elem);" << be_uidt_nl
      << "}\n\n";

  *os << "CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, const "
      << node->name () << " *&_tao_elem)" << be_nl
      << "{" << be_idt_nl
      << "_tao_elem = 0;" << be_nl
      << "ACE_TRY_NEW_ENV" << be_nl
      << "{" << be_idt_nl
      << "CORBA::TypeCode_var type = _tao_any.type ();" << be_nl << be_nl
      << "CORBA::Boolean result = type->equivalent (" << node->tc_name ()
      << " ACE_ENV_ARG_PARAMETER);" << be_nl
      << "ACE_TRY_CHECK;" << be_nl << be_nl
      << "if (!result)" << be_idt_nl
      << "return 0; // not equivalent" << be_uidt_nl << be_nl
      << "if (_tao_any.any_owns_data ())" << be_nl
      << "{" << be_idt_nl
      << "_tao_elem = ACE_static_cast(" << be_idt << be_idt_nl
      << "const " << node->name () << "*," << be_nl
      << "_tao_any.value ()" << be_uidt_nl
      << ");" << be_uidt_nl
      << "return 1;" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_nl  // else any does not own the data
      << "{" << be_idt_nl
      << node->name () << " *tmp;" << be_nl
      << "ACE_NEW_RETURN (tmp, " << node->name () << ", 0);" << be_nl
      << "TAO_InputCDR stream (" << be_idt << be_idt_nl
      << "_tao_any._tao_get_cdr ()," << be_nl
      << "_tao_any._tao_byte_order ()" << be_uidt_nl
      << ");" << be_uidt_nl
      << "if (stream >> *tmp)" << be_nl
      << "{" << be_idt_nl
      << "((CORBA::Any *)&_tao_any)->_tao_replace (" << be_idt << be_idt_nl
      << node->tc_name () << "," << be_nl
      << "1," << be_nl
      << "ACE_static_cast (void *, tmp)," << be_nl
      << node->name () << "::_tao_any_destructor" << be_uidt_nl
      << ");" << be_uidt_nl
      << "_tao_elem = tmp;" << be_nl
      << "return 1;" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_nl
      << "{" << be_idt_nl
      << "delete tmp;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "ACE_CATCHANY" << be_nl
      << "{" << be_nl
      << "}" << be_nl
      << "ACE_ENDTRY;" << be_nl
      << "return 0;" << be_uidt_nl
      << "}\n\n";

  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure::visit_structure - "
                         "codegen for scope failed\n"), -1);
    }

  node->cli_stub_any_op_gen (1);
  return 0;
}

int
be_visitor_structure_any_op_cs::visit_field (be_field *node)
{
  be_type *bt; // field's type

  // first generate the type information
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_any_op_cs::"
                         "visit_field - "
                         "Bad field type\n"
                         ), -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_structure_any_op_cs::"
                         "visit_field - "
                         "codegen for field type failed\n"
                         ), -1);
    }
  return 0;
}
