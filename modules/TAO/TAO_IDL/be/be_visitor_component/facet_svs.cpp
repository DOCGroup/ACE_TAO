//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    facet_svs.cpp
//
// = DESCRIPTION
//    Visitor generating code for a facet servant class in the
//    servant source.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

be_visitor_facet_svs::be_visitor_facet_svs (be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_facet_svs::~be_visitor_facet_svs (void)
{
}

int
be_visitor_facet_svs::visit_component (be_component *node)
{
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_facet_svh::")
                         ACE_TEXT ("visit_component() - ")
                         ACE_TEXT ("visit_scope() failed\n")),
                        -1);
    }
    
  return 0;
}

int
be_visitor_facet_svs::visit_provides (be_provides *node)
{
  if (node->gen_facet_svnt_defn (os_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_facet_svs")
                         ACE_TEXT ("::visit_provides - ")
                         ACE_TEXT ("gen_facet_svnt_defn() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }
                
  return 0;
}

int
be_visitor_facet_svs::visit_extended_port (be_extended_port *node)
{
  be_visitor_extended_port_facet_svs visitor (this->ctx_);
  
  return visitor.visit_extended_port (node);
}

int
be_visitor_facet_svs::visit_mirror_port (be_mirror_port *)
{
  // TODO
  return 0;
}

