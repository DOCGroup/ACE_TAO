//
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interceptors_ch.cpp
//
// = DESCRIPTION
//    This provides code generation for interceptor classes for an
//    interface in the client header.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_interface.h"

ACE_RCSID(be_visitor_interface, interceptors_ch, "$Id$")


// ************************************************************
//  interceptor class in header
// ************************************************************


be_visitor_interface_interceptors_ch::be_visitor_interface_interceptors_ch
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_interceptors_ch::~be_visitor_interface_interceptors_ch (void)
{
}

int be_visitor_interface_interceptors_ch::visit_interface (be_interface *node)
{
  if (node->is_local ())
    return 0;

  TAO_OutStream *os = this->ctx_->stream ();

  // generate code for the interface definition by traversing thru the
  // elements of its scope. We depend on the front-end to have made sure
  // that only legal syntactic elements appear in our scope.

  os->indent ();
  *os << "\n\n#if (TAO_HAS_INTERCEPTORS == 1)" << be_nl;

  // Generate code for the interface definition by traversing thru the
  // elements of its scope. We depend on the front-end to have made sure
  // that only legal syntactic elements appear in our scope.

  *os << "// Generation of interceptors related RequestInfo"
      << " classes per operation." << be_nl
      << "// This needed to be able to store the arguments, "
      << "exceptions, contexts" << be_nl
      << "// and build the lists dynamically on demand so that "
      << "unnecessary time overhead" << be_nl
      << "// of building these lists when they arent used is "
      << "avoided." << be_nl;

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
