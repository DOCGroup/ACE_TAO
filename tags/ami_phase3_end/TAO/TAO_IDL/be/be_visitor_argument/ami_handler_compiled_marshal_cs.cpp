//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_compiled_marshal_cs.cpp
//
// = DESCRIPTION
//    Visitor generating the code that passes arguments to the CDR operators
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "idl.h"
#include "be.h"
#include "be_visitor_argument.h"

ACE_RCSID(be_visitor_argument, ami_handler_compiled_marshal_cs, "$Id$")


// ****************************************************************************
// visitor for arguments passing to the CDR operators.
// ****************************************************************************

be_visitor_args_ami_handler_compiled_marshal_cs::
be_visitor_args_ami_handler_compiled_marshal_cs (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_ami_handler_compiled_marshal_cs::
~be_visitor_args_ami_handler_compiled_marshal_cs (void)
{
}

int be_visitor_args_ami_handler_compiled_marshal_cs::visit_argument (be_argument *node)
{
  this->ctx_->node (node); // save the argument node

  // retrieve the type of the argument
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_compiled_marshal_cs::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  //TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARGS_DEMARSHAL_CS)

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          os->indent ();
          *os << "(_tao_in >> ";
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          os->indent ();
          *os << "(_tao_out << ";
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_compiled_marshal_cs::"
                         "visit_argument - "
                         "Bad substate\n"),
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_compiled_marshal_cs::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << ")";
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << ")";
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_compiled_marshal_cs::"
                         "visit_argument - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_ami_handler_compiled_marshal_cs::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << "_tao_forany_" << arg->local_name ();
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << node->name () << "_forany ("
              << "(" << node->name () << "_slice *)"
              << arg->local_name () << ")";
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_compiled_marshal_cs::"
                         "visit_array - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_args_ami_handler_compiled_marshal_cs::visit_enum (be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name ();
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name ();
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_compiled_marshal_cs::"
                         "visit_enum - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_args_ami_handler_compiled_marshal_cs::visit_interface (be_interface *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name () << ".out ()";
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name ();
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_compiled_marshal_cs::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_args_ami_handler_compiled_marshal_cs::visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name () << "out ()";
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name ();
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_compiled_marshal_cs::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

#ifdef IDL_HAS_VALUETYPE

int be_visitor_args_ami_handler_compiled_marshal_cs::visit_valuetype (be_valuetype *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name () << ".out ()";
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name ();
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_compiled_marshal_cs::"
                         "visit_valuetype - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_args_ami_handler_compiled_marshal_cs::visit_valuetype_fwd (be_valuetype_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name () << "out ()";
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name ();
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_compiled_marshal_cs::"
                         "visit_valuetype_fwd - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

#endif /* IDL_HAS_VALUETYPE */

int be_visitor_args_ami_handler_compiled_marshal_cs::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get argument node

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          switch (node->pt ())
            {
            case AST_PredefinedType::PT_pseudo:
              *os << arg->local_name () << ".out ()";
              break;
            case AST_PredefinedType::PT_any:
            case AST_PredefinedType::PT_long:
            case AST_PredefinedType::PT_ulong:
            case AST_PredefinedType::PT_longlong:
            case AST_PredefinedType::PT_ulonglong:
            case AST_PredefinedType::PT_short:
            case AST_PredefinedType::PT_ushort:
            case AST_PredefinedType::PT_float:
            case AST_PredefinedType::PT_double:
            case AST_PredefinedType::PT_longdouble:
              *os << arg->local_name ();
              break;
            case AST_PredefinedType::PT_char:
              *os << "CORBA::Any::to_char (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_wchar:
              *os << "CORBA::Any::to_wchar (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_boolean:
              *os << "CORBA::Any::to_boolean (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_octet:
              *os << "CORBA::Any::to_octet (" << arg->local_name () << ")";
              break;
            default:
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_args_compiled_marshal_ss::"
                                 "visit_predefined_type - "
                                 "Bad predefined type\n"),
                                -1);
            }
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          switch (node->pt ())
            {
            case AST_PredefinedType::PT_pseudo:
            case AST_PredefinedType::PT_any:
            case AST_PredefinedType::PT_long:
            case AST_PredefinedType::PT_ulong:
            case AST_PredefinedType::PT_longlong:
            case AST_PredefinedType::PT_ulonglong:
            case AST_PredefinedType::PT_short:
            case AST_PredefinedType::PT_ushort:
            case AST_PredefinedType::PT_float:
            case AST_PredefinedType::PT_double:
            case AST_PredefinedType::PT_longdouble:
              *os << arg->local_name ();
              break;
            case AST_PredefinedType::PT_char:
              *os << "CORBA::Any::from_char (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_wchar:
              *os << "CORBA::Any::from_wchar (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_boolean:
              *os << "CORBA::Any::from_boolean (" << arg->local_name () << ")";
              break;
            case AST_PredefinedType::PT_octet:
              *os << "CORBA::Any::from_octet (" << arg->local_name () << ")";
              break;
            default:
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "be_visitor_args_compiled_marshal_cs::"
                                 "visit_predefined_type - "
                                 "Bad predefined type\n"),
                                -1);
            }
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_compiled_marshal_cs::"
                         "visit_array - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_args_ami_handler_compiled_marshal_cs::visit_sequence (be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name ();
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name ();
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_compiled_marshal_cs::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_args_ami_handler_compiled_marshal_cs::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          // we need to make a distinction between bounded and unbounded strings
          if (node->max_size ()->ev ()->u.ulval == 0)
            {
              *os << arg->local_name () << ".out ()";
            }
          else
            {
              *os << "CORBA::Any::to_string ("
                  << arg->local_name () << ".out (), "
                  << node->max_size ()->ev ()->u.ulval - 1
                  << ")";
            }
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name ();
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_compiled_marshal_cs::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_args_ami_handler_compiled_marshal_cs::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name ();
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name ();
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_compiled_marshal_cs::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_args_ami_handler_compiled_marshal_cs::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  be_argument *arg = this->ctx_->be_node_as_argument (); // get the argument
                                                         // node

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name ();
          break;
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          break;
        case AST_Argument::dir_INOUT:
        case AST_Argument::dir_OUT:
          *os << arg->local_name ();
          break;
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_compiled_marshal_cs::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_args_ami_handler_compiled_marshal_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_ami_handler_compiled_marshal_cs::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}
