//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    rettype_marshal_ss.cpp
//
// = DESCRIPTION
//    Visitor generating code for marshaling the return type variable
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, rettype_marshal_ss, "$Id$")


// ****************************************************************************
// Visitor that passes the return value to the marshal/demarshal routine
// ****************************************************************************

be_visitor_operation_rettype_marshal_ss::
be_visitor_operation_rettype_marshal_ss (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_rettype_marshal_ss::
~be_visitor_operation_rettype_marshal_ss (void)
{
}

int
be_visitor_operation_rettype_marshal_ss::visit_array (be_array *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  //*os << "_tao_ptr_retval";
  *os << "_tao_retval.inout ()";
  return 0;
}

int
be_visitor_operation_rettype_marshal_ss::visit_enum (be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  *os << "&_tao_retval";

  return 0;
}

int
be_visitor_operation_rettype_marshal_ss::visit_interface (be_interface *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  //  *os << "&_tao_ptr_retval";
  *os << "&_tao_retval.inout ()";

  return 0;
}

int
be_visitor_operation_rettype_marshal_ss::
visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  //  *os << "&_tao_ptr_retval";
  *os << "&_tao_retval.inout ()";

  return 0;
}

int
be_visitor_operation_rettype_marshal_ss::
visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_void:
      os->indent ();
      *os << "0";
      break;
    case AST_PredefinedType::PT_pseudo:
      os->indent ();
      //*os << "&_tao_ptr_retval";
      *os << "&_tao_retval.inout ()";
      break;
    case AST_PredefinedType::PT_any:
      os->indent ();
      //      *os << "_tao_ptr_retval";
      *os << "&_tao_retval.inout ()";
      break;
    default:
      os->indent ();
      *os << "&_tao_retval";
      break;
    }
  return 0;
}

int
be_visitor_operation_rettype_marshal_ss::visit_sequence (be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  //  *os << "_tao_ptr_retval";
  *os << "&_tao_retval.inout ()";

  return 0;
}

int
be_visitor_operation_rettype_marshal_ss::visit_string (be_string * /* node*/)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  //  *os << "&_tao_ptr_retval";
  *os << "&_tao_retval.inout ()";

  return 0;
}

int
be_visitor_operation_rettype_marshal_ss::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  if (node->size_type () == be_type::VARIABLE)
    //*os << "_tao_ptr_retval";
    *os << "&_tao_retval.inout ()";
  else
    *os << "&_tao_retval";

  return 0;
}

int
be_visitor_operation_rettype_marshal_ss::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node); // set the alias node
  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_rettype_marshal_ss::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }
  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_operation_rettype_marshal_ss::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // grab the out stream

  os->indent ();
  if (node->size_type () == be_type::VARIABLE)
    //  *os << "_tao_ptr_retval";
    *os << "&_tao_retval.inout ()";
  else
    *os << "&_tao_retval";

  return 0;
}
