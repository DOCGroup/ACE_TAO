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
//    Visitor generating Any operators for EventTypeFwd node in the 
//    client header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_eventtype_fwd, 
           any_op_ch, 
           "$Id$")

be_visitor_eventtype_fwd_any_op_ch::be_visitor_eventtype_fwd_any_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_eventtype_fwd_any_op_ch::~be_visitor_eventtype_fwd_any_op_ch (void)
{
}

int
be_visitor_eventtype_fwd_any_op_ch::visit_eventtype_fwd (be_eventtype_fwd *node)
{
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_VALUETYPE_FWD_ANY_OP_CH);
  be_visitor_valuetype_fwd_any_op_ch visitor (&ctx);
  return visitor.visit_valuetype_fwd (node);
}
