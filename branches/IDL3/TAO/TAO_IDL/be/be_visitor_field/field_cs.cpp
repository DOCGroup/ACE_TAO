//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    field_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Field in the client stubs file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "be_visitor_field.h"
#include "be_visitor_array/array_cs.h"
#include "be_visitor_enum/enum_cs.h"
#include "be_visitor_sequence/sequence_cs.h"
#include "be_visitor_structure/structure_cs.h"
#include "be_visitor_union/union_cs.h"

ACE_RCSID (be_visitor_field, 
           field_cs, 
           "$Id$")


// **********************************************
//  Visitor for field in the client stubs file.
// **********************************************

be_visitor_field_cs::be_visitor_field_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_field_cs::~be_visitor_field_cs (void)
{
}

int
be_visitor_field_cs::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cs::"
                         "visit_field - "
                         "Bad field type\n"), 
                        -1);
    }

  this->ctx_->node (node);

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cs::"
                         "visit_field - "
                         "codegen for field type failed\n"), 
                        -1);
    }

  return 0;
}

int
be_visitor_field_cs::visit_array (be_array *node)
{
  if (!this->ctx_->alias ()
      && node->is_child (this->ctx_->scope ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      ctx.state (TAO_CodeGen::TAO_ARRAY_CS);
      be_visitor_array_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_cs::"
                             "visit_array - "
                             "codegen failed\n"), 
                            -1);
        }
    }

  return 0;
}

int
be_visitor_field_cs::visit_enum (be_enum *node)
{
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      ctx.state (TAO_CodeGen::TAO_ENUM_CS);
      be_visitor_enum_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_cs::"
                             "visit_enum - "
                             "codegen failed\n"), 
                            -1);
        }
    }

  return 0;
}

int
be_visitor_field_cs::visit_sequence (be_sequence *node)
{
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_CS);
      be_visitor_structure_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_cs::"
                             "visit_sequence - "
                             "codegen failed\n"), 
                            -1);
        }
    }

  return 0;
}

int
be_visitor_field_cs::visit_structure (be_structure *node)
{
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      ctx.state (TAO_CodeGen::TAO_STRUCT_CS);
      be_visitor_structure_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_cs::"
                             "visit_struct - "
                             "codegen failed\n"), 
                            -1);
        }
    }

  return 0;
}

int
be_visitor_field_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  be_type *bt = node->primitive_base_type ();

  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_public_ci::"
                         "visit_typedef - "
                         "Bad primitive type\n"), 
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_field_cs::visit_union (be_union *node)
{
  if (node->node_type () != AST_Decl::NT_typedef
      && node->is_child (this->ctx_->scope ()))
    {
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node);
      ctx.state (TAO_CodeGen::TAO_UNION_CS);
      be_visitor_union_cs visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_cs::"
                             "visit_union - "
                             "codegen failed\n"), 
                            -1);
        }
    }

  return 0;
}
