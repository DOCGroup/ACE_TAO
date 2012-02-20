
//=============================================================================
/**
 *  @file    component_fwd_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Component_Fwd node in the client header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

#include "component_fwd.h"

be_visitor_component_fwd_ch::be_visitor_component_fwd_ch (
    be_visitor_context *ctx)
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
