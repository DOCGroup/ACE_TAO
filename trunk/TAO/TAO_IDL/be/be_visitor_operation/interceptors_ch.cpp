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

#include        "idl.h"
#include        "idl_extern.h"
#include        "be.h"

#include "be_visitor_operation.h"

ACE_RCSID(be_visitor_operation, operation_interceptors_ch, "$Id$")


  // ******************************************************
  // primary visitor for "operation" in client header
  // ******************************************************

be_visitor_operation_interceptors_ch::be_visitor_operation_interceptors_ch (
    be_visitor_context *ctx
  )
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_interceptors_ch::~be_visitor_operation_interceptors_ch (
    void
  )
{
}

int
be_visitor_operation_interceptors_ch::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();
  be_type *bt = 0;
  be_visitor_context ctx (*this->ctx_);
  be_visitor *visitor = tao_cg->make_visitor (&ctx);

  // Save the node
  this->ctx_->node (node);

  // Generate the ClientRequestInfo object per operation to
  // be used by the interecptors.

  // Start with the current indentation level.
  *os << "class TAO_ClientRequestInfo_" << node->flat_name ();

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

  *os << " : public TAO_ClientRequestInfo" << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl

    // Need to declare the stub as a friend so that it can access the
    // private members of the Request Info class.
      << "friend class ";

  be_decl *parent =
    be_scope::narrow_from_scope (node->defined_in ())->decl ();

  *os << parent->full_name () << ";" << be_nl << be_nl;

  // The Proxy Implementation actually perform calls to
  // the ClientRequestInfo class, so these class need
  // to be friend as well.
  be_interface *iface = be_interface::narrow_from_scope (node->defined_in ());
  *os << "friend class " << iface->remote_proxy_impl_name () << ";" << be_nl
      << "friend class " << iface->thru_poa_proxy_impl_name () << ";" << be_nl
      << "friend class " << iface->direct_proxy_impl_name () << ";" << be_uidt_nl << be_nl;

  *os << "TAO_ClientRequestInfo_" << node->flat_name ();

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

  *os << " (" << be_idt << be_idt_nl
      << "TAO_GIOP_Invocation *_tao_invocation," << be_nl
      << "CORBA::Object_ptr _tao_target";

  // Generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing.
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

  if (be_global->exception_support ())
    {
      *os << be_uidt;
    }

  *os << ");" << be_uidt_nl << be_nl;

  // Here I still need to generate the other methods + private args.
  *os << "virtual Dynamic::ParameterList * arguments "
      << "(" << be_idt << be_idt_nl
      << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
      << "TAO_default_environment ()" << be_uidt << be_uidt_nl
      << ")" << be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException));"
      << be_uidt_nl << be_nl;

  *os << "virtual Dynamic::ExceptionList * exceptions "
      << "(" << be_idt << be_idt_nl
      << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
      << "TAO_default_environment ()" << be_uidt << be_uidt_nl
      << ")" << be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException));"
      << be_uidt_nl << be_nl;

  *os << "virtual CORBA::Any * result "
      << "(" << be_idt << be_idt_nl
      << "CORBA::Environment &ACE_TRY_ENV = " << be_idt_nl
      << "TAO_default_environment ()" << be_uidt << be_uidt_nl
      << ")" << be_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException));"
      << be_uidt_nl << be_uidt_nl;

  *os << "private:" << be_idt_nl;

  *os << "TAO_ClientRequestInfo_" << node->flat_name ();

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

  *os << " (const " << "TAO_ClientRequestInfo_" << node->flat_name ();

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
      << "TAO_ClientRequestInfo_"<< node->flat_name ();

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

  // Need to generate the args as reference memebers...
  // Generate the member list with the appropriate mapping. For these
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
  // void since we can't have a private member to be of void type.
  if (!this->void_return_type (bt))
    {
      *os << "void result (";
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_RETTYPE_CH);
      ctx.sub_state (TAO_CodeGen::TAO_INTERCEPTORS_INFO_STUB);
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
                         "(%N:%l) be_visitor_interceptors_ch::"
                         "visit_operation - "
                         "Bad return type\n"),
                        -1);
    }

  // Grab the right visitor to generate the return type if it's not
  // void since we can't have a private member to be of void type.
  if (!this->void_return_type (bt))
    {
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_RETTYPE_CH);
      ctx.sub_state (TAO_CodeGen::TAO_INTERCEPTORS_INFO_STUB);
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

      *os << " _result;" << be_uidt_nl;
    }
  else
    {
      *os << be_uidt_nl;
    }

  *os << "};" << be_nl << be_nl;

  return 0;
}
