//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_arglist_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for the parameter list of the AMI
//    Handler's call back Operation signature.
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

ACE_RCSID(be_visitor_operation, ami_handler_arglist_ch, "$Id$")


// ************************************************************
//   operation visitor  to generate the argument list.
//   We have separated code generation for this from the 4 main
//   visitors to avoid code duplication and tight coupling
// ************************************************************

be_visitor_operation_ami_handler_arglist_ch::be_visitor_operation_ami_handler_arglist_ch (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ami_handler_arglist_ch::~be_visitor_operation_ami_handler_arglist_ch (void)
{
}

int
be_visitor_operation_ami_handler_arglist_ch::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  
  *os << " (" << be_idt << be_idt << "\n";

  // All we do is hand over code generation to our scope. 
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_arglist_ch::"
                         "visit_operation - "
                         "codegen for scope failed\n"),
                        -1);
    }

  // No argument for exception. What will you do if you get the
  // exception from the call back method?

  // Arglist is over.
  *os << be_uidt_nl << ")" << be_uidt;

  // @@ No THROW SPECs. You can do anything if the call back guy
  //    throws an exception. But I am not too sure about
  //    this. (Alex). 
  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARGLIST_CH:
      // Each method is pure virtual in the header.
      *os << " = 0;\n\n";
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_handler_arglist_ch::"
                         "visit_operation - "
                         "Bad operation\n"),
                        -1);
    }

  return 0;
}

int
be_visitor_operation_ami_handler_arglist_ch::visit_argument (be_argument *node)
{
  // Get the visitor that will dump the argument's mapping in the
  // operation signature.
  be_visitor_context ctx (*this->ctx_);

  // first grab the interface definition inside which this operation is
  // defined. We need this since argument types may very well be declared
  // inside the scope of the interface node. In such cases, we would like to
  // generate the appropriate relative scoped names.
  be_operation *op = this->ctx_->be_scope_as_operation ();
  if (!op)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_handler_arglist_ch::"
                         "visit_argument - "
                         "Bad operation\n"),
                        -1);
    }

  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context.
  be_interface *intf;
  intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (op->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_handler_arglist_ch::"
                         "visit_argument - "
                         "Bad interface\n"),
                        -1);
    }
  ctx.scope (intf); // set new scope

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARGLIST:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_AMI_HANDLER_ARGLIST);
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_ami_handler_arglist_ch::"
                           "visit_argument - "
                           "Bad context\n"),
                          -1);
      }
    }

  // Grab a visitor.
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_handler_arglist_ch::"
                         "visit_argument - "
                         "Bad visitor\n"),
                        -1);
    }
  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_handler_arglist_ch::"
                         "visit_argument - "
                         "codegen for ami_handler_arglist_ch failed\n"),
                        -1);
    }
  delete visitor;
  return 0;
}

int
be_visitor_operation_ami_handler_arglist_ch::post_process (be_decl *bd)
{
  TAO_OutStream *os = this->ctx_->stream ();

  // if we are not the last node in the list of arguments, generate a comma
  // else decide if we are generating code to support true exceptions - in
  // which case there will not be any CORBA::Environment parameter
  if (!this->last_node (bd))
    {
      *os << ",\n";
    }
  return 0;
}

