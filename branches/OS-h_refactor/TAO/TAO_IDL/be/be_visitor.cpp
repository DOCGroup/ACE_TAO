//
// $Id$
//
/* -*- c++ -*- */
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    be_visitor.h
//
// = DESCRIPTION
//    Abstract class for back end visitors.
//
// = NOTES
//    This is not a pure abstract class
//
// = AUTHOR
//    Aniruddha Gokhale and Carlos O'Ryan
//
// ============================================================================

#include "be_visitor.h"
#include "ace/config-all.h"

ACE_RCSID (be, 
           be_visitor, 
           "$Id$")

be_visitor::be_visitor (void)
{
}

be_visitor::~be_visitor (void)
{
}

// all the visit methods. Concrete visitors will selectively override each one
int be_visitor::visit_decl (be_decl *)
{
  return 0;
}

int be_visitor::visit_scope (be_scope *)
{
  return 0;
}

int be_visitor::visit_type (be_type *)
{
  return 0;
}

int be_visitor::visit_predefined_type (be_predefined_type *)
{
  return 0;
}

int be_visitor::visit_module (be_module *)
{
  return 0;
}

int be_visitor::visit_interface (be_interface *)
{
  return 0;
}

int be_visitor::visit_interface_fwd (be_interface_fwd *)
{
  return 0;
}

int be_visitor::visit_valuetype (be_valuetype *)
{
  return 0;
}

int be_visitor::visit_valuetype_fwd (be_valuetype_fwd *)
{
  return 0;
}

int be_visitor::visit_component (be_component *)
{
  return 0;
}

int be_visitor::visit_component_fwd (be_component_fwd *)
{
  return 0;
}

int be_visitor::visit_home (be_home *)
{
  return 0;
}

int be_visitor::visit_factory (be_factory *)
{
  return 0;
}

int be_visitor::visit_structure (be_structure *)
{
  return 0;
}

int be_visitor::visit_structure_fwd (be_structure_fwd *)
{
  return 0;
}

int be_visitor::visit_exception (be_exception *)
{
  return 0;
}

int be_visitor::visit_expression (be_expression *)
{
  return 0;
}

int be_visitor::visit_enum (be_enum *)
{
  return 0;
}

int be_visitor::visit_operation (be_operation *)
{
  return 0;
}

int be_visitor::visit_field (be_field *)
{
  return 0;
}

int be_visitor::visit_argument (be_argument *)
{
  return 0;
}

int be_visitor::visit_attribute (be_attribute *)
{
  return 0;
}

int be_visitor::visit_union (be_union *)
{
  return 0;
}

int be_visitor::visit_union_fwd (be_union_fwd *)
{
  return 0;
}

int be_visitor::visit_union_branch (be_union_branch *)
{
  return 0;
}

int be_visitor::visit_union_label (be_union_label *)
{
  return 0;
}

int be_visitor::visit_constant (be_constant *)
{
  return 0;
}

int be_visitor::visit_enum_val (be_enum_val *)
{
  return 0;
}

int be_visitor::visit_array (be_array *)
{
  return 0;
}

int be_visitor::visit_sequence (be_sequence *)
{
  return 0;
}

int be_visitor::visit_string (be_string *)
{
  return 0;
}

int be_visitor::visit_typedef (be_typedef *)
{
  return 0;
}

int be_visitor::visit_root (be_root *)
{
  return 0;
}

int be_visitor::visit_native (be_native *)
{
  return 0;
}

