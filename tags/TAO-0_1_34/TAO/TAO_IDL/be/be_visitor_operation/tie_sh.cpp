//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    tie_sh.cpp
//
// = DESCRIPTION
//    Visitor generating code for operations of the TIE class in the header
//    file.
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"


// ************************************************************
// Operation visitor for server header for TIE class operations
// ************************************************************

be_visitor_operation_tie_sh::be_visitor_operation_tie_sh (be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_tie_sh::~be_visitor_operation_tie_sh (void)
{
}

int
be_visitor_operation_tie_sh::visit_operation (be_operation *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node representing the return type

  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node

  os->indent (); // start with the current indentation level

  // STEP I: generate the return type
  bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_tie_sh::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // grab the right visitor to generate the return type
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_OTHERS);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_tie_sh::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (bt->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_tie_sh::"
                         "visit_operation - "
                         "codegen for return type failed\n"),
                        -1);
    }
  delete visitor;

  // STEP 2: generate the operation name
  *os << " " << node->local_name ();

  // STEP 3: generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing
  ctx = *this->ctx_;
  // we use the _CH state here because the _SH state produces pure virtual
  // methods.
  ctx.state (TAO_CodeGen::TAO_OPERATION_ARGLIST_CH);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_tie_sh::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_tie_sh::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;
  return 0;
}
