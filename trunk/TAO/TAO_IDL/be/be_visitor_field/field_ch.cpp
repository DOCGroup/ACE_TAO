//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    field_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Field node in the client header
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_field.h"

ACE_RCSID(be_visitor_field, field_ch, "$Id$")


// **********************************************
//  visitor for field in the client header file
// **********************************************

// constructor
be_visitor_field_ch::be_visitor_field_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

// destructor
be_visitor_field_ch::~be_visitor_field_ch (void)
{
}

// visit the field node
int
be_visitor_field_ch::visit_field (be_field *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt; // field's type

  os = this->ctx_->stream ();
  // first generate the type information
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_ch::"
                         "visit_field - "
                         "Bad field type\n"
                         ), -1);
    }

  this->ctx_->node (node); // save the node
  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_ch::"
                         "visit_field - "
                         "codegen for field type failed\n"
                         ), -1);
    }
  // now output the field name. 
  *os << " " << node->local_name () << ";\n";
  return 0;
}

// =visit operations on all possible data types  that a field can be

// visit array type
int
be_visitor_field_ch::visit_array (be_array *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;

  os = this->ctx_->stream ();
  // set the right type;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

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
      ctx.state (TAO_CodeGen::TAO_ARRAY_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ch::"
                             "visit_array - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ch::"
                             "visit_array - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;

      // having defined all array type and its supporting operations, now
      // generate the actual variable that is a field of the structure
      os->indent ();
      *os << "_" << bt->local_name ();
    }
  else
    {
      // this was a typedefed array
      os->indent (); // start from current indentation level
      *os << bt->nested_type_name (this->ctx_->scope ());
    }
  return 0;
}

// visit enum type
int
be_visitor_field_ch::visit_enum (be_enum *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;

  os = this->ctx_->stream ();

  // set the right type;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

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
      ctx.state (TAO_CodeGen::TAO_ENUM_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ch::"
                             "visit_enum - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ch::"
                             "visit_enum - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  // now use this enum as a "type" for the subsequent declarator
  os->indent (); // start from current indentation level
  *os << bt->nested_type_name (this->ctx_->scope ());

  return 0;
}

// visit interface type
int
be_visitor_field_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;

  os = this->ctx_->stream ();
  // set the right type;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  // if not a typedef and we are defined in the use scope, we must be defined
  os->indent ();
  *os << "TAO_Object_Field_T<"
      <<  bt->nested_type_name (this->ctx_->scope (), "")
      << ">";
  return 0;
}

// visit interface forward type
int
be_visitor_field_ch::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;

  os = this->ctx_->stream ();
  // set the right type;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  // if not a typedef and we are defined in the use scope, we must be defined
  os->indent ();
  *os << "TAO_Object_Field_T<"
      <<  bt->nested_type_name (this->ctx_->scope (), "")
      << ">";
  return 0;
}

// visit predefined type
int
be_visitor_field_ch::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;

  os = this->ctx_->stream ();
  // set the right type;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  // if not a typedef and we are defined in the use scope, we must be defined
  os->indent (); // start from current indentation level
  if (node->pt () == AST_PredefinedType::PT_pseudo) // is a psuedo obj
    {
      // check if we are dealing with a CORBA::Object
      if (!ACE_OS::strcmp (bt->local_name ()->get_string (), "Object"))
        {
          *os << "TAO_Object_Field_T<CORBA::Object>";
        }
      else
        *os << bt->nested_type_name (this->ctx_->scope (), "_var");
    }
  else
    *os << bt->nested_type_name (this->ctx_->scope ());
  return 0;
}

// visit sequence type
int
be_visitor_field_ch::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;

  os = this->ctx_->stream ();
  // set the right type;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

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
      ctx.state (TAO_CodeGen::TAO_SEQUENCE_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ch::"
                             "visit_sequence - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ch::"
                             "visit_sequence - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  os->indent (); // start from current indentation level
  *os << bt->nested_type_name (this->ctx_->scope ());
  return 0;
}

// visit string type
int
be_visitor_field_ch::visit_string (be_string *)
{
  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();
  os->indent (); // start from current indentation level
  // set the right type;

#if 0
  if (this->ctx_->alias ())
    {
      *os << this->ctx_->alias ()->nested_type_name (this->ctx_->scope ())
          << "_var";
    }
  else
    *os << "CORBA::String_var";
#endif
  *os << "TAO_String_Manager";
  return 0;
}

// visit structure type
int
be_visitor_field_ch::visit_structure (be_structure *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;

  os = this->ctx_->stream ();
  // set the right type;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

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
      ctx.state (TAO_CodeGen::TAO_STRUCT_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ch::"
                             "visit_struct - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ch::"
                             "visit_struct - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  os->indent (); // start from current indentation level
  *os << bt->nested_type_name (this->ctx_->scope ());
  return 0;
}

// visit typedefed type
int
be_visitor_field_ch::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);    // save the node for use in code generation and
                               // indicate that the type of the field node
                               // is a typedefed quantity

  // make a decision based on the primitive base type
  be_type *bt = node->primitive_base_type ();
  if (!bt || (bt->accept (this) == -1))
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_field_ch::"
                         "visit_typedef - "
                         "Bad primitive type\n"
                         ), -1);
    }
  // reset the alias
  this->ctx_->alias (0);
  return 0;
}

// visit union type
int
be_visitor_field_ch::visit_union (be_union *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;

  os = this->ctx_->stream ();
  // set the right type;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

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
      ctx.state (TAO_CodeGen::TAO_ENUM_CH);
      be_visitor *visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ch::"
                             "visit_enum - "
                             "Bad visitor\n"
                             ), -1);
        }
      if (node->accept (visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_field_ch::"
                             "visit_enum - "
                             "codegen failed\n"
                             ), -1);
        }
      delete visitor;
    }

  os->indent (); // start from current indentation level
  *os << bt->nested_type_name (this->ctx_->scope ());
  return 0;
}
