//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    union_branch_cdr_op_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Union_Branch in the client stubs file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_union_branch.h"

ACE_RCSID(be_visitor_union_branch, cdr_op_cs, "$Id$")


// **********************************************
//  visitor for union_branch in the client stubs file
// **********************************************

// constructor
be_visitor_union_branch_cdr_op_cs::be_visitor_union_branch_cdr_op_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

// destructor
be_visitor_union_branch_cdr_op_cs::~be_visitor_union_branch_cdr_op_cs (void)
{
}

// visit the union_branch node
int
be_visitor_union_branch_cdr_op_cs::visit_union_branch (be_union_branch *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_union_branch - "
                         "Bad union_branch type\n"
                         ), -1);
    }

  this->ctx_->node (node); // save the node
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_union_branch - "
                         "codegen for union_branch type failed\n"
                         ), -1);
    }
  return 0;
}

// visit array
int
be_visitor_union_branch_cdr_op_cs::visit_array (be_array *node)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the union_branch node
  be_union_branch *f = this->ctx_->be_node_as_union_branch ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_array - "
                         "cannot retrieve union_branch node\n"
                         ), -1);
    }

  // check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> _tao_union." << f->local_name () << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << _tao_union." << f->local_name () << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // proceed further
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_array - "
                         "bad sub state\n"
                         ), -1);
    }

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

      // first generate the  declaration
      ctx.state (TAO_CodeGen::TAO_ARRAY_CDR_OP_CS);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                             "visit_array - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
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
be_visitor_union_branch_cdr_op_cs::visit_enum (be_enum *node)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the union_branch node
  be_union_branch *f = this->ctx_->be_node_as_union_branch ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_array - "
                         "cannot retrieve union_branch node\n"
                         ), -1);
    }

  // check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> _tao_union." << f->local_name () << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << _tao_union." << f->local_name () << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // proceed further
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_enum - "
                         "bad sub state\n"
                         ), -1);
    }

  if (node->node_type () != AST_Decl::NT_typedef // not a typedef
      && node->is_child (this->ctx_->scope ())) // node is defined inside the
    // structure
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // generate the typcode for enums
      ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CS);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                             "visit_enum - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                             "visit_enum - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }
  return 0;
}

// visit interface type
int
be_visitor_union_branch_cdr_op_cs::visit_interface (be_interface *)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the union_branch node
  be_union_branch *f = this->ctx_->be_node_as_union_branch ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_interface - "
                         "cannot retrieve union_branch node\n"
                         ), -1);
    }

  // check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> _tao_union." << f->local_name () << ".out ())";
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << _tao_union." << f->local_name () << ".in ())";
      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // nothing to be done because an interface cannit be declared inside a
      // structure 
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_interface - "
                         "bad sub state\n"
                         ), -1);
    }
  return 0;
}

// visit interface forward type
int
be_visitor_union_branch_cdr_op_cs::visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the union_branch node
  be_union_branch *f = this->ctx_->be_node_as_union_branch ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_interface_fwd - "
                         "cannot retrieve union_branch node\n"
                         ), -1);
    }

  // check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> _tao_union." << f->local_name () << ").out ()";
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << _tao_union." << f->local_name () << ").in ()";
      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // nothing to be done because an interface cannit be declared inside a
      // structure 
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_interface_fwd - "
                         "bad sub state\n"
                         ), -1);
    }
  return 0;
}

// visit predefined type
int
be_visitor_union_branch_cdr_op_cs::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the union_branch node
  be_union_branch *f = this->ctx_->be_node_as_union_branch ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_predefined_type - "
                         "cannot retrieve union_branch node\n"
                         ), -1);
    }

  // check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      // is a psuedo obj
      if (node->pt () == AST_PredefinedType::PT_pseudo) 
        *os << "(strm >> _tao_union." << f->local_name () << ".out ())";
      else
        *os << "(strm >> _tao_union." << f->local_name () << ")";
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      // is a psuedo obj
      if (node->pt () == AST_PredefinedType::PT_pseudo) 
        *os << "(strm << _tao_union." << f->local_name () << ".in ())";
      else
        *os << "(strm << _tao_union." << f->local_name () << ")";
      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // nothing to be done
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_array - "
                         "bad sub state\n"
                         ), -1);
    }

  return 0;
}

int
be_visitor_union_branch_cdr_op_cs::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the union_branch node
  be_union_branch *f = this->ctx_->be_node_as_union_branch ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_sequence - "
                         "cannot retrieve union_branch node\n"
                         ), -1);
    }

  // check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> _tao_union." << f->local_name () << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << _tao_union." << f->local_name () << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // proceed further
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_sequence - "
                         "bad sub state\n"
                         ), -1);
    }

  if (node->node_type () != AST_Decl::NT_typedef 
      && node->is_child (this->ctx_->scope ())) 
    // not a typedef AND
    // node is defined inside the structure
    {
      // Anonymous sequence
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope is
      // still the same

      // generate the inline code for structs
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_CDR_OP_CS);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                             "visit_sequence - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
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
be_visitor_union_branch_cdr_op_cs::visit_string (be_string *)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the union_branch node
  be_union_branch *f = this->ctx_->be_node_as_union_branch ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_string - "
                         "cannot retrieve union_branch node\n"
                         ), -1);
    }

  // check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> _tao_union." << f->local_name () << ".out ())";
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << _tao_union." << f->local_name () << ".in ())";
      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // nothing to be done
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_array - "
                         "bad sub state\n"
                         ), -1);
    }

  return 0;
}

// visit structure type
int
be_visitor_union_branch_cdr_op_cs::visit_structure (be_structure *node)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the union_branch node
  be_union_branch *f = this->ctx_->be_node_as_union_branch ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_structure - "
                         "cannot retrieve union_branch node\n"
                         ), -1);
    }

  // check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> _tao_union." << f->local_name () << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << _tao_union." << f->local_name () << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // proceed further
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_structure - "
                         "bad sub state\n"
                         ), -1);
    }

  if (node->node_type () != AST_Decl::NT_typedef // not a typedef
      && node->is_child (this->ctx_->scope ())) // node is defined inside the
    // structure
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope is
      // still the same

      // generate the inline code for structs
      ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CS);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                             "visit_struct - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
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
be_visitor_union_branch_cdr_op_cs::visit_typedef (be_typedef *node)
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
be_visitor_union_branch_cdr_op_cs::visit_union (be_union *node)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the union_branch node
  be_union_branch *f = this->ctx_->be_node_as_union_branch ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_union - "
                         "cannot retrieve union_branch node\n"
                         ), -1);
    }

  // check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> _tao_union." << f->local_name () << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << _tao_union." << f->local_name () << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // proceed further
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                         "visit_union - "
                         "bad sub state\n"
                         ), -1);
    }

  if (node->node_type () != AST_Decl::NT_typedef // not a typedef
      && node->is_child (this->ctx_->scope ())) // node is defined inside the
    // structure
    {
      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope is
      // still the same

      // generate the inline code for union
      ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CS);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                             "visit_union - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_union_branch_cdr_op_cs::"
                             "visit_union - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }
  return 0;
}
