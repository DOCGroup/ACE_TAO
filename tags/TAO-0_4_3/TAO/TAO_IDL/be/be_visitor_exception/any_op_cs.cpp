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
//    Visitor generating code for the Any operators for Exceptions.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_exception.h"

ACE_RCSID(be_visitor_exception, any_op_cs, "$Id$")


// ***************************************************************************
// Exception visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_exception_any_op_cs::be_visitor_exception_any_op_cs
(be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_exception_any_op_cs::~be_visitor_exception_any_op_cs (void)
{
}

int
be_visitor_exception_any_op_cs::visit_exception (be_exception *node)
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
      << node->name () << " *_tao_any_val = new " << node->name ()
      << " (_tao_elem);" << be_nl
      << "if (!_tao_any_val) return;" << be_nl
      << "ACE_TRY_NEW_ENV" << be_nl
      << "{" << be_idt_nl
      << "_tao_any.replace (" << node->tc_name () << ", _tao_any_val, "
      << "1, ACE_TRY_ENV);" << be_nl
      << "ACE_TRY_CHECK;" << be_uidt_nl
      << "}" << be_nl
      << "ACE_CATCHANY " << be_nl
      << "{" << be_idt_nl
      << "delete _tao_any_val;" << be_uidt_nl
      << "}" << be_nl
      << "ACE_ENDTRY;" << be_uidt_nl
      << "}\n" << be_nl;

  *os << "void operator<<= (CORBA::Any &_tao_any, "
      << node->name () << " *_tao_elem) // non copying" << be_nl
      << "{" << be_idt_nl
      << "ACE_TRY_NEW_ENV" << be_nl
      << "{" << be_idt_nl
      << "_tao_any.replace (" << node->tc_name () << ", "
      << "_tao_elem, 1, ACE_TRY_ENV); // consume it" << be_nl
      << "ACE_TRY_CHECK;" << be_uidt_nl
      << "}" << be_nl
      << "ACE_CATCHANY {}" << be_nl
      << "ACE_ENDTRY;" << be_uidt_nl
      << "}\n" << be_nl;

  *os << "CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, "
      << node->name () << " *&_tao_elem)" << be_nl
      << "{" << be_idt_nl
      << "ACE_TRY_NEW_ENV" << be_nl
      << "{" << be_idt_nl
      << "CORBA::TypeCode_var type = _tao_any.type ();" << be_nl
      << "if (!type->equal (" << node->tc_name ()
      << ", ACE_TRY_ENV)) return 0; // not equal" << be_nl
      << "ACE_TRY_CHECK;" << be_nl
      << "if (_tao_any.any_owns_data ())" << be_nl
      << "{" << be_idt_nl
      << "_tao_elem = (" << node->name () << " *)_tao_any.value ();"
      << be_nl
      << "return 1;" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_nl  // else any does not own the data
      << "{" << be_idt_nl
      << "ACE_NEW_RETURN (_tao_elem, " << node->name () << ", 0);"
      << be_nl
      << "TAO_InputCDR stream (_tao_any._tao_get_cdr ());"
      << be_nl
      << "if (stream.decode (" << node->tc_name ()
      << ", _tao_elem, 0, ACE_TRY_ENV)" << be_nl
      << "  == CORBA::TypeCode::TRAVERSE_CONTINUE)" << be_nl
      << "{" << be_idt_nl
      << "((CORBA::Any *)&_tao_any)->replace ("
      << node->tc_name () << ", _tao_elem, 1, ACE_TRY_ENV);" << be_nl
      << "ACE_TRY_CHECK;" << be_nl
      << "return 1;" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_nl  // decode failed
      << "{" << be_idt_nl
      << "delete _tao_elem;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "ACE_CATCHANY" << be_nl
      << "{" << be_idt_nl
      << "delete _tao_elem;" << be_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_nl
      << "ACE_ENDTRY;" << be_nl
      << "return 0;" << be_uidt_nl
      << "}\n\n";


  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception::visit_exception - "
                         "codegen for scope failed\n"), -1);
    }

  node->cli_stub_any_op_gen (1);
  return 0;
}

int
be_visitor_exception_any_op_cs::visit_field (be_field *node)
{
  be_type *bt; // field's type

  // first generate the type information
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception_any_op_cs::"
                         "visit_field - "
                         "Bad field type\n"
                         ), -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_exception_any_op_cs::"
                         "visit_field - "
                         "codegen for field type failed\n"
                         ), -1);
    }
  return 0;
}
