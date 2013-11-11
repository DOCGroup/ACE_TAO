
//=============================================================================
/**
 *  @file    interface_fwd_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Interface_Fwd node in the client header.
 *
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================

#include "interface_fwd.h"

be_visitor_interface_fwd_ch::be_visitor_interface_fwd_ch (
    be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_interface_fwd_ch::~be_visitor_interface_fwd_ch (void)
{
}

// Visit the interface_fwd_ch node and its scope.
int
be_visitor_interface_fwd_ch::visit_interface_fwd (be_interface_fwd *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  AST_Interface *fd = node->full_definition ();
  be_interface *bfd = be_interface::narrow_from_decl (fd);

  // This will be a no-op if it has already been done for this node.
  bfd->gen_var_out_seq_decls ();

  // Copy the fwd helper name back to the forward declared interface node.
  node->fwd_helper_name (bfd->fwd_helper_name ());

  node->cli_hdr_gen (true);
  return 0;
}
