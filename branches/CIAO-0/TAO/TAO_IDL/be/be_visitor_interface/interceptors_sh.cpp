//
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interceptors_sh.cpp
//
// = DESCRIPTION
//    This provides code generation for interceptor classes for an
//    interface in the server header.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, interceptors_sh, "$Id$")


// ************************************************************
//  interceptor class in header
// ************************************************************


be_visitor_interface_interceptors_sh::be_visitor_interface_interceptors_sh
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_interceptors_sh::~be_visitor_interface_interceptors_sh (void)
{
}

int be_visitor_interface_interceptors_sh::visit_interface (be_interface *)
{
  return 0;
}
