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
  /*  if ((node->next_state (TAO_CodeGen::TAO_OPERATION_CS, 
                         node->has_extra_code_generation (TAO_CodeGen::TAO_OPERATION_CS)) 
       == TAO_CodeGen::TAO_AMI_SENDC_OPERATION_CS)
      ||(node->next_state (TAO_CodeGen::TAO_OPERATION_CS,
                           node->has_extra_code_generation (TAO_CodeGen::TAO_OPERATION_CS)) 
         == TAO_CodeGen::TAO_AMI_HANDLER_REPLY_STUB_OPERATION_CS)
      ||(node->next_state (TAO_CodeGen::TAO_OPERATION_CS,
                           node->has_extra_code_generation (TAO_CodeGen::TAO_OPERATION_CS)) 
         == TAO_CodeGen::TAO_AMI_EXCEPTION_HOLDER_RAISE_OPERATION_CS))
         return 0;*/

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
  *os << "TAO_ClientRequest_Info_"<<node->flat_name ();
  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context and add a "_get"/"_set" to the flat
  // name to get around the problem of overloaded methods which are
  // generated for attributes.
  if (this->ctx_->attribute ())
    {
      bt = be_type::narrow_from_decl (node->return_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_ch::"
                             "visit_operation - "
                             "Bad return type\n"),
                            -1);
        }
      
      // grab the right visitor to generate the return type if its not
      // void it means it is not the accessor.
      if (!this->void_return_type (bt))
        *os <<"_get";
      else
        *os <<"_set";
    }

  *os << "::"
      << "TAO_ClientRequest_Info_"<< node->flat_name ();
  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context and add a "_get"/"_set" to the flat
  // name to get around the problem of overloaded methods which are
  // generated for attributes.
  if (this->ctx_->attribute ())
    {
      bt = be_type::narrow_from_decl (node->return_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_ch::"
                             "visit_operation - "
                             "Bad return type\n"),
                            -1);
        }
      
      // grab the right visitor to generate the return type if its not
      // void it means it is not the accessor.
      if (!this->void_return_type (bt))
        *os <<"_get";
      else
        *os <<"_set";
    }

  *os<< " (const char *_tao_operation," << be_nl
      << "IOP::ServiceContextList &_tao_service_context_list," << be_nl               
      << "CORBA::Object *_tao_target";
  
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
  *os << "  : TAO_ClientRequest_Info (_tao_operation, _tao_service_context_list, _tao_target)";  

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
  *os << "TAO_ClientRequest_Info_"<<node->flat_name ();
  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context and add a "_get"/"_set" to the flat
  // name to get around the problem of overloaded methods which are
  // generated for attributes.
  if (this->ctx_->attribute ())
    {
      bt = be_type::narrow_from_decl (node->return_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_ch::"
                             "visit_operation - "
                             "Bad return type\n"),
                            -1);
        }
      
      // grab the right visitor to generate the return type if its not
      // void it means it is not the accessor.
      if (!this->void_return_type (bt))
        *os <<"_get";
      else
        *os <<"_set";
    }

  *os<< "::"
      << "arguments (";
  // generate the CORBA::Environment parameter for the alternative mapping
  if (!be_global->exception_support ())
    *os<<"CORBA::Environment &";
  else
    *os << "void";
  *os<<")"<< be_idt_nl;
  *os<<"ACE_THROW_SPEC ((CORBA::SystemException))"<<be_uidt_nl
      << "{" << be_idt_nl 
      << " // Generate the arg list on demand" << be_nl;

  if (node->argument_count () == 0 ||
      // Now make sure that we have some in and inout
      // parameters. Otherwise, there is nothing to be put into
      // the Dyanmic::Paramlist.
      (!(this->has_param_type (node, AST_Argument::dir_IN)) &&
       !(this->has_param_type (node, AST_Argument::dir_INOUT))))
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
  
  os->decr_indent ();
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

  *os << "TAO_ClientRequest_Info_"<<node->flat_name ();
  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context and add a "_get"/"_set" to the flat
  // name to get around the problem of overloaded methods which are
  // generated for attributes.
  if (this->ctx_->attribute ())
    {
      bt = be_type::narrow_from_decl (node->return_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_ch::"
                             "visit_operation - "
                             "Bad return type\n"),
                            -1);
        }
      
      // grab the right visitor to generate the return type if its not
      // void it means it is not the accessor.
      if (!this->void_return_type (bt))
        *os <<"_get";
      else
        *os <<"_set";
    }

  *os<< "::"
      << "exceptions (";
   // generate the CORBA::Environment parameter for the alternative mapping
  if (!be_global->exception_support ())
    *os<<"CORBA::Environment &";
  else
    *os << "void";
  *os<<")"<< be_idt_nl;
  *os<<"ACE_THROW_SPEC ((CORBA::SystemException))"<<be_uidt_nl
    << "{" << be_idt_nl 
     <<"// Generate the exception list on demand " << be_nl;
  if (!node->exceptions ())
    {
      *os << "return 0;\n}\n\n" << be_nl;
    }
  else
    {
      // We change our scope to be able to generate the exceptionlist
        ctx = *this->ctx_;
        ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_EXCEPTLIST);
        visitor = tao_cg->make_visitor (&ctx);
        if (!visitor || (node->accept (visitor) == -1))
          {
            delete visitor;
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%N:%l) be_visitor_operation_cs::"
                               "visit_operation - "
                               "codegen for exceptlist failed\n"),
                              -1);
          }
        delete visitor;

        *os << be_nl << "return &this->exception_list_;"<< be_nl<< "}\n\n";
    }

  os->decr_indent ();
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

  *os << "TAO_ClientRequest_Info_"<<node->flat_name ();
  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context and add a "_get"/"_set" to the flat
  // name to get around the problem of overloaded methods which are
  // generated for attributes.
  if (this->ctx_->attribute ())
    {
      bt = be_type::narrow_from_decl (node->return_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_ch::"
                             "visit_operation - "
                             "Bad return type\n"),
                            -1);
        }
      
      // grab the right visitor to generate the return type if its not
      // void it means it is not the accessor.
      if (!this->void_return_type (bt))
        *os <<"_get";
      else
        *os <<"_set";
    }

  *os<< "::"
      << "result (";
  // generate the CORBA::Environment parameter for the alternative mapping
  if (!be_global->exception_support ())
    *os<<"CORBA::Environment &";
  else
    *os << "void";
  *os<<")"<< be_idt_nl;
  *os<<"ACE_THROW_SPEC ((CORBA::SystemException))"<<be_uidt_nl
     << "{\n // Generate the result on demand :" << be_nl;

  bt = be_type::narrow_from_decl (node->return_type ());
  if (this->void_return_type (bt)) 
    {      
      *os << " CORBA::TypeCode tc (CORBA::tk_void);"<<be_nl
          << "this->result_val_.type (&tc);" << be_nl;
    }
  else
    {
      //      bt = be_type::narrow_from_decl (node->arguments ()->return_type ());
      // Generate the insertion of result into Any.
        ctx = *this->ctx_;
        ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_RESULT);
        visitor = tao_cg->make_visitor (&ctx);
        //        if (!visitor || (node->accept (visitor) == -1))
        if (!visitor || (bt->accept (visitor) == -1))
          {
            delete visitor;
            ACE_ERROR_RETURN ((LM_ERROR,
                               "(%N:%l) be_visitor_operation_cs::"
                               "visit_operation - "
                               "codegen for result failed\n"),
                              -1);
          }
        delete visitor;
    }
  *os  << be_nl << "return &this->result_val_;" << be_nl <<"}\n\n";

  os->decr_indent ();  

  // Update the result.
  bt = be_type::narrow_from_decl (node->return_type ());
  if (!bt)
    {
          ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_ch::"
                             "visit_operation - "
                             "Bad return type\n"),
                            -1);
    }
  // grab the right visitor to generate the return type accessor if
  // its not void since we cant have a private member to be of void
  // type.
  if (!this->void_return_type (bt))
    {
      *os << "void " << be_nl;
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

      *os << "TAO_ClientRequest_Info_"<<node->flat_name ();
  // We need the interface node in which this operation was defined. However,
  // if this operation node was an attribute node in disguise, we get this
  // information from the context and add a "_get"/"_set" to the flat
  // name to get around the problem of overloaded methods which are
  // generated for attributes.
  if (this->ctx_->attribute ())
    {
      bt = be_type::narrow_from_decl (node->return_type ());
      if (!bt)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_ch::"
                             "visit_operation - "
                             "Bad return type\n"),
                            -1);
        }
      
      // grab the right visitor to generate the return type if its not
      // void it means it is not the accessor.
      if (!this->void_return_type (bt))
        *os <<"_get";
      else
        *os <<"_set";
    }

      *os<< "::"
          << "result (";

      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_CH);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor || (bt->accept (visitor) == -1))
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for retval pre invoke failed\n"),
                            -1);
        }
      os->indent ();
      *os << "  result)" << be_uidt << be_uidt << be_uidt_nl
          << "  {" << be_idt_nl
          << "// update the result " << be_nl
          << " this->result_ = result;" << be_uidt_nl
          << "  }\n\n";
    }
  return 0;   
}
