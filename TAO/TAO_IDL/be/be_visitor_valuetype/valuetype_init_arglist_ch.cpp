//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    valuetype_init_arglist_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for the parameter list of the OBV factory 
//    signature.
//
// = AUTHOR
//    Boris Kolpackov <bosk@ipmce.ru>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_valuetype.h"

ACE_RCSID(be_visitor_valuetype_init, arglist_ch, "$Id$")


be_visitor_valuetype_init_arglist_ch::
be_visitor_valuetype_init_arglist_ch (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_valuetype_init_arglist_ch::
~be_visitor_valuetype_init_arglist_ch (void)
{
}

int
be_visitor_valuetype_init_arglist_ch::visit_factory (be_factory *node)
{
  TAO_OutStream& os = *(this->ctx_->stream ());
  os << " (" << be_idt // idt = 1
     << be_idt_nl; // idt = 2

  // all we do is hand over code generation to our scope
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_init_arglist_ch::"
                         "visit_factory - "
                         "codegen for scope failed\n"),
                        -1);
    }

  os << be_uidt_nl // idt = 1
     << ")" << be_uidt; // idt = 0

  return 0;
}

int
be_visitor_valuetype_init_arglist_ch::visit_argument (be_argument *node)
{
  // get the visitor that will dump the argument's mapping in the operation
  // signature.
  be_visitor_context ctx (*this->ctx_);

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_VALUETYPE_INIT_ARGLIST_CH:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_ARGLIST_CH);
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_valuetype_init_arglist_ch::"
                           "visit_argument - "
                           "Bad context\n"),
                          -1);
      }
    }

  // grab a visitor
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_init_arglist_ch::"
                         "visit_argument - "
                         "Bad visitor\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_init_arglist_ch::"
                         "visit_argument - "
                         "codegen for arglist failed\n"),
                        -1);
    }

  delete visitor;
  return 0;
}

int
be_visitor_valuetype_init_arglist_ch::post_process (be_decl *bd)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // if we are not the last node in the list of arguments, generate a comma.
  if (!this->last_node (bd))
    {
      *os << "," << be_nl;
    }

  return 0;
}
