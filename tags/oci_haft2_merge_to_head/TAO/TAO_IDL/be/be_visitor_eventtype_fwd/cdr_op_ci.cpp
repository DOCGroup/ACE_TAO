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
//    Visitor generating CDR operators for EventTypeFwd node in the 
//    client inline.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_eventtype_fwd, 
           cdr_op_ci, 
           "$Id$")

be_visitor_eventtype_fwd_cdr_op_ci::be_visitor_eventtype_fwd_cdr_op_ci (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_eventtype_fwd_cdr_op_ci::~be_visitor_eventtype_fwd_cdr_op_ci (void)
{
}

int
be_visitor_eventtype_fwd_cdr_op_ci::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_valuetype_fwd_cdr_op_ci visitor (&ctx);
  return visitor.visit_valuetype_fwd (node);
}
