//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interceptors_result.cpp
//
// = DESCRIPTION
//    Visitor that generates the Dyanmic::ParameterList
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#include "idl.h"
#include "be.h"
#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_argument, interceptors_result, "$Id$")


// ************************************************************
// be_visitor_operation_interceptors_result for generating the result as
// stored in the request info for interceptors
// ************************************************************

be_visitor_operation_interceptors_result::be_visitor_operation_interceptors_result (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_operation_interceptors_result::~be_visitor_operation_interceptors_result (void)
{
}

int
be_visitor_operation_interceptors_result::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // if the current type is an alias, use that
  be_type *bt;

  if (this->ctx_->alias ())
    {
      bt = this->ctx_->alias ();
    }
  else
    {
      bt = node;
    }

  *os << bt->name () <<  "_forany _tao_forany_result"
      << " (this->_result);" << be_nl
      << "(*result_any) <<= _tao_forany_result;" << be_nl;

  return 0;
}

int
be_visitor_operation_interceptors_result::visit_enum (be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  *os << "(*result_any) <<= this->_result;" << be_nl;

  return 0;
}

int
be_visitor_operation_interceptors_result::visit_interface (be_interface *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  *os << "(*result_any) <<= this->_result;" << be_nl;

  return 0;
}

int
be_visitor_operation_interceptors_result::visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  *os << "(*result_any) <<= this->_result;" << be_nl;

  return 0;
}
#ifdef IDL_HAS_VALUETYPE

int
be_visitor_operation_interceptors_result::visit_valuetype (be_valuetype *)
{
  // Not supported since TAO doesnt support Any operators for valuetype yet.

  // TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // *os << "(*result_any) <<= this->_result;" << be_nl;

  return 0;
}

int
be_visitor_operation_interceptors_result::visit_valuetype_fwd (be_valuetype_fwd *)
{
  // Not supported since TAO doesnt support Any operators for valuetype yet.

  // TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // *os << "(*result_any) <<= this->_result;" << be_nl;

  return 0;
}

#endif /* IDL_HAS_VALUETYPE */

int
be_visitor_operation_interceptors_result::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  *os << "(*result_any) <<= ";

  switch (node->pt ())
    {
    case AST_PredefinedType::PT_boolean:
      *os << "CORBA::Any::from_boolean (this->_result);" << be_nl;
      break;
    case AST_PredefinedType::PT_char:
      *os << "CORBA::Any::from_char (this->_result);" << be_nl;
      break;
    case AST_PredefinedType::PT_wchar:
      *os << "CORBA::Any::from_wchar (this->_result);" << be_nl;
      break;
    case AST_PredefinedType::PT_octet:
      *os << "CORBA::Any::from_octet (this->_result);" << be_nl;
      break;
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
      *os << "this->_result;" << be_nl;
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_interceptors_result::"
                         "visit_predefined_type - "
                         "Bad predefined type\n"),
                        -1);

    }

  return 0;
}

int
be_visitor_operation_interceptors_result::visit_sequence (be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  *os << "(*result_any) <<= this->_result;" << be_nl;

  return 0;
}

int
be_visitor_operation_interceptors_result::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  *os << "(*result_any) <<= ";

  // we need to make a distinction between bounded and unbounded strings
  if (node->max_size ()->ev ()->u.ulval != 0)
    {
      // bounded strings
      if (node->width () == (long) sizeof (char))
        {
          *os << "CORBA::Any::from_string ((char *)";
        }
      else
        {
          *os << "CORBA::Any::from_wstring ((CORBA::WChar *)";
        }

      *os <<"this->_result, "
          << node->max_size ()->ev ()->u.ulval
          << ");" << be_nl;
    }
  else
    {
      *os << "this->_result;" << be_nl;
    }

return 0;
}

int
be_visitor_operation_interceptors_result::visit_structure (be_structure *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  *os << "(*result_any) <<= this->_result;" << be_nl;

  return 0;

}

int
be_visitor_operation_interceptors_result::visit_union (be_union *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  *os << "(*result_any) <<= this->_result;" << be_nl;

  return 0;
}

int
be_visitor_operation_interceptors_result::visit_typedef (be_typedef *node)
{
  node->primitive_base_type ()->accept (this);

  return 0;
}
