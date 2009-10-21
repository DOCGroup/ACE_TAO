//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    facet_svh.cpp
//
// = DESCRIPTION
//    Visitor generating code for a facet servant class in the
//    servant header.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

be_visitor_facet_svh::be_visitor_facet_svh (be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_facet_svh::~be_visitor_facet_svh (void)
{
}

int
be_visitor_facet_svh::visit_provides (be_provides *node)
{
  if (node->gen_facet_svnt_decl (os_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_facet_svh")
                         ACE_TEXT ("::visit_provides - ")
                         ACE_TEXT ("gen_facet_svnt_decl() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }
                
  return 0;
}

int
be_visitor_facet_svh::visit_extended_port (be_extended_port *node)
{
  return this->visit_porttype (node->port_type ());
}

int
be_visitor_facet_svh::visit_mirror_port (be_mirror_port *)
{
  // TODO
  return 0;
}

int
be_visitor_facet_svh::visit_porttype (be_porttype *node)
{
  return this->visit_scope (node);
}

