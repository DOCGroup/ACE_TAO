
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
//    Visitor generating CDR operator declarator for union_branch in the client header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union_branch.h"

ACE_RCSID(be_visitor_union_branch, cdr_op_ch, "$Id$")


// **********************************************
//  visitor for union_branch in the client header file
// **********************************************

// constructor
be_visitor_union_branch_cdr_op_ch::be_visitor_union_branch_cdr_op_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

// destructor
be_visitor_union_branch_cdr_op_ch::~be_visitor_union_branch_cdr_op_ch (void)
{
}

// visit the union_branch node
int
be_visitor_union_branch_cdr_op_ch::visit_union_branch (be_union_branch *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_ch::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"
                         ), -1);
    }

  this->ctx_->node (node); // save the node
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_ch::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"
                         ), -1);
    }

  return 0;
}

// =visit operations on all possible data types  that a union_branch can be

// visit array type
int
be_visitor_union_branch_cdr_op_ch::visit_array (be_array *node)
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
      ctx.state (TAO_CodeGen::TAO_ARRAY_CDR_OP_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_ch::"
                             "visit_array - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_ch::"
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
be_visitor_union_branch_cdr_op_ch::visit_enum (be_enum *node)
{
  // if not a typedef and we are defined in the use scope, we must be defined
  if (!this->ctx_->alias () // not a typedef
      && node->is_child (this->ctx_->scope ()))
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the enum declaration
      ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_ch::"
                             "visit_enum - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_ch::"
                             "visit_enum - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  return 0;
}

// visit sequence type
int
be_visitor_union_branch_cdr_op_ch::visit_sequence (be_sequence *node)
{
  // if not a typedef and we are defined in the use scope, we must be defined
  if (!this->ctx_->alias () // not a typedef
      && node->is_child (this->ctx_->scope ()))
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the sequence declaration
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_CDR_OP_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_ch::"
                             "visit_sequence - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_ch::"
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
be_visitor_union_branch_cdr_op_ch::visit_structure (be_structure *node)
{
  // if not a typedef and we are defined in the use scope, we must be defined

  if (!this->ctx_->alias () // not a typedef
      && node->is_child (this->ctx_->scope ()))
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the struct declaration
      ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_ch::"
                             "visit_struct - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_ch::"
                             "visit_struct - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  return 0;
}

// visit typedefed type
int
be_visitor_union_branch_cdr_op_ch::visit_typedef (be_typedef *node)
{
  // save the node for use in code generation and
  // indicate that the union_branch of the union_branch node
  // is a typedefed quantity  
  this->ctx_->alias (node);    

  // make a decision based on the primitive base type
  be_type *bt = node->primitive_base_type ();
  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_ch::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ), -1);
    }
  this->ctx_->alias (0);
  return 0;
}

// visit union type
int
be_visitor_union_branch_cdr_op_ch::visit_union (be_union *node)
{
  // if not a typedef and we are defined in the use scope, we must be defined
  if (!this->ctx_->alias () // not a typedef
      && node->is_child (this->ctx_->scope ()))
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the enum declaration
      ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_ch::"
                             "visit_enum - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_ch::"
                             "visit_enum - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  return 0;
}
