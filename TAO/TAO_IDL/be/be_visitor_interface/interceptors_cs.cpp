//
// $Id$
//
// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    interceptors_cs.cpp
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

ACE_RCSID(be_visitor_interface, interceptors_cs, "$Id$")


// ************************************************************
//  interceptor class in header
// ************************************************************


be_visitor_interface_interceptors_cs::be_visitor_interface_interceptors_cs
(be_visitor_context *ctx)
  : be_visitor_interface (ctx)
{
}

be_visitor_interface_interceptors_cs::~be_visitor_interface_interceptors_cs (void)
{
}

int be_visitor_interface_interceptors_cs::visit_interface (be_interface *node)
{

  TAO_OutStream *os = this->ctx_->stream ();

  be_type *bt;
  
  // set the right type;
  if (this->ctx_->alias ())
    bt = this->ctx_->alias ();
  else
    bt = node;
  // output the class defn

  // generate code for the interface definition by traversing thru the
  // elements of its scope. We depend on the front-end to have made sure
  // that only legal syntactic elements appear in our scope.
  
  os->indent ();
  *os << "\n#if (TAO_HAS_INTERCEPTORS == 1)" << be_nl;
  
   // generate code for the interface definition by traversing thru the
      // elements of its scope. We depend on the front-end to have made sure
      // that only legal syntactic elements appear in our scope.

      os->indent ();
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interface_ch::"
                             "visit_interface - "
                             "codegen for scope failed\n"), -1);
        }
  /*
  be_visitor_context ctx (*this->ctx_);
  be_visitor *visitor = 0;
  // Interceptor related classes.
  ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_CS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_interface_cs::"
                         "visit_interface - "
                         "codegen for interceptor classes failed\n"),
                                -1);
    }
    delete visitor;*/

  *os  << "#endif /* TAO_HAS_INTERCEPTORS */\n";
  
  return 0;

}
