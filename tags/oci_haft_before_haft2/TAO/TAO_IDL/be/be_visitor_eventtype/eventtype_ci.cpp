//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    eventtype_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Eventtypes in the client inline
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_eventtype, 
           eventtype_ci, 
           "$Id$")

// ******************************************************
// Eventtype visitor for client inline
// ******************************************************

be_visitor_eventtype_ci::be_visitor_eventtype_ci (be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_eventtype_ci::~be_visitor_eventtype_ci (void)
{
}

int
be_visitor_eventtype_ci::visit_eventtype (be_eventtype *node)
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_valuetype_ci visitor (&ctx);
  return visitor.visit_valuetype (node);
}
