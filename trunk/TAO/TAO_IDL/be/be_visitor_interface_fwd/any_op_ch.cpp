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
//    Visitor generating code for Any operators for a forward declared interface
//    in the client header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_interface_fwd, 
           any_op_ch, 
           "$Id$")

// ***************************************************************************
// Generates Any operator declarations in the client header
// ***************************************************************************

be_visitor_interface_fwd_any_op_ch::be_visitor_interface_fwd_any_op_ch (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_interface_fwd_any_op_ch::~be_visitor_interface_fwd_any_op_ch (void)
{
}

int
be_visitor_interface_fwd_any_op_ch::visit_interface_fwd (be_interface_fwd *)
{
  return 0;
}
