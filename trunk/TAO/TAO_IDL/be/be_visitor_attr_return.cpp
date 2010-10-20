// $Id$

#include "be_visitor_attr_return.h"

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

be_visitor_attr_return::be_visitor_attr_return  (
      be_visitor_context *ctx)
  : be_visitor_decl (ctx),
    attr_name_string_ ("this->")
{
}

be_visitor_attr_return::~be_visitor_attr_return (void)
{
}

int
be_visitor_attr_return::visit_array (be_array *node)
{
  os_ << be_nl
      << "return" << be_idt_nl
      << "::" << node->full_name () << "_dup ("
      << this->attr_name_string_.c_str ()
      << ".in ());" << be_uidt;

  return 0;
}

int
be_visitor_attr_return::visit_component (be_component *node)
{
  os_ << be_nl
      << "return" << be_idt_nl
      << "::" << node->full_name () << "::_duplicate ("
      << this->attr_name_string_.c_str ()
      << ".in ());" << be_uidt;
      
  return 0;
}

int
be_visitor_attr_return::visit_enum (be_enum *)
{
  os_ << be_nl
      << "return " << this->attr_name_string_.c_str () << ";";
      
  return 0;
}

int
be_visitor_attr_return::visit_eventtype (be_eventtype *)
{
  os_ << be_nl
      << "this->" << this->attr_name_string_.c_str ()
      << "->_add_ref ();" << be_nl << be_nl
      << "return" << be_idt_nl
      << "this->" << this->attr_name_string_.c_str ()
      << ".in ();" << be_uidt;
      
  return 0;
}

int
be_visitor_attr_return::visit_home (be_home *node)
{
  os_ << be_nl
      << "return" << be_idt_nl
      << "::" << node->full_name () << "::_duplicate ("
      << this->attr_name_string_.c_str ()
      << ".in ());" << be_uidt;
      
  return 0;
}

int
be_visitor_attr_return::visit_interface (
  be_interface *node)
{
  os_ << be_nl
      << "return" << be_idt_nl
      << "::" << node->full_name () << "::_duplicate ("
      << this->attr_name_string_.c_str ()
      << ".in ());" << be_uidt;
      
  return 0;
}

int
be_visitor_attr_return::visit_predefined_type (
  be_predefined_type *node)
{
  os_ << be_nl;
      
  switch (node->pt ())
    {
      case AST_PredefinedType::PT_abstract:
      case AST_PredefinedType::PT_object:
      case AST_PredefinedType::PT_pseudo:
        os_ << "return" << be_idt_nl
            << "::" << node->full_name ()
            << "::_duplicate (" << be_idt_nl
            << this->attr_name_string_.c_str () << ".in ());"
            << be_uidt << be_uidt;
        break;
      case AST_PredefinedType::PT_any:
        os_ << "::CORBA::Any * retval = 0;" << be_nl
            << "ACE_NEW_RETURN (" << be_idt_nl
            << "retval," << be_nl
            << "::CORBA::Any ("
            << this->attr_name_string_.c_str ()
            << ".in ())," << be_nl
            << "0);" << be_uidt_nl << be_nl
            << "return retval;";
        break;
      case AST_PredefinedType::PT_value:
        os_ << "::CORBA::ValueBase * retval =" << be_idt_nl
            << this->attr_name_string_.c_str () << ".in ();"
            << be_uidt_nl
            << "retval->_add_ref ();" << be_nl
            << "return retval;";
        break;
      default:
        os_ << "return" << be_idt_nl
            << this->attr_name_string_.c_str ()
            << ";" << be_uidt;
        break;
    }
  
  return 0;
}

/// Unused if anonymous types are not allowed.
int
be_visitor_attr_return::visit_sequence (be_sequence *node)
{
  os_ << be_nl
      << "::" << node->full_name () << " * retval = 0;" << be_nl
      << "ACE_NEW_RETURN (" << be_idt_nl
      << "retval," << be_nl
      << "::" << node->full_name () << " (" << be_idt_nl
      << this->attr_name_string_.c_str ()
      << ".in ())," << be_uidt_nl
      << "0);" << be_uidt_nl << be_nl
      << "return retval;";

  return 0;
}

int
be_visitor_attr_return::visit_string (be_string *node)
{
  os_ << be_nl
      << "return" << be_idt_nl
      << "::CORBA::"
      << (node->width () == sizeof (char) ? "" : "w")
      << "string_dup (" << this->attr_name_string_.c_str ()
      << ".in ());" << be_uidt;
    
  return 0;
}

int
be_visitor_attr_return::visit_structure (be_structure *node)
{
  os_ << be_nl;
  
  if (node->size_type () == AST_Type::FIXED)
    {
      os_ << "::" << node->full_name () << " retval ="
          << be_idt_nl
          << this->attr_name_string_.c_str () << ".in ();"
          << be_uidt_nl << be_nl
          << "return retval;";
    }
  else
    {
      os_ << node->full_name () << " * retval = 0;" << be_nl
          << "ACE_NEW_RETURN (" << be_idt_nl
          << "retval," << be_nl
          << "::" << node->full_name () << "," << be_nl
          << "0);" << be_uidt_nl << be_nl
          << "*retval =" << be_idt_nl
          << this->attr_name_string_.c_str () << ".in ();"
          << be_uidt_nl << be_nl
          << "return retval;";
    }
    
  return 0;
}

int
be_visitor_attr_return::visit_typedef (be_typedef *node)
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
be_visitor_attr_return::visit_union (be_union *node)
{
  os_ << be_nl;
  
  if (node->size_type () == AST_Type::FIXED)
    {
      os_ << "::" << node->full_name () << " retval ="
          << be_idt_nl
          << this->attr_name_string_.c_str () << ".in ();"
          << be_uidt_nl << be_nl
          << "return retval;";
    }
  else
    {
      os_ << node->full_name () << " * retval = 0;" << be_nl
          << "ACE_NEW_RETURN (" << be_idt_nl
          << "retval," << be_nl
          << "::" << node->full_name () << "," << be_nl
          << "0);" << be_uidt_nl << be_nl
          << "*retval =" << be_idt_nl
          << this->attr_name_string_.c_str () << ".in ();"
          << be_uidt_nl << be_nl
          << "return retval;";
    }
    
  return 0;
}

int
be_visitor_attr_return::visit_valuebox (be_valuebox *)
{
  os_ << be_nl
      << this->attr_name_string_.c_str () << "->_add_ref ();"
      << be_nl << be_nl
      << "return" << be_idt_nl
      << this->attr_name_string_.c_str ()
      << ".in ();" << be_uidt;
      
  return 0;
}

int
be_visitor_attr_return::visit_valuetype (be_valuetype *)
{
  os_ << be_nl
      << this->attr_name_string_.c_str () << "->_add_ref ();"
      << be_nl << be_nl
      << "return" << be_idt_nl
      << this->attr_name_string_.c_str ()
      << ".in ();" << be_uidt;
      
  return 0;
}

void
be_visitor_attr_return::attr_name (const char *name)
{
  this->attr_name_string_ += this->ctx_->port_prefix ();
  this->attr_name_string_ += name;
  this->attr_name_string_ += '_';
}
