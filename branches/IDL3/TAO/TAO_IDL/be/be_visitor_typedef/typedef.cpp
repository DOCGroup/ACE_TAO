//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    typedef.cpp
//
// = DESCRIPTION
//    Generic visitor generating code for Typedefs
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "be.h"
#include "be_visitor_typedef.h"
#include "be_visitor_array.h"
#include "be_visitor_enum.h"
#include "be_visitor_sequence.h"
#include "be_visitor_structure.h"
#include "be_visitor_union.h"

ACE_RCSID (be_visitor_typedef, 
           typedef, 
           "$Id$")


// ******************************************************
// Generic typedef visitor.
// ******************************************************

be_visitor_typedef::be_visitor_typedef (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_typedef::~be_visitor_typedef (void)
{
}

int be_visitor_typedef::visit_typedef (be_typedef *)
{
  return -1; // Must be overridden.
}

// All common visit methods for typedef visitor.

int
be_visitor_typedef::visit_array (be_array *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_TYPEDEF_CH:
      {
        ctx.state (TAO_CodeGen::TAO_ARRAY_CH);
        be_visitor_array_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CS:
      {
        ctx.state (TAO_CodeGen::TAO_ARRAY_CI);
        be_visitor_array_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CI:
      {
        ctx.state (TAO_CodeGen::TAO_ARRAY_CS);
        be_visitor_array_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_ARRAY_ANY_OP_CH);
        be_visitor_array_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_ARRAY_ANY_OP_CS);
        be_visitor_array_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_ARRAY_CDR_OP_CH);
        be_visitor_array_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_ARRAY_CDR_OP_CI);
        be_visitor_array_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_ARRAY_CDR_OP_CS);
        be_visitor_array_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typedef::"
                           "visit_array - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typedef::"
                         "visit_array - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_typedef::visit_enum (be_enum *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_TYPEDEF_CH:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_CH);
        be_visitor_enum_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CS:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_CS);
        be_visitor_enum_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_ANY_OP_CH);
        be_visitor_enum_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_ANY_OP_CS);
        be_visitor_enum_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CH);
        be_visitor_enum_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CI);
        be_visitor_enum_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CS:
    case TAO_CodeGen::TAO_TYPEDEF_CI:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typedef::"
                           "visit_enum - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typedef::"
                         "visit_enum - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_typedef::visit_predefined_type (be_predefined_type *)
{
  return 0;
}

int
be_visitor_typedef::visit_sequence (be_sequence *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_TYPEDEF_CH:
      {
        ctx.state (TAO_CodeGen::TAO_SEQUENCE_CH);
        be_visitor_sequence_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CI:
      {
        ctx.state (TAO_CodeGen::TAO_SEQUENCE_CI);
        be_visitor_sequence_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CS:
      {
        ctx.state (TAO_CodeGen::TAO_SEQUENCE_CS);
        be_visitor_sequence_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_SEQUENCE_ANY_OP_CH);
        be_visitor_sequence_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_SEQUENCE_ANY_OP_CS);
        be_visitor_sequence_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_SEQUENCE_CDR_OP_CH);
        be_visitor_sequence_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_SEQUENCE_CDR_OP_CI);
        be_visitor_sequence_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_SEQUENCE_CDR_OP_CS);
        be_visitor_sequence_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typedef::"
                           "visit_sequence - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typedef::"
                         "visit_sequence - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_typedef::visit_string (be_string *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_TYPEDEF_CH:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
        be_visitor_structure_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CI:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CI);
        be_visitor_structure_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CS:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CS);
        be_visitor_structure_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH:
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS:
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CH:
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CI:
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CS:
      return 0; // nothing to be done
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typedef::"
                           "visit_string - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typedef::"
                         "visit_string - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

// visit an structure
int
be_visitor_typedef::visit_structure (be_structure *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_TYPEDEF_CH:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
        be_visitor_structure_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CI:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CI);
        be_visitor_structure_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CS:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CS);
        be_visitor_structure_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CH);
        be_visitor_structure_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_ANY_OP_CS);
        be_visitor_structure_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CH);
        be_visitor_structure_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CI);
        be_visitor_structure_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CS);
        be_visitor_structure_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typedef::"
                           "visit_structure - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typedef::"
                         "visit_structure - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}

int
be_visitor_typedef::visit_union (be_union *node)
{
  // Instantiate a visitor context with a copy of our context. This info
  // will be modified based on what type of node we are visiting.
  be_visitor_context ctx (*this->ctx_);
  ctx.node (node);
  int status = 0;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_TYPEDEF_CH:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CH);
        be_visitor_union_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CI:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CI);
        be_visitor_union_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CS:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CS);
        be_visitor_union_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_ANY_OP_CH);
        be_visitor_union_any_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_ANY_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_ANY_OP_CS);
        be_visitor_union_any_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CH:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CH);
        be_visitor_union_cdr_op_ch visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CI:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CI);
        be_visitor_union_cdr_op_ci visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    case TAO_CodeGen::TAO_TYPEDEF_CDR_OP_CS:
      {
        ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CS);
        be_visitor_union_cdr_op_cs visitor (&ctx);
        status = node->accept (&visitor);
        break;
      }
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_typedef::"
                           "visit_union - "
                           "Bad context state\n"), 
                          -1);
      }
    }

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_typedef::"
                         "visit_union - "
                         "failed to accept visitor\n"),  
                        -1);
    }

  return 0;
}
