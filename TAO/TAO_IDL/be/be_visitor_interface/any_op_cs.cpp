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
//    Visitor generating code for Any operators for Interface in the stubs
//    file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface.h"


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

  TAO_OutStream *os = tao_cg->client_stubs ();

  // generate the Any <<= and >>= operator declarations
  // Any <<= and >>= operators
  os->indent ();
  *os << "void operator<<= (CORBA::Any &_tao_any, "
      << node->name () << "_ptr _tao_elem) // copying" << be_nl
      << "{" << be_idt_nl
      << "CORBA::Environment _tao_env;" << be_nl
      << "CORBA::Object_ptr *_tao_obj_ptr;" << be_nl
      << "ACE_NEW (_tao_obj_ptr, CORBA::Object_ptr);" << be_nl
      << "*_tao_obj_ptr = " << node->name ()
      << "::_duplicate (_tao_elem);" << be_nl
      << "_tao_any.replace (" << node->tc_name () << ", "
      << "_tao_obj_ptr, 1, _tao_env);" << be_uidt_nl
      << "}\n" << be_nl;

  *os << "void operator<<= (CORBA::Any &_tao_any, "
      << node->name () << "_ptr *_tao_elem) // non copying" << be_nl
      << "{" << be_idt_nl
      << "CORBA::Environment _tao_env;" << be_nl
      << "_tao_any.replace (" << node->tc_name () << ", "
      << "_tao_elem, 1, _tao_env); // consume it" << be_uidt_nl
      << "}\n" << be_nl;

  *os << "CORBA::Boolean operator>>= (const CORBA::Any &_tao_any, "
      << node->name () << "_ptr &_tao_elem)" << be_nl
      << "{" << be_idt_nl
      << "CORBA::Environment _tao_env;" << be_nl
      << "_tao_elem = " << node->name () << "::_nil ();" << be_nl
      << "CORBA::TypeCode_var type = _tao_any.type ();" << be_nl
      << "if (!type->equal (" << node->tc_name ()
      << ", _tao_env)) return 0; // not equal" << be_nl
      << "TAO_InputCDR stream ((ACE_Message_Block *)_tao_any.value ());"
      << be_nl
      << "CORBA::Object_ptr *_tao_obj_ptr;" << be_nl
      << "ACE_NEW_RETURN (_tao_obj_ptr, CORBA::Object_ptr, 0);" << be_nl
      << "if (stream.decode (" << node->tc_name ()
      << ", _tao_obj_ptr, 0, _tao_env)" << be_nl
      << "  == CORBA::TypeCode::TRAVERSE_CONTINUE)" << be_nl
      << "{" << be_idt_nl
      << "_tao_elem = " << node->name ()
      << "::_narrow (*_tao_obj_ptr, _tao_env);" << be_nl
      << "if (_tao_env.exception ()) return 0; // narrow failed" << be_nl
      << "CORBA::release (*_tao_obj_ptr);" << be_nl
      << "*_tao_obj_ptr = _tao_elem;" << be_nl
      << "((CORBA::Any *)&_tao_any)->replace ("
      << node->tc_name () << ", _tao_obj_ptr, 1, _tao_env);" << be_nl
      << "if (_tao_env.exception ()) return 0; // narrow failed" << be_nl
      << "return 1;" << be_uidt_nl
      << "}" << be_nl
      << "return 0; // failure" << be_uidt_nl
      << "}\n\n";


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
