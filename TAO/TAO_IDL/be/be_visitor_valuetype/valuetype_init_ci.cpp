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

be_visitor_valuetype_init_ci::be_visitor_valuetype_init_ci (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype_init (ctx)
{
}

be_visitor_valuetype_init_ci::~be_visitor_valuetype_init_ci (void)
{
}

