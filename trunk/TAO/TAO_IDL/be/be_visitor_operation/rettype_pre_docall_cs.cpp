//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype_pre_docall_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for doing any pre-processing of return type prior
//    to making the do_static_call.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"


// *****************************************************************************
//    be_visitor_operation_rettype_pre_docall_cs
//
//    This visitor generates code that passes the return type variable to the
//    do_static_call method
// *****************************************************************************

be_visitor_operation_rettype_pre_docall_cs::
be_visitor_operation_rettype_pre_docall_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_pre_docall_cs::
~be_visitor_operation_rettype_pre_docall_cs (void)
{
}

int
be_visitor_operation_rettype_pre_docall_cs::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "ACE_ALLOCATOR_RETURN (_tao_retval, " << node->name ()
      << "_alloc (), _tao_retval);\n";
  return 0;
}

int
be_visitor_operation_rettype_pre_docall_cs::visit_interface (be_interface *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "CORBA::Object_ptr _tao_base_retval = CORBA::Object::_nil ();\n";
  return 0;
}

int
be_visitor_operation_rettype_pre_docall_cs::visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "CORBA::Object_ptr _tao_base_retval = CORBA::Object::_nil ();\n";
  return 0;
}

int
be_visitor_operation_rettype_pre_docall_cs::
visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_any:
      os->indent ();
      *os << "ACE_NEW_RETURN (_tao_retval, CORBA::Any, _tao_retval);\n";
      break;
    default:
      break;
    }
  return 0;
}

int
be_visitor_operation_rettype_pre_docall_cs::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "ACE_NEW_RETURN (_tao_retval, " << node->name () << ", _tao_retval);\n";
  return 0;
}

int
be_visitor_operation_rettype_pre_docall_cs::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  // check if the union is variable
  if (node->size_type () == be_type::VARIABLE)
    {
      os->indent ();
      *os << "ACE_NEW_RETURN (_tao_retval, " << node->name () << ", _tao_retval);\n";
    }
  return 0;
}

int
be_visitor_operation_rettype_pre_docall_cs::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_pre_docall_cs::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_rettype_pre_docall_cs::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  // check if the union is variable
  if (node->size_type () == be_type::VARIABLE)
    {
      os->indent ();
      *os << "ACE_NEW_RETURN (_tao_retval, " << node->name () << ", _tao_retval);\n";
    }
  return 0;
}
