// $id: any_op_cs.cpp,v 1.12 1998/10/30 19:46:55 levine Exp $

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

ACE_RCSID (be_visitor_component, 
           any_op_cs, 
           "$Id$")

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
