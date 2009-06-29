// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    interface_ex_idl.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the
//    CIAO executor IDL file.
//
// = AUTHOR
//   Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

// ************************************************************
// Interface visitor for CIAO executor IDL.
// ************************************************************

ACE_RCSID (be_visitor_interface,
           interface_ex_idl,
           "$Id$")

be_visitor_interface_ex_idl::be_visitor_interface_ex_idl (
  be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ex_idl::~be_visitor_interface_ex_idl (void)
{
}

int
be_visitor_interface_ex_idl::visit_interface (be_interface *node)
{
  if (! be_global->gen_lem_force_all ())
    {
      return 0;
    }
    
  // Skip implied IDL nodes.  
  if (node->original_interface () != 0)
    {
      return 0;
    }
    
  if (node->is_local () || node->imported () || node->is_abstract ())
    {
      return 0;
    }

  node->gen_facet_idl (*this->ctx_->stream ());

  return 0;
}


