
//=============================================================================
/**
 *  @file    serializer_op_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating TAO::DCPS::Serializer operator declarator for field
 *  in the client header
 *
 *
 *  @author Scott Harris <harris_s@ociweb.com> based on code by Aniruddha Gokhale
 */
//=============================================================================


#include "be_visitor_array/serializer_op_ch.h"
#include "be_visitor_enum/serializer_op_ch.h"
#include "be_visitor_sequence/serializer_op_ch.h"
#include "be_visitor_structure/structure.h"
#include "be_visitor_structure/serializer_op_ch.h"
#include "be_visitor_union/union.h"
#include "be_visitor_union/serializer_op_ch.h"

// **********************************************
//  Visitor for field in the client header file.
// **********************************************

// Constructor.
be_visitor_field_serializer_op_ch::be_visitor_field_serializer_op_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

// Destructor.
be_visitor_field_serializer_op_ch::~be_visitor_field_serializer_op_ch (void)
{
}

// Visit the field node.
int
be_visitor_field_serializer_op_ch::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_serializer_op_ch::"
                         "visit_field - "
                         "Bad field type\n"),
                        -1);
    }

  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_serializer_op_ch::"
                         "visit_field - "
                         "codegen for field type failed\n"),
                        -1);
    }

  return 0;
}

// Visit operations on all possible data types that a field can be.

int
be_visitor_field_serializer_op_ch::visit_array (be_array *node)
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
      be_visitor_array_serializer_op_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_serializer_op_ch::"
                             "visit_array - "
                             "codegen failed\n"),
                            -1);
        }
    }

  return 0;
}

int
be_visitor_field_serializer_op_ch::visit_enum (be_enum *node)
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
      be_visitor_enum_serializer_op_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_serializer_op_ch::"
                             "visit_enum - "
                             "codegen failed\n"
                             ), -1);
        }
    }

  return 0;
}

int
be_visitor_field_serializer_op_ch::visit_sequence (be_sequence *node)
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
      be_visitor_sequence_serializer_op_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_serializer_op_ch::"
                             "visit_sequence - "
                             "codegen failed\n"
                             ), -1);
        }
    }

  return 0;
}

int
be_visitor_field_serializer_op_ch::visit_structure (be_structure *node)
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
      be_visitor_structure_serializer_op_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_serializer_op_ch::"
                             "visit_struct - "
                             "codegen failed\n"
                             ), -1);
        }
    }

  return 0;
}

int
be_visitor_field_serializer_op_ch::visit_typedef (be_typedef *node)
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
                         "(%N:%l) be_visitor_field_serializer_op_ch::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ), -1);
    }

  this->ctx_->alias (0);
  return 0;
}

// visit union type
int
be_visitor_field_serializer_op_ch::visit_union (be_union *node)
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
      be_visitor_union_serializer_op_ch visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_serializer_op_ch::"
                             "visit_enum - "
                             "codegen failed\n"
                             ), -1);
        }
    }

  return 0;
}
