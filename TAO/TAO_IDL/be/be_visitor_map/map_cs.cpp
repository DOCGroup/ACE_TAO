
//=============================================================================
/**
 *  @file    map_cs.cpp
 *
 *  Visitor generating code for Maps in the client stubs file
 *
 *  @author Tyler Mayoff
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

int be_visitor_map_cs::visit_map (be_map *node)
{
  if (node->defined_in () == nullptr)
    {
      // The node is a nested map, and has had no scope defined.
      node->set_defined_in (DeclAsScope (this->ctx_->scope ()->decl ()));
    }

  // First create a name for ourselves.
  if (node->create_name (this->ctx_->tdef ()) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_map_ch::")
                         ACE_TEXT ("visit_map - ")
                         ACE_TEXT ("failed creating name\n")),
                        -1);
    }

  TAO_OutStream *os = this->ctx_->stream ();

  *os << be_nl_2;

  TAO_INSERT_COMMENT (os);

  os->gen_ifdef_macro (node->flat_name ());

  *os << be_nl_2;

  *os << "typedef std::map< ";

  be_type* kt = node->key_type();
  be_type* vt = node->value_type();

  *os << kt->full_name ();
  *os << ", ";
  *os << vt->full_name ();

  *os << "> " << node->local_name () << ";";

  os->gen_endif ();
  node->cli_hdr_gen (true);

  return 0;
}
