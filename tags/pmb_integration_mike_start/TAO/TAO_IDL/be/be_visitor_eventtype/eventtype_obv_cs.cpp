
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    eventtype_obv_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Eventtypes in the client stub
//    OBV_ class
//    (see C++ mapping OMG 20.17)
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_eventtype, 
           eventtype_obv_cs, 
           "$Id$")

// ******************************************************
// Eventtype visitor for client stub
// ******************************************************

be_visitor_eventtype_obv_cs::be_visitor_eventtype_obv_cs (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype (ctx)
{
}

be_visitor_eventtype_obv_cs::~be_visitor_eventtype_obv_cs (void)
{
}


// OBV_ class must be in OBV_ namespace.
int
be_visitor_eventtype_obv_cs::visit_eventtype (be_eventtype *node)
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_valuetype_obv_cs visitor (&ctx);
  return visitor.visit_valuetype (node);
}
