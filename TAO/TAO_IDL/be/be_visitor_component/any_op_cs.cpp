// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    any_op_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Any operators for Component in the stubs
//    file.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

// ***************************************************************************
// Interface visitor for generating Any operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_component_any_op_cs::be_visitor_component_any_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_component (ctx)
{
}

be_visitor_component_any_op_cs::~be_visitor_component_any_op_cs (void)
{
}

int
be_visitor_component_any_op_cs::visit_component (be_component *node)
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_interface_any_op_cs visitor (&ctx);
  return visitor.visit_interface (node);
}

int
be_visitor_component_any_op_cs::visit_connector (be_connector *node)
{
  return this->visit_component (node);
}
