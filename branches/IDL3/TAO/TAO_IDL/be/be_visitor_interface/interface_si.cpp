//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_si.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the server inline file
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

ACE_RCSID (be_visitor_interface, 
           interface_si, 
           "$Id$")


// ************************************************************************
// Interface visitor for server inline
// ************************************************************************

be_visitor_interface_si::be_visitor_interface_si (be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_si::~be_visitor_interface_si (void)
{
}

int
be_visitor_interface_si::visit_interface (be_interface *node)
{
  if (node->srv_inline_gen () || node->imported () || node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Determine if we are in some form of a multiple inheritance.
  int status = 
    node->traverse_inheritance_graph (be_interface::in_mult_inheritance_helper,
                                      0);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_si::visit_interface "
                         "error determining mult inheritance\n"),
                        -1);
    }

  // Generate skeletons for operations of our base classes. These skeletons
  // just cast the pointer to the appropriate type before invoking the
  // call. Hence we generate these in the inline file.
  status = node->traverse_inheritance_graph (be_interface::gen_skel_helper, 
                                             os);
  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_si::"
                         "visit_interface - "
                         "codegen for base class skeletons failed\n"), -1);
    }

  if (be_global->gen_tie_classes ())
    {
      // Generate the TIE class.
      be_visitor_context ctx (*this->ctx_);
      ctx.state (TAO_CodeGen::TAO_INTERFACE_TIE_SI);
      ctx.stream (tao_cg->server_template_inline ());
      be_visitor_interface_tie_si visitor (&ctx);

      if (node->accept (&visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_interface_sh::"
                             "visit_interface - "
                             "codegen for TIE class failed\n"),
                            -1);
        }
    }

  return 0;
}
