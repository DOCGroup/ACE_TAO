//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    extended_port_svh.cpp
//
// = DESCRIPTION
//    Visitor generating code for extended ports in the servant header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

// ******************************************************
// Extended port visitor for server header
// ******************************************************

be_visitor_extended_port_svh::be_visitor_extended_port_svh (
      be_visitor_context *ctx)
  : be_visitor_extended_port (ctx)
{
}

be_visitor_extended_port_svh::~be_visitor_extended_port_svh (void)
{
}

int
be_visitor_extended_port_svh::visit_extended_port (
  be_extended_port *)
{
  return 0;
}

