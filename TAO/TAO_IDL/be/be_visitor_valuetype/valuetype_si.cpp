//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_si.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the server inline file
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_valuetype, 
           valuetype_si, 
           "$Id$")


// ************************************************************************
// Valuetype visitor for server inline
// ************************************************************************

be_visitor_valuetype_si::be_visitor_valuetype_si (be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_si::~be_visitor_valuetype_si (void)
{
}

int
be_visitor_valuetype_si::visit_valuetype (be_valuetype *node)
{
  if (node->srv_inline_gen () 
      || node->imported () 
      || node->is_local ()
      || node->is_abstract ())
    {
      return 0;
    }

  // We generate a skeleton class only if the valuetype supports a
  // non-abstract interface.
  if (node->supports_concrete () == 0)
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Generate skeletons for operations of the concrete interface that we
  // support (if any) and its base classes.
  int status =
    node->traverse_concrete_inheritance_graph (be_interface::gen_skel_helper,
                                               os);
  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_si::"
                         "visit_valuetype - "
                         "codegen for base class skeletons failed\n"), 
                        -1);
    }

  return 0;
}
