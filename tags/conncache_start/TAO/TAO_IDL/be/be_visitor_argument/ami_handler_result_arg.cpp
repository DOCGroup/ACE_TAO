//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_result_arg.cpp
//
// = DESCRIPTION
//    Visitor that generates the result argument in a call back method
//    of a Reply Handler.
//
// = AUTHOR
//    Aniruddha Gokhale and Alexander Babu Arulanthu
//    <alex@cs.wustl.edu>
//
// ============================================================================

#include "idl.h"
#include "be.h"
#include "be_visitor_argument.h"

ACE_RCSID(be_visitor_args, ami_handler_result_arg, "$Id$")


// ************************************************************
// Visitor for parameter list in AMI Handler call back declarations
// and definitions.
// ************************************************************

be_visitor_args_ami_handler_result_arg::be_visitor_args_ami_handler_result_arg (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_ami_handler_result_arg::~be_visitor_args_ami_handler_result_arg (void)
{
}

int
be_visitor_args_ami_handler_result_arg::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  *os << "const " << this->type_name (node) << " _tao_retval, ";
  return 1;
}

int
be_visitor_args_ami_handler_result_arg::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  *os << this->type_name (node) << " _tao_retval";
  return 1;
}

int
be_visitor_args_ami_handler_result_arg::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  *os << this->type_name (node, "_ptr") << " _tao_retval";
  return 1;
}

int
be_visitor_args_ami_handler_result_arg::visit_interface_fwd (be_interface_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  *os << this->type_name (node, "_ptr") << " _tao_retval";
  return 1;
}

int
be_visitor_args_ami_handler_result_arg::visit_native (be_native *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  *os << this->type_name (node) << " _tao_retval";
  return 1;
}

int
be_visitor_args_ami_handler_result_arg::visit_predefined_type (be_predefined_type *node)
{

  // Nothing to do if the return type is void.
  if (node->pt () == AST_PredefinedType::PT_void)
    return 0;

  // Get output stream.
  TAO_OutStream *os = this->ctx_->stream ();

  // Check if the type is an any.
  if (node->pt () == AST_PredefinedType::PT_any)
    {
      *os << "const " << this->type_name (node) << " &" << " _tao_retval";
    }
  else if (node->pt () == AST_PredefinedType::PT_pseudo) // e.g., CORBA::Object
    {
      *os << this->type_name (node, "_ptr") << " _tao_retval";
    }
  else
    {
      *os << this->type_name (node) << " _tao_retval";
    }

  return 1;
}

int
be_visitor_args_ami_handler_result_arg::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream
  *os << "const " << this->type_name (node) << " &" << " _tao_retval";
  return 1;
}

int
be_visitor_args_ami_handler_result_arg::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream
  *os << "const char *" << " _tao_retval";
  return 1;
}

int
be_visitor_args_ami_handler_result_arg::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream
  *os << "const " << this->type_name (node) << " &" << " _tao_retval";
  return 1;
}

int
be_visitor_args_ami_handler_result_arg::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream
  *os << "const " << this->type_name (node) << " &" << " _tao_retval";
  return 1;
}

int
be_visitor_args_ami_handler_result_arg::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_result_arg::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 1;
}


#ifdef IDL_HAS_VALUETYPE

int
be_visitor_args_ami_handler_result_arg::visit_valuetype (be_valuetype *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream
  *os << this->type_name (node) << " *" << " _tao_retval";
  return 1;
}

int
be_visitor_args_ami_handler_result_arg::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream
  *os << "const " << this->type_name (node) << " *" << " _tao_retval";
  return 1;
}

#endif /* IDL_HAS_VALUETYPE */
