//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    public_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Union Branch in the client stubs
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union_branch.h"

ACE_RCSID(be_visitor_union_branch, public_cs, "$Id$")


// **********************************************
//  visitor for union_branch in the client stubs file
// **********************************************

// constructor
be_visitor_union_branch_public_cs::be_visitor_union_branch_public_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

// destructor
be_visitor_union_branch_public_cs::~be_visitor_union_branch_public_cs (void)
{
}

// visit the union_branch node
int
be_visitor_union_branch_public_cs::visit_union_branch (be_union_branch *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"
                         ), -1);
    }

  this->ctx_->node (node); // save the node
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"
                         ), -1);
    }
  return 0;
}

// visit array type
int
be_visitor_union_branch_public_cs::visit_array (be_array *node)
{
  // if not a typedef and we are defined in the use scope, we must be
  // defined

  if (!this->ctx_->alias () // not a typedef
      && node->is_child (this->ctx_->scope ()))
    {
      // anonymous array case

      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the inline operations for this anonymous array type
      ctx.state (TAO_CodeGen::TAO_ARRAY_CS);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_cs::"
                             "visit_array - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_cs::"
                             "visit_array - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  return 0;
}

// visit enum type
int
be_visitor_union_branch_public_cs::visit_enum (be_enum *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // generate the typcode for enums
  ctx.state (TAO_CodeGen::TAO_ENUM_CS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_enum - "
                         "Bad visitor\n"
                         ), -1);
    }
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_enum - "
                         "codegen failed\n"
                         ), -1);
    }
  delete visitor;
  return 0;
}

// visit sequence type
int
be_visitor_union_branch_public_cs::visit_sequence (be_sequence *node)
{
  // if not a typedef and we are defined in the use scope, we must be
  // defined

  if (!this->ctx_->alias () // not a typedef
      && node->is_child (this->ctx_->scope ()))
    {
      // anonymous array case

      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the inline operations for this anonymous sequence type
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_CS);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_cs::"
                             "visit_sequence - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_public_cs::"
                             "visit_sequence - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  return 0;
}

// visit string type
int
be_visitor_union_branch_public_cs::visit_string (be_string *)
{
  return 0;
}

// visit structure type
int
be_visitor_union_branch_public_cs::visit_structure (be_structure *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // generate the inline code for structs
  ctx.state (TAO_CodeGen::TAO_STRUCT_CS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_struct - "
                         "Bad visitor\n"
                         ), -1);
    }
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_struct - "
                         "codegen failed\n"
                         ), -1);
    }
  delete visitor;
  return 0;
}

// visit union type
int
be_visitor_union_branch_public_cs::visit_union (be_union *node)
{
  // instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node); // set the node to be the node being visited. The scope is
                   // still the same

  // generate the inline code for union
  ctx.state (TAO_CodeGen::TAO_UNION_CS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_union - "
                         "Bad visitor\n"
                         ), -1);
    }
  if (node->accept (visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_cs::"
                         "visit_union - "
                         "codegen failed\n"
                         ), -1);
    }
  delete visitor;
  return 0;
}
