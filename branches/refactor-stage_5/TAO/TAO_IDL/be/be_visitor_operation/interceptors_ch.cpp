//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_interceptors_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation node in the client header.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

ACE_RCSID (be_visitor_operation, 
           operation_interceptors_ch, 
           "$Id$")

// ******************************************************
// primary visitor for "operation" in client header
// ******************************************************

be_visitor_operation_interceptors_ch::be_visitor_operation_interceptors_ch (
    be_visitor_context *ctx
  )
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_interceptors_ch::~be_visitor_operation_interceptors_ch (
    void
  )
{
}

int
be_visitor_operation_interceptors_ch::visit_operation (be_operation *)
{
  return 0;
}
