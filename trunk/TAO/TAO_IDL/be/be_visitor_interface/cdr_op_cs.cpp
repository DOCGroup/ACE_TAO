//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for CDR operators for interfaces
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, cdr_op_cs, "$Id$")

// ***************************************************************************
// Interface visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_interface_cdr_op_cs::be_visitor_interface_cdr_op_cs
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_cdr_op_cs::~be_visitor_interface_cdr_op_cs (void)
{
}

int
be_visitor_interface_cdr_op_cs::visit_interface (be_interface *node)
{
  // already generated and/or we are imported. Don't do anything.
  if (node->cli_stub_cdr_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  //  set the sub state as generating code for the output operator
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_OUTPUT);
  *os << "ACE_INLINE CORBA::Boolean operator<< (TAO_OutputCDR &strm, "
      << "const " << node->name () << "_ptr _tao_objref)" << be_nl
      << "{" << be_idt_nl;
  // hand over the encoding to the TAO's internal engine
  //  *os << "CORBA::Environment env;" << be_nl;
  *os << "TAO_TRY" << be_nl
      << "{" << be_idt_nl;
  // resolve the nastiness created due to casting to void* and then to
  // CORBA::Object_ptr
  *os << "CORBA::Object_ptr _tao_corba_obj = _tao_objref;" << be_nl;
  *os << "if (TAO_MARSHAL_OBJREF::instance ()->" << be_nl
      << "        encode (0, &_tao_corba_obj, 0, &strm, TAO_TRY_ENV) == "
      << be_nl
      << "        CORBA::TypeCode::TRAVERSE_CONTINUE)" << be_idt_nl
      << "return 1;" << be_uidt_nl
      << "else" << be_idt_nl
      << "return 0;" << be_uidt << be_uidt_nl;
  *os << "}" << be_nl
      << "TAO_CATCHANY" << be_nl
      << "{" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_nl
      << "TAO_ENDTRY;" << be_nl
      << "return 0;" << be_uidt_nl;
  *os << "}\n\n";

  // set the substate as generating code for the input operator
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);
  *os << "ACE_INLINE CORBA::Boolean operator>> (TAO_InputCDR &strm, "
      << node->name () << "_ptr &_tao_objref)" << be_nl
      << "{" << be_idt_nl;
  // hand over to the TAO's internal marshaling engine
  //  *os << "CORBA::Environment env;" << be_nl;
  *os << "TAO_TRY" << be_nl
      << "{" << be_idt_nl;
  *os << "CORBA::Object_ptr obj;" << be_nl;
  *os << "if (TAO_MARSHAL_OBJREF::instance ()->" << be_nl
      << "        decode (0, &obj, 0, &strm, TAO_TRY_ENV) == " << be_nl
      << "        CORBA::TypeCode::TRAVERSE_CONTINUE)" << be_nl
      << "{" << be_idt_nl;
  *os << "TAO_CHECK_ENV;" << be_nl;
  *os << "// narrow to the right type" << be_nl;
  *os << "_tao_objref = " << node->name ()
      << "::_narrow (obj, TAO_TRY_ENV);" << be_nl;
  *os << "TAO_CHECK_ENV;" << be_nl;
  *os << "CORBA::release (obj);" << be_nl;
  *os << "return 1;" << be_uidt_nl;
  *os << "}" << be_nl;
  *os << "return 0; // error" << be_uidt_nl;
  *os << "}" << be_nl
      << "TAO_CATCHANY" << be_nl
      << "{" << be_idt_nl
      << "return 0;" << be_uidt_nl
      << "}" << be_nl
      << "TAO_ENDTRY;" << be_nl
      << "return 0;" << be_uidt_nl;
  *os << "}\n\n";

  // set the substate as generating code for the types defined in our scope
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_SCOPE);
  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_cdr_op_cs"
                         "::visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  node->cli_stub_cdr_op_gen (1);
  return 0;
}
