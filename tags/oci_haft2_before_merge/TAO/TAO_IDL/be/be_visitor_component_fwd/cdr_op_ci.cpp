//
// $Id$
//

// ================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for CDR operators for forward
//    declarations of components. This uses compiled marshaling.
//
// = AUTHOR
//    Jeff Parsons
//
// ================================================================

ACE_RCSID (be_visitor_component_fwd, 
           cdr_op_ci, 
           "$Id$")

// ****************************************************************
// Interface visitor for generating CDR operator declarations in the
// client header
// ****************************************************************

be_visitor_component_fwd_cdr_op_ci::be_visitor_component_fwd_cdr_op_ci (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_component_fwd_cdr_op_ci::~be_visitor_component_fwd_cdr_op_ci (
    void
  )
{
}

int
be_visitor_component_fwd_cdr_op_ci::visit_component_fwd (
    be_component_fwd *node
  )
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_interface_fwd_cdr_op_ci visitor (&ctx);
  return visitor.visit_interface_fwd (node);
}
