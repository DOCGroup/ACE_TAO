//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    ami_exception_holder_operation_ch.cpp
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

ACE_RCSID(be_visitor_operation, ami_exception_holder_operation_ch, "$Id$")


// ************************************************************
// Operation visitor for server header
// ************************************************************

be_visitor_operation_ami_exception_holder_operation_ch::be_visitor_operation_ami_exception_holder_operation_ch (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_ami_exception_holder_operation_ch::~be_visitor_operation_ami_exception_holder_operation_ch (void)
{
}

int
be_visitor_operation_ami_exception_holder_operation_ch::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  this->ctx_->node (node);

  // Every operation is declared virtual in the client code.
  *os << be_idt << "virtual ";

  // STEP I: Generate the return type. Return type is simpy void.
  *os << "void raise_";

  // Check if we are an attribute node in disguise.
  if (this->ctx_->attribute ())
    {
      // Now check if we are a "get" or "set" operation.
      if (node->nmembers () == 1)
        *os << "set_";
      else
        *os << "get_";
    }

  // Generate the operation name.
  *os  << node->local_name ()
       << " (";

  if (!idl_global->exception_support ())
    {
      *os << "CORBA::Environment &ACE_TRY_ENV =" << be_idt << be_idt_nl
          << "TAO_default_environment ()" << be_uidt << be_uidt_nl
          << ")";
    }
  else
    {
      *os << ")";
    }

  // Now generate the throw specs.
  if (this->gen_throw_spec (node) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%N:%l) be_visitor_operation_ami_exception_holder_operation_ch")
                         ACE_TEXT ("::visit_operation - ")
                         ACE_TEXT ("Failed to generate throw spec\n")),
                        -1);
    }

  *os << be_uidt << be_uidt_nl;

  return 0;
}
