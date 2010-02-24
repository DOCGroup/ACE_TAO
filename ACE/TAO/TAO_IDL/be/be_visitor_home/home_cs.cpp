//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    home_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Component Home node in the client source.
//
// = AUTHOR
//    Jeff Parsons
//
// ============================================================================

ACE_RCSID (be_visitor_home, 
           home_cs, 
           "$Id$")

be_visitor_home_cs::be_visitor_home_cs (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_home_cs::~be_visitor_home_cs (void)
{
}

int
be_visitor_home_cs::visit_home (be_home *node)
{
  if (node->cli_stub_gen () || node->imported ())
    {
      return 0;
    }

    if (be_global->tc_support ())
      {
        be_visitor_context ctx (*this->ctx_);
        ctx.node (node);
        TAO::be_visitor_objref_typecode tc_visitor (&ctx);

        // No double dispatch here so the objref_typecode visitor
        // can dynamic_cast the node to tell it's a home.
        if (tc_visitor.visit_interface (node) == -1)
          {
            ACE_ERROR_RETURN ((LM_ERROR,
                              "(%N:%l) be_visitor_home_cs::"
                              "visit_home - "
                              "TypeCode definition failed\n"),
                              -1);
          }
      }

  node->cli_stub_gen (true);
  return 0;
}
