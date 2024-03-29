
//=============================================================================
/**
 *  @file    cdr_op_ch.cpp
 *
 *  Visitor generating CDR operator declarator for field in the client header
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "field.h"
#include "be_visitor_array/cdr_op_ch.h"
#include "be_visitor_enum/cdr_op_ch.h"
#include "be_visitor_sequence/cdr_op_ch.h"
#include "be_visitor_structure/structure.h"
#include "be_visitor_structure/cdr_op_ch.h"
#include "be_visitor_map/cdr_op_ch.h"
#include "be_visitor_union/union.h"
#include "be_visitor_union/cdr_op_ch.h"

// Constructor.
be_visitor_field_cdr_op_ch::be_visitor_field_cdr_op_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

// Destructor.
be_visitor_field_cdr_op_ch::~be_visitor_field_cdr_op_ch ()
{
}

// Visit the field node.
int
be_visitor_field_cdr_op_ch::visit_field (be_field *node)
{
  be_type *bt = dynamic_cast<be_type*> (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_ch::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_ch::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

// Visit operations on all possible data types that a field can be.

int
be_visitor_field_cdr_op_ch::visit_array (be_array *node)
{
  // If not a typedef and we are defined in the use scope, we must be defined.

  if (!this->ctx_->alias ()
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      // This is the case for anonymous arrays.

      // Instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);

      // First generate the array declaration.
      be_visitor_array_cdr_op_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_cdr_op_ch::"
                             "visit_array - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_field_cdr_op_ch::visit_enum (be_enum *node)
{
  // If not a typedef and we are defined in the use scope, we must be defined
  if (!this->ctx_->alias () // not a typedef.
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      // Instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);

      // First generate the enum declaration.
      be_visitor_enum_cdr_op_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_cdr_op_ch::"
                             "visit_enum - "
                             "codegen failed\n"
                             ), -1);
        }
    }

  return 0;
}

int
be_visitor_field_cdr_op_ch::visit_sequence (be_sequence *node)
{
  // If not a typedef and we are defined in the use scope, we must be defined.
  if (!this->ctx_->alias () // not a typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      // Instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);

      // First generate the sequence declaration.
      be_visitor_sequence_cdr_op_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_cdr_op_ch::"
                             "visit_sequence - "
                             "codegen failed\n"
                             ), -1);
        }
    }

  return 0;
}

int
be_visitor_field_cdr_op_ch::visit_map (be_map *node)
{
  // If not a typedef and we are defined in the use scope, we must be defined.
  if (!this->ctx_->alias () // not a typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      // Instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);

      // First generate the map declaration.
      be_visitor_map_cdr_op_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_cdr_op_ch::"
                             "visit_map - "
                             "codegen failed\n"
                             ), -1);
        }
    }

  return 0;
}

int
be_visitor_field_cdr_op_ch::visit_structure (be_structure *node)
{
  // if not a typedef and we are defined in the use scope, we must be defined

  if (!this->ctx_->alias () // not a typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the struct declaration
      be_visitor_structure_cdr_op_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_cdr_op_ch::"
                             "visit_struct - "
                             "codegen failed\n"
                             ), -1);
        }
    }

  return 0;
}

int
be_visitor_field_cdr_op_ch::visit_structure_fwd (
  be_structure_fwd *node)
{
  be_structure *s =
    dynamic_cast<be_structure*> (node->full_definition ());

  return this->visit_structure (s);
}

int
be_visitor_field_cdr_op_ch::visit_typedef (be_typedef *node)
{
  // Save the node for use in code generation and
  // indicate that the field of the field node
  // is a typedefed quantity.
  this->ctx_->alias (node);

  // Make a decision based on the primitive base type.
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cdr_op_ch::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ), -1);
    }

  this->ctx_->alias (nullptr);
  return 0;
}

// visit union type
int
be_visitor_field_cdr_op_ch::visit_union (be_union *node)
{
  // If not a typedef and we are defined in the use scope, we must be defined.
  if (!this->ctx_->alias () // not a typedef
      && node->is_child (this->ctx_->scope ()->decl ()))
    {
      // Instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting.
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);

      // First generate the enum declaration.
      be_visitor_union_cdr_op_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_cdr_op_ch::"
                             "visit_enum - "
                             "codegen failed\n"
                             ), -1);
        }
    }

  return 0;
}

int
be_visitor_field_cdr_op_ch::visit_union_fwd (be_union_fwd *node)
{
  be_union *u =
    dynamic_cast<be_union*> (node->full_definition ());

  return this->visit_union (u);
}

