//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    request_info_ch.cpp
//
// = DESCRIPTION
//    Visitor that generates the private members in Request_Info class for interceptors
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#include "idl.h"
#include "be.h"
#include "be_visitor_argument.h"

ACE_RCSID(be_visitor_argument, request_info_ch, "$Id$")


// ************************************************************
// be_visitor_args_request_info_ch for parameter list in method declarations and
// definitions
// ************************************************************

be_visitor_args_request_info_ch::be_visitor_args_request_info_ch (be_visitor_context *ctx)
  : be_visitor_args (ctx)
{
}

be_visitor_args_request_info_ch::~be_visitor_args_request_info_ch (void)
{
}

int be_visitor_args_request_info_ch::visit_argument (be_argument *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream
  this->ctx_->node (node); // save the argument node

  // retrieve the type
  be_type *bt = be_type::narrow_from_decl (node->field_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_request_info_ch::"
                         "visit_argument - "
                         "Bad argument type\n"),
                        -1);
    }

  // Different types have different mappings when used as in/out or
  // inout parameters. Let this visitor deal with the type

  if (bt->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_request_info_ch::"
                         "visit_argument - "
                         "cannot accept visitor\n"),
                        -1);
    }

  // As we visit each type we print out the &.
  *os << " " << node->local_name () << "_;" << be_nl;

  return 0;
}

int be_visitor_args_request_info_ch::visit_array (be_array *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      {
        *os << "const " << this->type_name (node) << "_slice *";

        break;
      }
    case AST_Argument::dir_INOUT:
      {
        *os << this->type_name (node) << "_slice *";

        break;
      }
    case AST_Argument::dir_OUT:
      {
        *os << this->type_name (node, "_out");

        break;
      }
    }

  return 0;
}

int be_visitor_args_request_info_ch::visit_enum (be_enum *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      {
        *os << "const " << this->type_name (node) << " &";

        break;
      }
    case AST_Argument::dir_INOUT:
      {
        *os << this->type_name (node) << " &";

        break;
      }
    case AST_Argument::dir_OUT:
      {
        *os << this->type_name (node, "_out");

        break;
      }

    }

  return 0;
}

int be_visitor_args_request_info_ch::visit_interface (be_interface *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << this->type_name (node, "_ptr");
      break;
    case AST_Argument::dir_INOUT: // inout
      *os << this->type_name (node, "_ptr");
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }
  return 0;
}

int be_visitor_args_request_info_ch::visit_interface_fwd (
    be_interface_fwd *node
  )
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << this->type_name (node, "_ptr");
      break;
    case AST_Argument::dir_INOUT: // inout
      *os << this->type_name (node, "_ptr");
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }
  return 0;
}

int be_visitor_args_request_info_ch::visit_native (be_native *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " &";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node) << " &";
      break;
    }
  return 0;
}

int be_visitor_args_request_info_ch::visit_predefined_type (
    be_predefined_type *node
  )
{
  TAO_OutStream *os = this->ctx_->stream (); // get output stream

  // Check if the type is an any.
  if (node->pt () == AST_PredefinedType::PT_any)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          {
            *os << "const " << this->type_name (node) << " &";

            break;
          }
        case AST_Argument::dir_INOUT:
          {
            *os << this->type_name (node) << " &";

            break;
          }
        case AST_Argument::dir_OUT:
          {
            *os << this->type_name (node, "_out");

            break;
          }
        } // end switch direction
    } // end of if
  else if (node->pt () == AST_PredefinedType::PT_pseudo) // e.g., CORBA::Object
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          *os << this->type_name (node, "_ptr");
          break;
        case AST_Argument::dir_INOUT:
          *os << this->type_name (node, "_ptr");
          break;
        case AST_Argument::dir_OUT:
          *os << this->type_name (node, "_out");
          break;
        } // end switch direction
    } // end else if
  else // Simple predefined types.
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          {
            *os << "const " << this->type_name (node) << " &";

            break;
          }
        case AST_Argument::dir_INOUT:
          {
            *os << this->type_name (node) << " &";

            break;
          }
        case AST_Argument::dir_OUT:
          {
            *os << this->type_name (node, "_out");

            break;
          }
        } // end switch direction
    } // end of else

  return 0;
}

int be_visitor_args_request_info_ch::visit_sequence (be_sequence *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      {
        *os << "const " << this->type_name (node) << " &";
        break;
      }
    case AST_Argument::dir_INOUT:
      {
        *os << this->type_name (node) << " &";
        break;
      }
    case AST_Argument::dir_OUT:
      {
        *os << this->type_name (node, "_out");
        break;
      }

    }

  return 0;
}

int be_visitor_args_request_info_ch::visit_string (be_string *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  if (node->width () == 1)
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          *os << "const char *";
          break;
        case AST_Argument::dir_INOUT:
          *os << "char *";
          break;
        case AST_Argument::dir_OUT:
          *os << "CORBA::String_out ";
          break;
        }
    }
  else
    {
      switch (this->direction ())
        {
        case AST_Argument::dir_IN:
          *os << "const CORBA::WChar *";
          break;
        case AST_Argument::dir_INOUT:
          *os << "CORBA::WChar *";
          break;
        case AST_Argument::dir_OUT:
          *os << "CORBA::WString_out ";
          break;
        }
    }

  return 0;
}

int be_visitor_args_request_info_ch::visit_structure (be_structure *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      {
        *os << "const " << this->type_name (node) << " &";

        break;
      }
    case AST_Argument::dir_INOUT:
      {
        *os << this->type_name (node) << " &";

        break;
      }
    case AST_Argument::dir_OUT:
      {
        *os << this->type_name (node, "_out");

        break;
      }

    }
  return 0;
}

int be_visitor_args_request_info_ch::visit_union (be_union *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      {
        *os << "const " << this->type_name (node) << " &";

        break;
      }
    case AST_Argument::dir_INOUT:
      {
        *os << this->type_name (node) << " &";

        break;
      }
    case AST_Argument::dir_OUT:
      {
        *os << this->type_name (node, "_out");

        break;
      }

    }
  return 0;
}

int be_visitor_args_request_info_ch::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_args_request_info_ch::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}


#ifdef IDL_HAS_VALUETYPE

int be_visitor_args_request_info_ch::visit_valuetype (be_valuetype *node)
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << this->type_name (node) << " *";
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " *&";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }
  return 0;
}

int be_visitor_args_request_info_ch::visit_valuetype_fwd (
    be_valuetype_fwd *node
  )
{
  TAO_OutStream *os = this->ctx_->stream (); // get the stream

  switch (this->direction ())
    {
    case AST_Argument::dir_IN:
      *os << "const " << this->type_name (node) << " *";
      break;
    case AST_Argument::dir_INOUT:
      *os << this->type_name (node) << " *&";
      break;
    case AST_Argument::dir_OUT:
      *os << this->type_name (node, "_out");
      break;
    }
  return 0;
}

#endif /* IDL_HAS_VALUETYPE */
