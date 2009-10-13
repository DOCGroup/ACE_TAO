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

be_visitor_extended_port_facet_svh::be_visitor_extended_port_facet_svh (
      be_visitor_context *ctx)
  : be_visitor_extended_port (ctx)
{
}

be_visitor_extended_port_facet_svh::~be_visitor_extended_port_facet_svh (
  void)
{
}

int
be_visitor_extended_port_facet_svh::visit_extended_port (
  be_extended_port *node)
{
  be_porttype *pt =
    be_porttype::narrow_from_decl (node->port_type ());
    
  return this->visit_porttype (pt);
}

int
be_visitor_extended_port_facet_svh::visit_porttype (
  be_porttype *node)
{
  return this->visit_scope (node);
}

int
be_visitor_extended_port_facet_svh::visit_provides (
  be_provides *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (node->gen_facet_svnt_decl (*os) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_extended_port_facet_svh::")
                         ACE_TEXT ("visit_provides - ")
                         ACE_TEXT ("gen_facet_svnt_decl() failed\n")),
                        -1);
    }
    
  return 0;
}

