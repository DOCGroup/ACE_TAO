//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    field_cdr_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Field in the client stubs file.
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>
//    derived from be_visitor_field/cdr_op_ci.cpp
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_field.h"

ACE_RCSID(be_visitor_valuetype, field_cdr_op_ci, "$Id$")


// **********************************************
//  visitor for field in the client stubs file
// **********************************************

// constructor
be_visitor_valuetype_field_cdr_ci::be_visitor_valuetype_field_cdr_ci (be_visitor_context *ctx)
  : be_visitor_decl (ctx),
    pre_ (""), post_ ("")
{
}

// destructor
be_visitor_valuetype_field_cdr_ci::~be_visitor_valuetype_field_cdr_ci (void)
{
}

// visit the field node
int
be_visitor_valuetype_field_cdr_ci::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_field - "
                         "Bad field type\n"
                         ), -1);
    }

  this->ctx_->node (node); // save the node
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_field - "
                         "codegen for field type failed\n"
                         ), -1);
    }
  return 0;
}

// visit array
int
be_visitor_valuetype_field_cdr_ci::visit_array (be_array *node)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the field node
  be_field *f = this->ctx_->be_node_as_field ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_array - "
                         "cannot retrieve field node\n"
                         ), -1);
    }

  // for anonymous arrays, the type name has a _ prepended. We compute
  // the full_name with or without the underscore and use it later on.
  char fname [NAMEBUFSIZE];  // to hold the full and

  // save the node's local name and full name in a buffer for quick
  // use later on
  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  if (!this->ctx_->alias () // not a typedef
      && node->is_child (this->ctx_->scope ()))
    {
      // for anonymous arrays ...
      // we have to generate a name for us that has an underscore
      // prepended to our local name. This needs to be inserted after
      // the parents's name

      if (node->is_nested ())
        {
          be_decl *parent =
            be_scope::narrow_from_scope (node->defined_in ())->decl ();
          ACE_OS::sprintf (fname, "%s::_%s", parent->full_name (),
                           node->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (fname, "_%s", node->full_name ());
        }
    }
  else
    {
      // typedefed node
      ACE_OS::sprintf (fname, "%s", node->full_name ());
    }

  // check what is the code generation substate. Are we generating
  // code for the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> "
          << "_tao_" << pre_ << f->local_name () << post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << "
          << "_tao_" << pre_ << f->local_name () << post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // proceed further
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_array - "
                         "bad sub state\n"
                         ), -1);
    }

  // if not a typedef and we are defined in the use scope, we must be defined

  if (!this->ctx_->alias () // not a typedef
      && node->is_child (this->ctx_->scope ()))
    {
      // this is the case for anonymous arrays. Generate the <<, >> operators
      // for the type defined by the anonymous array

      // instantiate a visitor context with a copy of our context. This info
      // will be modified based on what type of node we are visiting
      be_visitor_context ctx (*this->ctx_);
      ctx.node (node); // set the node to be the node being visited. The scope
                       // is still the same

      // first generate the  declaration
      ctx.state (TAO_CodeGen::TAO_ARRAY_CDR_OP_CI);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                             "visit_array - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
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
be_visitor_valuetype_field_cdr_ci::visit_enum (be_enum *node)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the field node
  be_field *f = this->ctx_->be_node_as_field ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_array - "
                         "cannot retrieve field node\n"
                         ), -1);
    }

  // check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> " << pre_ << f->local_name () << post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << " << pre_ << f->local_name () << post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // proceed further
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
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
      ctx.state (TAO_CodeGen::TAO_ENUM_CDR_OP_CI);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                             "visit_enum - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
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
be_visitor_valuetype_field_cdr_ci::visit_interface (be_interface *)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the field node
  be_field *f = this->ctx_->be_node_as_field ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_interface - "
                         "cannot retrieve field node\n"
                         ), -1);
    }

  // check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> " << pre_ << f->local_name () << post_ << ".out ())";
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << " << pre_ << f->local_name () << post_ << ".in ())";
      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // nothing to be done because an interface cannit be declared inside a
      // structure
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_interface - "
                         "bad sub state\n"
                         ), -1);
    }
  return 0;
}

// visit interface forward type
int
be_visitor_valuetype_field_cdr_ci::visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the field node
  be_field *f = this->ctx_->be_node_as_field ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_interface_fwd - "
                         "cannot retrieve field node\n"
                         ), -1);
    }

  // check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> " << pre_ << f->local_name () << post_ << ").out ()";
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << " << pre_ << f->local_name () << post_ << ").in ()";
      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // nothing to be done because an interface cannit be declared inside a
      // structure
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_interface_fwd - "
                         "bad sub state\n"
                         ), -1);
    }
  return 0;
}

// visit valuetype type
int
be_visitor_valuetype_field_cdr_ci::visit_valuetype (be_valuetype *)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the field node
  be_field *f = this->ctx_->be_node_as_field ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_valuetype - "
                         "cannot retrieve field node\n"
                         ), -1);
    }

  // check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> " << pre_ << f->local_name () << post_ << ".out ())";
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << " << pre_ << f->local_name () << post_ << ".in ())";
      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // nothing to be done because an valuetype cannit be declared inside a
      // structure
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_valuetype - "
                         "bad sub state\n"
                         ), -1);
    }
  return 0;
}

// visit valuetype forward type
int
be_visitor_valuetype_field_cdr_ci::visit_valuetype_fwd (be_valuetype_fwd *)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the field node
  be_field *f = this->ctx_->be_node_as_field ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_valuetype_fwd - "
                         "cannot retrieve field node\n"
                         ), -1);
    }

  // check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> " << pre_ << f->local_name () << post_ << ").out ()";
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << " << pre_ << f->local_name () << post_ << ").in ()";
      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // nothing to be done because an valuetype cannot be declared inside a
      // structure
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_valuetype_fwd - "
                         "bad sub state\n"
                         ), -1);
    }
  return 0;
}

// visit predefined type
int
be_visitor_valuetype_field_cdr_ci::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the field node
  be_field *f = this->ctx_->be_node_as_field ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_predefined_type - "
                         "cannot retrieve field node\n"
                         ), -1);
    }

  // check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      // is a psuedo obj
      if (node->pt () == AST_PredefinedType::PT_pseudo)
        *os << "(strm >> " << pre_ << f->local_name () << post_
            << ".out ())";
      else if (node->pt () == AST_PredefinedType::PT_char)
        *os << "(strm >> CORBA::Any::to_char (" << pre_
            << f->local_name () << post_ << "))";
      else if (node->pt () == AST_PredefinedType::PT_wchar)
        *os << "(strm >> CORBA::Any::to_wchar (" << pre_
            << f->local_name () << post_ << "))";
      else if (node->pt () == AST_PredefinedType::PT_octet)
        *os << "(strm >> CORBA::Any::to_octet (" << pre_
            << f->local_name () << post_ << "))";
      else if (node->pt () == AST_PredefinedType::PT_boolean)
        *os << "(strm >> CORBA::Any::to_boolean (" << pre_
            << f->local_name () << post_ << "))";
      else
        *os << "(strm >> " << pre_ << f->local_name () << post_ << ")";
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      // is a psuedo obj
      if (node->pt () == AST_PredefinedType::PT_pseudo)
        *os << "(strm << " << pre_ << f->local_name () << post_ << ".in ())";
      else if (node->pt () == AST_PredefinedType::PT_char)
        *os << "(strm << CORBA::Any::from_char (" << pre_
            << f->local_name () << post_ << "))";
      else if (node->pt () == AST_PredefinedType::PT_wchar)
        *os << "(strm << CORBA::Any::from_wchar (" << pre_
            << f->local_name () << post_ << "))";
      else if (node->pt () == AST_PredefinedType::PT_octet)
        *os << "(strm << CORBA::Any::from_octet (" << pre_
            << f->local_name () << post_ << "))";
      else if (node->pt () == AST_PredefinedType::PT_boolean)
        *os << "(strm << CORBA::Any::from_boolean (" << pre_
            << f->local_name () << post_ << "))";
      else
        *os << "(strm << " << pre_ << f->local_name () << post_ << ")";
      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // nothing to be done
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_array - "
                         "bad sub state\n"
                         ), -1);
    }

  return 0;
}

int
be_visitor_valuetype_field_cdr_ci::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the field node
  be_field *f = this->ctx_->be_node_as_field ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_sequence - "
                         "cannot retrieve field node\n"
                         ), -1);
    }

  // check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> " << pre_ << f->local_name () << post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << " << pre_ << f->local_name () << post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // proceed further
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
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
      ctx.node (node); // set the node to be the node being visited.
      // The scope is still the same

      // generate the inline code for structs
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_CDR_OP_CI);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                             "visit_sequence - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
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
be_visitor_valuetype_field_cdr_ci::visit_string (be_string *)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the field node
  be_field *f = this->ctx_->be_node_as_field ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_string - "
                         "cannot retrieve field node\n"
                         ), -1);
    }

  // check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> " << pre_ << f->local_name () << post_ << ".out ())";
      break;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << " << pre_ << f->local_name () << post_ << ".in ())";
      break;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // nothing to be done
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_array - "
                         "bad sub state\n"
                         ), -1);
    }

  return 0;
}

// visit structure type
int
be_visitor_valuetype_field_cdr_ci::visit_structure (be_structure *node)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the field node
  be_field *f = this->ctx_->be_node_as_field ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_structure - "
                         "cannot retrieve field node\n"
                         ), -1);
    }

  // check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> " << pre_ << f->local_name () << post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << " << pre_ << f->local_name () << post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // proceed further
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
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
      ctx.state (TAO_CodeGen::TAO_STRUCT_CDR_OP_CI);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                             "visit_struct - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
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
be_visitor_valuetype_field_cdr_ci::visit_typedef (be_typedef *node)
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
be_visitor_valuetype_field_cdr_ci::visit_union (be_union *node)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the field node
  be_field *f = this->ctx_->be_node_as_field ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                         "visit_union - "
                         "cannot retrieve field node\n"
                         ), -1);
    }

  // check what is the code generations substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
      *os << "(strm >> " << pre_ << f->local_name () << post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << "(strm << " << pre_ << f->local_name () << post_ << ")";
      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
      // proceed further
      break;
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
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
      ctx.state (TAO_CodeGen::TAO_UNION_CDR_OP_CI);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                             "visit_union - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_valuetype_field_cdr_ci::"
                             "visit_union - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }
  return 0;
}


// ****************************************************************

be_visitor_valuetype_field_cdr_decl::
    be_visitor_valuetype_field_cdr_decl (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

// This is a field, but the action depends on the type of the field,
// use this visitor to detect the type of the field.
// Notice that this is why the parent visitor (who create us) cannot
// do the job, because it may have another purpose for some or all of
// the visit_* methods; in other words, while running a visitor to
// generate CDR operators for structures we cannot use that one to
// generate the code of each field, because visit_struct already has a
// meaning in that visitor.
int
be_visitor_valuetype_field_cdr_decl::visit_field (be_field *node)
{
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_decl::"
                         "visit_field - "
                         "Bad field type\n"
                         ), -1);
    }

  // @@ Shouldn't this be saved in the visitor and not the context?!
  this->ctx_->node (node); // save the node
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_decl::"
                         "visit_field - "
                         "codegen for field type failed\n"
                         ), -1);
    }
  return 0;
}

// visit array
int
be_visitor_valuetype_field_cdr_decl::visit_array (be_array *node)
{
  TAO_OutStream *os; // output stream
  os = this->ctx_->stream ();

  // retrieve the field node
  be_field *f = this->ctx_->be_node_as_field ();
  if (!f)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_decl::"
                         "visit_array - "
                         "cannot retrieve field node\n"
                         ), -1);
    }

  // retrieve the valuetype scope in which the code is generated
  be_decl *sc = this->ctx_->scope ();
  be_valuetype *vt = be_valuetype::narrow_from_decl (sc);
  if (!vt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_decl::"
                         "visit_array - "
                         "cannot retrieve valuetype node\n"
                         ), -1);
    }

  // for anonymous arrays, the type name has a _ prepended. We compute
  // the full_name with or without the underscore and use it later on.
  char fname [NAMEBUFSIZE];  // to hold the full and

  ACE_OS::memset (fname, '\0', NAMEBUFSIZE);
  if (!this->ctx_->alias () // not a typedef
      && node->is_child (this->ctx_->scope ()))
    {
      // for anonymous arrays ...
      // we have to generate a name for us that has an underscope
      // prepended to our local name. This needs to be inserted after
      // the parents's name

      if (node->is_nested ())
        {
          be_decl *parent = be_scope::narrow_from_scope (node->defined_in ())->decl ();
          ACE_OS::sprintf (fname, "%s::_%s", parent->full_name (),
                           node->local_name ()->get_string ());
        }
      else
        {
          ACE_OS::sprintf (fname, "_%s", node->full_name ());
        }
    }
  else
    {
      // typedefed node
      ACE_OS::sprintf (fname, "%s", node->full_name ());
    }

  // check what is the code generation substate. Are we generating code for
  // the in/out operators for our parent or for us?
  switch (this->ctx_->sub_state ())
    {
    case TAO_CodeGen::TAO_CDR_INPUT:
    case TAO_CodeGen::TAO_CDR_OUTPUT:
      *os << fname << "_forany "
          << "_tao_" << vt->field_pd_prefix () << f->local_name ()
          << vt->field_pd_postfix () << be_idt << be_idt_nl
          << "(ACE_const_cast (" << be_idt << be_idt_nl
          << fname << "_slice*," << be_nl
          << vt->field_pd_prefix () << f->local_name ()
          << vt->field_pd_postfix () << be_uidt_nl
          << ")" << be_uidt << be_uidt_nl
          << ");" << be_uidt_nl;
      return 0;
    case TAO_CodeGen::TAO_CDR_SCOPE:
    default:
      // error
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_field_cdr_decl::"
                         "visit_array - "
                         "bad sub state\n"
                         ), -1);
    }
  ACE_NOTREACHED (return 0);
}

// visit typedef type
int
be_visitor_valuetype_field_cdr_decl::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // save the typedef node for use in code generation
                           // as we visit the base type

  // the node to be visited in the base primitve type that gets typedefed
  be_type *bt = node->primitive_base_type ();
  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_cdr_op_field_decl::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ), -1);
    }

  this->ctx_->alias (0);
  return 0;
}
