
//=============================================================================
/**
 *  @file    facet_svs.cpp
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

be_visitor_facet_svs::be_visitor_facet_svs (be_visitor_context *ctx)
  : be_visitor_component_scope (ctx)
{
}

be_visitor_facet_svs::~be_visitor_facet_svs (void)
{
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

