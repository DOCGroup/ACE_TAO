//
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interceptors_ss.cpp
//
// = DESCRIPTION
//    This provides code generation for interceptor classes for an
//    interface in the server source.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, interceptors_ss, "$Id$")


// ************************************************************
//  interceptor class in header
// ************************************************************


be_visitor_interface_interceptors_ss::be_visitor_interface_interceptors_ss
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_interceptors_ss::~be_visitor_interface_interceptors_ss (void)
{
}

int be_visitor_interface_interceptors_ss::visit_interface (be_interface *node)
{
  if (node->is_local ())
    {
      return 0;
    }

  TAO_OutStream *os = this->ctx_->stream ();

  // Generate code for the interface definition by traversing thru the
  // elements of its scope. We depend on the front-end to have made sure
  // that only legal syntactic elements appear in our scope.
  *os << "#if (TAO_HAS_INTERCEPTORS == 1)\n";

  os->reset ();

  // Generate code for the interface definition by traversing thru the
  // elements of its scope. We depend on the front-end to have made sure
  // that only legal syntactic elements appear in our scope.
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interface_ch::"
                         "visit_interface - "
                         "codegen for scope failed\n"), -1);
    }

  *os  << "#endif /* TAO_HAS_INTERCEPTORS */\n";

  return 0;
}
