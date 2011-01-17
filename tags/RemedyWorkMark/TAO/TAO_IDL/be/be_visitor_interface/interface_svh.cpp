
//=============================================================================
/**
 *  @file    interface_svh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Interfaces in the
 *  CIAO servant header file.
 *
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================


// ************************************************************
// Interface visitor for CIAO servant header.
// ************************************************************

be_visitor_interface_svh::be_visitor_interface_svh (
  be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_svh::~be_visitor_interface_svh (void)
{
}

int
be_visitor_interface_svh::visit_interface (be_interface *node)
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

  return node->gen_facet_svnt_hdr (this, *this->ctx_->stream ());
}

int
be_visitor_interface_svh::visit_operation (be_operation *node)
{
  be_visitor_operation_ch v (this->ctx_);
  return v.visit_operation (node);
}

int
be_visitor_interface_svh::visit_attribute (be_attribute *node)
{
  be_visitor_attribute v (this->ctx_);
  return v.visit_attribute (node);
}


