//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_upcall_cs.cpp
//
// = DESCRIPTION
//    Visitor that generates code that passes argument variables to the
//    upcall.
//
// = AUTHOR
//    Aniruddha Gokhale
//    Michael Kircher
//
// ============================================================================

#include "idl.h"
#include "be.h"
#include "be_visitor_argument.h"

ACE_RCSID(be_visitor_argument, upcall_cs, "$Id$")


// ************************************************************************
// visitor for passing arguments to the upcall
// ************************************************************************

be_visitor_args_ami_handler_upcall_cs::be_visitor_args_ami_handler_upcall_cs (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_ami_handler_upcall_cs::~be_visitor_args_ami_handler_upcall_cs (void)
{
}

int be_visitor_args_ami_handler_upcall_cs::visit_argument (be_argument *node)
{
  this->ctx_->node (node); // save the argument node

  // retrieve the type
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_upcall_cs::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_upcall_cs::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_ami_handler_upcall_cs::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_OUT:
    case AST_Argument::dir_INOUT:
      /*if (this->ctx_->state ()
          == TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_upcall_cs)
        *os << arg->local_name ();
      else
      */
      os->indent ();
      if (node->size_type () == be_decl::VARIABLE)
        *os << arg->local_name () << ".in ()";
      else
        *os << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_ami_handler_upcall_cs::visit_enum (be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      os->indent ();
      *os << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_ami_handler_upcall_cs::visit_interface (be_interface *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      /*if (this->ctx_->state ()
          == TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_upcall_cs)
        *os << arg->local_name ();
      else
      */
      os->indent ();
      *os << arg->local_name () << ".in ()";
      break;
    }
  return 0;
}

int be_visitor_args_ami_handler_upcall_cs::visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      /*if (this->ctx_->state ()
          == TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_upcall_cs)
        *os << arg->local_name ();
      else
      */
      os->indent ();
      *os << arg->local_name () << ".in ()";
      break;
    }
  return 0;
}

#ifdef IDL_HAS_VALUETYPE

int be_visitor_args_ami_handler_upcall_cs::visit_valuetype (be_valuetype *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      /*if (this->ctx_->state ()
          == TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_upcall_cs)
        *os << arg->local_name ();
      else
      */
      os->indent ();
      *os << arg->local_name () << ".in ()";
      break;
    }
  return 0;
}

int be_visitor_args_ami_handler_upcall_cs::visit_valuetype_fwd (be_valuetype_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      /*if (this->ctx_->state ()
          == TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_upcall_cs)
        *os << arg->local_name ();
      else
      */
      os->indent ();
      *os << arg->local_name () << ".in ()";
      break;
    }
  return 0;
}

#endif /* IDL_HAS_VALUETYPE */


int be_visitor_args_ami_handler_upcall_cs::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // check if the type is an any
  if (node->pt () == AST_PredefinedType::PT_any)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          /*
          if (this->ctx_->state ()
              == TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_upcall_cs)
            *os << arg->local_name ();
          else
          */
          os->indent ();
          *os << arg->local_name () << ".in ()";
          break;
        } // end switch direction
    } // end of if
  else if (node->pt () == AST_PredefinedType::PT_pseudo) // e.g., CORBA::Object
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          /*if (this->ctx_->state ()
              == TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_upcall_cs)
            *os << arg->local_name ();
          else
          */
          os->indent ();
          *os << arg->local_name () << ".in ()";
          break;
        } // end switch direction
    } // end else if
  else // simple predefined types
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          os->indent ();
          *os << arg->local_name ();
          break;
        } // end switch direction
    } // end of else

  return 0;
}

int be_visitor_args_ami_handler_upcall_cs::visit_sequence (be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      /*if (this->ctx_->state ()
          == TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_upcall_cs)
        *os << arg->local_name ();
      else
      */
      os->indent ();
      *os << arg->local_name () << ".in ()";
      break;
    }
  return 0;
}

int be_visitor_args_ami_handler_upcall_cs::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  // If we use it in conjunction with AMI Reply Handlers we do
  // not want to generate declarations for IN arguments
  if (this->direction () == AST_Argument::dir_IN
      && this->ctx_->state() == TAO_CodeGen::TAO_AMI_HANDLER_ARGUMENT_UPCALL_CS) 
    return 0;

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      /*
      if (this->ctx_->state ()
          == TAO_CodeGen::TAO_ARGUMENT_COLLOCATED_upcall_cs)
        *os << arg->local_name ();
      else
      */
      os->indent ();
      *os << arg->local_name () << ".in ()";
    }
  return 0;
}

int be_visitor_args_ami_handler_upcall_cs::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      os->indent ();
      if (node->size_type () == be_decl::VARIABLE)
        *os << arg->local_name () << ".in ()";
      else
        *os << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_ami_handler_upcall_cs::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node
  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      break;
    case AST_Argument::dir_INOUT:
    case AST_Argument::dir_OUT:
      os->indent ();
      if (node->size_type () == be_decl::VARIABLE)
        *os << arg->local_name () << ".in ()";
      else
        *os << arg->local_name ();
      break;
    }
  return 0;
}

int be_visitor_args_ami_handler_upcall_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_upcall_cs::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}
