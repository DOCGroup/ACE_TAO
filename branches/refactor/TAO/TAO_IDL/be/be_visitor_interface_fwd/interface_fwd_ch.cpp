//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_fwd_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interface_Fwd node in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_interface_fwd, 
           interface_fwd_ch, 
           "$Id$")

// ********************************************************************
// Visitor implementation for the Interface_Fwd type
// This one for the client header file
// ********************************************************************

be_visitor_interface_fwd_ch::be_visitor_interface_fwd_ch (
    be_visitor_context *ctx
  )
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

  if (!fd->is_defined ())
    {
      // To generate extern declarations after all modules are closed.
      be_global->non_defined_interfaces.enqueue_tail (node);
    }

  if (bfd->var_out_seq_decls_gen () == 0)
    {
      bfd->gen_var_out_seq_decls ();
      bfd->var_out_seq_decls_gen (1);
    }

  node->cli_hdr_gen (I_TRUE);
  return 0;
}
