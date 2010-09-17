
//=============================================================================
/**
 *  @file    facet_svh.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for a facet servant class in the
 *  servant header.
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


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

