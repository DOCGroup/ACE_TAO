/* -*- c++ -*- */
//=============================================================================
/**
 *  @file    be_visitor.cpp
 *
 *  $Id$
 *
 *  Abstract class for back end visitors.
 *
 * = NOTES
 *  This is not a pure abstract class
 *
 *
 *  @author Aniruddha Gokhale and Carlos O'Ryan
 */
//=============================================================================

#include "be_visitor.h"

#include "ace/config-all.h"

be_visitor::be_visitor (void)
{
}

be_visitor::~be_visitor (void)
{
}

// All the visit methods. Concrete visitors will selectively
// override each one.
int be_visitor::visit_decl (be_decl *)
{
  return 0;
}

int
be_visitor::visit_scope (be_scope *)
{
  return 0;
}

int
be_visitor::visit_type (be_type *)
{
  return 0;
}

int
be_visitor::visit_predefined_type (be_predefined_type *)
{
  return 0;
}

int
be_visitor::visit_module (be_module *)
{
  return 0;
}

int be_visitor::visit_interface (be_interface *)
{
  return 0;
}

int
be_visitor::visit_interface_fwd (be_interface_fwd *)
{
  return 0;
}

int
be_visitor::visit_valuebox (be_valuebox *)
{
  return 0;
}

int
be_visitor::visit_valuetype (be_valuetype *)
{
  return 0;
}

int
be_visitor::visit_valuetype_fwd (be_valuetype_fwd *)
{
  return 0;
}

int
be_visitor::visit_eventtype (be_eventtype *)
{
  return 0;
}

int
be_visitor::visit_eventtype_fwd (be_eventtype_fwd *)
{
  return 0;
}

int
be_visitor::visit_component (be_component *)
{
  return 0;
}

int
be_visitor::visit_component_fwd (be_component_fwd *)
{
  return 0;
}

int
be_visitor::visit_template_module (be_template_module *)
{
  return 0;
}

int
be_visitor::visit_template_module_inst (be_template_module_inst *)
{
  return 0;
}

int
be_visitor::visit_template_module_ref (be_template_module_ref *)
{
  return 0;
}

int
be_visitor::visit_param_holder (be_param_holder *)
{
  return 0;
}

int
be_visitor::visit_porttype (be_porttype *)
{
  return 0;
}

int
be_visitor::visit_provides (be_provides *)
{
  return 0;
}

int
be_visitor::visit_uses (be_uses *)
{
  return 0;
}

int
be_visitor::visit_publishes (be_publishes *)
{
  return 0;
}

int
be_visitor::visit_emits (be_emits *)
{
  return 0;
}

int
be_visitor::visit_consumes (be_consumes *)
{
  return 0;
}

int
be_visitor::visit_extended_port (be_extended_port *)
{
  return 0;
}

int
be_visitor::visit_mirror_port (be_mirror_port *)
{
  return 0;
}

int
be_visitor::visit_connector (be_connector *)
{
  return 0;
}

int
be_visitor::visit_home (be_home *)
{
  return 0;
}

int
be_visitor::visit_factory (be_factory *)
{
  return 0;
}

int
be_visitor::visit_finder (be_finder *)
{
  return 0;
}

int
be_visitor::visit_structure (be_structure *)
{
  return 0;
}

int
be_visitor::visit_structure_fwd (be_structure_fwd *)
{
  return 0;
}

int
be_visitor::visit_exception (be_exception *)
{
  return 0;
}

int
be_visitor::visit_expression (be_expression *)
{
  return 0;
}

int
be_visitor::visit_enum (be_enum *)
{
  return 0;
}

int
be_visitor::visit_operation (be_operation *)
{
  return 0;
}

int
be_visitor::visit_field (be_field *)
{
  return 0;
}

int
be_visitor::visit_argument (be_argument *)
{
  return 0;
}

int
be_visitor::visit_attribute (be_attribute *)
{
  return 0;
}

int
be_visitor::visit_union (be_union *)
{
  return 0;
}

int
be_visitor::visit_union_fwd (be_union_fwd *)
{
  return 0;
}

int
be_visitor::visit_union_branch (be_union_branch *)
{
  return 0;
}

int
be_visitor::visit_union_label (be_union_label *)
{
  return 0;
}

int
be_visitor::visit_constant (be_constant *)
{
  return 0;
}

int
be_visitor::visit_enum_val (be_enum_val *)
{
  return 0;
}

int
be_visitor::visit_array (be_array *)
{
  return 0;
}

int
be_visitor::visit_sequence (be_sequence *)
{
  return 0;
}

int
be_visitor::visit_string (be_string *)
{
  return 0;
}

int
be_visitor::visit_typedef (be_typedef *)
{
  return 0;
}

int
be_visitor::visit_root (be_root *)
{
  return 0;
}

int
be_visitor::visit_native (be_native *)
{
  return 0;
}

