
//=============================================================================
/**
 *  @file    any_op_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Any operators for a forward declared
 *  component in the client header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


// ***************************************************************************
// Generates Any operator declarations in the client header
// ***************************************************************************

be_visitor_component_fwd_any_op_ch::be_visitor_component_fwd_any_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_component_fwd_any_op_ch::~be_visitor_component_fwd_any_op_ch (void)
{
}

int
be_visitor_component_fwd_any_op_ch::visit_component_fwd (
    be_component_fwd *node
  )
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_interface_fwd_any_op_ch visitor (&ctx);
  return visitor.visit_interface_fwd (node);
}
