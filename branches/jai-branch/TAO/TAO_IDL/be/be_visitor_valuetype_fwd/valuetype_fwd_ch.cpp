//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_fwd_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for ValueTypeFwd node in the client header.
//
// = AUTHOR
//    Boris Kolpackov <bosk@ipmce.ru>
//    based on code from  Torsten Kuepper
//    based on code from Aniruddha Gokhale (interface_fwd_ch.cpp)
//
// ============================================================================

ACE_RCSID (be_visitor_valuetype_fwd, 
           valuetype_fwd_ch, 
           "$Id$")

be_visitor_valuetype_fwd_ch::be_visitor_valuetype_fwd_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_valuetype_fwd_ch::~be_visitor_valuetype_fwd_ch (void)
{
}

// Visit the valuetype_fwd node and its scope.
int
be_visitor_valuetype_fwd_ch::visit_valuetype_fwd (be_valuetype_fwd *node)
{
  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  AST_Interface *fd = node->full_definition ();
  be_valuetype *bfd = be_valuetype::narrow_from_decl (fd);

  // This will be a no-op if it has already been done for this node.
  bfd->gen_var_out_seq_decls ();

  // Copy the fwd helper name back to the forward declared interface node.
  node->fwd_helper_name (bfd->fwd_helper_name ());

  node->cli_hdr_gen (I_TRUE);
  return 0;
}
