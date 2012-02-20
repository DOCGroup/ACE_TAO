
//=============================================================================
/**
 *  @file    rettype_return_cs.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for returning the return type variable.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "operation.h"

// ************************************************************
//    be_visitor_operation_rettype_return_cs
//
//    code to generate the return statement of the stub.
// ************************************************************

be_visitor_operation_rettype_return_cs::
be_visitor_operation_rettype_return_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_return_cs::
~be_visitor_operation_rettype_return_cs (void)
{
}

int
be_visitor_operation_rettype_return_cs::visit_array (
  be_array *)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "_tao_retval._retn ()";

  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_enum (
  be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "_tao_retval";

  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_interface (
  be_interface *)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "_tao_retval._retn ()";

  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_interface_fwd (
  be_interface_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "_tao_retval";

  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_valuebox (
  be_valuebox *)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "_tao_retval._retn ()";

  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_valuetype (
  be_valuetype *)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "_tao_retval._retn ()";

  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_valuetype_fwd (
  be_valuetype_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "_tao_retval._retn ()";

  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_predefined_type (
    be_predefined_type *node
  )
{
  TAO_OutStream *os = this->ctx_->stream ();

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
    case AST_PredefinedType::PT_object:
      *os << "_tao_retval._retn ()";

      break;
    case AST_PredefinedType::PT_any:
      *os << "_tao_retval._retn ()";

      break;
    case AST_PredefinedType::PT_void:
      break;
    default:
      *os << "_tao_retval";

      break;
    }

  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_sequence (
  be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "_tao_retval._retn ()";

  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_string (
  be_string *)
{
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "_tao_retval._retn ()";

  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_structure (
  be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (node->size_type () == AST_Type::VARIABLE)
    {
      *os << "_tao_retval._retn ()";
    }
  else
    {
      *os << "_tao_retval";
    }

  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_typedef (
  be_typedef *node)
{
  this->ctx_->alias (node);

  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_return_cs::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_union (
  be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (node->size_type () == AST_Type::VARIABLE)
    {
      *os << "_tao_retval._retn ()";
    }
  else
    {
      *os << "_tao_retval";
    }

  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_component (
  be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_operation_rettype_return_cs::visit_component_fwd (
    be_component_fwd *node
  )
{
  return this->visit_interface_fwd (node);
}

int
be_visitor_operation_rettype_return_cs::visit_eventtype (
  be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int
be_visitor_operation_rettype_return_cs::visit_eventtype_fwd (
    be_eventtype_fwd *node
  )
{
  return this->visit_valuetype_fwd (node);
}

int
be_visitor_operation_rettype_return_cs::visit_home (
  be_home *node)
{
  return this->visit_interface (node);
}


