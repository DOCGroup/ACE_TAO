//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    any_op_ch.cpp
//
// = DESCRIPTION
//    Visitor generating Any operators for Eventtypes in the client header
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_eventtype, 
           any_op_ch, 
           "$Id$")

// ******************************************************
// Eventtype visitor for Any operators in the client header
// ******************************************************

be_visitor_eventtype_any_op_ch::be_visitor_eventtype_any_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype (ctx)
{
}

be_visitor_eventtype_any_op_ch::~be_visitor_eventtype_any_op_ch (void)
{
}

int
be_visitor_eventtype_any_op_ch::visit_eventtype (be_eventtype *node)
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_valuetype_any_op_ch visitor (&ctx);
  return visitor.visit_valuetype (node);
}
