//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_thru_poa_collocated_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for thru_poa_collocated operation in the client stub
//    Special code for the AMI Reply Handler
//
// = AUTHOR
//    Aniruddha Gokhale
//    Michael Kircher
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, ami_handler_thru_poa_collocated_ch, "$Id$")


// *************************************************************************
//  be_visitor_operation_ami_handler_thru_poa_collocated_ch --
//  This visitor generates code for the thru_poa_collocated operation signature in a
//  server header file
// *************************************************************************

be_visitor_operation_ami_handler_thru_poa_collocated_ch::be_visitor_operation_ami_handler_thru_poa_collocated_ch
(be_visitor_context *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_ami_handler_thru_poa_collocated_ch::~be_visitor_operation_ami_handler_thru_poa_collocated_ch (void)
{
}

int be_visitor_operation_ami_handler_thru_poa_collocated_ch::visit_operation (be_operation *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node representing the return type

  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node

  os->indent (); // start with the current indentation level

  // every operation is declared virtual in the client code
  *os << "virtual ";

  // STEP I: generate the return type
  bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_sh::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // STEP 2: generate the operation name
  *os << "void " << node->local_name ();

  // STEP 3: generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARGLIST_CH);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_sh::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_sh::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;

  return 0;
}
