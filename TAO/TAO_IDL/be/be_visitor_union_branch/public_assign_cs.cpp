//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    public_assign_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Union Branch for the assignment operator
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union_branch.h"


// **********************************************
//  visitor for union_branch in the client stubs file generating the code for
//  the copy ctor and assignment operator
// **********************************************

// constructor
be_visitor_union_branch_public_assign_cs::be_visitor_union_branch_public_assign_cs
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

// destructor
be_visitor_union_branch_public_assign_cs::~be_visitor_union_branch_public_assign_cs
(void)
{
}

// visit the union_branch node
int
be_visitor_union_branch_public_assign_cs::visit_union_branch (be_union_branch *node)
{
  TAO_OutStream *os; // output stream
  be_union_branch *ub =
    this->ctx_->be_node_as_union_branch (); // get union branch
  be_union *bu =
    this->ctx_->be_scope_as_union ();  // get the enclosing union backend

  os = this->ctx_->stream ();

  // This visitor is used when we are generating the copy ctor and
  // assignment operator for the union.
  // Individual assignment of the members takes place inside a case
  // statement because the type of member assigned is based on the value
  // of the discriminant
  os->indent ();
  if (ub->label ()->label_val ()->ec () == AST_Expression::EC_symbol)
    {
      *os << "case " << ub->label ()->label_val ()->n ()  << ":" << be_idt_nl;
    }
  else
    {
      *os << "case " << ub->label ()->label_val () << ":" << be_idt_nl;
    }

  *os << "this->" << ub->local_name () << "_ = u." << ub->local_name ()
      << "_;" << be_nl
      << "break;\n";
  os->decr_indent (0);

  return 0;
}
