// $Id$

#include "be_visitor_attr_setarg_type.h"

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

be_visitor_attr_setarg_type::be_visitor_attr_setarg_type  (
      be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_attr_setarg_type::~be_visitor_attr_setarg_type (void)
{
}

int
be_visitor_attr_setarg_type::visit_array (be_array *node)
{
  be_type *bt = this->ctx_->alias ();

  if (bt == 0)
    {
      /// Support anonymous types?
      return -1;
    }

  os_ << "const ::" << bt->full_name () << " ";

  return 0;
}

int
be_visitor_attr_setarg_type::visit_component (be_component *node)
{
  os_ << "::" << node->full_name () << "_ptr ";
  return 0;
}

int
be_visitor_attr_setarg_type::visit_enum (be_enum *node)
{
  os_ << "const ::" << node->full_name () << " ";
  return 0;
}

int
be_visitor_attr_setarg_type::visit_eventtype (
  be_eventtype *node)
{
  os_ << "::" << node->full_name () << " * ";
  return 0;
}

int
be_visitor_attr_setarg_type::visit_home (be_home *node)
{
  os_ << "::" << node->full_name () << "_ptr ";
  return 0;
}

int
be_visitor_attr_setarg_type::visit_interface (
  be_interface *node)
{
  os_ << "::" << node->full_name () << "_ptr ";
  return 0;
}

int
be_visitor_attr_setarg_type::visit_predefined_type (
  be_predefined_type *node)
{
  switch (node->pt ())
    {
      case AST_PredefinedType::PT_abstract:
      case AST_PredefinedType::PT_object:
      case AST_PredefinedType::PT_pseudo:
        os_ << "::" << node->full_name () << "_ptr";
        break;
      case AST_PredefinedType::PT_any:
      case AST_PredefinedType::PT_value:
        os_ << "::" << node->full_name () << " * ";
        break;
      default:
        os_ << "const ::" << node->full_name () << " ";
        break;
    }
    
  return 0;
}

/// Unused if anonymous types are not allowed.
int
be_visitor_attr_setarg_type::visit_sequence (be_sequence *node)
{
  be_type *bt = this->ctx_->alias ();

  if (bt == 0)
    {
      /// Support anonymous types?
      return -1;
    }

  os_ << "const " << bt->full_name () << " & ";

  return 0;
}

int
be_visitor_attr_setarg_type::visit_string (be_string *node)
{
  if (node->width () == sizeof (char))
    {
      os_ << "const char * ";
    }
  else
    {
      os_ << "const ::CORBA::WChar * ";
    }
    
  return 0;
}

int
be_visitor_attr_setarg_type::visit_structure (be_structure *node)
{
  os_ << "const ::" << node->full_name () << " & ";
  
  return 0;
}

int
be_visitor_attr_setarg_type::visit_typedef (be_typedef *node)
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
be_visitor_attr_setarg_type::visit_union (be_union *node)
{
  os_ << "const ::" << node->full_name () << " & ";
  return 0;
}

int
be_visitor_attr_setarg_type::visit_valuebox (be_valuebox *node)
{
  os_ << "::" << node->full_name () << " * ";
  return 0;
}

int
be_visitor_attr_setarg_type::visit_valuetype (be_valuetype *node)
{
  os_ << "::" << node->full_name () << " * ";
  return 0;
}

