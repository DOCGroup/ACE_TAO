//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ch.cpp
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

ACE_RCSID(be_visitor_valuetype, cdr_op_ch, "$Id$")

be_visitor_valuetype_cdr_op_ch::be_visitor_valuetype_cdr_op_ch
(be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_cdr_op_ch::~be_visitor_valuetype_cdr_op_ch (void)
{
}

int
be_visitor_valuetype_cdr_op_ch::visit_valuetype (be_valuetype *node)
{
  if (node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  os->indent (); //start with whatever indentation level we are at now

  if (!node->cli_hdr_cdr_op_gen ())
  {
      // Generate helper functions declaration.
      if (node->gen_helper_header () == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_cdr_op_ch::"
                             "visit_valuetype - "
                             "codegen for helper functions failed\n"), -1);
        }

      // generate the CDR << and >> operator declarations (prototypes)

      os->indent (); //start with whatever indentation level we are at now

      *os << be_global->stub_export_macro ()
          << "CORBA::Boolean operator<< (TAO_OutputCDR &, const "
          << node->full_name () << " *);" << be_nl;

      *os << be_global->stub_export_macro ()
          << "CORBA::Boolean operator>> (TAO_InputCDR &, "
          << node->full_name () << " *&);" << be_nl;

      node->cli_hdr_cdr_op_gen (1);
    }

  // set the substate as generating code for the types defined in our scope
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_SCOPE);
  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_cdr_op_ch::"
                         "visit_valuetype - "
                         "codegen for scope failed\n"), -1);
    }

  //@@ Boris: that's suck!
  if (!node->is_abstract_valuetype ())
    {
      // functions that marshal state
      be_visitor_context* new_ctx  =
        new be_visitor_context (*this->ctx_);
      be_visitor_valuetype_marshal_ch visitor (new_ctx);
      visitor.visit_valuetype (node);
  }

  return 0;
}
