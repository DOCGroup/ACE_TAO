//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_handler_arglist.cpp
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

ACE_RCSID(be_visitor_operation, ami_handler_arglist, "$Id$")


// ************************************************************
//   operation visitor  to generate the argument list.
//   We have separated code generation for this from the 4 main
//   visitors to avoid code duplication and tight coupling
// ************************************************************

be_visitor_operation_ami_handler_arglist::be_visitor_operation_ami_handler_arglist (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

// Visit the scope and its elements. 
// This implementation is the same as
// <be_visitor_scope::visit_scope>. The variation is that it calls the
// <post_process>  only if the return value of the <accept> is 1. We
// need to do this since we have to differentiate between an argument
// that was printed and an argument that was skipped in the signature.

int
be_visitor_operation_ami_handler_arglist::visit_scope (be_scope *node)
{
  // Proceed if the number of members in our scope is greater than 0. 
  if (node->nmembers () > 0)
    {
      // initialize an iterator to iterate thru our scope
      UTL_ScopeActiveIterator *si;
      ACE_NEW_RETURN (si,
                      UTL_ScopeActiveIterator (node,
                                               UTL_Scope::IK_decls),
                      -1);
      this->elem_number_ = 0;
      // continue until each element is visited
      while (!si->is_done ())
        {
          AST_Decl *d = si->item ();
          if (!d)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_scope::visit_scope - "
                                 "bad node in this scope\n"), -1);

            }
          be_decl *bd = be_decl::narrow_from_decl (d);
          // set the scope node as "node" in which the code is being
          // generated so that elements in the node's scope can use it
          // for code generation

          this->ctx_->scope (node->decl ());

          // set the node to be visited
          this->ctx_->node (bd);
          this->elem_number_++;

          // Do any pre processing using the next item info. 
          if (this->pre_process (bd) == -1)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_scope::visit_scope - "
                                 "pre processing failed\n"), -1);
            }

          // Send the visitor.
          int visitor_result = 0;
          if (bd == 0 ||  (visitor_result = bd->accept (this)) == -1)
            {
              delete si;
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_scope::visit_scope - "
                                 "codegen for scope failed\n"), -1);

            }

          // Do any post processing using this item info.
          if (visitor_result == 1)
            this->post_process (bd);
          
          // Next argument.
          si->next ();
        } // end of while loop
      delete si;
    } // end of if

  return 0;
}

be_visitor_operation_ami_handler_arglist::~be_visitor_operation_ami_handler_arglist (void)
{
}

int
be_visitor_operation_ami_handler_arglist::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  
  *os << " (" << be_idt << be_idt << be_nl;

  // First argument is a the return value of the operation.
  
  // Indent.
  os->indent ();

  // Get the return type.
  be_type *bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_arglist::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }
  
  // Grab the visitor.
  
  be_visitor_context ctx = *this->ctx_;

  // Set the state.
  ctx.state (TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_RESULT_ARG);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  // Pass the visitor.
  int result_printed = bt->accept (visitor);
  if (result_printed == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_arglist::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }
  delete visitor;
  visitor = 0;
  
  if (result_printed)
    *os << ", " << be_nl;

  // Rest of the arguments.

  // All we do is hand over code generation to our scope.
  int args_printed = this->visit_scope (node);
  if (args_printed == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_handler_arglist::"
                         "visit_operation - "
                         "codegen for scope failed\n"),
                        -1);
    }
  
  // Generate the CORBA::Environment parameter for the alternative
  // mapping.  
  if (!idl_global->exception_support ())
    {
      // If the operation node has parameters, then we need to insert
      // a comma.
      // @@ Fix this.
      // if (result_printed || args_printed)
      //  *os << "," << be_nl;
      
      os->indent ();

      switch (this->ctx_->state ())
        {
        case TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARGLIST_CH:
          // Last argument - is always CORBA::Environment
          *os << "CORBA::Environment &ACE_TRY_ENV";
          *os << " = " << be_idt_nl
              << "TAO_default_environment ()"
              << be_uidt;
          break;
        case TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARGLIST_CS:
          // Last argument - is always CORBA::Environment. 
          *os << "CORBA::Environment &ACE_TRY_ENV";
          break;
        }
    }
  *os << be_uidt_nl << ")" << be_uidt;

  // Now generate the throw specs.
  *os << be_idt_nl << "ACE_THROW_SPEC ((CORBA::SystemException))"
      << be_uidt_nl;

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARGLIST_CH:
      *os << ";" << be_nl << be_nl;
      break;
    default:
      *os << be_nl;
    }
  return 0;
}

int
be_visitor_operation_ami_handler_arglist::visit_argument (be_argument *node)
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
                         "(%N:%l) be_visitor_ami_handler_arglist::"
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
                         "(%N:%l) be_visitor_ami_handler_arglist::"
                         "visit_argument - "
                         "Bad interface\n"),
                        -1);
    }
  ctx.scope (intf); // set new scope

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARGLIST_CH:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_AMI_HANDLER_ARGLIST_CH);
      break;
    case TAO_CodeGen::TAO_AMI_HANDLER_OPERATION_ARGLIST_CS:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_AMI_HANDLER_ARGLIST_CS);
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_ami_handler_arglist::"
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
                         "(%N:%l) be_visitor_operation_ami_handler_arglist::"
                         "visit_argument - "
                         "Bad visitor\n"),
                        -1);
    }

  // Pass the visitor.

  int result = node->accept (visitor);
  if (result == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_handler_arglist::"
                         "visit_argument - "
                         "codegen for ami_handler_arglist failed\n"),
                        -1);
    }
  delete visitor;
  return result;
}

int
be_visitor_operation_ami_handler_arglist::post_process (be_decl *bd)
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

