//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_ch.cpp
//
// = DESCRIPTION
//    Visitor generating AMI stub code for Operation node in the
//    client header.  
//
// = AUTHOR
//    Alexander Babu Arulanthu <alex@cs.wustl.edu>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, operation_ami_ch, "$Id$")
  

// ******************************************************
// Visitor for generating AMI stub for "operation" in client header.
// ******************************************************
  
be_visitor_operation_ami_ch::be_visitor_operation_ami_ch (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ami_ch::~be_visitor_operation_ami_ch (void)
{
}

int
be_visitor_operation_ami_ch::visit_operation (be_operation *node)
{
  TAO_OutStream *os; // output stream

  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node

  os->indent (); // start with the current indentation level

  // every operation is declared virtual in the client code
  *os << "virtual ";

  // STEP I: Return type is void.
  *os << "void ";
  
  // STEP 2: generate the operation name.
  
  // First the sendc prefix.
  *os << "sendc_";
  *os << node->local_name ();

  // STEP 3: generate the argument list with the appropriate
  //         mapping. For these we grab a visitor that generates the
  //         parameter listing.
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_OPERATION_AMI_ARGLIST);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_ami_ch::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_ch::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;

  return 0;
}
