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

  be_visitor_operation_interceptors_cs::be_visitor_operation_interceptors_cs (be_visitor_context *ctx)
    : be_visitor_operation (ctx)
{
}

be_visitor_operation_interceptors_cs::~be_visitor_operation_interceptors_cs (void)
{
}

int
be_visitor_operation_interceptors_cs::visit_operation (be_operation *node)
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
  if (node->is_nested ())
    {
      be_decl *parent =
        be_scope::narrow_from_scope (node->defined_in ())->decl ();
      // But since we are at the interface level our parents full_name
      // will include the interface name which we dont want and so we 
      // get our parent's parent's full name.
      //    be_interface *parent_interface = be_interface::narrow_from_decl (parent);
      // be_decl *parents_parent = be_interface::narrow_from_scope (parent_interface->scope ())->decl ();
      // Generate the scope::operation name.
      //  *os << parents_parent->full_name () << "::";
      *os << parent->full_name () << "::";
    }
  *os << "TAO_ClientRequest_Info_"<<node->flat_name ()<< "::"
      << "TAO_ClientRequest_Info_"<< node->flat_name () << " (const char *  operation," << be_nl
      << "IOP::ServiceContextList &service_context_list," << be_nl               
      << "CORBA::Object * target";
  
  if (node->argument_count () > 0)
    *os << "," << be_nl;
  
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
  *os << ")"<<be_nl;

  // Generate the member list and set each member but before that, 
  // its necessary to pass on some args to the base class.
  os->indent (); 
  *os << "  : TAO_ClientRequest_Info (operation, service_context_list, target)";  
  if (node->argument_count () > 0)
    *os << "," << be_idt_nl;

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

  os->decr_indent (); 
  *os << be_nl << "{}\n\n";
  
  *os << "Dynamic::ParameterList *"<< be_nl;
  if (node->is_nested ())
    {
      be_decl *parent =
        be_scope::narrow_from_scope (node->defined_in ())->decl ();
      // But since we are at the interface level our parents full_name
      // will include the interface name which we dont want and so we 
      // get our parent's parent's full name.
      //    be_interface *parent_interface = be_interface::narrow_from_decl (parent);
      // be_decl *parents_parent = be_interface::narrow_from_scope (parent_interface->scope ())->decl ();
      // Generate the scope::operation name.
      //  *os << parents_parent->full_name () << "::";
      *os << parent->full_name () << "::";
    }

  // The interceptors cant modify "in" and "out" parameters.
  *os << "TAO_ClientRequest_Info_"<<node->flat_name ()<< "::"
      << "arguments (CORBA::Environment &)"<< be_nl
      << "{" << be_idt_nl 
      << " // Generate the arg list on demand" << be_nl;

  if (node->argument_count () == 0)
    {
      *os << "return 0;\n}\n\n" << be_nl;
    }
  else
    {
      // The insertion operator is different for different nodes.
      // We change our scope to go to the argument scope to
      // be able to decide this.
        ctx = *this->ctx_;
        ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_PARAMLIST);
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

        *os << be_nl << "return &this->parameter_list_;\n}\n\n";
    }

  *os << "Dynamic::ExceptionList *" << be_nl;
  if (node->is_nested ())
    {
      be_decl *parent =
        be_scope::narrow_from_scope (node->defined_in ())->decl ();
      // But since we are at the interface level our parents full_name
      // will include the interface name which we dont want and so we 
      // get our parent's parent's full name.
      //    be_interface *parent_interface = be_interface::narrow_from_decl (parent);
      // be_decl *parents_parent = be_interface::narrow_from_scope (parent_interface->scope ())->decl ();
      // Generate the scope::operation name.
      //  *os << parents_parent->full_name () << "::";
      *os << parent->full_name () << "::";
    }

  *os << "TAO_ClientRequest_Info_"<<node->flat_name ()<< "::"
      << "exceptions (CORBA::Environment &)"<< be_nl
      << "{" << be_idt_nl 
      <<"// Generate the exception list on demand \n return 0;\n}\n\n";

  *os << "CORBA::Any * " << be_nl;
  if (node->is_nested ())
    {
      be_decl *parent =
        be_scope::narrow_from_scope (node->defined_in ())->decl ();
      // But since we are at the interface level our parents full_name
      // will include the interface name which we dont want and so we 
      // get our parent's parent's full name.
      //    be_interface *parent_interface = be_interface::narrow_from_decl (parent);
      // be_decl *parents_parent = be_interface::narrow_from_scope (parent_interface->scope ())->decl ();
      // Generate the scope::operation name.
      //  *os << parents_parent->full_name () << "::";
      *os << parent->full_name () << "::";
    }

  *os << "TAO_ClientRequest_Info_"<<node->flat_name ()<< "::"
      << "result (CORBA::Environment &)"<< be_nl
      << "{\n // Generate the result on demand \n return 0;\n}\n\n";

  *os << "char * " << be_nl;
  if (node->is_nested ())
    {
      be_decl *parent =
        be_scope::narrow_from_scope (node->defined_in ())->decl ();
      // But since we are at the interface level our parents full_name
      // will include the interface name which we dont want and so we 
      // get our parent's parent's full name.
      //    be_interface *parent_interface = be_interface::narrow_from_decl (parent);
      // be_decl *parents_parent = be_interface::narrow_from_scope (parent_interface->scope ())->decl ();
      // Generate the scope::operation name.
      //  *os << parents_parent->full_name () << "::";
      *os << parent->full_name () << "::";
    }

  *os << "TAO_ClientRequest_Info_"<<node->flat_name ()<< "::"
      << "received_exception_id (CORBA::Environment &)"<< be_nl
      << "{\n // Return the exception thrown \n return 0;\n}\n\n";

  return 0;  
  
}
