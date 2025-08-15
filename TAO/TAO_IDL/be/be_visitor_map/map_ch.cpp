
//=============================================================================
/**
 *  @file    map_ch.cpp
 *
 *  Visitor generating code for Map in the client header
 *
 *  @author Tyler Mayoff
 */
//=============================================================================

#include "map.h"

// Root visitor for client header.
be_visitor_map_ch::be_visitor_map_ch (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

int be_visitor_map_ch::visit_map (be_map *node)
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

  *os << "using " << node->local_name () << " = std::map< ";

  be_type* kt = node->key_type();
  be_type* vt = node->value_type();

  // Generate the base type for the buffer.
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_MAP_BUFFER_TYPE_CH);
  be_visitor_map_buffer_type bt_visitor (&ctx);

  if (kt->accept (&bt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        ACE_TEXT ("be_visitor_map_ch::")
                        ACE_TEXT ("visit_map - ")
                        ACE_TEXT ("buffer type visit failed\n")),
                        -1);
    }

  *os << ", ";

  if (vt->accept (&bt_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                        ACE_TEXT ("be_visitor_map_ch::")
                        ACE_TEXT ("visit_map - ")
                        ACE_TEXT ("buffer type visit failed\n")),
                        -1);
    }

  *os << ">;";

  os->gen_endif ();
  node->cli_hdr_gen (true);

  return 0;
}
