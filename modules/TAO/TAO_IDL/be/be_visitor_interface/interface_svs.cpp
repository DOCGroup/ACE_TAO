// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO_IDL_BE
//
// = FILENAME
//    interface_svs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interfaces in the
//    CIAO servant source file.
//
// = AUTHOR
//   Jeff Parsons <j.parsons@vanderbilt.edu>
//
// ============================================================================

// ************************************************************
// Interface visitor for CIAO servant source.
// ************************************************************

ACE_RCSID (be_visitor_interface,
           interface_svs,
           "$Id$")

be_visitor_interface_svs::be_visitor_interface_svs (
  be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_svs::~be_visitor_interface_svs (void)
{
}

int
be_visitor_interface_svs::visit_interface (be_interface *node)
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

  return node->gen_facet_svnt_src (this, *this->ctx_->stream ());
}

int
be_visitor_interface_svs::visit_operation (be_operation *node)
{
  be_visitor_operation_svs v (this->ctx_);
  v.for_facets (true);
  v.scope (be_interface::narrow_from_decl (this->ctx_->scope ()));
  
  return v.visit_operation (node);
}

int
be_visitor_interface_svs::visit_attribute (be_attribute *node)
{
  be_visitor_attribute v (this->ctx_);
  
  v.for_facets (true);
  v.op_scope (be_interface::narrow_from_decl (this->ctx_->scope ()));
  
  return v.visit_attribute (node);
}


