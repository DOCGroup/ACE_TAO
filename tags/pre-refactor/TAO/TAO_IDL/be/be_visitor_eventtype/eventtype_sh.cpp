//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    eventtype_sh.cpp
//
// = DESCRIPTION
//    Visitor generating code for Eventtypes in the server header
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_eventtype, 
           eventtype_sh, 
           "$Id$")

// ******************************************************
// Eventtype visitor for server header
// ******************************************************

be_visitor_eventtype_sh::be_visitor_eventtype_sh (be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_eventtype_sh::~be_visitor_eventtype_sh (void)
{
}

int
be_visitor_eventtype_sh::visit_eventtype (be_eventtype *node)
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_valuetype_sh visitor (&ctx);
  return visitor.visit_valuetype (node);
}
