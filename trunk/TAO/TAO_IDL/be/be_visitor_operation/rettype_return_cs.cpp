//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype_return_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for returning the return type variable.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, rettype_return_cs, "$Id$")


// ************************************************************
//    be_visitor_operation_rettype_return_cs
//
//    code to generate the return statement of the stub.
// ************************************************************

be_visitor_operation_rettype_return_cs::be_visitor_operation_rettype_return_cs
(be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_return_cs::~be_visitor_operation_rettype_return_cs (void)
{
}

int
be_visitor_operation_rettype_return_cs::visit_array (be_array *)
{
   // Grab the out stream
  TAO_OutStream *os = this->ctx_->stream ();

  *os << "_tao_retval._retn ()";
  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_enum (be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  *os << "_tao_retval";
  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_interface (be_interface *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  *os << "_tao_retval._retn ()";
  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  *os << "_tao_retval";
  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_valuetype (be_valuetype *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  *os << "_tao_retval._retn ()";
  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_valuetype_fwd (be_valuetype_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  *os << "_tao_retval._retn ()";
  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_pseudo:
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
be_visitor_operation_rettype_return_cs::visit_sequence (be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  *os << "_tao_retval._retn ()";
  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_string (be_string *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  *os << "_tao_retval._retn ()";
  return 0;
}

int
be_visitor_operation_rettype_return_cs::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  if (node->size_type () == be_decl::VARIABLE)
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
be_visitor_operation_rettype_return_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
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
be_visitor_operation_rettype_return_cs::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  if (node->size_type () == be_decl::VARIABLE)
    {
      *os << "_tao_retval._retn ()";
    }
  else
    {
      *os << "_tao_retval";
    }
  
  return 0;
}
