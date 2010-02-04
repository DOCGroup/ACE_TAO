
//=============================================================================
/**
 *  @file    component_ss.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Components in the server skeleton.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


// ******************************************************
// Component visitor for server skeleton
// ******************************************************

be_visitor_component_ss::be_visitor_component_ss (be_visitor_context *ctx)
  : be_visitor_component (ctx)
{
}

be_visitor_component_ss::~be_visitor_component_ss (void)
{
}

int
be_visitor_component_ss::visit_component (be_component *node)
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_interface_ss visitor (&ctx);
  return visitor.visit_interface (node);
}
