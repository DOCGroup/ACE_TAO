//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    discriminant_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for discriminant of the union.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union.h"

ACE_RCSID(be_visitor_union, discriminant_ci, "$Id$")


// *************************************************************************
// be_visitor_discriminant_ci - visitor for discriminant in client inline file
// *************************************************************************

be_visitor_union_discriminant_ci::be_visitor_union_discriminant_ci
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_union_discriminant_ci::~be_visitor_union_discriminant_ci (void)
{
}

int
be_visitor_union_discriminant_ci::visit_enum (be_enum *node)
{
  TAO_OutStream *os; // output stream
  be_union *bu =
    this->ctx_->be_node_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  os = this->ctx_->stream ();

  os->indent ();
  // the set method
  *os << "// accessor to set the discriminant" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::_d (" << bt->name ()
      << " discval)" << be_nl
      << "{" << be_idt_nl
      << "this->disc_ = discval;" << be_uidt_nl
      << "}" << be_nl;

  // the get method
  *os << "// accessor to get the discriminant" << be_nl
      << "ACE_INLINE " << bt->name () << be_nl
      << bu->name () << "::_d (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->disc_;" << be_uidt_nl
      << "}\n\n";
  return 0;
}

int
be_visitor_union_discriminant_ci::visit_predefined_type (be_predefined_type
                                                         *node)
{
  TAO_OutStream *os; // output stream
  be_union *bu =
    this->ctx_->be_node_as_union ();  // get the enclosing union backend
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  os = this->ctx_->stream ();

  os->indent ();
  // the set method
  *os << "// accessor to set the discriminant" << be_nl
      << "ACE_INLINE void" << be_nl
      << bu->name () << "::_d (" << bt->name ()
      << " discval)" << be_nl
      << "{" << be_idt_nl
      << "this->disc_ = discval;" << be_uidt_nl
      << "}" << be_nl;

  // the get method
  *os << "// accessor to get the discriminant" << be_nl
      << "ACE_INLINE " << bt->name () << be_nl
      << bu->name () << "::_d (void) const" << be_nl
      << "{" << be_idt_nl
      << "return this->disc_;" << be_uidt_nl
      << "}\n\n";
  return 0;
}

int
be_visitor_union_discriminant_ci::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // save the typedef node for use in code generation
                           // as we visit the base type

  // the node to be visited in the base primitve type that gets typedefed
  be_type *bt = node->primitive_base_type ();
  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_discriminant_ci::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ), -1);
    }
  this->ctx_->alias (0);

  return 0;
}
