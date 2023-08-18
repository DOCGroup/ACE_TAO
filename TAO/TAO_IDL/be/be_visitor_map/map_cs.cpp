
//=============================================================================
/**
 *  @file    map_cs.cpp
 *
 *  Visitor generating code for Maps in the client stubs file
 *
 *  @author Tyler Mayoff
 */
//=============================================================================

#include "map.h"

// ************************************************************
// Root visitor for client stub class
// ************************************************************

be_visitor_map_cs::be_visitor_map_cs (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

int be_visitor_map_cs::visit_map (be_map *)
{
  return 0;
}
