//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    any_op_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Any operators for Union.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union.h"

ACE_RCSID(be_visitor_union, any_op_ch, "$Id$")


// ***************************************************************************
// Union visitor for generating Any operator declarations in the client header
// ***************************************************************************

be_visitor_union_any_op_ch::be_visitor_union_any_op_ch
(be_visitor_context *ctx)
  : be_visitor_union (ctx)
{
}

be_visitor_union_any_op_ch::~be_visitor_union_any_op_ch (void)
{
}

int
be_visitor_union_any_op_ch::visit_union (be_union *node)
{
  if (node->cli_hdr_any_op_gen () || node->imported ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  // generate the Any <<= and >>= operator declarations
  os->indent ();
  *os << "void " << idl_global->export_macro ()
      << " operator<<= (CORBA::Any &, const " << node->name ()
      << " &); // copying version" << be_nl;
  *os << "void " << idl_global->export_macro ()
      << " operator<<= (CORBA::Any &, " << node->name ()
      << "*); // noncopying version" << be_nl;
  *os << "CORBA::Boolean " << idl_global->export_macro ()
      << " operator>>= (const CORBA::Any &, "
      << node->name () << " *&);\n";


  // all we have to do is to visit the scope and generate code
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union::visit_union - "
                         "codegen for scope failed\n"), -1);
    }

  node->cli_hdr_any_op_gen (1);
  return 0;
}

int
be_visitor_union_any_op_ch::visit_union_branch (be_union_branch *node)
{
  be_type *bt; // field's type

  // first generate the type information
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_any_op_ch::"
                         "visit_union_branch - "
                         "Bad field type\n"
                         ), -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_any_op_ch::"
                         "visit_field - "
                         "codegen for field type failed\n"
                         ), -1);
    }
  return 0;
}
