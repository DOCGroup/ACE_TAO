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
//    Visitor generating code for Any operator for the Sequence node
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "be_visitor_sequence.h"

ACE_RCSID(be_visitor_sequence, any_op_cs, "$Id$")


// ***************************************************************************
// Sequence visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_sequence_any_op_cs::be_visitor_sequence_any_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_sequence_any_op_cs::~be_visitor_sequence_any_op_cs (void)
{
}

int
be_visitor_sequence_any_op_cs::visit_sequence (be_sequence *node)
{
  if (node->cli_stub_any_op_gen ()
      || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Any <<= and >>= operators.
  os->indent ();

  *os << be_nl 
      << "// TAO_IDL - Generated from " << be_nl
      << "// " << __FILE__ << ":" << __LINE__ << be_nl << be_nl;

  if (node->is_local ())
    {
      // Copying insertion.
      *os << "// Copying insertion." << be_nl
          << "void operator<<= (" << be_idt << be_idt_nl
          << "CORBA::Any &_tao_any," << be_nl
          << "const " << node->name () << " &_tao_elem" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << node->name  () << " *_tao_elem_copy = 0;" << be_nl
          << "ACE_NEW (" << be_idt << be_idt_nl
          << "_tao_elem_copy," << be_nl
          << node->name () << " (_tao_elem)" << be_uidt_nl
          << ");" << be_uidt_nl << be_nl
          << "_tao_any._tao_replace (" << be_idt << be_idt_nl
          << node->tc_name () << "," << be_nl
          << "1," << be_nl
          << "_tao_elem_copy," << be_nl
          << node->name () << "::_tao_any_destructor" << be_uidt_nl
          << ");" << be_uidt_nl << be_nl
          << "_tao_any.contains_local (1);" << be_uidt_nl
          << "}\n" << be_nl;

      // Non-copying insertion.
      *os << "// Non-copying insertion." << be_nl
          << "void operator<<= (" << be_idt << be_idt_nl
          << "CORBA::Any &_tao_any," << be_nl
          << node->name () << " *_tao_elem" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "_tao_any._tao_replace (" << be_idt << be_idt_nl
          << node->tc_name () << "," << be_nl
          << "1," << be_nl
          << "_tao_elem," << be_nl
          << node->name () << "::_tao_any_destructor" << be_uidt_nl
          << ");" << be_uidt_nl << be_nl
          << "_tao_any.contains_local (1);" << be_uidt_nl
          << "}\n" << be_nl;

      // Extraction to non-const pointer (deprecated, just calls the other).
      *os << "// Extraction to non-const pointer (deprecated)." << be_nl
          << "CORBA::Boolean operator>>= (" << be_idt << be_idt_nl
          << "const CORBA::Any &_tao_any," << be_nl
          << node->name () << " *&_tao_elem" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "return _tao_any >>= ACE_const_cast(" << be_idt << be_idt_nl
          << "const " << node->name () << "*&," << be_nl
          << "_tao_elem" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "}\n\n";

      // Extraction to const pointer.
      *os << "// Extraction to const pointer." << be_nl
          << "CORBA::Boolean operator>>= (" << be_idt << be_idt_nl
          << "const CORBA::Any &_tao_any," << be_nl
          << "const " << node->name () << " *&_tao_elem" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "_tao_elem = 0;" << be_nl << be_nl
          << "ACE_TRY_NEW_ENV" << be_nl
          << "{" << be_idt_nl
          << "CORBA::TypeCode_var type = _tao_any.type ();" 
          << be_nl << be_nl
          << "CORBA::Boolean result =" << be_idt_nl
          << "type->equivalent (" << be_idt << be_idt_nl
          << node->tc_name () << be_nl
          << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "ACE_TRY_CHECK;" << be_nl << be_nl
          << "if (!result)" << be_idt_nl
          << "{" << be_idt_nl
          << "return 0; // not equivalent" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "_tao_elem = ACE_static_cast(" << be_idt << be_idt_nl
          << "const " << node->name () << "*," << be_nl
          << "_tao_any.value ()" << be_uidt_nl
          << ");" << be_uidt_nl << be_nl
          << "return 1;" << be_uidt_nl
          << "}" << be_nl
          << "ACE_CATCHANY" << be_nl
          << "{" << be_nl
          << "}" << be_nl
          << "ACE_ENDTRY;" << be_nl << be_nl
          << "return 0;" << be_uidt_nl
          << "}\n\n";
    }
  else
    {
      // Copying insertion.
      *os << "// Copying insertion." << be_nl
          << "void operator<<= (" << be_idt << be_idt_nl
          << "CORBA::Any &_tao_any," << be_nl
          << "const " << node->name () << " &_tao_elem" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "TAO_OutputCDR stream;" << be_nl << be_nl
          << "if (stream << _tao_elem)" << be_idt_nl
          << "{" << be_idt_nl
          << "_tao_any._tao_replace (" << be_idt << be_idt_nl
          << node->tc_name () << "," << be_nl
          << "TAO_ENCAP_BYTE_ORDER," << be_nl
          << "stream.begin ()" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl
          << "}\n" << be_nl;

      // Non-copying insertion.
      *os << "// Non-copying insertion." << be_nl
          << "void operator<<= (" << be_idt << be_idt_nl
          << "CORBA::Any &_tao_any," << be_nl
          << node->name () << " *_tao_elem" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "TAO_OutputCDR stream;" << be_nl << be_nl
          << "if (stream << *_tao_elem)" << be_idt_nl
          << "{" << be_idt_nl
          << "_tao_any._tao_replace (" << be_idt << be_idt_nl
          << node->tc_name () << "," << be_nl
          << "TAO_ENCAP_BYTE_ORDER," << be_nl
          << "stream.begin ()," << be_nl
          << "1," << be_nl
          << "_tao_elem," << be_nl
          << node->name () << "::_tao_any_destructor" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl
          << "}\n" << be_nl;

      // Extraction to non-const pointer (deprecated, just calls the other).
      *os << "// Extraction to non-const pointer (deprecated)." << be_nl
          << "CORBA::Boolean operator>>= (" << be_idt << be_idt_nl
          << "const CORBA::Any &_tao_any," << be_nl
          << node->name () << " *&_tao_elem" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "return _tao_any >>= ACE_const_cast(" << be_idt << be_idt_nl
          << "const " << node->name () << "*&," << be_nl
          << "_tao_elem" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "}\n\n";

      // Extraction to const pointer.
      *os << "// Extraction to const pointer." << be_nl
          << "CORBA::Boolean operator>>= (" << be_idt << be_idt_nl
          << "const CORBA::Any &_tao_any," << be_nl
          << "const " << node->name () << " *&_tao_elem" << be_uidt_nl
          << ")" << be_uidt_nl
          << "{" << be_idt_nl
          << "_tao_elem = 0;" << be_nl << be_nl
          << "ACE_TRY_NEW_ENV" << be_nl
          << "{" << be_idt_nl
          << "CORBA::TypeCode_var type = _tao_any.type ();" 
          << be_nl << be_nl
          << "CORBA::Boolean result =" << be_idt_nl
          << "type->equivalent (" << be_idt << be_idt_nl
          << node->tc_name () << be_nl
          << "ACE_ENV_ARG_PARAMETER" << be_uidt_nl
          << ");" << be_uidt << be_uidt_nl
          << "ACE_TRY_CHECK;" << be_nl << be_nl
          << "if (!result)" << be_idt_nl
          << "{" << be_idt_nl
          << "return 0; // not equivalent" << be_uidt_nl
          << "}" << be_uidt_nl << be_nl
          << "if (_tao_any.any_owns_data ())" << be_idt_nl
          << "{" << be_idt_nl
          << "_tao_elem = ACE_static_cast(" << be_idt << be_idt_nl
          << "const " << node->name () << "*," << be_nl
          << "_tao_any.value ()" << be_uidt_nl
          << ");" << be_uidt_nl << be_nl
          << "return 1;" << be_uidt_nl
          << "}" << be_uidt_nl
          << "else" << be_idt_nl
          << "{" << be_idt_nl
          << node->name () << " *tmp = 0;" << be_nl
          << "ACE_NEW_RETURN (" << be_idt << be_idt_nl
          << "tmp," << be_nl
          << node->name () << "," << be_nl
          << "0" << be_uidt_nl
          <<");" << be_uidt_nl << be_nl
          << "TAO_InputCDR stream (" << be_idt << be_idt_nl
          << "_tao_any._tao_get_cdr ()," << be_nl
          << "_tao_any._tao_byte_order ()" << be_uidt_nl
          << ");" << be_uidt_nl << be_nl
          << "if (stream >> *tmp)" << be_idt_nl
          << "{" << be_idt_nl
          << "((CORBA::Any *)&_tao_any)->_tao_replace (" 
          << be_idt << be_idt_nl
          << node->tc_name () << "," << be_nl
          << "1," << be_nl
          << "ACE_static_cast (void *, tmp)," << be_nl
          << node->name () << "::_tao_any_destructor" << be_uidt_nl
          << ");" << be_uidt_nl << be_nl
          << "_tao_elem = tmp;" << be_nl
          << "return 1;" << be_uidt_nl
          << "}" << be_uidt_nl
          << "else" << be_idt_nl
          << "{" << be_idt_nl
          << "delete tmp;" << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl
          << "}" << be_uidt << be_uidt_nl
          << "}" << be_nl
          << "ACE_CATCHANY" << be_nl
          << "{" << be_nl
          << "}" << be_nl
          << "ACE_ENDTRY;" << be_nl << be_nl
          << "return 0;" << be_uidt_nl
          << "}\n\n";
    }

  node->cli_stub_any_op_gen (1);
  return 0;
}
