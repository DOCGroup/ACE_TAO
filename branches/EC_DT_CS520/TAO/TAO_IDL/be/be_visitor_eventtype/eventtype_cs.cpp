//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    eventtype_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Eventtypes in the client source
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_eventtype, 
           eventtype_cs, 
           "$Id$")

// ******************************************************
// Eventtype visitor for client source
// ******************************************************

be_visitor_eventtype_cs::be_visitor_eventtype_cs (be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_eventtype_cs::~be_visitor_eventtype_cs (void)
{
}

int
be_visitor_eventtype_cs::visit_eventtype (be_eventtype *node)
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_valuetype_cs visitor (&ctx);
  return visitor.visit_valuetype (node);
}
