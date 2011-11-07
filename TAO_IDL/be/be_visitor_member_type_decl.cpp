// $Id$

#include "be_visitor_member_type_decl.h"

#include "be_visitor_context.h"
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

be_visitor_member_type_decl::be_visitor_member_type_decl  (
      be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_member_type_decl::~be_visitor_member_type_decl (void)
{
}

int
be_visitor_member_type_decl::visit_array (be_array *)
{
  be_type *bt = this->ctx_->alias ();

  if (bt == 0)
    {
      /// Support anonymous types?
      return -1;
    }

  os_ << "::" << bt->full_name () << "_var ";

  return 0;
}

int
be_visitor_member_type_decl::visit_component (be_component *node)
{
  os_ << "::" << node->full_name () << "_var ";
  return 0;
}

int
be_visitor_member_type_decl::visit_enum (be_enum *node)
{
  os_ << "::" << node->full_name () << " ";
  return 0;
}

int
be_visitor_member_type_decl::visit_eventtype (
  be_eventtype *node)
{
  os_ << "::" << node->full_name () << " * ";
  return 0;
}

int
be_visitor_member_type_decl::visit_home (be_home *node)
{
  os_ << "::" << node->full_name () << "_var ";
  return 0;
}

int
be_visitor_member_type_decl::visit_interface (
  be_interface *node)
{
  os_ << "::" << node->full_name () << "_var ";
  return 0;
}

int
be_visitor_member_type_decl::visit_predefined_type (
  be_predefined_type *node)
{
  os_ << "::" << node->full_name ();

  switch (node->pt ())
    {
      case AST_PredefinedType::PT_abstract:
      case AST_PredefinedType::PT_object:
      case AST_PredefinedType::PT_pseudo:
      case AST_PredefinedType::PT_any:
        os_ << "_var";
        break;
      case AST_PredefinedType::PT_value:
        os_ << " *";
        break;
      default:
        break;
    }

  os_ << " ";

  return 0;
}

/// Unused if anonymous types are not allowed.
int
be_visitor_member_type_decl::visit_sequence (be_sequence *)
{
  be_type *bt = this->ctx_->alias ();

  if (bt == 0)
    {
      /// Support anonymous types?
      return -1;
    }

  os_ << "::" << bt->full_name () << "_var ";

  return 0;
}

int
be_visitor_member_type_decl::visit_string (be_string *node)
{
  os_ << "::CORBA::" << (node->width () == 1 ? "" : "W")
      << "String_var ";
  return 0;
}

int
be_visitor_member_type_decl::visit_structure (be_structure *node)
{
  os_ << "::" << node->full_name () << "_var ";
  return 0;
}

int
be_visitor_member_type_decl::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);
  int status = node->primitive_base_type ()->accept (this);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_memkber_type_decl::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }

  this->ctx_->alias (0);
  return 0;
}

int
be_visitor_member_type_decl::visit_union (be_union *node)
{
  os_ << "::" << node->full_name () << "_var ";
  return 0;
}

int
be_visitor_member_type_decl::visit_valuebox (be_valuebox *node)
{
  os_ << "::" << node->full_name () << "_var ";
  return 0;
}

int
be_visitor_member_type_decl::visit_valuetype (be_valuetype *node)
{
  os_ << "::" << node->full_name () << "_var ";
  return 0;
}

