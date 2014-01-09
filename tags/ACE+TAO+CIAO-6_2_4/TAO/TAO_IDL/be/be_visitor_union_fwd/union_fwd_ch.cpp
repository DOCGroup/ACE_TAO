
//=============================================================================
/**
 *  @file    union_fwd_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for be_union_fwd node in the client header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "union_fwd.h"

be_visitor_union_fwd_ch::be_visitor_union_fwd_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_union_fwd_ch::~be_visitor_union_fwd_ch (void)
{
}

// Visit the interface_fwd_ch node and its scope.
int
be_visitor_union_fwd_ch::visit_union_fwd (be_union_fwd *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();
  be_union *fd = be_union::narrow_from_decl (node->full_definition ());

  // This will be a no-op if it has already been done for this node.
  fd->gen_common_varout (os);

  node->cli_hdr_gen (true);
  return 0;
}
