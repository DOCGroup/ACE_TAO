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
//     Visitor for the Valuetype class.
//     This one generates code for accessor and modifier functions of
//     valuetype state members (in the inline file).
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_valuetype.h"

ACE_RCSID(be_visitor_valuetype, field_ci, "$Id$")


// constructor
be_visitor_valuetype_field_ci::
be_visitor_valuetype_field_ci (be_visitor_context *ctx)
  : be_visitor_decl (ctx),
    in_obv_space_ (0)
{
  setenclosings ("");
}

// destructor
be_visitor_valuetype_field_ci::
~be_visitor_valuetype_field_ci (void)
{
}

// visit the field node
int
be_visitor_valuetype_field_ci::
visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cs::"
                         "visit_field - "
                         "Bad field type\n"
                         ), -1);
    }

  this->ctx_->node (node); // save the node
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_cs::"
                         "visit_field - "
                         "codegen for field type failed\n"
                         ), -1);
    }
  return 0;
}

// =visit operations on all possible data types that a field can be

int
be_visitor_valuetype_field_ci::visit_array (be_array *node)
{
  be_decl *ub =
    this->ctx_->node (); // get field node
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ());
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ci::"
                         "visit_array - "
                         "bad context information\n"
                         ), -1);
    }

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the valuetype
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the inline operations for this anonymous array type
      ctx.state (TAO_CodeGen::TAO_ARRAY_CI);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ci::"
                             "visit_array - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ci::"
                             "visit_array - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  return 0;
}

int
be_visitor_valuetype_field_ci::visit_enum (be_enum *)
{
  return 0;
}

int
be_visitor_valuetype_field_ci::visit_interface (be_interface *)
{
  return 0;
}

int
be_visitor_valuetype_field_ci::visit_interface_fwd (be_interface_fwd *)
{
  return 0;
}

int
be_visitor_valuetype_field_ci::visit_valuetype (be_valuetype *)
{
 return 0;
}

int
be_visitor_valuetype_field_ci::visit_valuetype_fwd (be_valuetype_fwd *)
{
  return 0;
}

int
be_visitor_valuetype_field_ci::visit_predefined_type (be_predefined_type *)
{
  return 0;
}

int
be_visitor_valuetype_field_ci::visit_sequence (be_sequence *node)
{
  be_decl *ub =
    this->ctx_->node (); // get field node
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ());
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ci::"
                         "visit_sequence - "
                         "bad context information\n"
                         ), -1);
    }

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the inline operations for this anonymous sequence type
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_CI);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ci::"
                             "visit_sequence - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ci::"
                             "visit_sequence - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  return 0;
}

int
be_visitor_valuetype_field_ci::visit_string (be_string *)
{
  return 0;
}

int
be_visitor_valuetype_field_ci::visit_structure (be_structure *node)
{
  be_decl *ub =
    this->ctx_->node (); // get field node
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ());
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ci::"
                         "visit_structure - "
                         "bad context information\n"
                         ), -1);
    }

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the enum declaration
      ctx.state (TAO_CodeGen::TAO_STRUCT_CI);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ci::"
                             "visit_structure - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ci::"
                             "visit_structure - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  return 0;
}

int
be_visitor_valuetype_field_ci::visit_typedef (be_typedef *)
{
  return 0;
}

int
be_visitor_valuetype_field_ci::visit_union (be_union *node)
{
  be_decl *ub =
    this->ctx_->node (); // get field node
  be_valuetype *bu =
    be_valuetype::narrow_from_decl (this->ctx_->scope ());
  be_type *bt;

  // check if we are visiting this node via a visit to a typedef node
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  if (!ub || !bu)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_ci::"
                         "visit_union - "
                         "bad context information\n"
                         ), -1);
    }

  if (bt->node_type () != AST_Decl::NT_typedef // not a typedef
      && bt->is_child (bu)) // bt is defined inside the union
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the enum declaration
      ctx.state (TAO_CodeGen::TAO_UNION_CI);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ci::"
                             "visit_union - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_ci::"
                             "visit_union - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  return 0;
}

void
be_visitor_valuetype_field_ci::setenclosings (const char *pre)
{
  pre_op_ = pre;
}

const char*
be_visitor_valuetype_field_ci::pre_op ()
{
  return pre_op_;
}

// retrieve the fully scoped skeleton name
void
be_visitor_valuetype_field_ci::op_name (be_valuetype *node,
                                        TAO_OutStream *os)
{
  if (this->in_obv_space_)
    *os << node->full_obv_skel_name ();
  else
    *os << node->name ();
}
