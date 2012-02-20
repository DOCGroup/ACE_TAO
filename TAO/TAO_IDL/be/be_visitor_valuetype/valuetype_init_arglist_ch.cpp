
//=============================================================================
/**
 *  @file    valuetype_init_arglist_ch.cpp
 *
 *  $Id$
 *
 *  Visitor generating code for the parameter list of the OBV factory
 *  signature.
 *
 *
 *  @author Boris Kolpackov <bosk@ipmce.ru>
 */
//=============================================================================

#include "valuetype.h"

be_visitor_valuetype_init_arglist_ch::be_visitor_valuetype_init_arglist_ch (
    be_visitor_context *ctx
  )
  : be_visitor_scope (ctx)
{
}

be_visitor_valuetype_init_arglist_ch::~be_visitor_valuetype_init_arglist_ch (
  void)
{
}

int
be_visitor_valuetype_init_arglist_ch::visit_factory (be_factory *node)
{
  TAO_OutStream& os = *(this->ctx_->stream ());

  os << " (";

  if (node->nmembers () > 0)
    {
      os << be_idt_nl;

      // All we do is hand over code generation to our scope.
      if (this->visit_scope (node) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            "(%N:%l) be_visitor_valuetype_init_arglist_ch::"
                            "visit_factory - "
                            "codegen for scope failed\n"),
                            -1);
        }

      os << ")" << be_uidt;
    }
  else
    {
      os << "void)";
    }

   return 0;
}

int
be_visitor_valuetype_init_arglist_ch::visit_argument (be_argument *node)
{
  // Get the visitor that will dump the argument's mapping in the operation
  // signature.
  be_visitor_context ctx (*this->ctx_);
  be_visitor_args_arglist visitor (&ctx);
  int status = node->accept (&visitor);

  if (status == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_valuetype_init_arglist_ch::"
                         "visit_argument - "
                         "codegen for arglist failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_valuetype_init_arglist_ch::post_process (be_decl *bd)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // If we are not the last node in the list of arguments, generate a comma.
  if (! this->last_node (bd))
    {
      *os << "," << be_nl;
    }

  return 0;
}

