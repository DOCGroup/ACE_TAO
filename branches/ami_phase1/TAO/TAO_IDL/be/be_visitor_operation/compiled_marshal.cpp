//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    compiled_marshal.cpp
//
// = DESCRIPTION
//    Visitor generating the code that passes retval to the CDR operators. The
//    "OUTPUT" substate deals with the server side skeletons whereas the
//    "INPUT" substate deals with the client stubs. Also notice that the type
//    of parameter passed to the operator depends on the manner in which the
//    variable is declared i.e., in the skeleton, we may have _var types but
//    not in the stubs.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "idl.h"
#include "be.h"
#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, compiled_marshal, "$Id$")


// ****************************************************************************
// visitor for arguments passing to the CDR operators.
// ****************************************************************************

be_visitor_operation_rettype_compiled_marshal::
be_visitor_operation_rettype_compiled_marshal (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_compiled_marshal::
~be_visitor_operation_rettype_compiled_marshal (void)
{
}

int be_visitor_operation_rettype_compiled_marshal::
visit_operation (be_operation *node)
{
  this->ctx_->node (node); // save the argument node

  // retrieve the type of the argument
  be_type *bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_compiled_marshal::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      os->indent ();
      *os << "(_tao_out << ";
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      os->indent ();
      *os << "(_tao_in >> ";
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_compiled_marshal::"
                         "visit_operation - "
                         "Bad substate\n"),
                        -1);
    }

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_compiled_marshal::"
                         "visit_operation - "
                         "cannot accept visitor\n"),
                        -1);
    }

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      *os << ")";
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      *os << ")";
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_compiled_marshal::"
                         "visit_operation - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_operation_rettype_compiled_marshal::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      *os << node->name () << "_forany (";
      if (node->size_type () == be_decl::VARIABLE)
        *os << "(" << node->name () << "_slice *)"
            << "_tao_retval.in ()" << ")";
      else
        *os << "_tao_retval" << ")";
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      *os << node->name () << "_forany ("
          << "_tao_retval" << ")";
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_compiled_marshal::"
                         "visit_array - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_operation_rettype_compiled_marshal::visit_enum (be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      *os << "_tao_retval";
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      *os << "_tao_retval";
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_compiled_marshal::"
                         "visit_enum - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_operation_rettype_compiled_marshal::visit_interface (be_interface *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      *os << "_tao_retval.in ()";
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      *os << "_tao_retval";
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_compiled_marshal::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_operation_rettype_compiled_marshal::visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      *os << "_tao_retval.in ()";
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      *os << "_tao_retval";
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_compiled_marshal::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

#ifdef IDL_HAS_VALUETYPE

int be_visitor_operation_rettype_compiled_marshal::visit_valuetype (be_valuetype *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      *os << "_tao_retval.in ()";
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      *os << "_tao_retval";
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_compiled_marshal::"
                         "visit_valuetype - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_operation_rettype_compiled_marshal::visit_valuetype_fwd (be_valuetype_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      *os << "_tao_retval.in ()";
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      *os << "_tao_retval";
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_compiled_marshal::"
                         "visit_valuetype - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

#endif /* IDL_HAS_VALUETYPE */

int be_visitor_operation_rettype_compiled_marshal::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      switch (node->pt ())
        {
        case AST_PredefinedType::PT_pseudo:
          *os << "_tao_retval.in ()";
          break;
        case AST_PredefinedType::PT_any:
          *os << "_tao_retval.in ()";
          break;
        case AST_PredefinedType::PT_long:
        case AST_PredefinedType::PT_ulong:
        case AST_PredefinedType::PT_longlong:
        case AST_PredefinedType::PT_ulonglong:
        case AST_PredefinedType::PT_short:
        case AST_PredefinedType::PT_ushort:
        case AST_PredefinedType::PT_float:
        case AST_PredefinedType::PT_double:
        case AST_PredefinedType::PT_longdouble:
          *os << "_tao_retval";
          break;
        case AST_PredefinedType::PT_char:
          *os << "CORBA::Any::from_char (_tao_retval)";
          break;
        case AST_PredefinedType::PT_wchar:
          *os << "CORBA::Any::from_wchar (_tao_retval)";
          break;
        case AST_PredefinedType::PT_boolean:
          *os << "CORBA::Any::from_boolean (_tao_retval)";
          break;
        case AST_PredefinedType::PT_octet:
          *os << "CORBA::Any::from_octet (_tao_retval)";
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_operation_rettype_compiled_marshal::"
                             "visit_array - "
                             "Bad predefined type\n"),
                            -1);
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      switch (node->pt ())
        {
        case AST_PredefinedType::PT_pseudo:
          *os << "_tao_retval";
          break;
        case AST_PredefinedType::PT_any:
          *os << "*_tao_retval";
          break;
        case AST_PredefinedType::PT_long:
        case AST_PredefinedType::PT_ulong:
        case AST_PredefinedType::PT_longlong:
        case AST_PredefinedType::PT_ulonglong:
        case AST_PredefinedType::PT_short:
        case AST_PredefinedType::PT_ushort:
        case AST_PredefinedType::PT_float:
        case AST_PredefinedType::PT_double:
        case AST_PredefinedType::PT_longdouble:
          *os << "_tao_retval";
          break;
        case AST_PredefinedType::PT_char:
          *os << "CORBA::Any::to_char (_tao_retval)";
          break;
        case AST_PredefinedType::PT_wchar:
          *os << "CORBA::Any::to_wchar (_tao_retval)";
          break;
        case AST_PredefinedType::PT_boolean:
          *os << "CORBA::Any::to_boolean (_tao_retval)";
          break;
        case AST_PredefinedType::PT_octet:
          *os << "CORBA::Any::to_octet (_tao_retval)";
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_operation_rettype_compiled_marshal::"
                             "visit_array - "
                             "Bad predefined type\n"),
                            -1);
        }
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_compiled_marshal::"
                         "visit_array - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_operation_rettype_compiled_marshal::visit_sequence (be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      *os << "_tao_retval.in ()";
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      *os << "*_tao_retval";
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_compiled_marshal::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_operation_rettype_compiled_marshal::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      // differentiate between bounded and unbounded
      if (node->max_size ()->ev ()->u.ulval == 0)
        {
          *os << "_tao_retval.in ()";
        }
      else
        {
          *os << "CORBA::Any::from_string ((char *)_tao_retval.in (), "
              << node->max_size ()->ev ()->u.ulval - 1 << ")";
        }
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      // differentiate between bounded and unbounded
      if (node->max_size ()->ev ()->u.ulval == 0)
        // unbounded
        *os << "_tao_retval";
      else
        *os << "CORBA::Any::to_string (_tao_retval, "
            << node->max_size ()->ev ()->u.ulval - 1 << ")";
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_compiled_marshal::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_operation_rettype_compiled_marshal::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      if (node->size_type () == be_decl::VARIABLE)
        *os << "_tao_retval.in ()";
      else
        *os << "_tao_retval";
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      if (node->size_type () == be_decl::VARIABLE)
        *os << "*_tao_retval";
      else
        *os << "_tao_retval";
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_compiled_marshal::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }
  return 0;
}

int be_visitor_operation_rettype_compiled_marshal::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_OUTPUT)
    {
      if (node->size_type () == be_decl::VARIABLE)
        *os << "_tao_retval.in ()";
      else
        *os << "_tao_retval";
    }
  else if (this->ctx_->sub_state () == TAO_CodeGen::TAO_CDR_INPUT)
    {
      if (node->size_type () == be_decl::VARIABLE)
        *os << "*_tao_retval";
      else
        *os << "_tao_retval";
    }
  else
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_compiled_marshal::"
                         "visit_interface - "
                         "Bad substate\n"),
                        -1);
    }

  return 0;
}

int be_visitor_operation_rettype_compiled_marshal::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_compiled_marshal::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}
