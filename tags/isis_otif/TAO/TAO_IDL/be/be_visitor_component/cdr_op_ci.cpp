//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for CDR operators for components
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_component, 
           cdr_op_ci, 
           "$Id$")

// ***************************************************************************
// Component visitor for generating CDR operator declarations in the client
// stubs file
// ***************************************************************************

be_visitor_component_cdr_op_ci::be_visitor_component_cdr_op_ci (
    be_visitor_context *ctx
  )
  : be_visitor_component (ctx)
{
}

be_visitor_component_cdr_op_ci::~be_visitor_component_cdr_op_ci (void)
{
}

int
be_visitor_component_cdr_op_ci::visit_component (be_component *node)
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_interface_cdr_op_ci visitor (&ctx);
  return visitor.visit_interface (node);
}
