//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_interceptors_cs.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation node in the client stubs.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, operation_interceptors_cs, "$Id$")


// ******************************************************
// primary visitor for "operation" in client header
// ******************************************************

be_visitor_operation_interceptors_argument_cs::be_visitor_operation_interceptors_argument_cs (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_interceptors_argument_cs::~be_visitor_operation_interceptors_argument_cs (void)
{
}

int
be_visitor_operation_interceptors_argument_cs::visit_operation (be_operation *node)
{

  TAO_OutStream *os; // output stream
  be_type *bt;       // type node
  be_visitor_context ctx (*this->ctx_);
  be_visitor  *visitor = tao_cg->make_visitor (&ctx);
  
  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node
  os->indent (); // start with the current indentation level

  // Generate the ClientRequest_Info object definition per operation
  // to be used by the interecptors

  *os << "TAO_ClientRequest_Info_"<<node->flat_name ()<< "::"
      << "TAO_ClientRequest_Info_"<< node->flat_name () << " (char *  operation," << be_nl
      << "IOP::ServiceContextList &service_context_list," << be_nl               
      << "CORBA::Object * target";

  // generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing
  
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_CS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                         "codegen for argument pre invoke failed\n"),
                        -1);
    }
  delete visitor;
  os->indent (); 
  *os << ")\n";

  // Generate the member list and set each member
  os->indent (); 
  if (node->argument_count () > 0)
    *os << "  :";  
  
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARG_INFO_CS);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor || (node->accept (visitor) == -1))
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                         "codegen for argument pre invoke failed\n"),
                        -1);
    }
 delete visitor;

  os->indent (); 
  *os << "\n{}\n\n";
  
  *os << "Dynamic::ParameterList *"<< be_nl
      << "TAO_ClientRequest_Info_"<<node->flat_name ()<< "::"
      << "arguments (CORBA::Environment &)"<< be_nl
      << "{\n // Generate the arg list on demand \n return 0;\n}\n\n";

  *os << "Dynamic::ExceptionList *" << be_nl
      << "TAO_ClientRequest_Info_"<<node->flat_name ()<< "::"
      << "exceptions (CORBA::Environment &)"<< be_nl
      << "{\n // Generate the exception list on demand \n return 0;\n}\n\n";

  *os << "CORBA::Any * " << be_nl
      << "TAO_ClientRequest_Info_"<<node->flat_name ()<< "::"
      << "result (CORBA::Environment &)"<< be_nl
      << "{\n // Generate the result on demand \n return 0;\n}\n\n";

  *os << "char * " << be_nl
      << "TAO_ClientRequest_Info_"<<node->flat_name ()<< "::"
      << "received_exception_id (CORBA::Environment &)"<< be_nl
      << "{\n // Return the exception thrown \n return 0;\n}\n\n";

return 0;  
  
}
