//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    component_fwd_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Component_Fwd node in the client header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_component_fwd, 
           component_fwd_ch, 
           "$Id$")

// ********************************************************************
// Visitor implementation for the Component_Fwd type
// This one for the client header file
// ********************************************************************

be_visitor_component_fwd_ch::be_visitor_component_fwd_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_component_fwd_ch::~be_visitor_component_fwd_ch (void)
{
}

// Visit the component_fwd_ch node and its scope.
int
be_visitor_component_fwd_ch::visit_component_fwd (be_component_fwd *node)
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_interface_fwd_ch visitor (&ctx);
  return visitor.visit_interface_fwd (node);
}
