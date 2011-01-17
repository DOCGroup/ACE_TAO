// $Id$

#include "be_visitor_attr_assign.h"

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

be_visitor_attr_assign::be_visitor_attr_assign  (
      be_visitor_context *ctx)
  : be_visitor_decl (ctx),
    attr_name_ (0),
    attr_name_string_ ("this->")
{
}

be_visitor_attr_assign::~be_visitor_attr_assign (void)
{
}

int
be_visitor_attr_assign::visit_array (be_array *node)
{
  os_ << be_nl
      << this->attr_name_string_.c_str () << " = "
      << "::" << node->full_name () << "_dup ("
      << this->attr_name_ << ");";

  return 0;
}

int
be_visitor_attr_assign::visit_component (be_component *node)
{
  os_ << be_nl
      << this->attr_name_string_.c_str () << " = "
      << "::" << node->full_name () << "::_duplicate ("
      << this->attr_name_ << ");";

  return 0;
}

int
be_visitor_attr_assign::visit_enum (be_enum *)
{
  os_ << be_nl
      << this->attr_name_string_.c_str () << " = "
      << this->attr_name_ << ";";

  return 0;
}

int
be_visitor_attr_assign::visit_eventtype (be_eventtype *)
{
  os_ << be_nl
      << this->attr_name_ << "->_add_ref ();" << be_nl
      << this->attr_name_string_.c_str () << " = "
      << this->attr_name_ << ";";

  return 0;
}

int
be_visitor_attr_assign::visit_home (be_home *node)
{
  os_ << be_nl
      << this->attr_name_string_.c_str () << " = "
      << "::" << node->full_name () << "::_duplicate ("
      << this->attr_name_ << ");";

  return 0;
}

int
be_visitor_attr_assign::visit_interface (
  be_interface *node)
{
  os_ << be_nl
      << this->attr_name_string_.c_str () << " = "
      << "::" << node->full_name () << "::_duplicate ("
      << this->attr_name_ << ");";

  return 0;
}

int
be_visitor_attr_assign::visit_predefined_type (
  be_predefined_type *)
{
  os_ << be_nl
      << this->attr_name_string_.c_str () << " = "
      << this->attr_name_ << ";";

  return 0;
}

/// Unused if anonymous types are not allowed.
int
be_visitor_attr_assign::visit_sequence (be_sequence *node)
{
  os_ << be_nl
      << "::" << node->full_name () << " * _ciao_tmp = 0;" << be_nl
      << "ACE_NEW (" << be_idt_nl
      << "_ciao_tmp," << be_nl
      << node->full_name () << " (" << this->attr_name_
      << "));" << be_uidt_nl << be_nl
      << this->attr_name_string_.c_str () << " = _ciao_tmp;";

  return 0;
}

int
be_visitor_attr_assign::visit_string (be_string *node)
{
  os_ << be_nl
      << this->attr_name_string_.c_str () << " = "
      << "::CORBA::" << (node->width () == sizeof (char) ? "" : "w")
      << "string_dup (" << this->attr_name_ << ");";

  return 0;
}

int
be_visitor_attr_assign::visit_structure (be_structure *node)
{
  os_ << be_nl
      << "::" << node->full_name () << " * _ciao_tmp = 0;" << be_nl
      << "ACE_NEW (" << be_idt_nl
      << "_ciao_tmp," << be_nl
      << "::" << node->full_name () << ");" << be_uidt_nl << be_nl
      << "*_ciao_tmp = " << this->attr_name_ << ";" << be_nl
      << this->attr_name_string_.c_str () << " = _ciao_tmp;";

  return 0;
}

int
be_visitor_attr_assign::visit_typedef (be_typedef *node)
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
be_visitor_attr_assign::visit_union (be_union *node)
{
  os_ << be_nl
      << "::" << node->full_name () << " * _ciao_tmp = 0;" << be_nl
      << "ACE_NEW (" << be_idt_nl
      << "_ciao_tmp," << be_nl
      << "::" << node->full_name () << ");" << be_uidt_nl << be_nl
      << "*_ciao_tmp = " << this->attr_name_ << ";" << be_nl
      << this->attr_name_string_.c_str () << " = _ciao_tmp;";

  return 0;
}

int
be_visitor_attr_assign::visit_valuebox (be_valuebox *)
{
  os_ << be_nl
      << this->attr_name_ << "->_add_ref ();" << be_nl
      << this->attr_name_string_.c_str () << " = "
      << this->attr_name_ << ";";

  return 0;
}

int
be_visitor_attr_assign::visit_valuetype (be_valuetype *)
{
  os_ << be_nl
      << this->attr_name_ << "->_add_ref ();" << be_nl
      << this->attr_name_string_.c_str () << " = "
      << this->attr_name_ << ";";

  return 0;
}

void
be_visitor_attr_assign::attr_name (const char *name)
{
  // No need for copying, the lifetime of the attr
  // the arg is from is longer than that of this visitor.
  this->attr_name_ = name;

  // Add to this string for convenience, it's generated
  // in each method above at least once.
  this->attr_name_string_ += this->ctx_->port_prefix ();
  this->attr_name_string_ += name;
  this->attr_name_string_ += '_';
}
