//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_fwd_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interface_Fwd node in the client inline.
//
// = AUTHOR
//    Torsten Kuepper
//    based on code from Aniruddha Gokhale (interface_fwd_ci.cpp)
//
// ============================================================================

ACE_RCSID (be_visitor_valuetype_fwd, 
           valuetype_fwd_ci, 
           "$Id$")

be_visitor_valuetype_fwd_ci::be_visitor_valuetype_fwd_ci (
    be_visitor_context *ctx
  )
  : be_visitor_decl (ctx)
{
}

be_visitor_valuetype_fwd_ci::~be_visitor_valuetype_fwd_ci (void)
{
}

int
be_visitor_valuetype_fwd_ci::visit_valuetype_fwd (be_valuetype_fwd *)
{
  return 0;
}
