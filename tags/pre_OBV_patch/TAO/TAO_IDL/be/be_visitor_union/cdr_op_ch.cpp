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
//    Visitor generating code for CDR operators for unions. This uses
//    compiled marshaling.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union.h"

ACE_RCSID(be_visitor_union, cdr_op_ch, "$Id$")

// ***************************************************************************
// Union visitor for generating CDR operator declarations in the client header
// ***************************************************************************

be_visitor_union_cdr_op_ch::be_visitor_union_cdr_op_ch
(be_visitor_context *ctx)
  : be_visitor_union (ctx)
{
}

be_visitor_union_cdr_op_ch::~be_visitor_union_cdr_op_ch (void)
{
}

int
be_visitor_union_cdr_op_ch::visit_union (be_union *node)
{
  if (node->cli_hdr_cdr_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  // generate the CDR << and >> operator declarations
  os->indent ();
  *os << "CORBA::Boolean " << idl_global->export_macro ()
      << " operator<< (TAO_OutputCDR &, const " << node->name ()
      << " &); // " << be_nl;
  *os << "CORBA::Boolean " << idl_global->export_macro ()
      << " operator>> (TAO_InputCDR &, "
      << node->name () << " &);\n";


  // set the substate as generating code for the types defined in our scope
  this->ctx_->sub_state(TAO_CodeGen::TAO_CDR_SCOPE);
  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_cdr_op_ch::"
                         "visit_union - "
                         "codegen for scope failed\n"), -1);
    }


  node->cli_hdr_cdr_op_gen (1);
  return 0;
}

