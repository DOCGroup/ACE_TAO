
//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_obv_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Valuetypes
//    OBV_ class implementation
//    (see C++ mapping OMG 20.17)
//
// = AUTHOR
//    Torsten Kuepper  <kuepper2@lfa.uni-wuppertal.de>,
//    based on interface_ch.cpp from Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_valuetype.h"

ACE_RCSID(be_visitor_valuetype_obv_ci, valuetype_obv_ci, "$Id$")


// ******************************************************
// Valuetype visitor for OBV_ class implementation
// ******************************************************

be_visitor_valuetype_obv_ci::be_visitor_valuetype_obv_ci (be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_obv_ci::~be_visitor_valuetype_obv_ci (void)
{
}


// OBV_ class must be in OBV_ namespace
int
be_visitor_valuetype_obv_ci::visit_valuetype (be_valuetype *node)
{
return 0; // %! dead code
  // only visit non-abstract valuetype
  if (node->is_abstract_valuetype ())
    return 0;

  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();
  os->indent ();

  // OBV_ class is only a typedef if we are optimizing accessors
  if (node->opt_accessor ())
    {
    }
  else
    {
    } // if !opt_accessor ()
  return 0;
}


int
be_visitor_valuetype_obv_ci::visit_field (be_field *node)
{
  // dead code
  return 0;
}
