// $id: any_op_cs.cpp,v 1.12 1998/10/30 19:46:55 levine Exp $

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    any_op_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Any operators for valuetypes in the stubs
//    file.
//
// = AUTHOR
//    Jeff Parsons <parsons@cs.wustl.edu>
//    Boris Kolpackov <bosk@ipmce.ru>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_valuetype.h"

ACE_RCSID(be_visitor_valuetype, any_op_cs, "$Id$")


// ***************************************************************************
// Valuetype visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_valuetype_any_op_cs::be_visitor_valuetype_any_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_any_op_cs::~be_visitor_valuetype_any_op_cs (void)
{
}

int
be_visitor_valuetype_any_op_cs::visit_valuetype (be_valuetype *node)
{
  if (node->cli_stub_any_op_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  os->indent ();

  // Generate the Any <<= and >>= operator declarations

  *os << be_global->stub_export_macro () << "void" << be_nl
      << "operator<<= (CORBA::Any &any, " << node->name ()
      << " *value) // copying" << be_nl
      << "{" << be_idt_nl
      << "TAO_OutputCDR stream;" << be_nl
      << "if (stream << value)" << be_nl
      << "{" << be_idt_nl
      << "any._tao_replace (" << be_idt << be_idt_nl
      << node->tc_name () << ", " << be_nl
      << "TAO_ENCAP_BYTE_ORDER," << be_nl
      << "stream.begin ());" << be_uidt << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << be_global->stub_export_macro () << "void" << be_nl
      << "operator<<= (CORBA::Any &any, " << node->name ()
      << " **value) // non-copying" << be_nl  
      << "{" << be_idt_nl
      << "TAO_OutputCDR stream;" << be_nl
      << "if (stream << *value)" << be_nl
      << "{" << be_idt_nl
      << "any._tao_replace (" << be_idt << be_idt_nl
      << node->tc_name () << ", " << be_nl
      << "TAO_ENCAP_BYTE_ORDER," << be_nl
      << "stream.begin ()," << be_nl
      << "1," << be_nl
      << "*value," << be_nl
      << node->name () << "::_tao_any_destructor);" 
      << be_uidt << be_uidt << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl << be_nl;


  *os << be_global->stub_export_macro () << "CORBA::Boolean" << be_nl
      << "operator>>= (const CORBA::Any &any, "
      << node->name () << " *&value)" << be_nl
      << "{" << be_idt_nl
      << "ACE_TRY_NEW_ENV" << be_nl
      << "{" << be_idt_nl
      << "value = 0;" << be_nl
      << "CORBA::TypeCode_var type = any.type ();" << be_nl << be_nl
      << "CORBA::Boolean result = type->equivalent (" << node->tc_name ()
      << " TAO_ENV_ARG_PARAMETER);" << be_nl
      << "ACE_TRY_CHECK;" << be_nl << be_nl
      << "if (!result)" << be_nl
      << "{" << be_idt_nl
      << "return 0; // not equivalent" << be_uidt_nl
      << "}" << be_nl
      << "if (any.any_owns_data ())" << be_nl
      << "{" << be_idt_nl
      << "value = ACE_static_cast(" << be_idt << be_idt_nl
      << node->name () << "*," << be_nl
      << "any.value ());" << be_uidt << be_uidt_nl
      << "return 1;" << be_uidt_nl
      << "}" << be_nl
      << "else" << be_nl  // else any does not own the data
      << "{" << be_idt_nl
      << node->name () << " *tmp;" << be_nl
      << "TAO_InputCDR stream (" << be_idt << be_idt_nl
      << "any._tao_get_cdr ()," << be_nl
      << "any._tao_byte_order ());" << be_uidt << be_uidt_nl
      << "if (stream >> tmp)" << be_nl
      << "{" << be_idt_nl
      << "((CORBA::Any *)&any)->_tao_replace (" << be_idt << be_idt_nl
      << node->tc_name () << "," << be_nl
      << "1," << be_nl
      << "ACE_static_cast (void *, tmp)," << be_nl
      << node->name () << "::_tao_any_destructor);" << be_uidt << be_uidt_nl
      << "value = tmp;" << be_nl
      << "return 1;" << be_uidt_nl
      << "}" << be_nl
      << be_uidt_nl
      << "}" << be_uidt_nl
      << "}" << be_nl
      << "ACE_CATCHANY" << be_nl
      << "{" << be_nl
      << "}" << be_nl
      << "ACE_ENDTRY;" << be_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_nl << be_nl;

  *os << "#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)  || \\"
      << be_idt_nl
      << "  defined (ACE_HAS_GNU_REPO)" << be_idt_nl
      << "template class TAO_Valuetype_Manager<"
      << node->full_name () << ", "
      << node->full_name () << "_var>;" << be_uidt_nl
      << "#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)" << be_nl
      << "#  pragma instantiate TAO_Valuetype_Manager<"
      << node->full_name () << ", "
      << node->full_name () << "_var>" << be_uidt_nl
      << "#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */\n\n";

  node->cli_stub_any_op_gen (1);

  return 0;
}
