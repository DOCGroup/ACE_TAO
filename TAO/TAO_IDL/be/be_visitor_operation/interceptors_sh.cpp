//
// $Id$
//

// ============================================================================
//
// = LIBRARY
//    TAO IDL
//
// = FILENAME
//    operation_interceptors_sh.cpp
//
// = DESCRIPTION
//    Visitor generating code for Operation node in the server header.
//
// = AUTHOR
//    Kirthika Parameswaran  <kirthika@cs.wustl.edu>
//
// ============================================================================

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, operation_interceptors_sh, "$Id$")


  // ******************************************************
  // primary visitor for "operation" in server header
  // ******************************************************

  be_visitor_operation_interceptors_sh::be_visitor_operation_interceptors_sh (be_visitor_context *ctx)
    : be_visitor_operation (ctx)
{
}

be_visitor_operation_interceptors_sh::~be_visitor_operation_interceptors_sh (void)
{
}

int
be_visitor_operation_interceptors_sh::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = 0;
  be_visitor_context ctx (*this->ctx_);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  // save the node.
  this->ctx_->node (node);

  // Generate the ServerRequestInfo object per operation to 
  // be used by the interecptors.

  *os << "class TAO_ServerRequestInfo_"<< node->flat_name ();

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

      // Grab the right visitor to generate the return type if its not
      // void it means it is not the accessor.
      if (!this->void_return_type (bt))
        {
          *os << "_get";
        }
      else
        {
          *os << "_set";
        }
    }

  *os << " : public TAO_ServerRequestInfo" << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl

    // Need to declare the stub as a friend so that it can access the
    // private members of the Request Info class.
      << "friend class ";

  be_decl *parent =
    be_scope::narrow_from_scope (node->defined_in ())->decl ();

  *os << "POA_" << parent->full_name () << ";"<<be_nl;
  *os << "TAO_ServerRequestInfo_"<< node->flat_name ();

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
        {
          *os << "_get";
        }
      else
        {
          *os << "_set";
        }
    }


  *os << " (" << be_idt_nl
      << "TAO_ServerRequest &_tao_server_request," << be_nl;

  be_interface *intf;
  intf = this->ctx_->attribute ()
    ? be_interface::narrow_from_scope (this->ctx_->attribute ()->defined_in ())
    : be_interface::narrow_from_scope (node->defined_in ());

  if (!intf)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_sh::"
                         "visit_operation - "
                         "bad interface scope\n"),
                        -1);
    }

  // Get the right object implementation.
  *os << intf->full_skel_name () << " *tao_impl";

  // Generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing.

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_SH);
  visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_interceptors_sh::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_interceptors_sh::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  delete visitor;

 *os  << ");\n\n";

  os->indent ();
  // Here I still need to generate the other methods + private args
  *os << "virtual Dynamic::ParameterList * arguments (" 
      << be_idt << be_idt_nl
      << "CORBA::Environment &ACE_TRY_ENV =" << be_idt_nl 
      << "TAO_default_environment ()" << be_uidt << be_uidt_nl
      << ")" << be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException));"
      << be_uidt_nl << be_nl;

  *os << "virtual Dynamic::ExceptionList * exceptions (" 
      << be_idt << be_idt_nl
      << "CORBA::Environment &ACE_TRY_ENV =" << be_idt_nl 
      << "TAO_default_environment ()" << be_uidt << be_uidt_nl
      << ")" << be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException));"
      << be_uidt_nl << be_nl;

  *os << "virtual CORBA::Any * result ("
      << be_idt << be_idt_nl
      << "CORBA::Environment &ACE_TRY_ENV =" << be_idt_nl 
      << "TAO_default_environment ()" << be_uidt << be_uidt_nl
      << ")" << be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException));"
      << be_uidt_nl << be_nl;

  *os << "virtual char * target_most_derived_interface ("
      << be_idt << be_idt_nl
      << "CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())"
      << be_uidt_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException));"
      << be_uidt_nl << be_nl;

  *os << "virtual CORBA::Boolean target_is_a (" << be_idt << be_idt_nl
      << "const char * id," << be_nl
      << "CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())"
      << be_uidt_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException));"
      << be_uidt_nl;

  *os << be_uidt_nl << "private:" << be_idt_nl;

  *os << "TAO_ServerRequestInfo_" << node->flat_name ();

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

      // Grab the right visitor to generate the return type if its not
      // void it means it is not the accessor.
      if (!this->void_return_type (bt))
        {
          *os << "_get";
        }
      else
        {
          *os << "_set";
        }
    }

  *os << " (const "<< "TAO_ServerRequestInfo_"<< node->flat_name ();

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

      // Grab the right visitor to generate the return type if its not
      // void it means it is not the accessor.
      if (!this->void_return_type (bt))
        {
          *os << "_get";
        }
      else
        {
          *os << "_set";
        }
    }


  *os << " &);" << be_nl
      << "void operator= (const "
      << "TAO_ServerRequestInfo_"<< node->flat_name ();

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

      // Grab the right visitor to generate the return type if its not
      // void it means it is not the accessor.
      if (!this->void_return_type (bt))
        {
          *os << "_get";
        }
      else
        {
          *os << "_set";
        }
    }


  *os << " &);" << be_nl;

  *os << be_uidt_nl << "private:" << be_idt_nl;

  // Get the right object implementation.
  *os << intf->full_skel_name () << " *_tao_impl;" << be_nl;

  // Need to generate the args as reference memebers...
  // generate the member list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing and
  // modify it to generate reference members.

  ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_SH);
  visitor = tao_cg->make_visitor (&ctx);

  if (!visitor)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "be_visitor_operation_interceptors_sh::"
                         "visit_operation - "
                         "Bad visitor to return type\n"),
                        -1);
    }

  if (node->accept (visitor) == -1)
    {
      delete visitor;
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_interceptors_sh::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  delete visitor;

  // Store the result for later use.
  // generate the return type.
  bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_ch::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // Grab the right visitor to generate the return type if its not
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

      *os << " result);" << be_nl
          << "// update the result " << be_nl;
    }

  // Generate the result data member.
  // Generate the return type.

  bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_interceptors_sh::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // Grab the right visitor to generate the return type if its not
  // void since we cant have a private member to be of void type.
  if (!this->void_return_type (bt))
    {
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_SH);
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

      *os << " _result;";
    }

  *os << be_uidt_nl << "};" << be_nl << be_nl;

  return 0;
}
