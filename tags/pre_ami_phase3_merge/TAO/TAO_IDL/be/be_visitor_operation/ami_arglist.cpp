//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_arglist.cpp
//
// = DESCRIPTION
//    Visitor generating code for the parameter list of the Operation signature.
//
// = AUTHOR
//    Alexander Babu Arulanthu <alex@cs.wustl.edu> 
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, ami_arglist, "$Id$")


// ************************************************************
//   operation visitor  to generate the argument list for the AMI
//   stub. 
//   We have separated code generation for this from the 4 main
//   visitors to avoid code duplication and tight coupling
  // ************************************************************
  
be_visitor_operation_ami_arglist::be_visitor_operation_ami_arglist (be_visitor_context
                                                                    *ctx)
  : be_visitor_scope (ctx)
{
}

be_visitor_operation_ami_arglist::~be_visitor_operation_ami_arglist (void)
{
}

int
be_visitor_operation_ami_arglist::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  
  *os << " (" << be_idt << be_idt << "\n";

  
  // Start with current indentation level.
  os->indent ();

  
  be_decl *interface =
    be_interface::narrow_from_scope (node->defined_in ())->decl ();
  if (interface == 0)
    cerr << "Invalid interface";

  // AMI Handler argument.
  *os << "AMI_"
      << interface->fullname ()
      << "_ptr "
      << "ami_handler"
      << ",\n";
  // #endif /* TAO_IDL_HAS_AMI */

  // all we do is hand over code generation to our scope
  if (this->visit_scope (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_ami_arglist::"
                         "visit_operation - "
                         "codegen for scope failed\n"),
                        -1);
    }


  // @@ When some out arguments are skipped, the indentation seems to
  //    get messed up. Fix that. (Alex).
  
  // Last argument - is always CORBA::Environment.
  os->indent ();
  *os << "CORBA::Environment &ACE_TRY_ENV";
  *os << " = " << be_idt_nl
      << "TAO_default_environment ()"
      << be_uidt;
  
  // Done with the argument list.
  *os << be_uidt_nl << ")" << be_uidt << "\n";
  
  return 0;
}

int
be_visitor_operation_ami_arglist::visit_argument (be_argument *node)
{
  // Get the visitor that will dump the argument's mapping in the operation
  // signature.
  be_visitor_context ctx (*this->ctx_);

  // First grab the interface definition inside which this operation is
  // defined. We need this since argument types may very well be declared
  // inside the scope of the interface node. In such cases, we would like to
  // generate the appropriate relative scoped names.
  be_operation *op = this->ctx_->be_scope_as_operation ();
  if (!op)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_arglist::"
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
                         "(%N:%l) be_visitor_ami_arglist::"
                         "visit_argument - "
                         "Bad interface\n"),
                        -1);
    }
  ctx.scope (intf); // set new scope

  switch (this->ctx_->state ())
    {
    case TAO_CodeGen::TAO_OPERATION_AMI_ARGLIST:
      ctx.state (TAO_CodeGen::TAO_ARGUMENT_AMI_ARGLIST);
      break;
    default:
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%N:%l) be_visitor_ami_arglist::"
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
                         "(%N:%l) be_visitor_ami_arglist::"
                         "visit_argument - "
                         "Bad visitor\n"),
                        -1);
    }
  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_ami_arglist::"
                         "visit_argument - "
                         "codegen for ami_arglist failed\n"),
                        -1);
    }
  delete visitor;
  return 0;
}
