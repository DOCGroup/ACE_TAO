// $Id$

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, inv_arglist, "$Id$")


be_visitor_operation_inv_arglist::
be_visitor_operation_inv_arglist (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_inv_arglist::~be_visitor_operation_inv_arglist (void)
{
}

int
be_visitor_operation_inv_arglist::visit_operation (be_operation *node)
{
  // all we do is hand over code generation to our scope
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_inv_arglist::"
                         "visit_operation - "
                         "codegen for scope failed\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_operation_inv_arglist::visit_argument (be_argument *node)
{
  // get the visitor that will dump the argument's mapping in the operation
  // signature.
  be_visitor_context ctx (*this->ctx_);

  // first grab the interface definition inside which this operation is
  // defined. We need this since argument types may very well be declared
  // inside the scope of the interface node. In such cases, we would like to
  // generate the appropriate relative scoped names.
  be_operation *op = this->ctx_->be_scope_as_operation ();
  if (!op)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_inv_arglist::"
                         "visit_argument - "
                         "Bad operation\n"),
                        -1);
    }

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context
  be_interface *intf;
  intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (op->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_inv_arglist::"
                         "visit_argument - "
                         "Bad interface\n"),
                        -1);
    }
  ctx.scope (intf); // set new scope

  ctx.state (TAO_CodeGen::TAO_ARGUMENT_INVOKE_ARG_LIST);

  // grab a visitor
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_inv_arglist::"
                         "visit_argument - "
                         "Bad visitor\n"),
                        -1);
    }
  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_inv_arglist::"
                         "visit_argument - "
                         "codegen for inv_arglist failed\n"),
                        -1);
    }
  delete visitor;
  return 0;
}


