//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interface_fwd_ci.cpp
//
// = DESCRIPTION
//    Visitor generating code for Interface_Fwd node in the client inline.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface_fwd.h"

ACE_RCSID(be_visitor_interface_fwd, interface_fwd_ci, "$Id$")


// ********************************************************************
// Visitor implementation for the Interface_Fwd type
// This one for the client inline file
// ********************************************************************

be_visitor_interface_fwd_ci::be_visitor_interface_fwd_ci (be_visitor_context *ctx)
  : be_visitor_decl (ctx)
{
}

be_visitor_interface_fwd_ci::~be_visitor_interface_fwd_ci (void)
{
}

int
be_visitor_interface_fwd_ci::visit_interface_fwd (be_interface_fwd *)
{
  // _var and _out class generation for interfaces has been moved to the
  // .cpp file - nothing to do here at the moment.

  return 0;
}
