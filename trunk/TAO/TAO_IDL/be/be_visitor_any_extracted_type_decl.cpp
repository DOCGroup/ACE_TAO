// $Id$

#include "be_visitor_any_extracted_type_decl.h"

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

#include "ast_root.h"

#include "utl_identifier.h"

be_visitor_any_extracted_type_decl::be_visitor_any_extracted_type_decl (
    be_visitor_context *ctx)
  : be_visitor_decl (ctx),
    os_ (*ctx->stream ()),
    var_name_ ("_extract_val"),
    tmp_name_ ("_extract_tmp")
{
}

be_visitor_any_extracted_type_decl::~be_visitor_any_extracted_type_decl (
  void)
{
}

int
be_visitor_any_extracted_type_decl::visit_array (be_array *node)
{
  os_ << node->full_name () << "_forany " << var_name_ << ";";
  return 0;
}

int
be_visitor_any_extracted_type_decl::visit_component (be_component *node)
{
  os_ << node->full_name () << "_ptr " << var_name_ << " = "
      << node->full_name () << "::_nil ();";
  return 0;
}

int
be_visitor_any_extracted_type_decl::visit_enum (be_enum *node)
{
  os_ << node->full_name () << " " << var_name_ << ";";
  return 0;
}

int
be_visitor_any_extracted_type_decl::visit_eventtype (
  be_eventtype *node)
{
  os_ << node->full_name () << " * " << var_name_ << " = 0;";
  return 0;
}

int
be_visitor_any_extracted_type_decl::visit_home (be_home *node)
{
  os_ << node->full_name () << "_ptr " << var_name_ << " = "
      << node->full_name () << "::_nil ();";
  return 0;
}

int
be_visitor_any_extracted_type_decl::visit_interface (
  be_interface *node)
{
  os_ << node->full_name () << "_ptr " << var_name_ << " = "
      << node->full_name () << "::_nil ();";
  return 0;
}

int
be_visitor_any_extracted_type_decl::visit_predefined_type (
  be_predefined_type *node)
{
  os_ << node->full_name () << " ";

  switch (node->pt ())
    {
      case AST_PredefinedType::PT_boolean:
        os_ << tmp_name_ << " = false;" << be_nl
            << "::CORBA::Any::to_boolean " << var_name_
            << " (" << tmp_name_ << ");";
        break;
      case AST_PredefinedType::PT_octet:
        os_ << tmp_name_ << " = 0;" << be_nl
            << "::CORBA::Any::to_octet " << var_name_
            << " (" << tmp_name_ << ");";
        break;
      case AST_PredefinedType::PT_char:
        os_ << tmp_name_ << " = 0;" << be_nl
            << "::CORBA::Any::to_char " << var_name_
            << " (" << tmp_name_ << ");";
        break;
      case AST_PredefinedType::PT_wchar:
        os_ << tmp_name_ << " = 0;" << be_nl
            << "::CORBA::Any::to_wchar " << var_name_
            << " (" << tmp_name_ << ");";
        break;
      case AST_PredefinedType::PT_short:
        os_ << var_name_ << " = 0;";
        break;
      case AST_PredefinedType::PT_ushort:
        os_ << var_name_ << " = 0;";
        break;
      case AST_PredefinedType::PT_long:
        os_ << var_name_ << " = 0;";
        break;
      case AST_PredefinedType::PT_ulong:
        os_ << var_name_ << " = 0;";
        break;
      case AST_PredefinedType::PT_longlong:
        os_ << var_name_ << " = ACE_CDR_LONGLONG_INITIALIZER;";
        break;
      case AST_PredefinedType::PT_ulonglong:
        os_ << var_name_ << " = 0;";
        break;
      case AST_PredefinedType::PT_float:
        os_ << var_name_ << " = 0;";
        break;
      case AST_PredefinedType::PT_double:
        os_ << var_name_ << " = 0;";
        break;
      case AST_PredefinedType::PT_longdouble:
        os_ << var_name_ << " = ACE_CDR_LONG_DOUBLE_INITIALIZER;";
        break;
      case AST_PredefinedType::PT_object:
        os_ << "_ptr " << var_name_ << " = ::CORBA::Object::_nil ();";
        break;
      case AST_PredefinedType::PT_value:
        os_ << " * " << var_name_ << " = 0;";
        break;
      case AST_PredefinedType::PT_abstract:
        os_ << "_ptr " << var_name_ << " = ::CORBA::AbstractBase::_nil ();";
        break;
      case AST_PredefinedType::PT_any:
        os_ << " * " << var_name_ << " = 0;";
        break;
      case AST_PredefinedType::PT_pseudo:
        if (ACE_OS::strcmp (node->local_name ()->get_string (), "TCKind") == 0)
          {
            os_ << var_name_ << " = ::CORBA::tk_null;";
          }
        else
          {
            os_ << "_ptr " << var_name_
                << " = ::CORBA::TypeCode::_nil ();";
          }

        break;
      default: // PT_void is left out of the case list.
        break;
    }

  return 0;
}

int
be_visitor_any_extracted_type_decl::visit_sequence (be_sequence *node)
{
  be_type *bt = this->ctx_->alias ();

  if (bt == 0)
    {
      bt = node;
    }

  os_ << "const " << bt->nested_type_name (idl_global->root ())
      << " * " << var_name_ << " = 0;";

  return 0;
}

int
be_visitor_any_extracted_type_decl::visit_string (be_string *node)
{
  os_ << "const " << node->full_name () << " " << var_name_ << " = 0;";
  return 0;
}

int
be_visitor_any_extracted_type_decl::visit_structure (be_structure *node)
{
  os_ << "const " << node->full_name () << " * " << var_name_ << " = 0;";
  return 0;
}

int
be_visitor_any_extracted_type_decl::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  int status = node->primitive_base_type ()->accept (this);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_any_extracted_type_decl::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_any_extracted_type_decl::visit_union (be_union *node)
{
  os_ << node->full_name () << " * " << var_name_ << " = 0;";
  return 0;
}

int
be_visitor_any_extracted_type_decl::visit_valuebox (be_valuebox *node)
{
  os_ << node->full_name () << " * " << var_name_ << " = 0;";
  return 0;
}

int
be_visitor_any_extracted_type_decl::visit_valuetype (be_valuetype *node)
{
  os_ << node->full_name () << " * " << var_name_ << " = 0;";
  return 0;
}

