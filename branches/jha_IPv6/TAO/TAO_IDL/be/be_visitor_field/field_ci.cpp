//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    field_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Field in the client inline file
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_field.h"

ACE_RCSID(be_visitor_field, field_ci, "$Id$")


// **********************************************
//  visitor for field in the client inline file.
//
//  Generate inline code for types that are defined
//  inside the structure
// **********************************************

// constructor
be_visitor_field_ci::be_visitor_field_ci (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

// destructor
be_visitor_field_ci::~be_visitor_field_ci (void)
{
}

// visit the field node
int
be_visitor_field_ci::visit_field (be_field *node)
{
  be_type *bt =
    be_type::narrow_from_decl (node->field_type ());
  // first generate the type information
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_ci::"
                         "visit_field - "
                         "Bad field type\n"
                         ), -1);
    }

  this->ctx_->node (node); // save the node
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_ci::"
                         "visit_field - "
                         "codegen for field type failed\n"
                         ), -1);
    }
  return 0;
}

// =visit operations on all possible data types  that a field can be

// visit array
int
be_visitor_field_ci::visit_array (be_array *node)
{
  // if not a typedef and we are defined in the use scope, we must be defined

  if (!this->ctx_->alias () // not a typedef
      && node->is_child (this->ctx_->scope ()))
    {
      // this is the case for anonymous arrays.

      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the struct declaration
      ctx.state (TAO_CodeGen::TAO_ARRAY_CI);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ci::"
                             "visit_array - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ci::"
                             "visit_array - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }
  return 0;
}

// visit sequence
int
be_visitor_field_ci::visit_sequence (be_sequence *node)
{
  if (node->node_type () != AST_Decl::NT_typedef // not a typedef
      && node->is_child (this->ctx_->scope ())) // node is defined inside the
    // structure
    {
      // anonymous sequence

      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope is
      // still the same

      // generate the inline code for structs
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_CI);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ci::"
                             "visit_sequence - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ci::"
                             "visit_sequence - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }
  return 0;
}

// visit structure type
int
be_visitor_field_ci::visit_structure (be_structure *node)
{
  if (node->node_type () != AST_Decl::NT_typedef // not a typedef
      && node->is_child (this->ctx_->scope ())) // node is defined inside the
                                                // structure
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // generate the inline code for structs
      ctx.state (TAO_CodeGen::TAO_STRUCT_CI);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ci::"
                             "visit_struct - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ci::"
                             "visit_struct - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }
  return 0;
}

// visit typedef type
int
be_visitor_field_ci::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // save the typedef node for use in code generation
                           // as we visit the base type

  // the node to be visited in the base primitve type that gets typedefed
  be_type *bt = node->primitive_base_type ();
  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ), -1);
    }

  this->ctx_->alias (0);
  return 0;
}

// visit union type
int
be_visitor_field_ci::visit_union (be_union *node)
{
  if (node->node_type () != AST_Decl::NT_typedef // not a typedef
      && node->is_child (this->ctx_->scope ())) // node is defined inside the
                                                // structure
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // generate the inline code for union
      ctx.state (TAO_CodeGen::TAO_UNION_CI);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ci::"
                             "visit_union - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ci::"
                             "visit_union - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }
  return 0;
}
