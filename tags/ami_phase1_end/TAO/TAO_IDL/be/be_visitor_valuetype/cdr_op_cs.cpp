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
//    Concrete visitor for valuetypes.
//    This one provides code generation for the CDR operators.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    based on code from Aniruddha Gokhale
//
// ============================================================================


#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_valuetype.h"
// #include "be_visitor_field.h"

ACE_RCSID(be_visitor_valuetype, cdr_op_cs, "$Id$")

be_visitor_valuetype_cdr_op_cs::be_visitor_valuetype_cdr_op_cs
(be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_cdr_op_cs::~be_visitor_valuetype_cdr_op_cs (void)
{
}

int
be_visitor_valuetype_cdr_op_cs::visit_valuetype (be_valuetype *node)
{
  // already generated and/or we are imported. Don't do anything.
  if (node->cli_stub_cdr_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  // First generate code for our children. The reason we do this first is
  // because the inlined code for our children must be available before we use
  // it in our parent

  // set the substate as generating code for the types defined in our scope
  //this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_SCOPE);
  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cdr_op_cs"
                         "::visit_valuetype - "
                         "codegen for scope failed\n"), -1);
    }

  //  set the sub state as generating code for the output operator
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_OUTPUT);
  os->indent ();
  *os << "ACE_INLINE CORBA::Boolean" << be_nl
      << "operator<< (TAO_OutputCDR &strm, const "
      << node->name ()
      << " *_tao_valuetype)" << be_nl
      << "{" << be_idt_nl;
  *os << "return CORBA_ValueBase::_tao_marshal (strm,"  << be_idt_nl
      << "ACE_const_cast (" << node->name () << "*, _tao_valuetype)," << be_nl
      << "(ptr_arith_t) &" << node->name() <<"::_downcast);"
      << be_uidt<< be_uidt_nl
      << "}\n\n";

  // set the substate as generating code for the input operator
  //this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);
  *os << "ACE_INLINE CORBA::Boolean" << be_nl
      << "operator>> (TAO_InputCDR &strm, "
      << node->name ()
      << " *&_tao_valuetype)" << be_nl
      << "{" << be_idt_nl;
  *os << "return " << node->name() << "::_tao_unmarshal (strm, _tao_valuetype);"
#ifdef obv_marshal_old_version
  *os << "CORBA::ValueBase *ptr;" << be_nl
      << "int retval = CORBA_ValueBase::_tao_unmarshal (strm,"
      << be_idt_nl << "ptr, (ptr_arith_t) &" << node->name() <<"::_downcast);"
      << be_uidt_nl
      << "if (retval) {" << be_idt_nl
      << "_tao_valuetype = " << node->name() << "::_downcast (ptr);"
      << be_nl << "if (_tao_valuetype) retval = 1;"
      << be_uidt_nl << "}" << be_idt_nl
      << "return retval;"
#endif /* obv_marshal_old_version */
      << be_uidt_nl
      << "}\n\n";

  if (!node->is_abstract_valuetype ())
    { // functions that marshal state
      be_visitor_context* new_ctx  =
        new be_visitor_context (*this->ctx_);
      be_visitor_valuetype_marshal_cs visitor (new_ctx);
      visitor.visit_valuetype (node);
    }

  node->cli_stub_cdr_op_gen (1);
  return 0;
}
