//=============================================================================
/**
 *  @file    buffer_type.cpp
 *
 *  Visitor generating code for the internal buffer type of the Map node
 *
 *  @author Tyler Mayoff
 */
//=============================================================================

#include "map.h"

// ****************************************************************
// We have to generate the buffer type in the constructor
// ****************************************************************

be_visitor_map_buffer_type::be_visitor_map_buffer_type (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_map_buffer_type::~be_visitor_map_buffer_type ()
{
}

int
be_visitor_map_buffer_type::visit_node (be_type *node, bool var)
{
  TAO_OutStream *const os = this->ctx_->stream ();

  be_typedef *const td = this->ctx_->alias ();
  be_type *const bt = td ? static_cast<be_type *> (td) : node;

  *os << bt->nested_type_name (this->ctx_->scope ()->decl (), var ? "_var" : nullptr);
  return 0;
}

int
be_visitor_map_buffer_type::visit_predefined_type (be_predefined_type *node)
{
  TAO_OutStream *const os = this->ctx_->stream ();

  be_typedef *const td = this->ctx_->alias ();
  be_type *const bt = td ? static_cast<be_type *> (td) : node;

  *os << bt->nested_type_name (this->ctx_->scope ()->decl ());

  switch (node->pt ())
    {
      case AST_PredefinedType::PT_object:
      case AST_PredefinedType::PT_abstract:
      case AST_PredefinedType::PT_pseudo:
      case AST_PredefinedType::PT_value:
        *os << "_var";
        break;
      default:
        break;
    }

  return 0;
}


int
be_visitor_map_buffer_type::visit_sequence (be_sequence *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_buffer_type::visit_interface (be_interface *node)
{
  return this->visit_node (node, true);
}

int
be_visitor_map_buffer_type::visit_interface_fwd (be_interface_fwd *node)
{
  return this->visit_node (node, true);
}

int
be_visitor_map_buffer_type::visit_component (be_component *node)
{
  return this->visit_interface (node);
}

int
be_visitor_map_buffer_type::visit_component_fwd (be_component_fwd *node)
{
  return this->visit_interface_fwd (node);
}

int
be_visitor_map_buffer_type::visit_valuebox (be_valuebox *node)
{
  return this->visit_node (node, true);
}

int
be_visitor_map_buffer_type::visit_valuetype (be_valuetype *node)
{
  return this->visit_node (node, true);
}

int
be_visitor_map_buffer_type::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  return this->visit_node (node, true);
}

int
be_visitor_map_buffer_type::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

int
be_visitor_map_buffer_type::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_valuetype_fwd (node);
}

int
be_visitor_map_buffer_type::visit_string (be_string *node)
{
  *this->ctx_->stream () <<
    (node->width () == 1 ? "::TAO::String_Manager" : "::TAO::WString_Manager");

  return 0;
}

int
be_visitor_map_buffer_type::visit_structure (be_structure *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_buffer_type::visit_structure_fwd (be_structure_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_buffer_type::visit_union (be_union *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_buffer_type::visit_union_fwd (be_union_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_buffer_type::visit_enum (be_enum *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_buffer_type::visit_exception (be_exception *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_buffer_type::visit_array (be_array *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_buffer_type::visit_typedef (be_typedef *node)
{
  this->ctx_->alias (node);

  if (node->primitive_base_type ()->accept (this) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_map_buffer_type::"
                         "visit_typedef - "
                         "accept on primitive type failed\n"),
                        -1);
    }

  this->ctx_->alias (nullptr);
  return 0;
}

int be_visitor_map_buffer_type::visit_map (be_map *node)
{
  return this->visit_node (node);
}
