
//=============================================================================
/**
 *  @file    valuetype_si.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Interfaces in the server inline file
 *
 *
 *  @author Jeff Parsons
 */
//=============================================================================


// ************************************************************************
// Valuetype visitor for server inline
// ************************************************************************

be_visitor_valuetype_si::be_visitor_valuetype_si (be_visitor_context *ctx)
  : be_visitor_valuetype (ctx)
{
}

be_visitor_valuetype_si::~be_visitor_valuetype_si (void)
{
}

int
be_visitor_valuetype_si::visit_valuetype (be_valuetype *)
{
  // Nothing needed for now, but the visitor is here just in case.
  return 0;
}

int
be_visitor_valuetype_si::visit_eventtype (be_eventtype *node)
{
  return this->visit_valuetype (node);
}

