//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    eventtype_fwd_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for EventTypeFwd node in the client header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_eventtype_fwd, 
           eventtype_fwd_ch, 
           "$Id$")

be_visitor_eventtype_fwd_ch::be_visitor_eventtype_fwd_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_eventtype_fwd_ch::~be_visitor_eventtype_fwd_ch (void)
{
}

int
be_visitor_eventtype_fwd_ch::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_valuetype_fwd_ch visitor (&ctx);
  return visitor.visit_valuetype_fwd (node);
}
