
//=============================================================================
/**
 *  @file    cdr_op_cs.cpp
 *
 *  Visitor for code generation of Maps for the CDR operators
 *  in the client stubs.
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "map.h"

// ***************************************************************************
// Map visitor for generating CDR operator declarations in the client
// stubs file.
// ***************************************************************************

be_visitor_map_cdr_op_cs::be_visitor_map_cdr_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_map_cdr_op_cs::~be_visitor_map_cdr_op_cs ()
{
}

int
be_visitor_map_cdr_op_cs::visit_map (be_map *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // *os << be_nl
  //     << "#if !defined _TAO_CDR_OP_"
  //     << node->flat_name () << "_CPP_" << be_nl
  //     << "#define _TAO_CDR_OP_" << node->flat_name () << "_CPP_"
  //     << be_nl;

  // *os << be_global->core_versioning_begin () << be_nl;

  // be_type* kt = node->key_type();
  // be_type* vt = node->key_type();

  // *os << "::CORBA::Boolean operator<< (" << be_idt_nl
  //     << "TAO_OutputCDR &strm," << be_nl
  //     << "const std::map<" << kt->full_name ()
  //     << "," << vt->full_name()
  //     << "> &_tao_map)"
  //     << be_uidt_nl
  //     << "{" << be_idt_nl
  //     << "return true;" << be_uidt_nl
  //     << "}" << be_nl_2;

  // *os << "::CORBA::Boolean operator>> (" << be_idt_nl
  //     << "TAO_OutputCDR &strm," << be_nl
  //     << "const std::map<" << kt->full_name ()
  //     << "," << vt->full_name()
  //     << "> &_tao_map)"
  //     << be_uidt_nl
  //     << "{" << be_idt_nl
  //     << "return true;" << be_uidt_nl
  //     << "}" << be_nl_2;

  // *os << be_nl << be_global->core_versioning_end ();

  // *os << be_nl
  //     << "#endif /* _TAO_CDR_OP_"
  //     << node->flat_name () << "_CPP_ */"
  //     << be_nl;

  return 0;
}

int
be_visitor_map_cdr_op_cs::visit_array (be_array *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_cdr_op_cs::visit_enum (be_enum *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_cdr_op_cs::visit_interface (be_interface *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_cdr_op_cs::visit_interface_fwd (be_interface_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_cdr_op_cs::visit_component (be_component *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_cdr_op_cs::visit_component_fwd (be_component_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_cdr_op_cs::visit_home (be_home *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_cdr_op_cs::visit_valuebox (be_valuebox *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_cdr_op_cs::visit_valuetype (be_valuetype *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_cdr_op_cs::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_cdr_op_cs::visit_eventtype (be_eventtype *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_cdr_op_cs::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_cdr_op_cs::visit_predefined_type (
    be_predefined_type *node
  )
{
  return this->visit_node (node);
}

int
be_visitor_map_cdr_op_cs::visit_string (be_string *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_cdr_op_cs::visit_structure (be_structure *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_cdr_op_cs::visit_union (be_union *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_cdr_op_cs::visit_typedef (be_typedef *node)
{
  return this->visit_node (node);
}

int
be_visitor_map_cdr_op_cs::visit_node (be_type *)
{
  return 0;
}
