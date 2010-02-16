
//=============================================================================
/**
 *  @file    any_op_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Any operators for an Component in the client
 *  header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


// ***************************************************************************
// Interface visitor for generating Any operator declarations in the client header
// ***************************************************************************

be_visitor_component_any_op_ch::be_visitor_component_any_op_ch
(be_visitor_context *ctx)
  : be_visitor_component (ctx)
{
}

be_visitor_component_any_op_ch::~be_visitor_component_any_op_ch (void)
{
}

int
be_visitor_component_any_op_ch::visit_component (be_component *node)
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_interface_any_op_ch visitor (&ctx);
  return visitor.visit_interface (node);
}

int
be_visitor_component_any_op_ch::visit_connector (be_connector *node)
{
  return this->visit_component (node);
}
