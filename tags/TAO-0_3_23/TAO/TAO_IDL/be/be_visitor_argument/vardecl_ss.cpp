//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    vardecl_ss.cpp
//
// = DESCRIPTION
//    Visitor that generates the variable declaration in the skeleton
//    corresponding to the Argument node
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "idl.h"
#include "be.h"
#include "be_visitor_argument.h"

ACE_RCSID(be_visitor_argument, vardecl_ss, "$Id$")


// ************************************************************************
// Visitor to generate code for argument variable declaration
// ************************************************************************

be_visitor_args_vardecl_ss::be_visitor_args_vardecl_ss (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_vardecl_ss::~be_visitor_args_vardecl_ss (void)
{
}

int be_visitor_args_vardecl_ss::visit_argument (be_argument *node)
{
  this->ctx_->node (node); // save the argument node

  // retrieve the type
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_vardecl_ss::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_vardecl_ss::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_vardecl_ss::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << bt->name () << " " << arg->local_name () << ";" << be_nl
          << bt->name () << "_forany _tao_forany_"
          << arg->local_name () << " (" << be_idt << be_idt_nl
          << arg->local_name () << be_uidt_nl
          << ");\n" << be_uidt;
      break;

    case AST_Argument::dir_OUT:
      os->indent ();
      if (node->size_type () == be_type::VARIABLE)
        {
          *os << bt->name () << "_var " << arg->local_name ()
              << ";\n\n";
        }
      else
        {
          *os << bt->name () << " " << arg->local_name ()
              << ";\n\n";
        }
      break;
    }
  return 0;
}

int be_visitor_args_vardecl_ss::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << bt->name () << " " << arg->local_name () << ";\n";
      break;
    }
  return 0;
}

int be_visitor_args_vardecl_ss::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
#if 0 /* ASG */
      *os << bt->name () << "_var " << arg->local_name () << ";" << be_nl;
      *os << "CORBA::Object_var _tao_base_var_" << arg->local_name ()
          << ";" << be_nl;
      *os << "CORBA::Object_ptr &_tao_base_ptr_" << arg->local_name ()
          << " = _tao_base_var_" << arg->local_name () << ".out ();\n";
#endif
      *os << bt->name () << "_var " << arg->local_name () << ";" << be_nl;
      *os << "CORBA::Object_var _tao_base_var_" << arg->local_name ()
          << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
#if 0 /* ASG */
      *os << bt->name () << "_var _tao_var_"
          << arg->local_name () << ";" << be_nl;
      *os << "CORBA::Object_ptr _tao_base_ptr_"
          << arg->local_name () << ";" << be_nl;
      *os << bt->name () << "_out " << arg->local_name ()
          << " (_tao_var_" << arg->local_name () << ".out ());\n";
#endif
      *os << bt->name () << "_var "
          << arg->local_name () << ";" << be_nl;
      *os << "CORBA::Object_var _tao_base_var_"
          << arg->local_name () << ";\n";
      break;
    }
  return 0;
}

int be_visitor_args_vardecl_ss::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
#if 0 /* ASG */
      *os << bt->name () << "_var " << arg->local_name () << ";" << be_nl;
      *os << "CORBA::Object_var _tao_base_var_" << arg->local_name ()
          << ";" << be_nl;
      *os << "CORBA::Object_ptr &_tao_base_ptr_" << arg->local_name ()
          << " = _tao_base_var_" << arg->local_name () << ".out ();\n";
#endif
      *os << bt->name () << "_var " << arg->local_name () << ";" << be_nl;
      *os << "CORBA::Object_var _tao_base_var_" << arg->local_name ()
          << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
#if 0 /* ASG */
      *os << bt->name () << "_var _tao_var_"
          << arg->local_name () << ";" << be_nl;
      *os << "CORBA::Object_ptr _tao_base_ptr_"
          << arg->local_name () << ";" << be_nl;
      *os << bt->name () << "_out " << arg->local_name ()
          << " (_tao_var_" << arg->local_name () << ".out ());\n";
#endif
      *os << bt->name () << "_var "
          << arg->local_name () << ";" << be_nl;
      *os << "CORBA::Object_var _tao_base_var_"
          << arg->local_name () << ";\n";
      break;
    }
  return 0;
}


#ifdef IDL_HAS_VALUETYPE

int be_visitor_args_vardecl_ss::visit_valuetype (be_valuetype *node)
{
  return -1;
}

int be_visitor_args_vardecl_ss::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  return -1;
}

#endif /* IDL_HAS_VALUETYPE */

int be_visitor_args_vardecl_ss::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  // check if the type is an any
  if (node->pt () == AST_PredefinedType::PT_any)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          os->indent ();
          *os << bt->name () << " " << arg->local_name () << ";\n";
          break;
        case AST_Argument::dir_OUT:
          os->indent ();
#if 0 /* ASG */
          *os << bt->name () << "_var _tao_var_"
              << arg->local_name () << ";" << be_nl;
          *os << bt->name () << "_ptr &_tao_ptr_" << arg->local_name ()
              << " = _tao_var_" << arg->local_name () << ".out ();" << be_nl;
          *os << bt->name () << "_out " << arg->local_name ()
              << " (_tao_ptr_" << arg->local_name () << ");\n";
#endif
          *os << bt->name () << "_var "
              << arg->local_name () << ";\n";
          break;
        } // end switch direction
    } // end of if
  else if (node->pt () == AST_PredefinedType::PT_pseudo) // e.g., CORBA::Object
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
          os->indent ();
#if 0 /* ASG */
          *os << bt->name () << "_var " << arg->local_name ()
              << ";" << be_nl;
          *os << bt->name () << "_ptr &_tao_ptr_" << arg->local_name ()
              << " = " << arg->local_name () << ".out ();\n";
#endif
          *os << bt->name () << "_var " << arg->local_name ()
              << ";\n";
          break;
        case AST_Argument::dir_OUT:
          os->indent ();
#if 0 /* ASG */
          *os << bt->name () << "_var _tao_var_"
              << arg->local_name () << ";" << be_nl;
          *os << bt->name () << "_ptr &_tao_ptr_" << arg->local_name ()
              << " = _tao_var_" << arg->local_name () << ".out ();" << be_nl;
          *os << bt->name () << "_out " << arg->local_name ()
              << " (_tao_ptr_" << arg->local_name () << ");" << be_nl;
#endif
          *os << bt->name () << "_var "
              << arg->local_name () << ";\n";
          break;
        } // end switch direction
    } // end else if
  else // simple predefined types
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          os->indent ();
          *os << bt->name () << " " << arg->local_name () << ";\n";
          break;
        } // end switch direction
    } // end of else

  return 0;
}

int be_visitor_args_vardecl_ss::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << bt->name () << " " << arg->local_name () << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
#if 0 /* ASG */
      *os << bt->name () << "_var _tao_var_"
          << arg->local_name () << ";" << be_nl;
      *os << bt->name () << " *&_tao_ptr_" << arg->local_name ()
          << " = _tao_var_" << arg->local_name () << ".out ();" << be_nl;
      *os << bt->name () << "_out " << arg->local_name ()
          << " (_tao_ptr_" << arg->local_name () << ");\n";
#endif
      *os << bt->name () << "_var "
          << arg->local_name () << ";" << be_nl;
      break;
    }
  return 0;
}

int be_visitor_args_vardecl_ss::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
#if 0 /* ASG */
      *os << "CORBA::String_var _tao_var_"
          << arg->local_name () << ";" << be_nl;
      *os << "char *&" << arg->local_name () << " = _tao_var_"
          << arg->local_name () << ".out ();" << be_nl;
#endif
      *os << "CORBA::String_var "
          << arg->local_name () << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
#if 0 /* ASG */
      *os << "CORBA::String_var _tao_var_"
          << arg->local_name () << ";" << be_nl;
      *os << "char *&_tao_ptr_" << arg->local_name () << " = _tao_var_"
          << arg->local_name () << ".out ();" << be_nl;
      *os << "CORBA::String_out " << arg->local_name ()
          << " (_tao_ptr_" << arg->local_name () << ");\n";
#endif
      *os << "CORBA::String_var "
          << arg->local_name () << ";\n";
      break;
    }
  return 0;
}

int be_visitor_args_vardecl_ss::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << bt->name () << " " << arg->local_name () << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      // check if it is variable sized
      if (node->size_type () == be_type::VARIABLE)
        {
#if 0 /* ASG */
          *os << bt->name () << "_var _tao_var_"
              << arg->local_name () << ";" << be_nl;
          *os << bt->name () << " *&_tao_ptr_" << arg->local_name ()
              << " = _tao_var_" << arg->local_name () << ".out ();" << be_nl;
          *os << bt->name () << "_out " << arg->local_name ()
              << " (_tao_ptr_" << arg->local_name () << ");\n";
#endif
          *os << bt->name () << "_var "
              << arg->local_name () << ";\n";
        }
      else
        *os << bt->name () << " " << arg->local_name () << ";\n";
      break;
    }
  return 0;
}

int be_visitor_args_vardecl_ss::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << bt->name () << " " << arg->local_name () << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      // check if it is variable sized
      if (node->size_type () == be_type::VARIABLE)
        {
#if 0 /* ASG */
          *os << bt->name () << "_var _tao_var_"
              << arg->local_name () << ";" << be_nl;
          *os << bt->name () << " *&_tao_ptr_" << arg->local_name ()
              << " = _tao_var_" << arg->local_name () << ".out ();" << be_nl;
          *os << bt->name () << "_out " << arg->local_name ()
              << " (_tao_ptr_" << arg->local_name () << ");\n";
#endif
          *os << bt->name () << "_var "
              << arg->local_name () << ";\n";

        }
      else
        *os << bt->name () << " " << arg->local_name () << ";\n";
      break;
    }
  return 0;
}

int be_visitor_args_vardecl_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_vardecl_ss::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}


// ************************************************************************
// Visitor to generate code for argument variable declaration for compiled
// marshaling. This provides only the overriden methods. The rest is handled by
// the base class that works for interpretiveskeletons.
// ************************************************************************

be_compiled_visitor_args_vardecl_ss::
be_compiled_visitor_args_vardecl_ss (be_visitor_context *ctx)
  : be_visitor_args_vardecl_ss (ctx)
{
}

be_compiled_visitor_args_vardecl_ss::~be_compiled_visitor_args_vardecl_ss (void)
{
}

int be_compiled_visitor_args_vardecl_ss::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << bt->name () << "_var " << arg->local_name () << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << bt->name () << "_var "
          << arg->local_name () << ";\n";
      break;
    }
  return 0;
}

int be_compiled_visitor_args_vardecl_ss::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << bt->name () << "_var " << arg->local_name () << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << bt->name () << "_var "
          << arg->local_name () << ";\n";
      break;
    }
  return 0;
}


#ifdef IDL_HAS_VALUETYPE

int be_compiled_visitor_args_vardecl_ss::visit_valuetype (be_valuetype *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << bt->name () << "_var " << arg->local_name () << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << bt->name () << "_var "
          << arg->local_name () << ";\n";
      break;
    }
  return 0;
}

int
be_compiled_visitor_args_vardecl_ss::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      os->indent ();
      *os << bt->name () << "_var " << arg->local_name () << ";\n";
      break;
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << bt->name () << "_var "
          << arg->local_name () << ";\n";
      break;
    }
  return 0;
}

#endif /* IDL_HAS_VALUETYPE */
