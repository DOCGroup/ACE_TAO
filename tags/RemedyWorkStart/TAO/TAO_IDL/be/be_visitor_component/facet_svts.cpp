
//=============================================================================
/**
 *  @file    facet_svts.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for a facet servant class in the
 *  servant source.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================

be_visitor_facet_svts::be_visitor_facet_svts (be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_facet_svts::~be_visitor_facet_svts (void)
{
}

int
be_visitor_facet_svts::visit_provides (be_provides *node)
{
  if (node->gen_facet_svnt_tmpl_defn (os_) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("be_visitor_facet_svts")
                         ACE_TEXT ("::visit_provides - ")
                         ACE_TEXT ("gen_facet_svnt_defn() ")
                         ACE_TEXT ("failed\n")),
                        -1);
    }
  return 0;
}

int
be_visitor_facet_svts::visit_component (be_component *node)
{
  this->visit_component_scope (node);
  return 0;
}

int
be_visitor_facet_svts::visit_connector (be_connector *node)
{
  return this->visit_component (node);
}


