//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    request_info_result.cpp
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
#include "be_visitor_argument.h"

ACE_RCSID(be_visitor_argument, request_info_result, "$Id$")


// ************************************************************
// be_visitor_args_request_info_result for generating the result as
// stored in the request info for interceptors
// ************************************************************

be_visitor_args_request_info_result::be_visitor_args_request_info_result (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_request_info_result::~be_visitor_args_request_info_result (void)
{
}

int be_visitor_args_request_info_result::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  this->ctx_->node (node); // save the argument node
  be_type *bt;

  os->indent ();

  // retrieve the type
  bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_::"
                         "visit_argument - "
                         "Bad return type\n"),
                        -1);
    }

    if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_vardecl_ss::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  *os << be_nl;
  // Set the appropriate mode for each parameter.
  return 0;
}

int be_visitor_args_request_info_result::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // if the current type is an alias, use that
  be_type *bt;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;

  os->indent ();
  *os << bt->name () <<  "_forany _tao_forany_result"
      << " (this->result_);" << be_nl
      << "this->result_val_ <<= _tao_forany_result;"<< be_nl;
  return 0;
}

int be_visitor_args_request_info_result::visit_enum (be_enum *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  os->indent ();
  *os << "this->result_val_ <<= this->result_;";

  return 0;
}

int be_visitor_args_request_info_result::visit_interface (be_interface *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  os->indent ();
  *os << "this->result_val_ <<= this->result_;";

  return 0;
}

int be_visitor_args_request_info_result::visit_interface_fwd (be_interface_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  os->indent ();
  *os << "this->result_val_ <<= this->result_;";

  return 0;

}

int be_visitor_args_request_info_result::visit_valuetype (be_valuetype *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  os->indent ();
  *os << "this->result_val_ <<= this->result_;";

  return 0;

}

int be_visitor_args_request_info_result::visit_valuetype_fwd (be_valuetype_fwd *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  os->indent ();
  *os << "this->result_val_ <<= this->result_;";

  return 0;

}

int
be_visitor_args_request_info_result::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  os->indent ();
  *os << "this->result_val_ <<= ";
  switch (node->pt ())
    {
    case AST_PredefinedType::PT_boolean:
      *os << "CORBA::Any::from_boolean (this->result_);" << be_nl;
      break;
    case AST_PredefinedType::PT_char:
      *os << "CORBA::Any::from_char (this->result_);"<<be_nl;
      break;
    case AST_PredefinedType::PT_wchar:
      *os << "CORBA::Any::from_wchar (this->result_);"<<be_nl;
      break;
    case AST_PredefinedType::PT_octet:
      *os << "CORBA::Any::from_octet (this->result_);"<<be_nl;
      break;

    default:
      *os << "this->result_;"<<be_nl;
      break;
    }
  return 0;

}

int be_visitor_args_request_info_result::visit_sequence (be_sequence *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  os->indent ();
  *os << "this->result_val_ <<= this->result_;";

  return 0;

}

int
be_visitor_args_request_info_result::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  os->indent ();
  *os << "this->result_val_ <<= ";
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
      *os <<"this->result_, "
          << node->max_size ()->ev ()->u.ulval
          << ");";
    }
  else
      *os << "this->result_; ";
return 0;
}

int be_visitor_args_request_info_result::visit_structure (be_structure *)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  os->indent ();
  *os << "this->result_val_ <<= this->result_;";

  return 0;

}

int be_visitor_args_request_info_result::visit_union (be_union *)
{
 TAO_OutStream *os = this->ctx_->stream (); // get output stream

  os->indent ();
  *os << "this->result_val_ <<= this->result_;";

  return 0;

}

int be_visitor_args_request_info_result::visit_typedef (be_typedef *)
{
 TAO_OutStream *os = this->ctx_->stream (); // get output stream

  os->indent ();
  *os << "this->result_val_ <<= this->result_;";

  return 0;

}
