//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_init_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Valuetypes factory in the client header
//    (see IDL to C++ mapping)
//
// = AUTHOR
//   Boris Kolpackov <bosk@ipmce.ru>
//
//
// ============================================================================

ACE_RCSID (be_visitor_valuetype,
           valuetype_init_ci,
           "$Id$")

be_visitor_valuetype_init_ci::be_visitor_valuetype_init_ci (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype_init (ctx)
{
}

be_visitor_valuetype_init_ci::~be_visitor_valuetype_init_ci (void)
{
}

