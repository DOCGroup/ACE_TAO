//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ci.cpp
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

ACE_RCSID(be_visitor_valuetype, cdr_op_ci, "$Id$")

be_visitor_valuetype_cdr_op_ci::be_visitor_valuetype_cdr_op_ci
(be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_cdr_op_ci::~be_visitor_valuetype_cdr_op_ci (void)
{
}

int
be_visitor_valuetype_cdr_op_ci::visit_valuetype (be_valuetype *node)
{
  // already generated and/or we are imported. Don't do anything.
  if (node->cli_inline_cdr_op_gen () || node->imported ())
    return 0;

  // Generate helper functions implementation.
  if (node->gen_helper_inline () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cdr_op_ci::"
                         "visit_valuetype - "
                         "codegen for helper functions failed\n"), -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();
  os->indent (); //start with whatever indentation level we are at now

  // Generate CDR << and >> operator signatures

  //@@ Boris: Can I move this to be_valuetype? (as with _var, _out, etc?)

  //This is just declaration so no ACE_INLINE
  *os << be_global->stub_export_macro () << " "
      << "CORBA::Boolean operator<< (TAO_OutputCDR &, const "
      << node->full_name () << " *);" << be_nl;

  *os << be_global->stub_export_macro () << " "
      << "CORBA::Boolean operator>> (TAO_InputCDR &, "
      << node->full_name () << " *&);" << be_nl;



  // Generate marshaling code if any

  // @@ Boris: the next statement is patrue anymore since I moved code
  //    to stubs.

  // First generate code for our children. The reason we do this first is
  // because the inlined code for our children must be available before we use
  // it in our parent

  // set the substate as generating code for the types defined in our scope
  //this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_SCOPE);
  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cdr_op_ci"
                         "::visit_valuetype - "
                         "codegen for scope failed\n"), -1);
    }

  node->cli_inline_cdr_op_gen (I_TRUE);
  return 0;
}
