//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    eventtype_si.cpp
//
// = DESCRIPTION
//    Visitor generating code for Eventtypes in the server inline
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_eventtype, 
           eventtype_si, 
           "$Id$")

// ******************************************************
// Eventtype visitor for server inline
// ******************************************************

be_visitor_eventtype_si::be_visitor_eventtype_si (be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_eventtype_si::~be_visitor_eventtype_si (void)
{
}

int
be_visitor_eventtype_si::visit_eventtype (be_eventtype *node)
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_valuetype_si visitor (&ctx);
  return visitor.visit_valuetype (node);
}
