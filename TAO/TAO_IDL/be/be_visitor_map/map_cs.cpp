
//=============================================================================
/**
 *  @file    map_cs.cpp
 *
 *  Visitor generating code for Sequences in the client stubs file
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "map.h"

// ************************************************************
// Root visitor for client stub class
// ************************************************************

be_visitor_map_cs::be_visitor_map_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_map_cs::~be_visitor_map_cs ()
{
}

int be_visitor_map_cs::visit_map (be_map *node)
{
    if (node->defined_in () == 0)
    {
      // The node is a nested map, and has had no scope defined.
      node->set_defined_in (DeclAsScope (this->ctx_->scope ()->decl ()));
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2;

  TAO_INSERT_COMMENT (os);

  *os << "std::map<"
      << node->key_type ()->full_name ()
      << ", "
      << node->value_type ()->full_name ()
      << ">";

  node->cli_hdr_gen (true);
  return 0;
}
