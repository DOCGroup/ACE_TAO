//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation node in the client header.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, operation_ch, "$Id$")


// ******************************************************
// primary visitor for "operation" in client header
// ******************************************************

be_visitor_operation_ch::be_visitor_operation_ch (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ch::~be_visitor_operation_ch (void)
{
}

int
be_visitor_operation_ch::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt;

  this->ctx_->node (node);

  // Every operation is declared virtual in the client code.
  *os << "virtual ";

  // STEP I: generate the return type.
  bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ch::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // Grab the right visitor to generate the return type.
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_CH);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_ch::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (bt->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ch::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }

  delete visitor;

  // STEP 2: generate the operation name.
  *os << " " << node->local_name ();

  // STEP 3: generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing.
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_CH);
  visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_ch::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ch::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  delete visitor;

  return 0;
}
