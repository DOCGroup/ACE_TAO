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
  if (node->cli_stub_any_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  // generate the Any <<= and >>= operator declarations
  // Any <<= and >>= operators
  os->indent ();
  *os << "void operator<<= (CORBA::Any &_tao_any, const "
      << node->name () << " &_tao_elem) // copying" << be_nl
      << "{" << be_idt_nl
      << node->name () << " *_any_val;" << be_nl
      << "ACE_NEW (_any_val, " << node->name () << " (_tao_elem));" << be_nl
      << "if (!_any_val) return;" << be_nl
      << "TAO_TRY" << be_nl
      << "{" << be_idt_nl
      << "_tao_any.replace (" << node->tc_name ()
      << ", _any_val, 1, TAO_TRY_ENV);" << " // copy the value" << be_nl
      << "TAO_CHECK_ENV;" << be_uidt_nl
      << "}" << be_nl
      << "TAO_CATCHANY" << be_nl
      << "{" << be_idt_nl
      << "delete _any_val;" << be_uidt_nl
      << "}" << be_nl
      << "TAO_ENDTRY;" << be_uidt_nl
      << "}\n" << be_nl;

  *os << "void operator<<= (CORBA::Any &_tao_any, "
      << node->name () << " *_tao_elem) // non copying" << be_nl
      << "{" << be_idt_nl
      << "TAO_TRY" << be_nl
      << "{" << be_idt_nl
      << "_tao_any.replace (" << node->tc_name () << ", "
      << "_tao_elem, 1, TAO_TRY_ENV); // consume it" << be_nl
      << "TAO_CHECK_ENV;" << be_uidt_nl
      << "}" << be_nl
      << "TAO_CATCHANY {}" << be_nl
      << "TAO_ENDTRY;" << be_uidt_nl
      << "}\n" << be_nl;

  *os << "CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, "
      << node->name () << " *&_tao_elem)" << be_nl
      << "{" << be_idt_nl
      << "TAO_TRY" << be_nl
      << "{" << be_idt_nl
      << "CORBA::TypeCode_var type = _tao_any.type ();" << be_nl
      << "if (!type->equal (" << node->tc_name ()
      << ", TAO_TRY_ENV)) return 0; // not equal" << be_nl
      << "TAO_CHECK_ENV;" << be_nl
      << "if (_tao_any.any_owns_data ())" << be_nl
      << "{" << be_idt_nl
      << "_tao_elem = (" << node->name () << " *)_tao_any.value ();"
      << be_nl
      << "return 1;" << be_nl
      << "}" << be_uidt_nl
      << "else" << be_nl  // else any does not own the data
      << "{" << be_idt_nl
      << "ACE_NEW_RETURN (_tao_elem, " << node->name () << ", 0);"
      << be_nl
      << "TAO_InputCDR stream ((ACE_Message_Block *)_tao_any._tao_get_cdr ());"
      << be_nl
      << "if (stream.decode (" << node->tc_name ()
      << ", _tao_elem, 0, TAO_TRY_ENV)" << be_nl
      << "  == CORBA::TypeCode::TRAVERSE_CONTINUE)" << be_nl
      << "{" << be_idt_nl
      << "((CORBA::Any *)&_tao_any)->replace ("
      << node->tc_name () << ", _tao_elem, 1, TAO_TRY_ENV);" << be_nl
      << "TAO_CHECK_ENV;" << be_nl
      << "return 1;" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_nl
      << "{" << be_idt_nl
      << "delete _tao_elem;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "TAO_CATCHANY" << be_nl
      << "{" << be_idt_nl
      << "delete _tao_elem;" << be_nl
      << "return 0; " << be_uidt_nl
      << "}" << be_nl
      << "TAO_ENDTRY;" << be_nl
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
