//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    docall_cs.cpp
//
// = DESCRIPTION
//    Visitor generating the code that passes arguments to the do_static_call
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "idl.h"
#include "be.h"
#include "be_visitor_argument.h"

ACE_RCSID(be_visitor_argument, docall_cs, "$Id$")


// ****************************************************************************
// visitor for argument passing to do_static_call. The do_static_call method takes a variable
// number of parameters. The total number of parameters is determined by the
// "calldata" parameter that must be passed before the variable list
// starts. Each argument to the do_static_call is considered to be of type "void *".
// Hence we pass the address of each argument. The case for _out is a bit
// tricky where we must first retrieve the pointer, allocate memory and pass it
// to do_static_call. This is done in the "pre_do_static_call" processing.
// ****************************************************************************

be_visitor_args_docall_cs::be_visitor_args_docall_cs (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_docall_cs::~be_visitor_args_docall_cs (void)
{
}

int be_visitor_args_docall_cs::visit_argument (be_argument *node)
{
  this->ctx_->node (node); // save the argument node

  // retrieve the type of the argument
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_docall_cs::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_docall_cs::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_docall_cs::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  os->indent ();
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << arg->local_name ();
      break;
    case AST_Argument::dir_OUT:
      if (node->size_type () == be_type::VARIABLE)
        // pass reference to the pointer to slice
        *os << "_tao_base_" << arg->local_name ();
      else
        *os << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_docall_cs::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  os->indent ();
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      // pass the address. Storage is already allocated
      *os << "&" << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_docall_cs::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  os->indent ();
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      *os << "&_tao_base_" << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_docall_cs::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  os->indent ();
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      *os << "&_tao_base_" << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_docall_cs::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get argument node

  os->indent ();
  // check if the type is an any
  if (node->pt () == AST_PredefinedType::PT_any)
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	case AST_Argument::dir_INOUT:
	  *os << "&" << arg->local_name ();
	  break;
	case AST_Argument::dir_OUT:
	  *os << "_tao_base_" << arg->local_name ();
	  break;
	} // end switch direction
    } // end of if any
  else if (node->pt () == AST_PredefinedType::PT_pseudo) // e.g.,
                                                         // CORBA::Object,
                                                         // CORBA::TypeCode
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	case AST_Argument::dir_INOUT:
          *os << "&" << arg->local_name ();
          break;
	case AST_Argument::dir_OUT:
          *os << "&_tao_base_" << arg->local_name ();
	  break;
	} // end switch direction
    } // end else if pseudo
  else // simple predefined types
    {
      switch (this->direction ())
	{
	case AST_Argument::dir_IN:
	case AST_Argument::dir_INOUT:
	case AST_Argument::dir_OUT:
	  *os << "&" << arg->local_name ();
	  break;
	} // end switch direction
    } // end of else

  return 0;
}

int be_visitor_args_docall_cs::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  os->indent ();
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << "&" << arg->local_name ();
      break;
    case AST_Argument::dir_OUT:
      *os << "_tao_base_" << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_docall_cs::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  os->indent ();
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << "&" << arg->local_name ();
      break;
    case AST_Argument::dir_OUT:
      *os << "&_tao_base_" << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_docall_cs::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  os->indent ();
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << "&" << arg->local_name ();
      break;
    case AST_Argument::dir_OUT:
      {
        // check if the size type is VARIABLE
        if (node->size_type () == be_type::VARIABLE)
          *os << "_tao_base_" << arg->local_name ();
        else
          *os << "&" << arg->local_name ();
      }
      break;
    }
  return 0;
}

int be_visitor_args_docall_cs::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  os->indent ();
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
    case AST_Argument::dir_INOUT:
      *os << "&" << arg->local_name ();
      break;
    case AST_Argument::dir_OUT:
      {
        // check if the size type is VARIABLE
        if (node->size_type () == be_type::VARIABLE)
          *os << "_tao_base_" << arg->local_name ();
        else
          *os << "&" << arg->local_name ();
      }
      break;
    }
  return 0;
}

int be_visitor_args_docall_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_docall_cs::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}
