
//=============================================================================
/**
 *  @file    interface_ex_idl.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for Interfaces in the
 *  CIAO executor IDL file.
 *
 *
 *  @author Jeff Parsons <j.parsons@vanderbilt.edu>
 */
//=============================================================================

#include "interface.h"

be_visitor_interface_ex_idl::be_visitor_interface_ex_idl (
  be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_ex_idl::~be_visitor_interface_ex_idl (void)
{
}

int
be_visitor_interface_ex_idl::visit_interface (be_interface *node)
{
  // Skip implied IDL nodes.
  if (node->original_interface () != 0)
    {
      return 0;
    }

  if (node->imported () || node->is_abstract ())
    {
      return 0;
    }

  node->gen_facet_idl (*this->ctx_->stream ());

  return 0;
}


