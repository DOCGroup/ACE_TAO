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
//    Visitor generating CDR operators for Eventtypes in the client inline
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_eventtype, 
           cdr_op_ci, 
           "$Id$")

// ******************************************************
// Eventtype visitor for CDR operators in the client inline
// ******************************************************

be_visitor_eventtype_cdr_op_ci::be_visitor_eventtype_cdr_op_ci (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype (ctx)
{
}

be_visitor_eventtype_cdr_op_ci::~be_visitor_eventtype_cdr_op_ci (void)
{
}

int
be_visitor_eventtype_cdr_op_ci::visit_eventtype (be_eventtype *node)
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_valuetype_cdr_op_ci visitor (&ctx);
  return visitor.visit_valuetype (node);
}
