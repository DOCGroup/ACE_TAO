//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_operation_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for AMI Handler call back operation in
//    the client header.  
//
// = AUTHOR
//    Aniruddha Gokhale and Alexander Babu Arulanthu
//    <alex@cs.wustl.edu> 
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, ami_handler_operation_ch, "$Id$")


// ************************************************************
// Operation visitor for server header
// ************************************************************

be_visitor_operation_ami_handler_operation_ch::be_visitor_operation_ami_handler_operation_ch (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ami_handler_operation_ch::~be_visitor_operation_ami_handler_operation_ch (void)
{
}

int
be_visitor_operation_ami_handler_operation_ch::visit_operation (be_operation *node)
{
  TAO_OutStream *os; // output stream
  
  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node

  os->indent (); // start with the current indentation level

  // every operation is declared virtual in the client code
  *os << "virtual ";

  // STEP I: generate the return type. Return type is simpy void. 
  *os << "void ";
  
  // STEP 2: generate the operation name.
  *os << " " << node->local_name ();

  // STEP 3: generate the argument list with the appropriate
  //         mapping. For these we grab a visitor that generates the
  //         parameter listing.
  be_visitor_context ctx (*this->ctx_);
  ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARGLIST);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_ami_handler_operation_ch::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_operation_ch::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }
  delete visitor;

  // Generate the corresponding static skeleton method for this
  // operation only if there was no "native" type.
  if (!node->has_native ())
    {
      os->indent ();
      *os << "static void ";
      // Check if we are an attribute node in disguise
      if (this->ctx_->attribute ())
        {
          // now check if we are a "get" or "set" operation
          if (node->nmembers () == 1) // set
            *os << "_set_";
          else
            *os << "_get_";
        }
      *os << node->local_name () <<
        "_skel (" << be_idt << be_idt_nl
          << "CORBA::ServerRequest &_tao_req, " << be_nl
          << "void *_tao_obj, " << be_nl
          << "void *_tao_context, " << be_nl
          << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
	  << "TAO_default_environment ()"
	  << be_uidt << be_uidt_nl
	  << ");" << be_uidt << "\n\n";
    }

  return 0;
}
