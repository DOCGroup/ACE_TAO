//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    discriminant_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for discriminant of the Union
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union.h"

ACE_RCSID(be_visitor_union, discriminant_ch, "$Id$")


// *************************************************************************
// be_visitor_discriminant_ch - visitor for discriminant in client header file
// *************************************************************************

be_visitor_union_discriminant_ch::be_visitor_union_discriminant_ch
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_union_discriminant_ch::~be_visitor_union_discriminant_ch (void)
{
}

int
be_visitor_union_discriminant_ch::visit_enum (be_enum *node)
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

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the enum declaration
      ctx.state (TAO_CodeGen::TAO_ENUM_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_discriminant_ch::"
                             "visit_enum - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_discriminant_ch::"
                             "visit_enum - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  os->indent ();
  // the set method
  *os << "void _d (" << bt->nested_type_name (bu) << ");" << be_nl;
  // the get method
  *os << bt->nested_type_name (bu) << " _d (void) const;\n\n";
  return 0;
}

int
be_visitor_union_discriminant_ch::visit_predefined_type (be_predefined_type
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
  *os << "void _d (" << bt->nested_type_name (bu) << ");" << be_nl;
  // the get method
  *os << bt->nested_type_name (bu) << " _d (void) const;\n\n";
  return 0;
}

int
be_visitor_union_discriminant_ch::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // save the typedef node for use in code generation
                           // as we visit the base type

  // the node to be visited in the base primitve type that gets typedefed
  be_type *bt = node->primitive_base_type ();
  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_discriminant_ch::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ), -1);
    }
  this->ctx_->alias (0);
  return 0;
}
