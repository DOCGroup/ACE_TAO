
//=============================================================================
/**
 *  @file    valuetype_init.cpp
 *
 *  $Id$
 *
 *  Coomon visitor for valuetypes factory construct
 *  (see IDL to C++ mapping). Based on ptc/00-01-02.
 *
 *
 *  @author Boris Kolpackov <bosk@ipmce.ru>
 */
//=============================================================================

#include "valuetype.h"

be_visitor_valuetype_init::be_visitor_valuetype_init (
    be_visitor_context *ctx
  )
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_init::~be_visitor_valuetype_init (void)
{
}

void
be_visitor_valuetype_init::begin_public (void)
{
}

void
be_visitor_valuetype_init::begin_private (void)
{
}
