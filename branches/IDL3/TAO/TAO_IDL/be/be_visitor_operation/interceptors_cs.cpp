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

#include "idl.h"
#include "idl_extern.h"
#include "be.h"
#include "be_visitor_operation.h"

ACE_RCSID (be_visitor_operation,
           operation_interceptors_cs,
           "$Id$")


// ******************************************************
// Primary visitor for "operation" in client header
// ******************************************************

be_visitor_operation_interceptors_cs::be_visitor_operation_interceptors_cs (
    be_visitor_context *ctx
  )
  : be_visitor_operation (ctx)
{
}

be_visitor_operation_interceptors_cs::~be_visitor_operation_interceptors_cs (
    void
  )
{
}

int
be_visitor_operation_interceptors_cs::visit_operation (be_operation *node)
{
  TAO_OutStream *os = this->ctx_->stream ();

  if (this->generate_class_declaration (os, node) == -1)
    {
      return -1;
    }

  if (this->generate_class_definition (os, node) == -1)
    {
      return -1;
    }

  return 0;
}

int
be_visitor_operation_interceptors_cs::generate_class_declaration (
    TAO_OutStream *os,
    be_operation *node
  )
{
  be_type *bt = 0;
  be_visitor_context ctx (*this->ctx_);

  // Save the node.
  this->ctx_->node (node);

  // Generate the ClientRequestInfo object per operation to
  // be used by the interceptors.

  // Start with the current indentation level.
  os->indent ();

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

  *os << " : public TAO_ClientRequestInfo_i" << be_nl
      << "{" << be_nl
      << "public:" << be_idt_nl;

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
  be_visitor_operation_interceptors_arglist oia_visitor (&ctx);

  if (node->accept (&oia_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_interceptors_ch::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
    }

  if (be_global->exception_support ())
    {
      *os << be_uidt;
    }

  *os << ");\n" << be_uidt_nl;

  // Here I still need to generate the other methods + private args.
  *os << "virtual Dynamic::ParameterList * arguments "
      << "(ACE_ENV_SINGLE_ARG_DECL)" << be_idt_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException));"
      << be_uidt_nl << be_nl;

  *os << "virtual Dynamic::ExceptionList * exceptions "
      << "(ACE_ENV_SINGLE_ARG_DECL)" << be_idt_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException));"
      << be_uidt_nl << be_nl;

  *os << "virtual CORBA::Any * result "
      << "(ACE_ENV_SINGLE_ARG_DECL)" << be_idt_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException));\n"
      << be_uidt;

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
      *os << be_nl << "void result (";
      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_RETTYPE_CH);
      be_visitor_operation_interceptors_info_rettype oiir_visitor (&ctx);

      if (bt->accept (&oiir_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for retval pre invoke failed\n"),
                            -1);
        }

      *os << " result);\n";
    }

  *os << be_uidt_nl << "private:" << be_idt_nl;
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

  *os << " &);\n" << be_nl;

  // Need to generate the args as reference memebers...
  // Generate the member list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing and
  // modify it to generate reference members.
  ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_INFO_ARGLIST_CH);
  be_visitor_operation_interceptors_arglist oiia_visitor (&ctx);

  if (node->accept (&oiia_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_interceptors_ch::"
                         "visit_operation - "
                         "codegen for argument list failed\n"),
                        -1);
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
      be_visitor_operation_interceptors_info_rettype toiir_visitor (&ctx);

      if (bt->accept (&toiir_visitor) == -1)
        {
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

  *os << "};\n" << be_nl;

  return 0;
}

int
be_visitor_operation_interceptors_cs::generate_class_definition (
    TAO_OutStream *os,
    be_operation *node
  )
{
  // Start with the current indentation level.
  os->indent ();

  be_type *bt = 0;
  be_visitor_context ctx (*this->ctx_);

  // Save the node.
  this->ctx_->node (node);

  *os << "TAO_ClientRequestInfo_"<< node->flat_name ();

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

  *os << "::"
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

  *os << " (" << be_idt << be_idt_nl
      << "TAO_GIOP_Invocation *_tao_invocation," << be_nl
      << "CORBA::Object_ptr _tao_target";

  // Generate the argument list with the appropriate mapping. For these
  // we grab a visitor that generates the parameter listing.

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARGLIST_CS);
  be_visitor_operation_interceptors_arglist toiia_visitor (&ctx);

  if (node->accept (&toiia_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for argument pre invoke failed\n"),
                        -1);
    }

  os->indent ();
  *os << be_uidt_nl << ")" << be_uidt_nl;

  // Generate the member list and set each member but before that,
  // its necessary to pass on some args to the base class.
  os->indent ();
  *os << "  : TAO_ClientRequestInfo_i (_tao_invocation, _tao_target)";

  ctx = *this->ctx_;
  ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_ARG_INFO_CS);
  be_visitor_operation_interceptors_arglist oiai_visitor (&ctx);

  if (node->accept (&oiai_visitor) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "(%N:%l) be_visitor_operation_cs::"
                         "visit_operation - "
                         "codegen for argument pre invoke failed\n"),
                        -1);
    }

  os->decr_indent ();
  *os << be_nl << "{}\n\n";

  // -----------------------------------------------------------------
  // PortableInterceptor::ClientRequestInfo::arguments()
  // -----------------------------------------------------------------
  *os << "Dynamic::ParameterList *" << be_nl;

  // The interceptors cant modify "in" and "out" parameters.
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

  *os << "::arguments (ACE_ENV_SINGLE_ARG_DECL)" << be_idt_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException))"<<  be_uidt_nl
      << "{" << be_idt_nl;

  if (be_global->any_support ())
    {
      *os << "// Generate the argument list on demand." << be_nl
          << "Dynamic::ParameterList *parameter_list ="  << be_idt_nl
          << "TAO_RequestInfo_Util::make_parameter_list "
          << "(ACE_ENV_SINGLE_ARG_PARAMETER);"
          << be_uidt_nl
          << "ACE_CHECK_RETURN (0);" << be_nl
          << be_nl;

      size_t parameter_count = this->count_non_out_parameters (node);

      if (node->argument_count () == 0
          || parameter_count == 0
          // Now make sure that we have some in and inout
          // parameters. Otherwise, there is nothing to be put into
          // the Dyanmic::Paramlist.
          || (!(this->has_param_type (node, AST_Argument::dir_IN)) 
              && !(this->has_param_type (node, AST_Argument::dir_INOUT))))
        {
          *os << "return parameter_list;" << be_uidt_nl;
        }
      else
        {
          *os << "Dynamic::ParameterList_var safe_parameter_list = "
              << "parameter_list;" << be_nl;

          // Precompute the length of the Dynamic::ParameterList.  This is
          // a nice optimization since it reduces the number of additional
          // allocations to one, instead of one for each argument, in
          // addition to remove all copying that occured when growing the
          // sequence for each parameter.

          *os << be_nl
              << "parameter_list->length (" << parameter_count << ");"
              << be_nl;

          *os << "CORBA::ULong len = 0;" << be_nl << be_nl;

          // The insertion operator is different for different nodes.  We
          // change our scope to go to the argument scope to be able to
          // decide this.
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_PARAMLIST);
          be_visitor_operation_interceptors_arglist oip_visitor (&ctx);

          if (node->accept (&oip_visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_operation_cs::"
                                 "visit_operation - "
                                 "codegen for argument pre invoke failed\n"),
                                -1);
            }

          *os << be_nl
              << "return safe_parameter_list._retn ();";
        }
    }
  else
    {
      if (be_global->use_raw_throw ())
        {
          *os << "throw (CORBA::NO_IMPLEMENT ());" << be_nl
              << "return 0;";
        }
      else
        {
          *os << "ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);";
        }
    }

  *os << be_uidt_nl << "}\n\n";

  os->decr_indent ();

  // -----------------------------------------------------------------
  // PortableInterceptor::ClientRequestInfo::exceptions()
  // -----------------------------------------------------------------
  *os << "Dynamic::ExceptionList *" << be_nl;

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

  *os << "::exceptions (ACE_ENV_SINGLE_ARG_DECL)"<< be_idt_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException))" << be_uidt_nl
      << "{" << be_idt_nl;

  if (be_global->tc_support ())
    {
      *os << "// Generate the exception list on demand." << be_nl
          << "Dynamic::ExceptionList *exception_list ="  << be_idt_nl
          << "TAO_RequestInfo_Util::make_exception_list "
          << "(ACE_ENV_SINGLE_ARG_PARAMETER);"
          << be_uidt_nl
          << "ACE_CHECK_RETURN (0);" << be_nl
          << be_nl;

      if (!node->exceptions ())
        {
          *os << "return exception_list;";
        }
      else
        {
          *os << "Dynamic::ExceptionList_var safe_exception_list = "
              << "exception_list;" << be_nl;

          // We change our scope to be able to generate the exceptionlist.
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_EXCEPTLIST);
          be_visitor_operation_interceptors_exceptlist oie_visitor (&ctx);

          if (node->accept (&oie_visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_operation_cs::"
                                 "visit_operation - "
                                 "codegen for exceptlist failed\n"),
                                -1);
            }

          *os << be_nl
              << "return safe_exception_list._retn ();";
        }
    }
  else
    {
      if (be_global->use_raw_throw ())
        {
          *os << "throw (CORBA::NO_IMPLEMENT ());" << be_nl
              << "return 0;";
        }
      else
        {
          *os << "ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);";
        }
    }

  *os << be_uidt_nl << "}\n\n";

  os->decr_indent ();

  // -----------------------------------------------------------------
  // PortableInterceptor::ClientRequestInfo::result()
  // -----------------------------------------------------------------
  *os << "CORBA::Any * " << be_nl;

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

  *os << "::result (ACE_ENV_SINGLE_ARG_DECL)" << be_idt_nl
      << "ACE_THROW_SPEC ((CORBA::SystemException))" << be_uidt_nl
      << "{" << be_idt_nl;

  if (be_global->any_support ())
    {
      *os << "// Generate the result on demand." << be_nl;

      bt = be_type::narrow_from_decl (node->return_type ());

      if (this->void_return_type (bt))
        {
          // Return an Any with tk_void TypeCode.
          *os << "CORBA::Boolean tk_void_any = 1;" << be_nl
              << "CORBA::Any *result_any ="  << be_idt_nl
              << "TAO_RequestInfo_Util::make_any "
              << "(tk_void_any ACE_ENV_ARG_PARAMETER);"
              << be_uidt_nl
              << "ACE_CHECK_RETURN (0);" << be_nl
              << be_nl
              << "return result_any;";
        }
      else
        {
          *os << "CORBA::Boolean tk_void_any = 0;" << be_nl
              << "CORBA::Any *result_any ="  << be_idt_nl
              << "TAO_RequestInfo_Util::make_any "
              << "(tk_void_any ACE_ENV_ARG_PARAMETER);"
              << be_uidt_nl
              << "ACE_CHECK_RETURN (0);" << be_nl
              << be_nl
              << "CORBA::Any_var safe_result_any = "
              << "result_any;" << be_nl << be_nl;

          // Generate the insertion of result into Any.
          ctx = *this->ctx_;
          ctx.state (TAO_CodeGen::TAO_OPERATION_INTERCEPTORS_RESULT);
          be_visitor_operation_interceptors_result oir_visitor (&ctx);

          if (bt->accept (&oir_visitor) == -1)
            {
              ACE_ERROR_RETURN ((LM_ERROR,
                                 "(%N:%l) be_visitor_operation_cs::"
                                 "visit_operation - "
                                 "codegen for result failed\n"),
                                -1);
            }

          *os << "return safe_result_any._retn ();";
        }
    }
  else
    {
      if (be_global->use_raw_throw ())
        {
          *os << "throw (CORBA::NO_IMPLEMENT ());" << be_nl
              << "return 0;";
        }
      else
        {
          *os << "ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (), 0);";
        }
    }

  *os << be_uidt_nl << "}\n\n";

  os->decr_indent ();

  // -----------------------------------------------------------------
  // Update the result.
  // -----------------------------------------------------------------
  bt = be_type::narrow_from_decl (node->return_type ());

  if (!bt)
    {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_interceptors_ch::"
                             "visit_operation - "
                             "Bad return type\n"),
                            -1);
    }

  // Grab the right visitor to generate the return type accessor if
  // its not void since we cant have a private member to be of void
  // type.
  if (!this->void_return_type (bt))
    {
      *os << "void " << be_nl;

      *os << "TAO_ClientRequestInfo_"<< node->flat_name ();

      // We need the interface node in which this operation was defined.
      // However, if this operation node was an attribute node in disguise,
      // we get this information from the context and add a "_get"/"_set"
      // to the flat name to get around the problem of overloaded methods
      // which are generated for attributes.
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

      *os << "::"
          << "result (";

      ctx = *this->ctx_;
      ctx.state (TAO_CodeGen::TAO_OPERATION_RETTYPE_CH);
      be_visitor_operation_rettype or_visitor (&ctx);

      if (bt->accept (&or_visitor) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "(%N:%l) be_visitor_operation_cs::"
                             "visit_operation - "
                             "codegen for retval pre invoke failed\n"),
                            -1);
        }

      os->indent ();

      *os << " result)" << be_uidt << be_uidt << be_uidt_nl
          << "{" << be_idt_nl
          << "// update the result " << be_nl
          << "this->_result = result;" << be_uidt_nl
          << "}\n\n";
    }

  return 0;
}
