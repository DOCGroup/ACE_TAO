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

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "be_visitor_array.h"

ACE_RCSID(be_visitor_array, any_op_cs, "$Id$")


// ***************************************************************************
// Array visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_array_any_op_cs::be_visitor_array_any_op_cs (
    be_visitor_context *ctx
  )
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

  *os << be_nl 
      << "// TAO_IDL - Generated from " << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  // Any <<= and >>= operators.
  os->indent ();
  *os << "void operator<<= (" << be_idt << be_idt_nl
      << "CORBA::Any &_tao_any," << be_nl
      << "const " << node->name () << "_forany &_tao_elem" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl;

  if (!node->is_local ())
    {
      *os << "TAO_OutputCDR stream;" << be_nl
          << "stream << _tao_elem;" << be_nl << be_nl;
    }

  *os << "if (_tao_elem.nocopy ()) // no copy" << be_idt_nl
      << "{" << be_idt_nl
      << "_tao_any._tao_replace (" << be_idt << be_idt_nl
      << node->tc_name () << "," << be_nl;

  if (!node->is_local ())
    {
      *os << "TAO_ENCAP_BYTE_ORDER," << be_nl
          << "stream.begin ()," << be_nl;
    }

  *os << "1," << be_nl
      << "(void *)_tao_elem.in ()," << be_nl
      << node->name () << "_forany::_tao_any_destructor" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl
      << "else                // copy" << be_idt_nl
      << "{" << be_idt_nl
      << "_tao_any._tao_replace (" << be_idt << be_idt_nl
      << node->tc_name () << "," << be_nl;

  if (!node->is_local ())
    {
      *os << "TAO_ENCAP_BYTE_ORDER," << be_nl
          << "stream.begin ()," << be_nl;
    }

  *os << "1," << be_nl
      << node->name () << "_dup (_tao_elem.in ())," << be_nl
      << node->name () << "_forany::_tao_any_destructor" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "}" << be_uidt;

  if (node->is_local ())
    {
      *os << be_nl << be_nl << "_tao_any.contains_local (1);";
    }

  *os << be_uidt_nl << "}\n\n";

  os->indent ();
  *os << "CORBA::Boolean operator>>= (" << be_idt << be_idt_nl
      << "const CORBA::Any &_tao_any," << be_nl
      << node->name () << "_forany &_tao_elem" << be_uidt_nl
      << ")" << be_uidt_nl
      << "{" << be_idt_nl
      << "ACE_TRY_NEW_ENV" << be_nl
      << "{" << be_idt_nl
      << "CORBA::TypeCode_var type = _tao_any.type ();" << be_nl << be_nl
      << "CORBA::Boolean result =" << be_idt_nl
      << "type->equivalent (" << be_idt << be_idt_nl
      << node->tc_name () << be_nl
      << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
      << ");" << be_uidt << be_uidt_nl
      << "ACE_TRY_CHECK;" << be_nl << be_nl
      << "if (result == 0)" << be_idt_nl
      << "{" << be_idt_nl
      << "return 0; // not equivalent" << be_uidt_nl
      << "}" << be_uidt_nl << be_nl;

  if (!node->is_local ())
    {
      *os << "if (_tao_any.any_owns_data ())" << be_idt_nl
          << "{" << be_idt_nl;
    }

  *os << "_tao_elem = (" << node->name () << "_slice *)_tao_any.value ();"
      << be_nl
      << "return 1;" << be_uidt_nl;

  if (!node->is_local ())
    {
      *os << "}" << be_uidt_nl
          << "else" << be_idt_nl  // else Any does not own the data
          << "{" << be_idt_nl
          << "_tao_elem.out () = " << node->name () << "_alloc ();" 
          << be_nl << be_nl
          << "if (!_tao_elem.in ())" << be_idt_nl
          << "{" << be_idt_nl
          << "return 0;" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "TAO_InputCDR stream (" << be_idt << be_idt_nl
          << "_tao_any._tao_get_cdr ()," << be_nl
          << "_tao_any._tao_byte_order ()" << be_uidt_nl
          << ");" << be_uidt_nl << be_nl
          << "if (stream >> _tao_elem)" << be_idt_nl
          << "{" << be_idt_nl
          << "((CORBA::Any *)&_tao_any)->_tao_replace (" 
          << be_idt << be_idt_nl
          << node->tc_name () << "," << be_nl
          << "1," << be_nl
          << "_tao_elem.inout ()," << be_nl
          << node->name () << "_forany::_tao_any_destructor" << be_uidt_nl
          << ");" << be_uidt_nl << be_nl
          << "return 1;" << be_uidt_nl
          << "}" << be_uidt_nl
          << "else" << be_idt_nl
          << "{" << be_idt_nl
          << node->name () << "_free (_tao_elem._retn ());" << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl;
    }

  *os << "}" << be_nl
      << "ACE_CATCHANY" << be_nl
      << "{" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_nl
      << "ACE_ENDTRY;";

  if (!node->is_local ())
    {
      *os << be_nl << be_nl << "return 0;";
    }

  *os << be_uidt_nl << "}\n\n";

  node->cli_stub_any_op_gen (1);
  return 0;
}
