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
  TAO_OutStream *os = this->ctx_->stream ();

  if (node->cli_hdr_gen () || node->imported ())
    {
      return 0;
    }

  // All we do in this is generate a forward declaration of the class.
  *os << "class " << node->local_name () << ";" << be_nl;

  // Generate the ifdefined macro for the _ptr type.
  os->gen_ifdef_macro (node->flat_name (), "_ptr");

  // Generate the _ptr typedef.
  *os << "typedef " << node->local_name () << " *" << node->local_name ()
      << "_ptr;" << be_nl;

  os->gen_endif ();

  AST_Interface *fd = node->full_definition ();
  be_interface *bfd = be_interface::narrow_from_decl (fd);

  if (!fd->is_defined ())
    {
      // To generate extern declarations after all modules are closed.
      be_global->non_defined_interfaces.enqueue_tail (node);
    }

  // Enclose under an ifdef macro.
  os->gen_ifdef_macro (node->flat_name (), "_var");

  // Generate the _var declaration.
  if (bfd->gen_var_defn () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_fwd_ch::"
                         "visit_interface_fwd - "
                         "codegen for _var failed\n"),
                        -1);
    }

  // Generate an endif.
  os->gen_endif ();

  // Enclose under an ifdef macro.
  os->gen_ifdef_macro (node->flat_name (), "_out");

  // generate the _out declaration - ORBOS/97-05-15 pg 16-20 spec
  if (bfd->gen_out_defn () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_fwd_ch::"
                         "visit_interface_fwd - "
                         "codegen for _out failed\n"),
                        -1);
    }

  // Generate the endif macro.
  os->gen_endif ();

  node->cli_hdr_gen (I_TRUE);

  return 0;
}
