//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    cdr_op_cs.cpp
//
// = DESCRIPTION
//    Visitor generating CDR operators for Eventtypes in the client source
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_eventtype, 
           cdr_op_cs, 
           "$Id$")

// ******************************************************
// Eventtype visitor for CDR operators in the client source
// ******************************************************

be_visitor_eventtype_cdr_op_cs::be_visitor_eventtype_cdr_op_cs (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype (ctx)
{
}

be_visitor_eventtype_cdr_op_cs::~be_visitor_eventtype_cdr_op_cs (void)
{
}

int
be_visitor_eventtype_cdr_op_cs::visit_eventtype (be_eventtype *node)
{
  be_visitor_context ctx (*this->ctx_);
  be_visitor_valuetype_cdr_op_cs visitor (&ctx);
  return visitor.visit_valuetype (node);
}
