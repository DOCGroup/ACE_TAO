//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for CDR operators for components. This uses
//    compiled marshaling.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_component, 
           cdr_op_ch, 
           "$Id$")

// ***************************************************************************
// Interface visitor for generating CDR operator declarations in the client header
// ***************************************************************************

be_visitor_component_cdr_op_ch::be_visitor_component_cdr_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_component (ctx)
{
}

be_visitor_component_cdr_op_ch::~be_visitor_component_cdr_op_ch (void)
{
}

int
be_visitor_component_cdr_op_ch::visit_component (be_component *node)
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_interface_cdr_op_ch visitor (&ctx);
  return visitor.visit_interface (node);
}
