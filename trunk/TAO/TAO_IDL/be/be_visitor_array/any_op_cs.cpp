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
//    Visitor for code generation of Arrays for the Any operators in the client
//    stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_array.h"

ACE_RCSID(be_visitor_array, any_op_cs, "$Id$")


// ***************************************************************************
// Array visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_array_any_op_cs::be_visitor_array_any_op_cs
(be_visitor_context *ctx)
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
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  // generate the Any <<= and >>= operator declarations
  // Any <<= and >>= operators
  os->indent ();
  *os << "void operator<<= (CORBA::Any &_tao_any, const "
      << node->name () << "_forany &_tao_elem)" << be_nl
      << "{" << be_idt_nl
      << "ACE_TRY_NEW_ENV" << be_nl
      << "{" << be_idt_nl
      << "if (_tao_elem.nocopy ()) // no copy" << be_idt_nl
      << "_tao_any.replace (" << node->tc_name () << ", "
      << "_tao_elem.in (), 1, ACE_TRY_ENV); // consume it" << be_uidt_nl
      << "else // copy" << be_idt_nl
      << "_tao_any.replace (" << node->tc_name () << ", " << node->name ()
      << "_dup (_tao_elem.in ()), 1, ACE_TRY_ENV);" << be_uidt_nl
      << "ACE_TRY_CHECK;" << be_uidt_nl
      << "}" << be_nl
      << "ACE_CATCHANY {}" << be_nl
      << "ACE_ENDTRY;" << be_uidt_nl
      << "}\n\n";

  os->indent ();
  *os << "CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, "
      << node->name () << "_forany &_tao_elem)" << be_nl
      << "{" << be_idt_nl
      << "ACE_TRY_NEW_ENV" << be_nl
      << "{" << be_idt_nl
      << "CORBA::TypeCode_var type = _tao_any.type ();" << be_nl
      << "if (!type->equal (" << node->tc_name ()
      << ", ACE_TRY_ENV)) return 0; // not equal" << be_nl
      << "ACE_TRY_CHECK;" << be_nl
      << "if (_tao_any.any_owns_data ())" << be_nl
      << "{" << be_idt_nl
      << "_tao_elem = (" << node->name () << "_slice *)_tao_any.value ();"
      << be_nl
      << "return 1;" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_nl  // else Any does not own the data
      << "{" << be_idt_nl
      << "_tao_elem.out () = " << node->name () << "_alloc ();" << be_nl
      << "if (!_tao_elem.in ()) return 0;" << be_nl
      << "TAO_InputCDR stream ((ACE_Message_Block *)_tao_any._tao_get_cdr ());"
      << be_nl
      << "if (stream.decode (" << node->tc_name ()
      << ", _tao_elem.inout (), 0, ACE_TRY_ENV)" << be_nl
      << "  == CORBA::TypeCode::TRAVERSE_CONTINUE)" << be_nl
      << "{" << be_idt_nl
      << "((CORBA::Any *)&_tao_any)->replace ("
      << node->tc_name () << ", _tao_elem.inout (), 1, ACE_TRY_ENV);" << be_nl
      << "return 1;" << be_uidt_nl
      << "}" << be_nl
      << "ACE_TRY_CHECK;" << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "ACE_CATCHANY" << be_nl
      << "{" << be_idt_nl
      << node->name () << "_free (_tao_elem._retn ());" << be_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_nl
      << "ACE_ENDTRY;" << be_nl
      << "return 0;" << be_uidt_nl
      << "}\n\n";

  node->cli_stub_any_op_gen (1);
  return 0;
}
