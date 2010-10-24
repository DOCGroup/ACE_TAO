// $Id$

#include "be_visitor_null_return_value.h"

#include "be_visitor_context.h"
#include "be_helper.h"
#include "be_array.h"
#include "be_component.h"
#include "be_enum.h"
#include "be_eventtype.h"
#include "be_home.h"
#include "be_predefined_type.h"
#include "be_sequence.h"
#include "be_string.h"
#include "be_structure.h"
#include "be_typedef.h"
#include "be_union.h"
#include "be_valuebox.h"
#include "be_valuetype.h"

be_visitor_null_return_value::be_visitor_null_return_value (
    be_visitor_context *ctx)
  : be_visitor_decl (ctx),
    os_ (*ctx->stream ())
{
}

be_visitor_null_return_value::~be_visitor_null_return_value (void)
{
}

int
be_visitor_null_return_value::visit_array (be_array *)
{
  return 0;
}

int
be_visitor_null_return_value::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_null_return_value::visit_enum (be_enum *node)
{
  os_ << "static_cast< ::" << node->full_name () << "> (0UL)";

  return 0;
}

int
be_visitor_null_return_value::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int
be_visitor_null_return_value::visit_home (be_home *node)
{
  return this->visit_interface (node);
}

int
be_visitor_null_return_value::visit_interface (be_interface *node)
{
  os_ << " ::" << node->full_name () << "::_nil ()";

  return 0;
}

int
be_visitor_null_return_value::visit_predefined_type (be_predefined_type *node)
{
  switch (node->pt ())
    {
      case AST_PredefinedType::PT_boolean:
        os_ << "false";
        break;
      case AST_PredefinedType::PT_octet:
      case AST_PredefinedType::PT_char:
      case AST_PredefinedType::PT_wchar:
      case AST_PredefinedType::PT_short:
      case AST_PredefinedType::PT_ushort:
      case AST_PredefinedType::PT_long:
      case AST_PredefinedType::PT_ulong:
      case AST_PredefinedType::PT_ulonglong:
      case AST_PredefinedType::PT_value:
      case AST_PredefinedType::PT_any:
        os_ << "0";
        break;
      case AST_PredefinedType::PT_longlong:
        os_ << "ACE_CDR_LONGLONG_INITIALIZER";
        break;
      case AST_PredefinedType::PT_float:
        os_ << "0.0f";
        break;
      case AST_PredefinedType::PT_double:
        os_ << "0.0";
        break;
      case AST_PredefinedType::PT_longdouble:
        os_ << "ACE_CDR_LONG_DOUBLE_INITIALIZER";
        break;
      case AST_PredefinedType::PT_object:
        os_ << " ::CORBA::Object::_nil ()";
        break;
      case AST_PredefinedType::PT_abstract:
        os_ << " ::CORBA::AbstractBase::_nil ()";
        break;
      case AST_PredefinedType::PT_pseudo:
        os_ << " ::CORBA::TypeCode::_nil ()";
        break;
      default: // PT_void not handled.
        break;
    }

  return 0;
}

int
be_visitor_null_return_value::visit_sequence (be_sequence *)
{
  return 0;
}

int
be_visitor_null_return_value::visit_string (be_string *)
{
  return 0;
}

int
be_visitor_null_return_value::visit_structure (be_structure *)
{
  return 0;
}

int
be_visitor_null_return_value::visit_typedef (be_typedef *node)
{
  return node->primitive_base_type ()->accept (this);
}

int
be_visitor_null_return_value::visit_union (be_union *)
{
  return 0;
}

int
be_visitor_null_return_value::visit_valuebox (be_valuebox *node)
{
  os_ << "static_cast< ::" << node->full_name ()
      << " *> (0)";

  return 0;
}

int
be_visitor_null_return_value::visit_valuetype (be_valuetype *node)
{
  os_ << "static_cast< ::" << node->full_name ()
      << " *> (0)";

  return 0;
}

