
//=============================================================================
/**
 *  @file    set_from_extracted.cpp
 *
 *  $Id$
 *
 *  Visitor for generation of arguments to attribute mutators
 *  that were first extracted from an Any
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_attribute_set_from_extracted::be_visitor_attribute_set_from_extracted (
    be_visitor_context *ctx)
  : be_visitor_any_extracted_type_decl (ctx)
{
}

be_visitor_attribute_set_from_extracted::~be_visitor_attribute_set_from_extracted (
  void)
{
}

int
be_visitor_attribute_set_from_extracted::visit_array (
  be_array *)
{
  os_ << var_name_ << ".in ()";
  return 0;
}

int
be_visitor_attribute_set_from_extracted::visit_component (
  be_component *)
{
  os_ << var_name_;
  return 0;
}

int
be_visitor_attribute_set_from_extracted::visit_enum (
  be_enum *)
{
  os_ << var_name_;
  return 0;
}

int
be_visitor_attribute_set_from_extracted::visit_eventtype (
  be_eventtype *)
{
  os_ << var_name_;
  return 0;
}

int
be_visitor_attribute_set_from_extracted::visit_home (
  be_home *)
{
  os_ << var_name_;
  return 0;
}

int
be_visitor_attribute_set_from_extracted::visit_interface (
  be_interface *)
{
  os_ << var_name_;
  return 0;
}

int
be_visitor_attribute_set_from_extracted::visit_predefined_type (
  be_predefined_type *node)
{
  switch (node->pt ())
    {
      case AST_PredefinedType::PT_boolean:
      case AST_PredefinedType::PT_octet:
      case AST_PredefinedType::PT_char:
      case AST_PredefinedType::PT_wchar:
        os_ << var_name_ << ".ref_";
        break;
      case AST_PredefinedType::PT_short:
      case AST_PredefinedType::PT_ushort:
      case AST_PredefinedType::PT_long:
      case AST_PredefinedType::PT_ulong:
      case AST_PredefinedType::PT_longlong:
      case AST_PredefinedType::PT_ulonglong:
      case AST_PredefinedType::PT_float:
      case AST_PredefinedType::PT_double:
      case AST_PredefinedType::PT_longdouble:
      case AST_PredefinedType::PT_object:
      case AST_PredefinedType::PT_value:
      case AST_PredefinedType::PT_abstract:
      case AST_PredefinedType::PT_pseudo:
        os_ << var_name_;
        break;
      case AST_PredefinedType::PT_any:
        os_ << "*" << var_name_;
        break;
      default: // PT_void left out of case list.
        break;
    }

  return 0;
}

int
be_visitor_attribute_set_from_extracted::visit_sequence (
  be_sequence *)
{
  os_ << "*" << var_name_;
  return 0;
}

int
be_visitor_attribute_set_from_extracted::visit_string (
  be_string *)
{
  os_ << var_name_;
  return 0;
}

int
be_visitor_attribute_set_from_extracted::visit_structure (
  be_structure *)
{
  os_ << "*" << var_name_;
  return 0;
}

int
be_visitor_attribute_set_from_extracted::visit_typedef (
  be_typedef *node)
{
  be_type *pbt =
    be_type::narrow_from_decl (node->primitive_base_type ());

  return (pbt->accept (this));
}

int
be_visitor_attribute_set_from_extracted::visit_union (
  be_union *)
{
  os_ << "*" << var_name_;
  return 0;
}

int
be_visitor_attribute_set_from_extracted::visit_valuebox (
  be_valuebox *)
{
  os_ << var_name_;
  return 0;
}

int
be_visitor_attribute_set_from_extracted::visit_valuetype (
  be_valuetype *)
{
  os_ << var_name_;
  return 0;
}

