//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_interceptors_ch.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation node in the client header.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#include	"idl.h"
#include	"idl_extern.h"
#include	"be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, operation_interceptors_ch, "$Id$")


// ******************************************************
// primary visitor for "operation" in client header
// ******************************************************

be_visitor_operation_interceptors_ch::be_visitor_operation_interceptors_ch (be_visitor_context *ctx)
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_interceptors_ch::~be_visitor_operation_interceptors_ch (void)
{
}

int
be_visitor_operation_interceptors_ch::visit_operation (be_operation *node)
{
  TAO_OutStream *os; // output stream
  be_type *bt;       // type node
  be_visitor_context ctx (*this->ctx_);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);
  os = this->ctx_->stream ();
  this->ctx_->node (node); // save the node
  
  // Generate the ClientRequest_Info object per operation to be used by the interecptors
  os->indent (); // start with the current indentation level
  *os << "class TAO_ClientRequest_Info_"<< node->flat_name ()
      << " : public TAO_ClientRequest_Info" << be_nl
      << "{" << be_nl
      << "public:"<< be_idt_nl
    // Need to declare the stub as a friend so that it can access the 
    // private members of the Request Info class.
      << "friend class ";
  be_decl *parent =
    be_scope::narrow_from_scope (node->defined_in ())->decl ();
  *os << parent->full_name () <<";"<< be_nl;
  
  *os << "TAO_ClientRequest_Info_"<< node->flat_name () << " (" << be_idt_nl 
      << "const char *  operation,"<< be_nl 
      << "IOP::ServiceContextList &service_context_list," << be_nl
      << "CORBA::Object * target" << be_uidt;

  // This necesary becos: (a) a comma is needed if there are arguments
  // (b) not needed if exceptions enabled since thats done already (c)
  // not needed if there are no args and exceptions is disabled.

  os->indent ();
  if (node->argument_count () > 0)
    *os << ",\n";

  //generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing
  
  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_CH);
  visitor = tao_cg->make_visitor (&ctx);
  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_interceptors_ch::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_interceptors_ch::"
                             "visit_operation - "
                             "codegen for argument list failed\n"),
                            -1);
        }
      delete visitor;

      os->indent ();
      // Here I still need to generate the other methods + private args
      *os << " virtual Dynamic::ParameterList * arguments ("<<be_idt_nl 
          << "CORBA::Environment &ACE_TRY_ENV =" <<be_idt_nl
          << " TAO_default_environment ())" << be_uidt_nl
          << " ACE_THROW_SPEC ((CORBA::SystemException));" 
          << be_uidt_nl << be_nl;
      os->indent ();
      *os << "virtual Dynamic::ExceptionList * exceptions "<<be_idt_nl 
          <<"(CORBA::Environment &ACE_TRY_ENV = " <<be_idt_nl
          << "TAO_default_environment ())"<< be_uidt_nl
          <<"ACE_THROW_SPEC ((CORBA::SystemException));"
          << be_uidt_nl << be_nl;
      os->indent ();
      *os << "virtual CORBA::Any * result ( "<< be_idt_nl 
          <<"CORBA::Environment &ACE_TRY_ENV ="<< be_idt_nl
          << "TAO_default_environment ())"<< be_uidt_nl
          << "ACE_THROW_SPEC ((CORBA::SystemException));"
          << be_uidt_nl << be_nl;
  
      os->indent ();
      *os << be_uidt_nl << "private:" <<be_nl;

      *os << "TAO_ClientRequest_Info_"<< node->flat_name () 
          << " (const "<< "TAO_ClientRequest_Info_"<< node->flat_name ()
          << " &);" << be_nl 
          << "void operator= (const "
          << "TAO_ClientRequest_Info_"<< node->flat_name () << " &);"<< be_nl;
      // need to generate the args as reference memebers...
      //generate the member list with the appropriate mapping. For these
      // we grab a visitor that generates the parameter listing and
      // modify it to generate reference members.
      
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_CH);
      visitor = tao_cg->make_visitor (&ctx);
      if (!visitor)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "be_visitor_operation_interceptors_ch::"
                             "visit_operation - "
                             "Bad visitor to return type\n"),
                            -1);
        }

      if (node->accept (visitor) == -1)
        {
          delete visitor;
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_interceptors_ch::"
                             "visit_operation - "
                             "codegen for argument list failed\n"),
                            -1);
        }
      delete visitor;

      // Store the result for later use.
      // generate the return type 
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
      // void since we cant have a private member to be of void type.
      if (!this->void_return_type (bt))
        {
          *os << "void result (";
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
          *os << "  result);" << be_uidt << be_uidt << be_uidt_nl
              << " // update the result " << be_nl;
        }
      
      // Generate the result data member
      // generate the return type 

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
      // void since we cant have a private member to be of void type.
      if (!this->void_return_type (bt))
        {
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
          *os << "  result_;" << be_uidt << be_uidt << be_uidt_nl;
        }
      os->decr_indent ();
      *os << "};\n\n";
      return 0;
}

