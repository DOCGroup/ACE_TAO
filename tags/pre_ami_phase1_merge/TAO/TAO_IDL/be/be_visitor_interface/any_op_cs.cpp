//
// $id: any_op_cs.cpp,v 1.12 1998/10/30 19:46:55 levine Exp $
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
//    Visitor generating code for Any operators for Interface in the stubs
//    file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, any_op_cs, "$Id$")


// ***************************************************************************
// Interface visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_interface_any_op_cs::be_visitor_interface_any_op_cs
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_any_op_cs::~be_visitor_interface_any_op_cs (void)
{
}

int
be_visitor_interface_any_op_cs::visit_interface (be_interface *node)
{
  if (node->cli_stub_any_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  // generate the Any <<= and >>= operator declarations
  // Any <<= and >>= operators
  os->indent ();
  *os << "void operator<<= (CORBA::Any &_tao_any, "
      << node->name () << "_ptr _tao_elem)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::Object_ptr *_tao_obj_ptr = 0;" << be_nl
      << "ACE_TRY_NEW_ENV" << be_nl
      << "{" << be_idt_nl
      << "ACE_NEW (_tao_obj_ptr, CORBA::Object_ptr);" << be_nl
      << "*_tao_obj_ptr = " << node->name ()
      << "::_duplicate (_tao_elem);" << be_nl
      << "_tao_any.replace (" << node->tc_name () << ", "
      << "_tao_obj_ptr, 1, ACE_TRY_ENV);" << be_nl
      << "ACE_TRY_CHECK;" << be_uidt_nl
      << "}" << be_nl
      << "ACE_CATCHANY" << be_nl
      << "{" << be_idt_nl
      << "delete _tao_obj_ptr;" << be_uidt_nl
      << "}" << be_nl
      << "ACE_ENDTRY;" << be_uidt_nl
      << "}\n" << be_nl;

  *os << "CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, "
      << node->name () << "_ptr &_tao_elem)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::Object_ptr *tmp = 0;" << be_nl
      << "ACE_TRY_NEW_ENV" << be_nl
      << "{" << be_idt_nl
      << "_tao_elem = " << node->name () << "::_nil ();" << be_nl
      << "CORBA::TypeCode_var type = _tao_any.type ();" << be_nl
      << "if (!type->equal (" << node->tc_name ()
      << ", ACE_TRY_ENV)) return 0; // not equal" << be_nl
      << "ACE_TRY_CHECK;" << be_nl
      << "TAO_InputCDR stream (_tao_any._tao_get_cdr ());"
      << be_nl
      << "CORBA::Object_var _tao_obj_var;" << be_nl
      << "ACE_NEW_RETURN (tmp, CORBA::Object_ptr, 0);" << be_nl
      << "if (stream.decode (" << node->tc_name ()
      << ", &_tao_obj_var.out (), 0, ACE_TRY_ENV)" << be_nl
      << "   == CORBA::TypeCode::TRAVERSE_CONTINUE)" << be_nl
      << "{" << be_idt_nl
      << "_tao_elem = " << node->name ()
      << "::_narrow (_tao_obj_var.in (), ACE_TRY_ENV);" << be_nl
      << "ACE_TRY_CHECK;" << be_nl
      << "*tmp = (CORBA::Object_ptr) _tao_elem;  // any owns the object"
      << be_nl
      << "((CORBA::Any *)&_tao_any)->replace ("
      << node->tc_name () << ", tmp, 1, ACE_TRY_ENV);" << be_nl
      << "ACE_TRY_CHECK;" << be_nl
      << "return 1;" << be_uidt_nl
      << "}" << be_nl
      << "// failure" << be_uidt_nl
      << "}" << be_nl
      << "ACE_CATCHANY" << be_nl
      << "{" << be_idt_nl
      << "delete tmp;" << be_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_nl
      << "ACE_ENDTRY;" << be_nl
      << "return 0;" << be_uidt_nl
      << "}\n\n";

  *os << "#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)" << be_idt_nl
      << "template class TAO_Object_Field_T<"
      << node->name () << ">;" << be_uidt_nl
      << "#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)" << be_nl
      << "#  pragma instantiate TAO_Object_Field_T<"
      << node->name () << ">" << be_nl
      << "#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */\n\n";

  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface::visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  node->cli_stub_any_op_gen (1);
  return 0;
}
