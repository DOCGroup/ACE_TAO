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
//    Boris Kolpackov <bosk@ipmce.ru>
//    based on code from Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    based on code from Aniruddha Gokhale
//
// ============================================================================


#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_valuetype.h"

ACE_RCSID(be_visitor_valuetype, valuetype_cdr_op_cs, "$Id$")

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

  // Generate helper functions implementation.
  if (node->gen_helper_stubs () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cdr_op_cs::"
                         "visit_valuetype - "
                         "codegen for helper functions failed\n"), -1);
    }

  // Generate marshaling code


  TAO_OutStream *os = this->ctx_->stream ();

  // @@ Boris: Do I need to let our children a chance to generate anything?
  //    Sounds fair ;-)

  // @@ Boris: the next statement is not true anymore since this is a CS

  // First generate code for our children. The reason we do this first is
  // because the inlined code for our children must be available before we use
  // it in our parent

  // set the substate as generating code for the types defined in our scope
  // this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_SCOPE);
  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cdr_op_ci"
                         "::visit_valuetype - "
                         "codegen for scope failed\n"), -1);
    }

  //  set the sub state as generating code for the output operator
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_OUTPUT);
  *os << "CORBA::Boolean" << be_nl
      << "operator<< (TAO_OutputCDR &strm, const "
      << node->full_name ()
      << " *_tao_valuetype)" << be_nl
      << "{" << be_idt_nl;
  *os << "return CORBA_ValueBase::_tao_marshal (strm,"  << be_idt_nl
      << "ACE_const_cast (" << node->full_name () << "*, _tao_valuetype)," << be_nl
      << "(ptr_arith_t) &" << node->full_name() <<"::_downcast);"
      << be_uidt<< be_uidt_nl
      << "}\n\n";

  // set the substate as generating code for the input operator
  //this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_INPUT);
  *os << "CORBA::Boolean" << be_nl
      << "operator>> (TAO_InputCDR &strm, "
      << node->full_name ()
      << " *&_tao_valuetype)" << be_nl
      << "{" << be_idt_nl;
  *os << "return " << node->full_name() << "::_tao_unmarshal (strm, _tao_valuetype);"
#ifdef obv_marshal_old_version
  *os << "CORBA::ValueBase *ptr;" << be_nl
      << "int retval = CORBA_ValueBase::_tao_unmarshal (strm,"
      << be_idt_nl << "ptr, (ptr_arith_t) &" << node->full_name() <<"::_downcast);"
      << be_uidt_nl
      << "if (retval) {" << be_idt_nl
      << "_tao_valuetype = " << node->full_name() << "::_downcast (ptr);"
      << be_nl << "if (_tao_valuetype) retval = 1;"
      << be_uidt_nl << "}" << be_idt_nl
      << "return retval;"
#endif /* obv_marshal_old_version */
      << be_uidt_nl
      << "}\n\n";

  //@@ Boris: ment to be refactored
  if (!node->is_abstract_valuetype ())
  { 
    // functions that marshal state
    be_visitor_context* new_ctx  = new be_visitor_context (*this->ctx_);
    be_visitor_valuetype_marshal_cs visitor (new_ctx);
    visitor.visit_valuetype (node);
  }

  node->cli_stub_cdr_op_gen (I_TRUE);
  
  return 0;
}
